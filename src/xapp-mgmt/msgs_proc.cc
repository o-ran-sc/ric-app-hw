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
 * msgs_proc.cc
 * Created on: 2019
 * Author: Ashwin Shridharan, Shraboni Jana
 */


#include "msgs_proc.hpp"

//sending messages are encoded.
bool XappMsgHandler::encode_subscription_request(unsigned char* buffer, size_t *buf_len)
{
	int request_id = 2; // will be over-written by subscription handler
	int req_seq = 1;
	int function_id = 0;
	int action_id = 1;
	int action_type = 0;
	int subsequent_action = 0; // continue
	int time_to_wait = 4; // 10ms

	int message_type = 1;
	int procedure_code = 27;
	std::string egnb_id = "Testgnb";
	std::string plmn_id = "Testplmn";

	unsigned char event_buf[128];
	size_t event_buf_len = 128;
	bool res;


	e2sm_event_trigger_helper trigger_data;
	e2sm_event_trigger event_trigger;

	trigger_data.egNB_id = egnb_id;
	trigger_data.plmn_id = plmn_id;
	trigger_data.egNB_id_type = 2;
	trigger_data.interface_direction = 1;
	trigger_data.procedure_code = procedure_code;
	trigger_data.message_type = message_type;
	//======================================================

	// Encode the event trigger definition
	res = event_trigger.encode_event_trigger(&event_buf[0], &event_buf_len, trigger_data);
	if (!res){
		mdclog_write(MDCLOG_ERR, "Error : %s, %d: Could not encode subscription Request. Reason = %s\n", __FILE__, __LINE__, event_trigger.get_error().c_str());
		return false;
	}
	mdclog_write(MDCLOG_INFO, "Encoded event trigger definition into PDU of size %lu bytes\n", event_buf_len);


	// create the subscription
	subscription_helper subscr_req;
	subscription_request e2ap_sub_req;

	subscr_req.clear();
	subscr_req.set_request(request_id, req_seq);
	subscr_req.set_function_id(function_id);
	subscr_req.add_action(action_id, action_type, "", subsequent_action, time_to_wait);

	subscr_req.set_event_def(&event_buf[0], event_buf_len);
	// generate the request pdu
	res = e2ap_sub_req.encode_e2ap_subscription(&buffer[0], buf_len, subscr_req);
	if(! res){
		mdclog_write(MDCLOG_ERR, "%s, %d: Error encoding subscription pdu. Reason = ", __FILE__, __LINE__);
		return false;
	}
	return true;
}
bool XappMsgHandler::encode_subscription_delete_request(unsigned char* buffer, size_t *buf_len){

	subscription_helper sub_helper;
	sub_helper.set_request(0, 0); // requirement of subscription manager ... ?
	sub_helper.set_function_id(0);

	subscription_delete e2ap_sub_req_del;

	  // generate the delete request pdu

	  bool res = e2ap_sub_req_del.encode_e2ap_subscription(&buffer[0], buf_len, sub_helper);
	  if(! res){
	    mdclog_write(MDCLOG_ERR, "%s, %d: Error encoding subscription delete request pdu. Reason = %s", __FILE__, __LINE__, e2ap_sub_req_del.get_error().c_str());
	    return false;
	  }

	return true;

}

