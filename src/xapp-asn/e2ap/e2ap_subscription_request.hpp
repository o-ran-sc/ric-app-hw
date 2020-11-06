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
 * e2ap_subscription_request.hpp
 *
 *  Created on: Jun 30, 2020
 *      Author: Shraboni Jana
 */

#ifndef XAPP_ASN_REFACTOR_E2AP_SUBSCRIPTION_HPP_
#define XAPP_ASN_REFACTOR_E2AP_SUBSCRIPTION_HPP_
#include <E2AP-PDU.h>
#include <InitiatingMessage.h>
#include <ProtocolIE-Field.h>
#include <RICsubscriptionRequest.h>
#include <ProtocolIE-SingleContainer.h>

#include <RICactions-ToBeSetup-List.h>
#include <RICsubsequentAction.h>
#include <vector>

#include "e2ap_action.hpp"
/*
 RICsubscriptionRequest-IEs E2AP-PROTOCOL-IES ::= {
	{ ID id-RICrequestID				CRITICALITY reject	TYPE RICrequestID					PRESENCE mandatory}|
	{ ID id-RANfunctionID				CRITICALITY reject	TYPE RANfunctionID					PRESENCE mandatory}|
	{ ID id-RICsubscriptionDetails		CRITICALITY reject	TYPE RICsubscriptionDetails			PRESENCE mandatory},
	...
}
RICrequestID ::= SEQUENCE {
	ricRequestorID				INTEGER (0..65535),
	ricInstanceID	INTEGER (0..65535),
	...
}

RICsubscriptionDetails ::= SEQUENCE {
	ricEventTriggerDefinition	RICeventTriggerDefinition,
	ricAction-ToBeSetup-List	RICactions-ToBeSetup-List,
	...
}
 */

template <typename E2SMEventTriggerDefinition, typename E2SMActionDefinition>
class E2APSubscriptionRequest {
public:

	class SubscriptionRequestIEs{
	private:
		 long int ricRequestorID, ricInstanceID, ranFunctionID;
		 size_t ricEventTriggerDefinition_size = IE_SIZE;
		 unsigned char ricEventTriggerDefinition[IE_SIZE];

		 int ricAction_ToBeSetup_List_Count;
		 std::vector<typename E2APAction<E2SMActionDefinition>::ActionIEs> *ricAction_ToBeSetup_List;
	public:
 		 SubscriptionRequestIEs(void):ricRequestorID(0), ricInstanceID(0), ranFunctionID(0), ricAction_ToBeSetup_List(0), ricAction_ToBeSetup_List_Count(0){};
		 SubscriptionRequestIEs& set_ricRequestorID(long int req_id){ricRequestorID = req_id; return *this;};
		 SubscriptionRequestIEs& set_ricInstanceID(long int inst_id){ricInstanceID = inst_id; return *this;};
		 SubscriptionRequestIEs& set_ranFunctionID(long int func_id){ranFunctionID = func_id; return *this;};

		 SubscriptionRequestIEs& set_ricEventTriggerDefinition(E2SMEventTriggerDefinition &eventObj)
		 {
			 bool res = eventObj.encode(&(this->ricEventTriggerDefinition)[0],&ricEventTriggerDefinition_size);
		 	 if(!res){
						mdclog_write(MDCLOG_ERR, "Failed to encode: %s","RIC Event Trigger Definition");
						mdclog_write(MDCLOG_ERR, "Error during encode: %s",eventObj.get_error());
					} else {
						mdclog_write(MDCLOG_INFO, "Successfully encoded: %s of size: %d","RIC Event Trigger Definition",ricEventTriggerDefinition_size);
					}

		    return *this;
		 };

		 SubscriptionRequestIEs& set_ricAction_ToBeSetup_List(E2APAction<E2SMActionDefinition>  &actionObj)
		 { ricAction_ToBeSetup_List = actionObj.get_list();
		   ricAction_ToBeSetup_List_Count = actionObj.get_list_count();
		   return *this;
		 };

		 long int get_ricRequestorID(){return this->ricRequestorID;};
		 long int get_ricInstanceID(){return this->ricInstanceID;};
		 long int get_ranFunctionID(){return this->ranFunctionID;};

