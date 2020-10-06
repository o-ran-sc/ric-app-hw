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
 * subs_mgmt.cc
 * Created on: 2019
 * Author: Ashwin Shridharan, Shraboni Jana
 */
#include "subs_mgmt.hpp"
#include <thread>
#include <errno.h>

SubscriptionHandler::SubscriptionHandler(unsigned int timeout_seconds):_time_out(std::chrono::seconds(timeout_seconds)){
	  _data_lock = std::make_unique<std::mutex>();
	  _cv = std::make_unique<std::condition_variable>();
};

void SubscriptionHandler::clear(void){
  {
    std::lock_guard<std::mutex> lock(*(_data_lock).get());
    status_table.clear();
  }
  
};


bool SubscriptionHandler::add_request_entry(transaction_identifier id, transaction_status status){

  // add entry in hash table if it does not exist
  auto search = status_table.find(id);
  if(search != status_table.end()){
    return false;
  }
  
  status_table[id] = status;
  return true;

};



bool SubscriptionHandler::delete_request_entry(transaction_identifier id){

  auto search = status_table.find(id);

  if (!trans_table.empty()) {
	  auto search2 = trans_table.find(id);
	  if(search2 !=trans_table.end()){
		  trans_table.erase(search2);
	  }
  }

  if (search != status_table.end()){
    status_table.erase(search);
    mdclog_write(MDCLOG_INFO,"Entry for Transaction ID deleted: %d",id);
    return true;
  }
  mdclog_write(MDCLOG_INFO,"Entry not found in SubscriptionHandler for Transaction ID: %d",id);

  return false;
};


bool SubscriptionHandler::set_request_status(transaction_identifier id, transaction_status status){

  // change status of a request only if it exists.
	for(auto &it:status_table){
			if(strcmp(it.first.c_str(), id.c_str())==0) {
				it.second = status;
				return true;
			}
		}
  return false;

};


int SubscriptionHandler::get_request_status(transaction_identifier id){

	for(auto it:status_table){
		if(strcmp(it.first.c_str(), id.c_str())==0) {
			return it.second;
		}
	}


  return -1;
}
				   


bool SubscriptionHandler::is_request_entry(transaction_identifier id){
	for(auto it:status_table){
			if(strcmp(it.first.c_str(), id.c_str())==0) {
				return true;
			}
		}
    return false;
}




// Handles subscription responses
void SubscriptionHandler::manage_subscription_response(int message_type, transaction_identifier id){
	// Make This Thread sleep for 1 Second
  std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  {
	  std::unique_lock<std::mutex> _local_lock(*(_data_lock.get()));
	  mdclog_write(MDCLOG_INFO,"Subscription Handler: Status for me id %s WAS: %d",id.c_str(),this->get_request_status(id));

	  //from the message type we can know if its a success/failure etc.
	  if(message_type==RIC_SUB_RESP)
	  this->set_request_status(id, request_success);

	  if(message_type==RIC_SUB_FAILURE)
	  this->set_request_status(id,request_failed);

	  mdclog_write(MDCLOG_INFO,"Subscription Handler: Status for me id %s IS: %d",id.c_str(),this->get_request_status(id));

	  //this->print_subscription_status();
   }
  //_cv.get()->notify_all();

}

