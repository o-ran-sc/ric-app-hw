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
 * e2ap_E2APSubscriptionFailure.cc
 *
 *
 *      Author: SJana, Ashwin Sridharan
 */
#include<e2ap_subscription_failure.hpp>

E2APSubscriptionFailure::E2APSubscriptionFailure(SubscriptionFailureIEs& subObj){

  _failureIEs = std::make_unique<SubscriptionFailureIEs>();
  *_failureIEs = subObj;

  _e2ap_pdu_obj = 0;
  _e2ap_pdu_obj = (E2AP_PDU_t *)calloc(1, sizeof(E2AP_PDU_t));
  assert(_e2ap_pdu_obj != 0);

  _unsuccessMsg = 0;
  _unsuccessMsg = (UnsuccessfulOutcome_t *)calloc(1, sizeof(UnsuccessfulOutcome_t));
  assert(_unsuccessMsg != 0);



  IE_array = 0;
  IE_array = (RICsubscriptionFailure_IEs_t *)calloc(RIC_SUB_FAILURE_IES_COUNT, sizeof(RICsubscriptionFailure_IEs_t));
  assert(IE_array != 0);


  ie_not_admitted_list = 0;
  CD_array = 0;

};

// Clear assigned protocolIE list from RIC indication IE container
E2APSubscriptionFailure::~E2APSubscriptionFailure(void){

  RICsubscriptionFailure_t * ric_subscriptionFailure = &(_unsuccessMsg->value.choice.RICsubscriptionFailure);
  for(int i = 0; i < ric_subscriptionFailure->protocolIEs.list.size ; i++){
	  ric_subscriptionFailure->protocolIEs.list.array[i] = 0;
  }

  if (ric_subscriptionFailure->protocolIEs.list.size > 0){
    free(ric_subscriptionFailure->protocolIEs.list.array);
    ric_subscriptionFailure->protocolIEs.list.array = 0;
    ric_subscriptionFailure->protocolIEs.list.size = 0;
    ric_subscriptionFailure->protocolIEs.list.count = 0;
  }

  free(ie_not_admitted_list);
  free(IE_array);
  free(_unsuccessMsg);
  _e2ap_pdu_obj->choice.unsuccessfulOutcome = 0;

  ASN_STRUCT_FREE(asn_DEF_E2AP_PDU, _e2ap_pdu_obj);
  mdclog_write(MDCLOG_DEBUG, "Freed Subscription Failure Object memory");


};