		 unsigned char* get_ricEventTriggerDefinition(){return this->ricEventTriggerDefinition;};
		 size_t get_ricEventTriggerDefinitionSize(){return this->ricEventTriggerDefinition_size;};
		 std::vector<typename E2APAction<E2SMActionDefinition>::ActionIEs>* get_ricAction_ToBeSetup_List(){ return this->ricAction_ToBeSetup_List;};
		 int get_ricAction_ToBeSetup_List_Count(){return this->ricAction_ToBeSetup_List_Count;};

	};

	E2APSubscriptionRequest(SubscriptionRequestIEs&);
	~E2APSubscriptionRequest();
	bool encode(unsigned char *, size_t * );
	std::string  get_error (void) const {return _error_string ;};
	void add(SubscriptionRequestIEs &ies){_requestIEs = ies;};
	SubscriptionRequestIEs& getIEs(){ return *_requestIEs.get();};
private:

	InitiatingMessage_t *initMsg;
	E2AP_PDU_t * e2ap_pdu_obj;
	RICsubscriptionRequest_IEs_t * IE_array;
	RICaction_ToBeSetup_ItemIEs_t * action_array;
	unsigned int action_array_size;

	std::unique_ptr<SubscriptionRequestIEs> _requestIEs;
	std::string _error_string;
    char _errbuf[128];
	size_t _errbuf_len = 128;

	bool setfields(InitiatingMessage_t *);
};

template <typename T1,typename T2>
E2APSubscriptionRequest<T1,T2>::E2APSubscriptionRequest(SubscriptionRequestIEs &subRequestObj){

  _requestIEs = std::make_unique<SubscriptionRequestIEs>();
  *_requestIEs = subRequestObj;

  e2ap_pdu_obj = 0;
  e2ap_pdu_obj = (E2AP_PDU_t * )calloc(1, sizeof(E2AP_PDU_t));
  assert(e2ap_pdu_obj != 0);

  initMsg = 0;
  initMsg = (InitiatingMessage_t * )calloc(1, sizeof(InitiatingMessage_t));
  assert(initMsg != 0);


  IE_array = 0;
  if(RIC_SUB_REQUEST_IES_COUNT == 0) {
		  mdclog_write(MDCLOG_ERR, "E2AP Subscription Request IEs = 0.");
  }
  IE_array = (RICsubscriptionRequest_IEs_t *)calloc(RIC_SUB_REQUEST_IES_COUNT, sizeof(RICsubscriptionRequest_IEs_t));
  assert(IE_array != 0);


  action_array_size = subRequestObj.get_ricAction_ToBeSetup_List_Count();
  action_array = 0;
  action_array = (RICaction_ToBeSetup_ItemIEs_t *)calloc(action_array_size, sizeof(RICaction_ToBeSetup_ItemIEs_t));
  assert(action_array != 0);


  e2ap_pdu_obj->choice.initiatingMessage = initMsg;
  e2ap_pdu_obj->present = E2AP_PDU_PR_initiatingMessage;



};

// Clear assigned protocolIE list from RIC indication IE container
template <typename T1, typename T2>
E2APSubscriptionRequest<T1,T2>::~E2APSubscriptionRequest(void){

  mdclog_write(MDCLOG_DEBUG, "Freeing subscription request memory");;

  RICsubscriptionDetails_t * ricsubscription_ie = &(IE_array[2].value.choice.RICsubscriptionDetails);

  for(int i = 0; i < ricsubscription_ie->ricAction_ToBeSetup_List.list.size; i++){
    ricsubscription_ie->ricAction_ToBeSetup_List.list.array[i] = 0;
  }

  // clear action list
  if (ricsubscription_ie->ricAction_ToBeSetup_List.list.size > 0){
    free(ricsubscription_ie->ricAction_ToBeSetup_List.list.array);
    ricsubscription_ie->ricAction_ToBeSetup_List.list.size = 0;
    ricsubscription_ie->ricAction_ToBeSetup_List.list.count = 0;
    ricsubscription_ie->ricAction_ToBeSetup_List.list.array = 0;
  }

  // clear subsequent action array
  for (unsigned int i = 0; i < action_array_size; i++){
    free(action_array[i].value.choice.RICaction_ToBeSetup_Item.ricSubsequentAction );
  }

  free(action_array);
  RICsubscriptionRequest_t * subscription_request = &(initMsg->value.choice.RICsubscriptionRequest);

  for(int i = 0; i < subscription_request->protocolIEs.list.size; i++){
    subscription_request->protocolIEs.list.array[i] = 0;
  }

  if( subscription_request->protocolIEs.list.size > 0){
    free( subscription_request->protocolIEs.list.array);
    subscription_request->protocolIEs.list.array = 0;
    subscription_request->protocolIEs.list.size = 0;
    subscription_request->protocolIEs.list.count = 0;
  }

  free(IE_array);
  free(initMsg);
  e2ap_pdu_obj->choice.initiatingMessage = 0;

  ASN_STRUCT_FREE(asn_DEF_E2AP_PDU, e2ap_pdu_obj);
  mdclog_write(MDCLOG_DEBUG, "Freed subscription request memory ");
};

