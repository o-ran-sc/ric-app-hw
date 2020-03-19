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
 * subs_mgmt.cc
 * Created on: 2019
 * Author: Ashwin Shridharan, Shraboni Jana
 */
#include "subs_mgmt.hpp"

#include <errno.h>


SubscriptionHandler::SubscriptionHandler(unsigned int timeout_seconds, unsigned int num_tries):_time_out(std::chrono::seconds(timeout_seconds)), _num_retries(num_tries){
  init();   
};

void SubscriptionHandler::init(void){
  
  _data_lock = std::make_unique<std::mutex>();
  _cv = std::make_unique<std::condition_variable>();
  
}

void SubscriptionHandler::clear(void){
  {
    std::lock_guard<std::mutex> lock(*(_data_lock).get());
    requests_table.clear();
    subscription_responses.clear();
  }
  
};

size_t SubscriptionHandler::num_pending(void) const {
  return requests_table.size();
}

size_t SubscriptionHandler::num_complete(void) const {
  return subscription_responses.size();
}


void SubscriptionHandler::set_timeout(unsigned int timeout_seconds){
  _time_out = std::chrono::seconds(timeout_seconds);
}

void SubscriptionHandler::set_num_retries(unsigned int num_tries){
  _num_retries = num_tries;
};



bool SubscriptionHandler::add_request_entry(subscription_identifier id, int status){

  // add entry in hash table if it does not exist
  auto search = requests_table.find(id);
  if(search != requests_table.end()){
    return false;
  }
  
  requests_table[id] = status;
  return true;

};

bool SubscriptionHandler::set_request_status(subscription_identifier id, int status){
  
  // change status of a request only if it exists.
  auto search = requests_table.find(id);
  if(search != requests_table.end()){
    requests_table[id] = status;
    return true;
  }

  return false;
  
};


bool SubscriptionHandler::delete_request_entry(subscription_identifier id){

  auto search = requests_table.find(id);
  if (search != requests_table.end()){
    requests_table.erase(search);
    return true;
  }

  return false;
};
  
bool SubscriptionHandler::add_subscription_entry(subscription_identifier id, subscription_response_helper &he){

  auto search = subscription_responses.find(id);
  if (search == subscription_responses.end()){
    subscription_responses[id] = he;
    return true;
  }

  return false;
}


bool SubscriptionHandler::delete_subscription_entry(subscription_identifier id){

  auto search = subscription_responses.find(id);
  if(search == subscription_responses.end()){
    return false;
  }
  else{
    subscription_responses.erase(search);
    return true;
  }
  
}

subscription_response_helper *  const SubscriptionHandler::get_subscription(subscription_identifier id){
  auto search = subscription_responses.find(id);
  if(search == subscription_responses.end()){
    return NULL;
  }
  else{
    return &(subscription_responses[id]);
  }
};