bool E2APSubscriptionFailure::encode(unsigned char *buf, size_t *size){

    _e2ap_pdu_obj->present =  E2AP_PDU_PR_unsuccessfulOutcome;
    _e2ap_pdu_obj->choice.unsuccessfulOutcome = _unsuccessMsg;

    bool eres = setfields( _unsuccessMsg);
      if(!eres)
    	  return eres;

    _unsuccessMsg->procedureCode = ProcedureCode_id_RICsubscription;
    _unsuccessMsg->criticality = Criticality_reject;
    _unsuccessMsg->value.present = UnsuccessfulOutcome__value_PR_RICsubscriptionFailure;

  int ret_constr = asn_check_constraints(&asn_DEF_E2AP_PDU, (void *) _e2ap_pdu_obj, _errbuf, &_errbuf_len);
  if(ret_constr){
    _error_string.assign(_errbuf, _errbuf_len);
    _error_string = "Constraints failed for encoding subscription failure. Reason = " + _error_string;
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

bool E2APSubscriptionFailure::setfields(UnsuccessfulOutcome_t *_unsuccessMsg){

  int ie_index;


  RICsubscriptionFailure_t * subsFailure = &(_unsuccessMsg->value.choice.RICsubscriptionFailure);
  //reset list count ..
  subsFailure->protocolIEs.list.count = 0;

  ie_index = 0;
  RICsubscriptionFailure_IEs_t *ies_ricreq = &IE_array[ie_index];

  ies_ricreq->criticality = Criticality_reject;
  ies_ricreq->id = ProtocolIE_ID_id_RICrequestID;
  ies_ricreq->value.present = RICsubscriptionFailure_IEs__value_PR_RICrequestID;
  RICrequestID_t *ricrequest_ie = &ies_ricreq->value.choice.RICrequestID;
  ricrequest_ie->ricRequestorID = this->getIEs().get_ricRequestorID();
  ricrequest_ie->ricInstanceID = this->getIEs().get_ricInstanceID();
  ASN_SEQUENCE_ADD(&subsFailure->protocolIEs, &(IE_array[ie_index]));


  ie_index++;
  RICsubscriptionFailure_IEs_t *ies_ranfunc = &IE_array[ie_index];
  ies_ranfunc->criticality = Criticality_reject;
  ies_ranfunc->id = ProtocolIE_ID_id_RANfunctionID;
  ies_ranfunc->value.present = RICsubscriptionFailure_IEs__value_PR_RANfunctionID;
  RANfunctionID_t *ranfunction_ie = &ies_ranfunc->value.choice.RANfunctionID;
  *ranfunction_ie = this->getIEs().get_ranFunctionID();
  ASN_SEQUENCE_ADD(&subsFailure->protocolIEs, &(IE_array[ie_index]));


  if(this->getIEs().get_is_ricActionNotAdmitlst()){
	  int notadmit_lst_count = this->getIEs().get_actionsNotAdmitted_list().get_list_count();

	ie_index++;
	std::vector<RICactionNotAdmittedList::RICactionNotAdmittedItemIEs> * ref_notadmitted_action_array = this->getIEs().get_actionsNotAdmitted_list().get_list();

    RICsubscriptionFailure_IEs_t *ies_notadmitted_actid = &IE_array[ie_index];
    ies_notadmitted_actid->criticality = Criticality_reject;
    ies_notadmitted_actid->id = ProtocolIE_ID_id_RICactions_NotAdmitted;

    RICaction_NotAdmitted_List_t *ric_not_admitted_actions_ie = &ies_notadmitted_actid->value.choice.RICaction_NotAdmitted_List;
    ric_not_admitted_actions_ie->list.count = 0;


    ies_notadmitted_actid->value.present =  RICsubscriptionFailure_IEs__value_PR_RICaction_NotAdmitted_List;


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
    ASN_SEQUENCE_ADD(&subsFailure->protocolIEs, &(IE_array[ie_index]));

  }
  if(this->getIEs().get_is_criticality_diagnostic()) {
   ie_index++;
   RICsubscriptionFailure_IEs_t *ies_criticdiag = &IE_array[ie_index];
   ies_criticdiag->criticality =  Criticality_ignore;
   ies_criticdiag->id = ProtocolIE_ID_id_CriticalityDiagnostics;
   ies_criticdiag->value.present = RICsubscriptionFailure_IEs__value_PR_CriticalityDiagnostics;
   CriticalityDiagnostics_t *critic_diag = &ies_criticdiag->value.choice.CriticalityDiagnostics;

   TriggeringMessage_t *tmsg = (TriggeringMessage_t*) calloc(1, sizeof(TriggeringMessage_t));
   *tmsg = this->getIEs().get_critical_diagnostic()->get_triggeringMessage();
   critic_diag->triggeringMessage= tmsg;

   RICrequestID_t *ricreq = (RICrequestID_t *)calloc(1, sizeof(RICrequestID_t));
   ricrequest_ie->ricRequestorID = this->getIEs().get_critical_diagnostic()->get_ricRequestorID();
   ricrequest_ie->ricInstanceID = this->getIEs().get_critical_diagnostic()->get_ricInstanceID();
   critic_diag->ricRequestorID = ricreq;

   ProcedureCode_t *pcode = (ProcedureCode_t*)calloc(1, sizeof(ProcedureCode_t));
   *pcode = this->getIEs().get_critical_diagnostic()->get_procedureCode();
   critic_diag->procedureCode =pcode;

   Criticality_t *pcritical = (Criticality_t*)calloc(1, sizeof(Criticality_t));
   *pcritical = this->getIEs().get_critical_diagnostic()->get_procedureCriticality();
   critic_diag->procedureCriticality = pcritical;


   if(this->getIEs().get_critical_diagnostic()->get_is_criticalityDiagnostics_list()){

 	  critic_diag->iEsCriticalityDiagnostics = (CriticalityDiagnostics_IE_List_t*)calloc(1, sizeof(CriticalityDiagnostics_IE_List_t));
 	  critic_diag->iEsCriticalityDiagnostics->list.count=0;

 	  int lst_count = this->getIEs().get_critical_diagnostic()->get_list_count();
 	  auto *lst = this->getIEs().get_critical_diagnostic()->get_list();
 	  CD_array = (CriticalityDiagnostics_IE_Item_t*)calloc(lst_count, sizeof(CriticalityDiagnostics_IE_Item_t));
 	  int i = 0;
 	  for(auto l:*lst){
 		  CD_array[i].iE_ID =  l.get_typeOferror();
 		  CD_array[i].typeOfError = l.get_ieID();
 		  CD_array[i].iECriticality = l.get_iEcriticality();
 		 int result = ASN_SEQUENCE_ADD(&(critic_diag->iEsCriticalityDiagnostics->list.array), &(CD_array[i]));
 		 if (result == -1){
 		       _error_string = "Error : Unable to assign memory to add CriticalityDiagnostics_IE item to set up list";
 		       return false;
 		 	 }
 		     i++;
 	  	  }

   	  }
   ASN_SEQUENCE_ADD(&subsFailure->protocolIEs, &(IE_array[ie_index]));

   }


  return true;
}

E2APSubscriptionFailure::E2APSubscriptionFailure(unsigned char *buf, size_t *size){
	  _e2ap_pdu_obj = 0;
	  _unsuccessMsg = 0;
	  IE_array = 0;
	  ie_not_admitted_list = 0;

	  _failureIEs = std::make_unique<SubscriptionFailureIEs>();
	   bool status =  this->decode(buf, size);
	   if(!status)
		   throw "E2AP Subscription  Failure Decode Failed: "+this->get_error();
}
bool E2APSubscriptionFailure::decode(unsigned char *buf, size_t *size)
{

	asn_dec_rval_t dec_res  = asn_decode(0,ATS_ALIGNED_BASIC_PER, &asn_DEF_E2AP_PDU, (void**)&(_e2ap_pdu_obj), buf, *size);
	if(dec_res.code != RC_OK){
			mdclog_write(MDCLOG_ERR, "Failed to decode: %s","RIC Subscription Failure");
			return false;
	} else {

		mdclog_write(MDCLOG_INFO, "Successfully decoded: %s","RIC Subscription Failure");
	}

	 _unsuccessMsg = _e2ap_pdu_obj->choice.unsuccessfulOutcome;
	 //write the decoding code.

	 if (_unsuccessMsg == 0){
	   	_error_string = "Invalid reference to unsuccess message in get fields subscription failure";
	   	return false;
	 }

	RANfunctionID_t * ranfunctionid;
	RICaction_NotAdmitted_List_t * ric_not_admitted_action_list;
	RICrequestID_t* ricReq;

	for(int edx = 0; edx < _unsuccessMsg->value.choice.RICsubscriptionFailure.protocolIEs.list.count; edx++) {
		RICsubscriptionFailure_IEs_t *memb_ptr = _unsuccessMsg->value.choice.RICsubscriptionFailure.protocolIEs.list.array[edx];

		switch(memb_ptr->id)
		{
		case (ProtocolIE_ID_id_RICrequestID):
			  _failureIEs->set_ricRequestorID(memb_ptr->value.choice.RICrequestID.ricRequestorID);
			  _failureIEs->set_ricInstanceID(memb_ptr->value.choice.RICrequestID.ricInstanceID);
		break;
		case (ProtocolIE_ID_id_RANfunctionID):
			  _failureIEs->set_ranFunctionID(memb_ptr->value.choice.RANfunctionID);
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

		case (ProtocolIE_ID_id_CriticalityDiagnostics):

			_cdWrapperIEs.set_procedureCode(*(memb_ptr->value.choice.CriticalityDiagnostics.procedureCode));
			_cdWrapperIEs.set_procedureCriticality(*(memb_ptr->value.choice.CriticalityDiagnostics.procedureCriticality));
			_cdWrapperIEs.set_triggeringMessage(*(memb_ptr->value.choice.CriticalityDiagnostics.triggeringMessage));


			ricReq =  (RICrequestID_t*)memb_ptr->value.choice.CriticalityDiagnostics.ricRequestorID;
			_cdWrapperIEs.set_ricRequestorID(ricReq->ricRequestorID);
			_cdWrapperIEs.set_ricInstanceID(ricReq->ricInstanceID);

			int lstcount = memb_ptr->value.choice.CriticalityDiagnostics.iEsCriticalityDiagnostics->list.count;
			auto *lst = _failureIEs->get_critical_diagnostic()->get_list();
			for(int i=0; i < lstcount; i++){
			        CriticalityDiagnosticsIEs tmpObj;
			        tmpObj.set_iEcriticality(memb_ptr->value.choice.CriticalityDiagnostics.iEsCriticalityDiagnostics->list.array[i]->iECriticality);
			        tmpObj.set_ieID(memb_ptr->value.choice.CriticalityDiagnostics.iEsCriticalityDiagnostics->list.array[i]->iE_ID);
			        tmpObj.set_typeOferror(memb_ptr->value.choice.CriticalityDiagnostics.iEsCriticalityDiagnostics->list.array[i]->typeOfError);
			        _cdWrapperIEs.add(tmpObj);
			 }
			_failureIEs->set_critical_diagnostic(_cdWrapperIEs);
			break;

	}

   }

	return true;
}






