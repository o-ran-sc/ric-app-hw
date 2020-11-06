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
 * e2ap_E2APSubscriptionResponse.cc
 *
 *
 *      Author: SJana, Ashwin Sridharan
 */
#include<e2ap_subscription_response.hpp>

E2APSubscriptionResponse::E2APSubscriptionResponse(SubscriptionResponseIEs& subObj){

  _responseIEs = std::make_unique<SubscriptionResponseIEs>();
  *_responseIEs = subObj;

  _e2ap_pdu_obj = 0;
  _e2ap_pdu_obj = (E2AP_PDU_t *)calloc(1, sizeof(E2AP_PDU_t));
  assert(_e2ap_pdu_obj != 0);

  _successMsg = 0;
  _successMsg = (SuccessfulOutcome_t *)calloc(1, sizeof(SuccessfulOutcome_t));
  assert(_successMsg != 0);



  IE_array = 0;
  IE_array = (RICsubscriptionResponse_IEs_t *)calloc(RIC_SUB_RESPONSE_IES_COUNT, sizeof(RICsubscriptionResponse_IEs_t));
  assert(IE_array != 0);

  ie_admitted_list = 0;

  ie_not_admitted_list = 0;


};

// Clear assigned protocolIE list from RIC indication IE container
E2APSubscriptionResponse::~E2APSubscriptionResponse(void){

  RICsubscriptionResponse_t * ric_subscriptionResponse = &(_successMsg->value.choice.RICsubscriptionResponse);
  for(int i = 0; i < ric_subscriptionResponse->protocolIEs.list.size ; i++){
	  ric_subscriptionResponse->protocolIEs.list.array[i] = 0;
  }

  if (ric_subscriptionResponse->protocolIEs.list.size > 0){
    free(ric_subscriptionResponse->protocolIEs.list.array);
    ric_subscriptionResponse->protocolIEs.list.array = 0;
    ric_subscriptionResponse->protocolIEs.list.size = 0;
    ric_subscriptionResponse->protocolIEs.list.count = 0;
  }

  free(ie_admitted_list);
  free(ie_not_admitted_list);
  free(IE_array);
  free(_successMsg);
  _e2ap_pdu_obj->choice.successfulOutcome = 0;

  ASN_STRUCT_FREE(asn_DEF_E2AP_PDU, _e2ap_pdu_obj);
  mdclog_write(MDCLOG_DEBUG, "Freed Subscription Response Object memory");


};


bool E2APSubscriptionResponse::encode(unsigned char *buf, size_t *size){

    _e2ap_pdu_obj->present =  E2AP_PDU_PR_successfulOutcome;
    _e2ap_pdu_obj->choice.successfulOutcome = _successMsg;

    bool eres = setfields( _successMsg);
      if(!eres)
    	  return eres;

    _successMsg->procedureCode = ProcedureCode_id_RICsubscription;
    _successMsg->criticality = Criticality_reject;
    _successMsg->value.present = SuccessfulOutcome__value_PR_RICsubscriptionResponse;

  int ret_constr = asn_check_constraints(&asn_DEF_E2AP_PDU, (void *) _e2ap_pdu_obj, _errbuf, &_errbuf_len);
  if(ret_constr){
    _error_string.assign(_errbuf, _errbuf_len);
    _error_string = "Constraints failed for encoding subscription response. Reason = " + _error_string;
    return false;
  }

  asn_enc_rval_t res = asn_encode_to_buffer(0, ATS_ALIGNED_BASIC_PER, &asn_DEF_E2AP_PDU, _e2ap_pdu_obj, buf, *size);
  if(res.encoded == -1){
    mdclog_write(MDCLOG_DEBUG, "Error encoding PDU. Reason =%s",strerror(errno));

    return false;
  }
  else {
    if(*size < res.encoded){
      fprintf(stderr,  "Buffer assigned too small to encode: %s",(char *)(asn_DEF_E2AP_PDU.name));
      res.encoded = -1;
      return false;
    }
  }

  *size = res.encoded;
  xer_fprint(stdout, &asn_DEF_E2AP_PDU, _e2ap_pdu_obj);
  return true;

}

