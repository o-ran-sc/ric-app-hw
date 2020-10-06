/*
==================================================================================
        Copyright (c) 2019-2020 AT&T Intellectual Property.

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
==================================================================================
*/
/*
 * subs_mgmt.hpp
 * Created on: 2019
 * Author: Ashwin Shridharan, Shraboni Jana
 */

#pragma once

#ifndef SUBSCRIPTION_HANDLER
#define SUBSCRIPTION_HANDLER

#include <functional>
#include <mdclog/mdclog.h>
#include <mutex>
#include <condition_variable>
#include <unordered_map>
#include <algorithm>
#include <ctime>
#include <unistd.h>
#include <chrono>
#include <tuple>
#include <rmr/RIC_message_types.h>

#include "subscription_delete_request.hpp"
#include "subscription_delete_response.hpp"
#include "subscription_request.hpp"
#include "subscription_response.hpp"

#define SUBSCR_SUCCESS 1
#define SUBSCR_ERR_TX -1
#define SUBSCR_ERR_TIMEOUT -2
#define SUBSCR_ERR_FAIL -3
#define SUBSCR_ERR_UNKNOWN -4
#define SUBSCR_ERR_DUPLICATE -5

using namespace std;

class TransmitterBase
{
public:
    virtual ~TransmitterBase() {}

    template<class T>
    const T& getParam() const; //to be implemented after Parameter

    template<class T, class U>
    void setParam(const U& rhs); //to be implemented after Parameter
};

template <typename T>
class Transmitter : public TransmitterBase
{
public:
	Transmitter(const T& tx) :obj(tx) {}
    const T& getParam() const {return obj;}
    void setParam(const T& tx) {obj=tx;}
private:
    T obj;
};

//Here's the trick: dynamic_cast rather than virtual
template<class T> const T& TransmitterBase::getParam() const
{
	return dynamic_cast<const Transmitter<T>&>(*this).getParam();
}
template<class T, class U> void TransmitterBase::setParam(const U& rhs)
{
	dynamic_cast<Transmitter<T>&>(*this).setParam(rhs);
	return;
}

typedef enum {
    request_pending = 1,
    request_success,
    request_failed,
    request_duplicate
}Subscription_Status_Types;


using transaction_identifier = std::string;
using transaction_status = Subscription_Status_Types;

class SubscriptionHandler {
			    
public:

  SubscriptionHandler(unsigned int timeout_seconds = 30);
  
  template <typename AppTransmitter>
  int manage_subscription_request(transaction_identifier, AppTransmitter &&);

  template <typename AppTransmitter>
  int manage_subscription_delete_request(transaction_identifier, AppTransmitter &&);

  void manage_subscription_response(int message_type, transaction_identifier id);

  int  get_request_status(transaction_identifier);
  bool set_request_status(transaction_identifier, transaction_status);
  bool is_request_entry(transaction_identifier);
  void set_timeout(unsigned int);
  void clear(void);
  void set_ignore_subs_resp(bool b){_ignore_subs_resp = b;};

  void print_subscription_status(){ for(auto it:status_table){std::cout << it.first << "::" << it.second << std::endl;}};

private:
  
  bool add_request_entry(transaction_identifier, transaction_status);
  bool delete_request_entry(transaction_identifier);

  template <typename AppTransmitter>
  bool add_transmitter_entry(transaction_identifier, AppTransmitter&&);

  std::unordered_map<transaction_identifier, TransmitterBase> trans_table;
  std::unordered_map<transaction_identifier, transaction_status> status_table;

  std::unique_ptr<std::mutex> _data_lock;
  std::unique_ptr<std::condition_variable> _cv;

  std::chrono::seconds _time_out;
  
  bool _ignore_subs_resp = false;
};

template <typename AppTransmitter>
bool SubscriptionHandler::add_transmitter_entry(transaction_identifier id, AppTransmitter &&trans){
	  mdclog_write(MDCLOG_INFO,"Entry added for Transaction ID: %s",id.c_str());

  // add entry in hash table if it does not exist
  auto search = trans_table.find(id);
  if(search != trans_table.end()){
    return false;
  }

  Transmitter<AppTransmitter> tptr(trans);
  trans_table[id] = tptr;
  return true;

};

//this will work for both sending subscription request and subscription delete request.
//The handler is oblivious of the message content and follows the transaction id.
template<typename AppTransmitter>
int SubscriptionHandler::manage_subscription_request(transaction_identifier rmr_trans_id, AppTransmitter && tx){
	int res;
  // put entry in request table
  {
    std::lock_guard<std::mutex> lock(*(_data_lock.get()));

    res = add_request_entry(rmr_trans_id, request_pending);
    if(! res){
      mdclog_write(MDCLOG_ERR, "%s, %d : Error adding new subscription request %s to queue because request with identical key already present",  __FILE__, __LINE__, rmr_trans_id);
      return SUBSCR_ERR_DUPLICATE;
    }
  }


  // acquire lock ...
  std::unique_lock<std::mutex> _local_lock(*(_data_lock.get()));

  // Send the message
  bool flg = tx();

  if (!flg){
    // clear state
    delete_request_entry(rmr_trans_id);
    mdclog_write(MDCLOG_ERR, "%s, %d :: Error transmitting subscription request %s", __FILE__, __LINE__, rmr_trans_id.c_str() );
    return SUBSCR_ERR_TX;
  } else {
	  mdclog_write(MDCLOG_INFO, "%s, %d :: Transmitted subscription request for trans_id %s", __FILE__, __LINE__, rmr_trans_id.c_str() );
	  add_transmitter_entry(rmr_trans_id, tx);

  }

  // record time stamp ..
  auto start = std::chrono::system_clock::now();
  std::chrono::milliseconds t_out(_time_out);

  //the wait functionality has been removed.


  _local_lock.unlock();
  // std::cout <<"Returning  res = " << res << " for request = " << rmr_trans_id  << std::endl;
   return res;
};

#endif