// Handles responses from RMR
void SubscriptionHandler::Response(int message_type, unsigned char *payload, int payload_length, const char * node_id){

  bool res;
  std::string node(node_id);
  int type;
  int procedureCode;
  bool valid_response  =false;
  
  E2N_E2AP_PDU_t * e2ap_recv;
  asn_dec_rval_t retval;

  subscription_response sub_resp;
  subscription_delete_response sub_del_resp;

  subscription_response_helper he_response;


  e2ap_recv = 0;
  retval = asn_decode(0, ATS_ALIGNED_BASIC_PER, &asn_DEF_E2N_E2AP_PDU, (void**)&(e2ap_recv), payload, payload_length);

  if(retval.code != RC_OK){
    mdclog_write(MDCLOG_ERR, "%s, %d: Error decoding E2AP PDU of RMR type %d. Bytes decoded = %lu out of %d\n", __FILE__, __LINE__, message_type, retval.consumed, payload_length);
    ASN_STRUCT_FREE(asn_DEF_E2N_E2AP_PDU, e2ap_recv);
    return ;
  }
  
  type = e2ap_recv->present;
  mdclog_write(MDCLOG_INFO, "Received RMR message of type = %d", type);
  
  if(type == E2N_E2AP_PDU_PR_successfulOutcome){
    
    procedureCode =  e2ap_recv->choice.successfulOutcome->procedureCode;
    mdclog_write(MDCLOG_INFO, "Received E2N_E2AP PDU  successful outcome message with procedureCode = %d", procedureCode);  

    if( procedureCode == E2N_ProcedureCode_id_ricSubscription){  
      // subscription response
      // decode the message
      sub_resp.get_fields(e2ap_recv->choice.successfulOutcome, he_response);
      {
	std::lock_guard<std::mutex> lock(*(_data_lock.get()));
	// get the id
	subscription_identifier id = std::make_tuple (node, he_response.get_request_id());

	// get status of id 
	int req_status = get_request_status(id);
	if (req_status == request_pending ){
	  res = add_subscription_entry(id, he_response);
	  if(res)
	    set_request_status(id, request_success);
	  
	  else{
	    set_request_status(id, request_duplicate);
	    mdclog_write(MDCLOG_ERR, "Error:: %s, %d: Request %s, %d seems to be a duplicate. Subscription already present in subscription table\n", __FILE__, __LINE__, std::get<0>(id).c_str(), std::get<1>(id));
	  }
	  
	  valid_response = true;
	}
	else if (req_status > 0){
	  // we don't change status of response since it was not in pending
	  // we simply fail
	  mdclog_write(MDCLOG_ERR, "Error:: %s, %d: Request %s,%d is not in request_pending state, is in State = %d\n", __FILE__, __LINE__, std::get<0>(id).c_str(), std::get<1>(id), req_status);
	  
	}
	else{
	  mdclog_write(MDCLOG_ERR,  "%s, %d: Could not find id %s, %d in request queue for subscription", __FILE__, __LINE__,  std::get<0>(id).c_str(), std::get<1>(id));
	}	  
	
      }
      
    }
    
    else if( procedureCode == E2N_ProcedureCode_id_ricSubscriptionDelete){
      
      res = sub_del_resp.get_fields(e2ap_recv->choice.successfulOutcome, he_response);
      {
	std::lock_guard<std::mutex> lock(*(_data_lock.get()));

	// get the id
	subscription_identifier id = std::make_tuple (node, he_response.get_request_id());
	
	int req_status = get_request_status(id);
	if (req_status == delete_request_pending ){
	  // Remove the subscription from the table 
	  res = delete_subscription_entry(id);
	  if(res){
	    set_request_status(id, delete_request_success);
	    valid_response = true;
	  }
	  else{
	    set_request_status(id, delete_request_failed);
	    mdclog_write(MDCLOG_ERR,  "%s, %d: Error deleting subscription entry for  %s, %d", __FILE__, __LINE__, std::get<0>(id).c_str(), std::get<1>(id));
	    valid_response = true;
	  } 
	}      
	else if (req_status > 0){
	  // we don't change status since it was not in pending
	  // we simply fail
	  mdclog_write(MDCLOG_ERR, "Error:: %s, %d: Request %s, %d for deletion  is not in delete_pending state, is in State = %d\n", __FILE__, __LINE__, id, std::get<0>(id).c_str(), std::get<1>(id));
	}
	else{
	  mdclog_write(MDCLOG_ERR,  "%s, %d: Could not find request id  %s, %d in request queue for deletion ", __FILE__, __LINE__,  std::get<0>(id).c_str(), std::get<1>(id));
	}

      }
    }

    else{
      mdclog_write(MDCLOG_ERR,  "%s, %d: Subscription Handler Response received E2AP PDU success  response with an non-subscription response related type  %d", __FILE__, __LINE__, procedureCode);
    }
    
  }
  
  else if(type == E2N_E2AP_PDU_PR_unsuccessfulOutcome){
    
    procedureCode = e2ap_recv->choice.unsuccessfulOutcome->procedureCode;
    mdclog_write(MDCLOG_INFO, "Received E2AP PDU  unsuccessful outcome message with procedureCode = %d", procedureCode);  
    
    if(procedureCode == E2N_ProcedureCode_id_ricSubscription){
      
      sub_resp.get_fields(e2ap_recv->choice.unsuccessfulOutcome, he_response);
      {
	std::lock_guard<std::mutex> lock(*(_data_lock.get()));

	// get the id
	subscription_identifier id = std::make_tuple (node, he_response.get_request_id());

	int req_status = get_request_status(id);
	if(req_status == request_pending){
	  set_request_status(id, request_failed);
	  valid_response = true;
	  mdclog_write(MDCLOG_ERR, "Subscription request %d failed", id);
	}
	else if (req_status > 0){
	  // we don't changet status since it was not in pending
	  // we simply fail
	  mdclog_write(MDCLOG_ERR, "Error:: %s, %d: Request %s, %d is not in request_pending state, is in State = %d\n", __FILE__, __LINE__, std::get<0>(id).c_str(), std::get<1>(id), req_status);
	}
	else{
	  mdclog_write(MDCLOG_ERR,  "%s, %d: Could not find id %s, %d in request queue for subscription ", __FILE__, __LINE__, std::get<0>(id).c_str(), std::get<1>(id));
	}
      }
    }
    
    else if(procedureCode == E2N_ProcedureCode_id_ricSubscriptionDelete){
      
      res = sub_del_resp.get_fields(e2ap_recv->choice.unsuccessfulOutcome, he_response);	
      {
	std::lock_guard<std::mutex> lock(*(_data_lock.get()));
	// get the id
	subscription_identifier id = std::make_tuple (node, he_response.get_request_id());
	
	int req_status = get_request_status(id);
	if(req_status == delete_request_pending){
	  set_request_status(id, delete_request_failed);
	  mdclog_write(MDCLOG_INFO, "Subscription delete request %s,%d failed", std::get<0>(id).c_str(), std::get<1>(id));
	  valid_response = true;
	}
	else if (req_status > 0){
	  mdclog_write(MDCLOG_ERR, "Error:: %s, %d: Request %s,%d for deletion  is not in delete_pending state, is in State = %d\n", __FILE__, __LINE__, std::get<0>(id).c_str(), std::get<1>(id), req_status);
	}
	else{
	  mdclog_write(MDCLOG_ERR,  "%s, %d: Could not find id  %s,%d  in request queue for deletion ", __FILE__, __LINE__, std::get<0>(id).c_str(), std::get<1>(id));
	}
	
      }
    }
    else{
      mdclog_write(MDCLOG_ERR,  "%s, %d: Susbcription Handler Response received E2AP PDU failure response with a non-subscription response related type  %d", __FILE__, __LINE__,  procedureCode);

    }
  }
  else{
    mdclog_write(MDCLOG_ERR,  "%s, %d: Susbcription Handler Response received E2AP PDU with non response type  %d", __FILE__, __LINE__, type);
  }
  
  
  ASN_STRUCT_FREE(asn_DEF_E2N_E2AP_PDU, e2ap_recv);
  
  // wake up all waiting users ...
  if(valid_response){
    _cv.get()->notify_all();
  }
  
}


int const SubscriptionHandler::get_request_status(subscription_identifier id){
  auto search = requests_table.find(id);
  if (search == requests_table.end()){
    return -1;
  }
  
  return search->second;
}
				   
 bool SubscriptionHandler::is_subscription_entry(subscription_identifier id){
  auto search = subscription_responses.find(id);
  if (search != subscription_responses.end())
    return true;
  else
    return false;
}

bool SubscriptionHandler::is_request_entry(subscription_identifier id){
  auto search = requests_table.find(id);
  if (search != requests_table.end())
    return true;
  else
    return false;
}


void SubscriptionHandler::get_subscription_keys(std::vector<subscription_identifier> & key_list){
  for(auto & e: subscription_responses){
    key_list.push_back(e.first);
  }
}