bool E2APSubscriptionResponse::setfields(SuccessfulOutcome_t *_successMsg){

  int ie_index;


  RICsubscriptionResponse_t * subsResponse = &(_successMsg->value.choice.RICsubscriptionResponse);
  //reset list count ..
  subsResponse->protocolIEs.list.count = 0;

  ie_index = 0;
  RICsubscriptionResponse_IEs_t *ies_ricreq = &IE_array[ie_index];

  ies_ricreq->criticality = Criticality_reject;
  ies_ricreq->id = ProtocolIE_ID_id_RICrequestID;
  ies_ricreq->value.present = RICsubscriptionResponse_IEs__value_PR_RICrequestID;
  RICrequestID_t *ricrequest_ie = &ies_ricreq->value.choice.RICrequestID;
  ricrequest_ie->ricRequestorID = this->getIEs().get_ricRequestorID();
  ricrequest_ie->ricInstanceID = this->getIEs().get_ricInstanceID();
  ASN_SEQUENCE_ADD(&subsResponse->protocolIEs, &(IE_array[ie_index]));


  ie_index++;
  RICsubscriptionResponse_IEs_t *ies_ranfunc = &IE_array[ie_index];
  ies_ranfunc->criticality = Criticality_reject;
  ies_ranfunc->id = ProtocolIE_ID_id_RANfunctionID;
  ies_ranfunc->value.present = RICsubscriptionResponse_IEs__value_PR_RANfunctionID;
  RANfunctionID_t *ranfunction_ie = &ies_ranfunc->value.choice.RANfunctionID;
  *ranfunction_ie = this->getIEs().get_ranFunctionID();
  ASN_SEQUENCE_ADD(&subsResponse->protocolIEs, &(IE_array[ie_index]));


  int admit_lst_count = 0;
  if(this->getIEs().get_is_ricActionAdmitlst()){
	  admit_lst_count = this->getIEs().get_actionsAdmitted_list().get_list_count();

	  ie_index++;
	  RICsubscriptionResponse_IEs_t *ies_admitted_actid = &IE_array[ie_index];
	  ies_admitted_actid->criticality = Criticality_reject;
	  ies_admitted_actid->id = ProtocolIE_ID_id_RICactions_Admitted;
	  RICaction_Admitted_List_t *ric_admitted_actions_ie = &ies_admitted_actid->value.choice.RICaction_Admitted_List;
	  ric_admitted_actions_ie->list.count = 0;

	  std::vector<RICactionAdmittedList::RICactionAdmittedItemIEs> * ref_admitted_action_array = this->getIEs().get_actionsAdmitted_list().get_list();
	  if(admit_lst_count ==  0){
	     ies_admitted_actid->value.present =  RICsubscriptionResponse_IEs__value_PR_NOTHING;
	   }
	   else{
	     ies_admitted_actid->value.present =  RICsubscriptionResponse_IEs__value_PR_RICaction_Admitted_List;
	   }
	  ie_admitted_list = (RICaction_Admitted_ItemIEs_t *)calloc(admit_lst_count, sizeof(RICaction_Admitted_ItemIEs_t));

	  assert(ie_admitted_list != 0);
	  for(unsigned int i = 0; i < ref_admitted_action_array->size(); i ++){
	        ie_admitted_list[i].criticality = Criticality_ignore;
	        ie_admitted_list[i].id = ProtocolIE_ID_id_RICaction_Admitted_Item ;
	        ie_admitted_list[i].value.present = RICaction_Admitted_ItemIEs__value_PR_RICaction_Admitted_Item;
	        ie_admitted_list[i].value.choice.RICaction_Admitted_Item.ricActionID = (*ref_admitted_action_array)[i].get_ricActionID();
	        ASN_SEQUENCE_ADD(ric_admitted_actions_ie, &(ie_admitted_list[i]));
	      }
	    ASN_SEQUENCE_ADD(&subsResponse->protocolIEs, &(IE_array[ie_index]));

  }

  if(this->getIEs().get_is_ricActionNotAdmitlst()){
	  int notadmit_lst_count = this->getIEs().get_actionsNotAdmitted_list().get_list_count();

	ie_index++;
	std::vector<RICactionNotAdmittedList::RICactionNotAdmittedItemIEs> * ref_notadmitted_action_array = this->getIEs().get_actionsNotAdmitted_list().get_list();

    RICsubscriptionResponse_IEs_t *ies_notadmitted_actid = &IE_array[ie_index];
    ies_notadmitted_actid->criticality = Criticality_reject;
    ies_notadmitted_actid->id = ProtocolIE_ID_id_RICactions_NotAdmitted;

    RICaction_NotAdmitted_List_t *ric_not_admitted_actions_ie = &ies_notadmitted_actid->value.choice.RICaction_NotAdmitted_List;
    ric_not_admitted_actions_ie->list.count = 0;


    ies_notadmitted_actid->value.present =  RICsubscriptionResponse_IEs__value_PR_RICaction_NotAdmitted_List;


    ie_not_admitted_list = (RICaction_NotAdmitted_ItemIEs_t *)calloc(notadmit_lst_count, sizeof(RICaction_NotAdmitted_ItemIEs_t));
    assert(ie_not_admitted_list != 0);


    for(unsigned int i = 0; i < ref_notadmitted_action_array->size(); i ++){
      ie_not_admitted_list[i].criticality = Criticality_ignore;
      ie_not_admitted_list[i].id = ProtocolIE_ID_id_RICaction_NotAdmitted_Item ;
      ie_not_admitted_list[i].value.present = RICaction_NotAdmitted_ItemIEs__value_PR_RICaction_NotAdmitted_Item;;
      ie_not_admitted_list[i].value.choice.RICaction_NotAdmitted_Item.ricActionID = (*ref_notadmitted_action_array)[i].get_ricActionID();

      int cause = (*ref_notadmitted_action_array)[i].get_ricCause();
      switch(cause){
      	  case Cause_PR_ricService:
      		  ie_not_admitted_list[i].value.choice.RICaction_NotAdmitted_Item.cause.choice.ricService = (*ref_notadmitted_action_array)[i].get_ricSubCause();
      		  break;
      	  case Cause_PR_transport:
      		  ie_not_admitted_list[i].value.choice.RICaction_NotAdmitted_Item.cause.choice.transport = (*ref_notadmitted_action_array)[i].get_ricSubCause();
      		  break;
      	  case Cause_PR_protocol:
      		  ie_not_admitted_list[i].value.choice.RICaction_NotAdmitted_Item.cause.choice.protocol= (*ref_notadmitted_action_array)[i].get_ricSubCause();
      		  break;
      	  case Cause_PR_misc:
      		  ie_not_admitted_list[i].value.choice.RICaction_NotAdmitted_Item.cause.choice.misc = (*ref_notadmitted_action_array)[i].get_ricSubCause();
      		  break;
      	  case Cause_PR_ricRequest:
      		  ie_not_admitted_list[i].value.choice.RICaction_NotAdmitted_Item.cause.choice.ricRequest = (*ref_notadmitted_action_array)[i].get_ricSubCause();
      		  break;
      	  default:
      		  mdclog_write(MDCLOG_ERR, "Error :: %s, %d : Unknown RIC cause %d\n", __FILE__, __LINE__, cause);
      		  break;
      }

      ie_not_admitted_list[i].value.choice.RICaction_NotAdmitted_Item.cause.present = (Cause_PR)cause;
      ASN_SEQUENCE_ADD(ric_not_admitted_actions_ie, &(ie_not_admitted_list[i]));
    }
    ASN_SEQUENCE_ADD(&subsResponse->protocolIEs, &(IE_array[ie_index]));

  }

  return true;
}

