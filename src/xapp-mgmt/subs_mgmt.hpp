/*
==================================================================================
        Copyright (c) 2018-2019 AT&T Intellectual Property.

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
#include <chrono>
#include <tuple>

#include "../xapp-formats/e2ap/subscription_delete_request.hpp"
#include "../xapp-formats/e2ap/subscription_delete_response.hpp"
#include "../xapp-formats/e2ap/subscription_request.hpp"
#include "../xapp-formats/e2ap/subscription_response.hpp"

#define SUBSCR_SUCCESS 0
#define SUBSCR_ERR_TX 1
#define SUBSCR_ERR_TIMEOUT 2
#define SUBSCR_ERR_FAIL 3
#define SUBSCR_ERR_UNKNOWN 4
#define SUBSCR_ERR_DUPLICATE 5
#define SUBSCR_ERR_ENCODE  6
#define SUBSCR_ERR_MISSING 7

using namespace std;

typedef enum {
    request_pending = 1,
    request_success,
    request_failed,
    delete_request_pending,
    delete_request_success,
    delete_request_failed,
    request_duplicate
}Subscription_Status_Types;

using subscription_identifier = std::string; //here subscription_identifier is the rmr transaction id.

class SubscriptionHandler {
			    
public:

	SubscriptionHandler(unsigned int timeout_seconds = 5, unsigned int num_retries = 2);
  
  void init(void);

  template <typename Transmitter>
  int request_subscription(std::string, Transmitter &&);

  template<typename Transmitter>
  int request_subscription_delete(std::string,  int ,  Transmitter &&);

  void  Response(int, unsigned char *, int, const char *);
  int const get_request_status(subscription_identifier);
  subscription_response_helper * const get_subscription(subscription_identifier);
  
  unsigned int get_next_id(void);
  void set_timeout(unsigned int);
  void set_num_retries(unsigned int);
  
  bool is_subscription_entry(subscription_identifier); 
  bool is_request_entry(subscription_identifier);
  void get_subscription_keys(std::vector<subscription_identifier> &);
  void clear(void);
  size_t  num_pending(void) const;
  size_t  num_complete(void) const ;


  
private:

  
  bool add_request_entry(subscription_identifier, int);
  bool set_request_status(subscription_identifier, int);
  bool delete_request_entry(subscription_identifier);
 
  bool get_subscription_entry(subscription_identifier);
  bool add_subscription_entry(subscription_identifier, subscription_response_helper &he);
  bool delete_subscription_entry(subscription_identifier);

  std::unordered_map<subscription_identifier, int> requests_table;
  
  std::unique_ptr<std::mutex> _data_lock;
  std::unique_ptr<std::condition_variable> _cv;

  std::chrono::seconds _time_out;
  unsigned int _num_retries = 2;
  unsigned int unique_request_id = 0;
  
};

template <typename Transmitter>
int SubscriptionHandler::request_subscription(std::string rmr_trans_id, Transmitter && tx){
  bool res;

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
  res = tx();

  if (!res){
    // clear state
    delete_request_entry(rmr_trans_id);
    mdclog_write(MDCLOG_ERR, "%s, %d :: Error transmitting subscription request %s", __FILE__, __LINE__, rmr_trans_id );
    return SUBSCR_ERR_TX;
  };


  // record time stamp ..
  auto start = std::chrono::system_clock::now();
  res = SUBSCR_ERR_UNKNOWN;

  while(1){
    // release lock and wait to be woken up
    _cv.get()->wait_for(_local_lock, _time_out);

    // we have woken and acquired data_lock
    // check status and return appropriate object

    int status = get_request_status(rmr_trans_id);

    if (status == request_success){
      // retreive  & store the subscription response (why?)
      // response = subscription_responses[sub_id];
      mdclog_write(MDCLOG_INFO, "Successfully subscribed for request %s, %d", rmr_trans_id);
      res = SUBSCR_SUCCESS;
      break;
    }

    if (status == request_pending){
      // woken up spuriously or timed out
      auto end = std::chrono::system_clock::now();
      std::chrono::duration<double> f = end - start;

      if ( f > _num_retries * _time_out){
	mdclog_write(MDCLOG_ERR, "%s, %d:: Subscription request with transaction id %s timed out waiting for response ", __FILE__, __LINE__, rmr_trans_id);
	res = SUBSCR_ERR_TIMEOUT;
	break;
      }
      else{
	mdclog_write(MDCLOG_INFO, "Subscription request with transaction id %s Waiting for response ....", rmr_trans_id);
	continue;
      }
    }

    if(status == request_failed){
      mdclog_write(MDCLOG_ERR, "Error :: %s, %d : Subscription Request with transaction id %s  got failure response .. \n", __FILE__, __LINE__, rmr_trans_id);
      res = SUBSCR_ERR_FAIL;
      break;
    }

    if (status == request_duplicate){
      mdclog_write(MDCLOG_ERR, "Error :: %s, %d : Subscription Request with transaction id %s is duplicate : subscription already present in table .. \n", __FILE__, __LINE__, rmr_trans_id);
      res = SUBSCR_ERR_DUPLICATE;
      break;

    }

    // if we are here, some spurious
    // status obtained or request failed . we return appropriate error code
    mdclog_write(MDCLOG_ERR, "Error :: %s, %d : Spurious time out caused by invalid state of request %s, %d -- state = %d. Deleting request entry and failing .. \n", __FILE__, __LINE__, rmr_trans_id, status);
    res = SUBSCR_ERR_UNKNOWN;
    break;
  };

  delete_request_entry(rmr_trans_id);

  // release data lock
  _local_lock.unlock();
  std::cout <<"Returning  res = " << res << " for request = " << rmr_trans_id  << std::endl;
  return res;
};

#endif