template <typename T1, typename T2>
bool E2APSubscriptionRequest<T1,T2>::encode(unsigned char *buf, size_t *size){

  bool res;

  initMsg->procedureCode = ProcedureCode_id_RICsubscription;
  initMsg->criticality = Criticality_ignore;
  initMsg->value.present = InitiatingMessage__value_PR_RICsubscriptionRequest;

  res = setfields(initMsg);
  if (!res){
    return false;
  }

  int ret_constr = asn_check_constraints(&asn_DEF_E2AP_PDU, (void *) e2ap_pdu_obj, _errbuf, &_errbuf_len);
  if(ret_constr){
    _error_string.assign(_errbuf, _errbuf_len);
    _error_string = "Constraints failed for encoding subscription request. Reason = " + _error_string;
    return false;
  }

  asn_enc_rval_t retval = asn_encode_to_buffer(0, ATS_ALIGNED_BASIC_PER, &asn_DEF_E2AP_PDU, e2ap_pdu_obj, buf, *size);

  if(retval.encoded == -1){
    _error_string.assign(strerror(errno));
    _error_string = "Error encoding Subscription  Request. Reason = " + _error_string;
    return false;
  }
  else {
    if(*size < retval.encoded){
      std::stringstream ss;
      ss  <<"Error encoding Subscription  Request . Reason =  encoded pdu size " << retval.encoded << " exceeds buffer size " << *size << std::endl;
      _error_string = ss.str();
      retval.encoded = -1;
      return false;
    }
  }

  *size = retval.encoded;
  xer_fprint(stdout, &asn_DEF_E2AP_PDU, e2ap_pdu_obj);

  return true;

}