E2APSubscriptionResponse::E2APSubscriptionResponse(unsigned char *buf, size_t *size){
	  _e2ap_pdu_obj = 0;
	  _successMsg = 0;
	  IE_array = 0;
	  ie_admitted_list = 0;
	  ie_not_admitted_list = 0;

	  _responseIEs = std::make_unique<SubscriptionResponseIEs>();
	   bool status =  this->decode(buf, size);
	   if(!status)
		   throw "E2AP Subscription Delete Response Decode Failed: "+this->get_error();
}
bool E2APSubscriptionResponse:: decode(unsigned char *buf, size_t *size)
{

	asn_dec_rval_t dec_res  = asn_decode(0,ATS_ALIGNED_BASIC_PER, &asn_DEF_E2AP_PDU, (void**)&(_e2ap_pdu_obj), buf, *size);
	if(dec_res.code != RC_OK){
			mdclog_write(MDCLOG_ERR, "Failed to decode: %s","RIC Subscription Response");
			return false;
	} else {

		mdclog_write(MDCLOG_INFO, "Successfully decoded: %s","RIC Subscription Response");
	}

	 _successMsg = _e2ap_pdu_obj->choice.successfulOutcome;
	 //write the decoding code.

	 if (_successMsg == 0){
	   	_error_string = "Invalid reference to success message in get fields subscription response";
	   	return false;
	 }

	RANfunctionID_t * ranfunctionid;
	RICaction_Admitted_List_t  * ric_admitted_action_list;
	RICaction_NotAdmitted_List_t * ric_not_admitted_action_list;


	for(int edx = 0; edx < _successMsg->value.choice.RICsubscriptionResponse.protocolIEs.list.count; edx++) {
		RICsubscriptionResponse_IEs_t *memb_ptr = _successMsg->value.choice.RICsubscriptionResponse.protocolIEs.list.array[edx];

		switch(memb_ptr->id)
		{
		case (ProtocolIE_ID_id_RICrequestID):
			  _responseIEs->set_ricRequestorID(memb_ptr->value.choice.RICrequestID.ricRequestorID);
			  _responseIEs->set_ricInstanceID(memb_ptr->value.choice.RICrequestID.ricInstanceID);
		break;
		case (ProtocolIE_ID_id_RANfunctionID):
			  _responseIEs->set_ranFunctionID(memb_ptr->value.choice.RANfunctionID);
		break;

		case (ProtocolIE_ID_id_RICactions_Admitted):
			ric_admitted_action_list = &memb_ptr->value.choice.RICaction_Admitted_List;
			for(int index = 0; index < ric_admitted_action_list->list.count; index ++){
				RICaction_Admitted_ItemIEs_t * item = (RICaction_Admitted_ItemIEs_t *)ric_admitted_action_list->list.array[index];
				long int id = item->value.choice.RICaction_Admitted_Item.ricActionID;
				RICactionAdmittedList::RICactionAdmittedItemIEs ricActID; ricActID.set_ricActionID(id);
				_admitlst.add(ricActID);
			};
			this->getIEs().set_actionsAdmitted_list(_admitlst);
		break;

		case (ProtocolIE_ID_id_RICactions_NotAdmitted):
			ric_not_admitted_action_list = &memb_ptr->value.choice.RICaction_NotAdmitted_List;

			for(int index = 0; index < ric_not_admitted_action_list->list.count; index ++){
				RICaction_NotAdmitted_ItemIEs_t * item = (RICaction_NotAdmitted_ItemIEs_t *)ric_not_admitted_action_list->list.array[index];
				long int id = item->value.choice.RICaction_NotAdmitted_Item.ricActionID;
				int cause = item->value.choice.RICaction_NotAdmitted_Item.cause.present;
				int sub_cause;
				switch(cause){

					case  Cause_PR_ricService :
						sub_cause = item->value.choice.RICaction_NotAdmitted_Item.cause.choice.ricService;
						break;

					case Cause_PR_transport :
						sub_cause = item->value.choice.RICaction_NotAdmitted_Item.cause.choice.transport;
						break;

					case  Cause_PR_protocol :
						sub_cause = item->value.choice.RICaction_NotAdmitted_Item.cause.choice.protocol;
						break;

					case Cause_PR_misc :
						sub_cause = item->value.choice.RICaction_NotAdmitted_Item.cause.choice.misc;
						break;

					case Cause_PR_ricRequest :
						sub_cause = item->value.choice.RICaction_NotAdmitted_Item.cause.choice.ricRequest;
						break;

					default:
						mdclog_write(MDCLOG_DEBUG, "Error ! Illegal cause enum %d", cause);
						return false;
			}
			_notadmitlst.add(RICactionNotAdmittedList::RICactionNotAdmittedItemIEs().set_ricActionID(id).set_ricCause(cause).set_ricSubCause(sub_cause));
			this->getIEs().set_actionsNotAdmitted_list(_notadmitlst);
		 }
		break;
	}

   }

	return true;
}






