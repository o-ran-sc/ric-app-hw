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
 * msgs_proc.cc
 * Created on: 2019
 * Author: Ashwin Shridharan, Shraboni Jana
 */


#include "msgs_proc.hpp"


bool  XappMsgHandler::a1_policy_handler(char * message, int *message_len, a1_policy_helper &helper){

  rapidjson::Document doc;
  if (doc.Parse<kParseStopWhenDoneFlag>(message).HasParseError()){
    mdclog_write(MDCLOG_ERR, "Error: %s, %d :: Could not decode A1 JSON message %s\n", __FILE__, __LINE__, message);
    return false;
  }

  //Extract Operation
  rapidjson::Pointer temp1("/operation");
    rapidjson::Value * ref1 = temp1.Get(doc);
    if (ref1 == NULL){
      mdclog_write(MDCLOG_ERR, "Error : %s, %d:: Could not extract policy type id from %s\n", __FILE__, __LINE__, message);
      return false;
    }

   helper.operation = ref1->GetString();

  // Extract policy id type
  rapidjson::Pointer temp2("/policy_type_id");
  rapidjson::Value * ref2 = temp2.Get(doc);
  if (ref2 == NULL){
    mdclog_write(MDCLOG_ERR, "Error : %s, %d:: Could not extract policy type id from %s\n", __FILE__, __LINE__, message);
    return false;
  }
   //helper.policy_type_id = ref2->GetString();
    helper.policy_type_id = to_string(ref2->GetInt());

    // Extract policy instance id
    rapidjson::Pointer temp("/policy_instance_id");
    rapidjson::Value * ref = temp.Get(doc);
    if (ref == NULL){
      mdclog_write(MDCLOG_ERR, "Error : %s, %d:: Could not extract policy type id from %s\n", __FILE__, __LINE__, message);
      return false;
    }
    helper.policy_instance_id = ref->GetString();

    if (helper.policy_type_id == "1" && helper.operation == "CREATE"){
    	helper.status = "OK";
    	Document::AllocatorType& alloc = doc.GetAllocator();

    	Value handler_id;
    	handler_id.SetString(helper.handler_id.c_str(), helper.handler_id.length(), alloc);

    	Value status;
    	status.SetString(helper.status.c_str(), helper.status.length(), alloc);


    	doc.AddMember("handler_id", handler_id, alloc);
    	doc.AddMember("status",status, alloc);
    	doc.RemoveMember("operation");
    	StringBuffer buffer;
    	Writer<StringBuffer> writer(buffer);
    	doc.Accept(writer);
    	strncpy(message,buffer.GetString(), buffer.GetLength());
    	*message_len = buffer.GetLength();
    	return true;
    }
    return false;
 }


//For processing received messages.XappMsgHandler should mention if resend is required or not.
void XappMsgHandler::operator()(rmr_mbuf_t *message, bool *resend){

	if (message->len > MAX_RMR_RECV_SIZE){
		mdclog_write(MDCLOG_ERR, "Error : %s, %d, RMR message larger than %d. Ignoring ...", __FILE__, __LINE__, MAX_RMR_RECV_SIZE);
		return;
	}
	a1_policy_helper helper;
	bool res=false;
	switch(message->mtype){
		//need to fix the health check.
		case (RIC_HEALTH_CHECK_REQ):
				message->mtype = RIC_HEALTH_CHECK_RESP;        // if we're here we are running and all is ok
				message->sub_id = -1;
				strncpy( (char*)message->payload, "HELLOWORLD OK\n", rmr_payload_size( message) );
				*resend = true;
				break;

		case (RIC_SUB_RESP):
        		mdclog_write(MDCLOG_INFO, "Received subscription message of type = %d", message->mtype);
				unsigned char *me_id;
				if( (me_id = (unsigned char *) malloc( sizeof( unsigned char ) * RMR_MAX_MEID )) == NULL ) {
					mdclog_write(MDCLOG_ERR, "Error :  %s, %d : malloc failed for me_id", __FILE__, __LINE__);
					me_id = rmr_get_meid(message, NULL);
				} else {
					rmr_get_meid(message, me_id);
				}
				if(me_id == NULL){
					mdclog_write(MDCLOG_ERR, " Error :: %s, %d : rmr_get_meid failed me_id is NULL", __FILE__, __LINE__);
					break;
				}
				mdclog_write(MDCLOG_INFO,"RMR Received MEID: %s",me_id);
				if(_ref_sub_handler !=NULL){
					_ref_sub_handler->manage_subscription_response(message->mtype, reinterpret_cast< char const* >(me_id));
				} else {
					mdclog_write(MDCLOG_ERR, " Error :: %s, %d : Subscription handler not assigned in message processor !", __FILE__, __LINE__);
				}
				*resend = false;
				if (me_id != NULL) {
					mdclog_write(MDCLOG_INFO, "Free RMR Received MEID memory: %s(0x%x)", me_id, me_id);
					free(me_id);
				}
				break;

	case A1_POLICY_REQ:

		    mdclog_write(MDCLOG_INFO, "In Message Handler: Received A1_POLICY_REQ.");
			helper.handler_id = xapp_id;

			res = a1_policy_handler((char*)message->payload, &message->len, helper);
			if(res){
				message->mtype = A1_POLICY_RESP;        // if we're here we are running and all is ok
				message->sub_id = -1;
				*resend = true;
			}
			break;
	case RIC_INDICATION:

		mdclog_write(MDCLOG_INFO, "Received Indication message of type = %d", message->mtype);
		//pick the relevant decoding code from test_e2sm.h, section(E2SM, IndicationMessageDecode)
		break;

	default:
		{
			mdclog_write(MDCLOG_ERR, "Error :: Unknown message type %d received from RMR", message->mtype);
			*resend = false;
		}
	}

	return;

};