template <typename T1, typename T2>
bool E2APSubscriptionRequest<T1,T2>::setfields( InitiatingMessage_t * init_msg){


  int ie_index;
  int result = 0;

  if (init_msg == 0){
    _error_string = "Error. Invalid reference when getting fields from subscription request";
    return false;
  }

  RICsubscriptionRequest_t * ric_subscription = &(init_msg->value.choice.RICsubscriptionRequest);
  ric_subscription->protocolIEs.list.count = 0;

  ie_index = 0;
  RICsubscriptionRequest_IEs_t *ies_ricreq = &IE_array[ie_index];
  ies_ricreq->criticality = Criticality_reject;
  ies_ricreq->id = ProtocolIE_ID_id_RICrequestID;
  ies_ricreq->value.present = RICsubscriptionRequest_IEs__value_PR_RICrequestID;
  RICrequestID_t *ricrequest_ie = &ies_ricreq->value.choice.RICrequestID;
  ricrequest_ie->ricRequestorID = this->getIEs().get_ricRequestorID();
  result = ASN_SEQUENCE_ADD(&(ric_subscription->protocolIEs), &IE_array[ie_index]);
  	  assert(result == 0);

  ie_index++;
  RICsubscriptionRequest_IEs_t *ies_ranfunc = &IE_array[ie_index];
  ies_ranfunc->criticality = Criticality_reject;
  ies_ranfunc->id = ProtocolIE_ID_id_RANfunctionID;
  ies_ranfunc->value.present = RICsubscriptionRequest_IEs__value_PR_RANfunctionID;
  RANfunctionID_t *ranfunction_ie = &ies_ranfunc->value.choice.RANfunctionID;
  *ranfunction_ie = this->getIEs().get_ranFunctionID();
  result = ASN_SEQUENCE_ADD(&(ric_subscription->protocolIEs), &IE_array[ie_index]);
  assert(result == 0);


  ie_index++;
  RICsubscriptionRequest_IEs_t *ies_actid = &IE_array[ie_index];
  ies_actid->criticality = Criticality_reject;
  ies_actid->id = ProtocolIE_ID_id_RICsubscriptionDetails;
  ies_actid->value.present = RICsubscriptionRequest_IEs__value_PR_RICsubscriptionDetails;

  RICsubscriptionDetails_t *ricsubscription_ie = &ies_actid->value.choice.RICsubscriptionDetails;
  ricsubscription_ie->ricEventTriggerDefinition.buf = (uint8_t *) this->getIEs().get_ricEventTriggerDefinition();
  ricsubscription_ie->ricEventTriggerDefinition.size = this->getIEs().get_ricEventTriggerDefinitionSize();



  std::vector<typename E2APAction<T2>::ActionIEs> *ref_action_array = this->getIEs().get_ricAction_ToBeSetup_List();

  // reset the list count on ricAction_ToBeSetup_List;
  ricsubscription_ie->ricAction_ToBeSetup_List.list.count = 0;

  for(unsigned int i = 0; i < ref_action_array->size(); i ++){
    action_array[i].criticality = Criticality_ignore;
    action_array[i].id = ProtocolIE_ID_id_RICaction_ToBeSetup_Item ;
    action_array[i].value.present = RICaction_ToBeSetup_ItemIEs__value_PR_RICaction_ToBeSetup_Item;

    action_array[i].value.choice.RICaction_ToBeSetup_Item.ricActionID = (*ref_action_array)[i].get_ricActionID();
    action_array[i].value.choice.RICaction_ToBeSetup_Item.ricActionType = (*ref_action_array)[i].get_ricActionType();

    if((*ref_action_array)[i].get_is_ricSubsequentAction()){
    	action_array[i].value.choice.RICaction_ToBeSetup_Item.ricSubsequentAction = (RICsubsequentAction_t *)calloc(1, sizeof(RICsubsequentAction_t));
    	action_array[i].value.choice.RICaction_ToBeSetup_Item.ricSubsequentAction->ricSubsequentActionType = (*ref_action_array)[i].get_ricSubsequentActionType();
    	action_array[i].value.choice.RICaction_ToBeSetup_Item.ricSubsequentAction->ricTimeToWait = (*ref_action_array)[i].get_ricTimeToWait();
    }

    if((*ref_action_array)[i].get_is_ricActionDefinition()){
    	action_array[i].value.choice.RICaction_ToBeSetup_Item.ricActionDefinition = (RICactionDefinition_t*)calloc(1, sizeof(RICactionDefinition_t));

    	auto actionSize =  (*ref_action_array)[i].get_ricActionDefinition_size();
    	action_array[i].value.choice.RICaction_ToBeSetup_Item.ricActionDefinition->size = actionSize;
    	action_array[i].value.choice.RICaction_ToBeSetup_Item.ricActionDefinition->buf = (uint8_t *)calloc(1,actionSize);
    	memcpy(action_array[i].value.choice.RICaction_ToBeSetup_Item.ricActionDefinition->buf, (uint8_t*)(*ref_action_array)[i].get_ricActionDefinition(), actionSize);

    	action_array[i].value.choice.RICaction_ToBeSetup_Item.ricActionDefinition->size = actionSize;
    }

    result = ASN_SEQUENCE_ADD(&ricsubscription_ie->ricAction_ToBeSetup_List, &(action_array[i]));
    if (result == -1){
      _error_string = "Error : Unable to assign memory to add Action item to set up list";
      return false;
    }

  }

  result = ASN_SEQUENCE_ADD(&(ric_subscription->protocolIEs), &IE_array[ie_index]);
  assert(result == 0);



  return true;
};


#endif /* XAPP_ASN_REFACTOR_E2AP_SUBSCRIPTION_HPP_ */