bool XappMsgHandler::decode_subscription_response(unsigned char* data_buf, size_t data_size){

	bool res = true;
	E2N_E2AP_PDU_t *e2pdu = 0;

	asn_dec_rval_t rval;

	ASN_STRUCT_RESET(asn_DEF_E2N_E2AP_PDU, e2pdu);

	rval = asn_decode(0,ATS_ALIGNED_BASIC_PER, &asn_DEF_E2N_E2AP_PDU, (void**)&e2pdu, data_buf, data_size);
	switch(rval.code)
	{
		case RC_OK:
			  //Put in Subscription Response Object.
			   asn_fprint(stdout, &asn_DEF_E2N_E2AP_PDU, e2pdu);

			   break;
		case RC_WMORE:
				mdclog_write(MDCLOG_ERR, "RC_WMORE");
				res = false;
				break;
		case RC_FAIL:
				mdclog_write(MDCLOG_ERR, "RC_FAIL");
				res = false;
				break;
		default:
				break;
	 }
	ASN_STRUCT_FREE(asn_DEF_E2N_E2AP_PDU, e2pdu);
	return res;

}
bool XappMsgHandler::decode_subscription_delete_response(unsigned char* data_buf, size_t data_size){

	bool res = true;
	E2N_E2AP_PDU_t *e2pdu = 0;

	asn_dec_rval_t rval;

	ASN_STRUCT_RESET(asn_DEF_E2N_E2AP_PDU, e2pdu);

	rval = asn_decode(0,ATS_ALIGNED_BASIC_PER, &asn_DEF_E2N_E2AP_PDU, (void**)&e2pdu, data_buf, data_size);
	switch(rval.code)
	{
		case RC_OK:
			  //Put in Subscription Delete Response Object.
			   asn_fprint(stdout, &asn_DEF_E2N_E2AP_PDU, e2pdu);
			   break;
		case RC_WMORE:
				mdclog_write(MDCLOG_ERR, "RC_WMORE");
				res = false;
				break;
		case RC_FAIL:
				mdclog_write(MDCLOG_ERR, "RC_FAIL");
				res = false;
				break;
		default:
				break;
	 }
	ASN_STRUCT_FREE(asn_DEF_E2N_E2AP_PDU, e2pdu);
	return res;

}
bool XappMsgHandler::decode_subscription_response_failure(unsigned char* data_buf, size_t data_size){

	bool res = true;
	E2N_E2AP_PDU_t *e2pdu = 0;

	asn_dec_rval_t rval;

	ASN_STRUCT_RESET(asn_DEF_E2N_E2AP_PDU, e2pdu);

	rval = asn_decode(0,ATS_ALIGNED_BASIC_PER, &asn_DEF_E2N_E2AP_PDU, (void**)&e2pdu, data_buf, data_size);
	switch(rval.code)
	{
		case RC_OK:
			  //Extract Subscription Response Failure.
			   asn_fprint(stdout, &asn_DEF_E2N_E2AP_PDU, e2pdu);
			   break;
		case RC_WMORE:
				mdclog_write(MDCLOG_ERR, "RC_WMORE");
				res = false;
				break;
		case RC_FAIL:
				mdclog_write(MDCLOG_ERR, "RC_FAIL");
				res = false;
				break;
		default:
				break;
	 }
	ASN_STRUCT_FREE(asn_DEF_E2N_E2AP_PDU, e2pdu);
	return res;

}

bool XappMsgHandler::decode_subscription_delete_response_failure(unsigned char* data_buf, size_t data_size){

	bool res = true;
	E2N_E2AP_PDU_t *e2pdu = 0;

	asn_dec_rval_t rval;

	ASN_STRUCT_RESET(asn_DEF_E2N_E2AP_PDU, e2pdu);

	rval = asn_decode(0,ATS_ALIGNED_BASIC_PER, &asn_DEF_E2N_E2AP_PDU, (void**)&e2pdu, data_buf, data_size);
	switch(rval.code)
	{
		case RC_OK:
			  //Extract Subscription Delete Response Failure.
			   asn_fprint(stdout, &asn_DEF_E2N_E2AP_PDU, e2pdu);
				break;
		case RC_WMORE:
				mdclog_write(MDCLOG_ERR, "RC_WMORE");
				res = false;
				break;
		case RC_FAIL:
				mdclog_write(MDCLOG_ERR, "RC_FAIL");
				res = false;
				break;
		default:
				break;
	 }
	ASN_STRUCT_FREE(asn_DEF_E2N_E2AP_PDU, e2pdu);
	return res;

}

//For processing received messages.
rmr_mbuf_t * XappMsgHandler::operator()(rmr_mbuf_t *message){

  if (message->len > MAX_RMR_RECV_SIZE){
    mdclog_write(MDCLOG_ERR, "Error : %s, %d, RMR message larger than %d. Ignoring ...", __FILE__, __LINE__, MAX_RMR_RECV_SIZE);
    return message;
  }

  switch(message->mtype){
  	  //need to fix the health check.
  	  case (RIC_HEALTH_CHECK_REQ):
		message->mtype = RIC_HEALTH_CHECK_RESP;        // if we're here we are running and all is ok
  	    message->sub_id = -1;
	  break;

  	  case (RIC_SUB_RESP):
		//Received Subscription Response Message
		decode_subscription_response(message->payload,message->len);
  	    message = NULL;
	  break;

  	  case (RIC_SUB_DEL_RESP):
		decode_subscription_delete_response(message->payload,message->len);
	  	message = NULL;
  	  break;

  	  case (RIC_SUB_FAILURE):
		decode_subscription_response_failure(message->payload, message->len);
  	    message = NULL;
      break;

  	  case (RIC_SUB_DEL_FAILURE):
		decode_subscription_delete_response_failure(message->payload,message->len);
  	    message = NULL;
  	  break;
  	  //  case A1_POLICY_REQ:
  	 // break;


  default:
    mdclog_write(MDCLOG_ERR, "Error :: Unknown message type %d received from RMR", message->mtype);

  }

  return message;

};


