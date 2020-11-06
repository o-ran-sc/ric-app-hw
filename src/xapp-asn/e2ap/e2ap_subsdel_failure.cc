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
 * e2ap_subsdel_failure.cc
 *
 * Author: Shraboni Jana
 */



#include "e2ap_subsdel_failure.hpp"

E2APSubscriptionDeleteFailure::E2APSubscriptionDeleteFailure(SubscriptionDeleteFailureIEs& ieObj){



  _failureIEs = std::make_unique<SubscriptionDeleteFailureIEs>();
  *_failureIEs = ieObj;

  _e2ap_pdu_obj = (E2AP_PDU_t * )calloc(1, sizeof(E2AP_PDU_t));
  assert(_e2ap_pdu_obj != 0);

  _unsuccessMsg = (UnsuccessfulOutcome_t * )calloc(1, sizeof(UnsuccessfulOutcome_t));
  assert(_unsuccessMsg != 0);

  IE_array = (RICsubscriptionDeleteFailure_IEs_t *)calloc(RIC_SUBDEL_FAILURE_IES_COUNT, sizeof(RICsubscriptionDeleteFailure_IEs_t));
  assert(IE_array != 0);

  RICsubscriptionDeleteFailure_t * subDelfail = &(_unsuccessMsg->value.choice.RICsubscriptionDeleteFailure);
  for(int i = 0; i < RIC_SUBDEL_FAILURE_IES_COUNT; i++){
    ASN_SEQUENCE_ADD(&subDelfail->protocolIEs, &(IE_array[i]));
  }
  CD_array = 0;


};

E2APSubscriptionDeleteFailure::E2APSubscriptionDeleteFailure(unsigned char *buf, size_t *size){
	  _e2ap_pdu_obj = 0;
	  _unsuccessMsg = 0;
	  IE_array = 0;
	  CD_array = 0;
	  _failureIEs = std::make_unique<SubscriptionDeleteFailureIEs>();

	   bool status =  this->decode(buf, size);
	   if(!status)
		   throw "E2AP Subscription Delete Failure Decode Failed: "+this->get_error();
}

// Clear assigned protocolIE list from RIC indication IE container
E2APSubscriptionDeleteFailure::~E2APSubscriptionDeleteFailure(void){

  mdclog_write(MDCLOG_DEBUG, "Freeing subscription delete request object memory");
  RICsubscriptionDeleteFailure_t * E2APSubscriptionDeleteFailure = &(_unsuccessMsg->value.choice.RICsubscriptionDeleteFailure);

 for(int i = 0; i < E2APSubscriptionDeleteFailure->protocolIEs.list.size; i++){
    E2APSubscriptionDeleteFailure->protocolIEs.list.array[i] = 0;
  }

  if (E2APSubscriptionDeleteFailure->protocolIEs.list.size > 0){
    free(E2APSubscriptionDeleteFailure->protocolIEs.list.array);
    E2APSubscriptionDeleteFailure->protocolIEs.list.count = 0;
    E2APSubscriptionDeleteFailure->protocolIEs.list.size = 0;
    E2APSubscriptionDeleteFailure->protocolIEs.list.array = 0;
  }

  free(IE_array);
  free(CD_array);
  free(_unsuccessMsg);
  _e2ap_pdu_obj->choice.unsuccessfulOutcome = 0;

  ASN_STRUCT_FREE(asn_DEF_E2AP_PDU, _e2ap_pdu_obj);
  mdclog_write(MDCLOG_DEBUG, "Freed subscription delete request object memory");


};



bool E2APSubscriptionDeleteFailure::decode(unsigned char *buf, size_t *size)
{

	asn_dec_rval_t dec_res  = asn_decode(0,ATS_ALIGNED_BASIC_PER, &asn_DEF_E2AP_PDU, (void**)&(_e2ap_pdu_obj), buf, *size);
	if(dec_res.code != RC_OK){
				 mdclog_write(MDCLOG_ERR, "Failed to decode: %s","RIC Subscription Delete Failure");
				 return false;
	} else {
				 mdclog_write(MDCLOG_INFO, "Unsuccessfully decoded: %s","RIC Subscription Delete Failure");
	}

    _unsuccessMsg = _e2ap_pdu_obj->choice.unsuccessfulOutcome;
    //write the decoding code.

    if (_unsuccessMsg == 0){
    	_error_string = "Invalid reference to success message in get fields subscription delete failure";
    	return false;
    }


    for(int edx = 0; edx < _unsuccessMsg->value.choice.RICsubscriptionDeleteFailure.protocolIEs.list.count; edx++) {
		RICsubscriptionDeleteFailure_IEs_t *memb_ptr = _unsuccessMsg->value.choice.RICsubscriptionDeleteFailure.protocolIEs.list.array[edx];

		switch(memb_ptr->id)
		{
		//1.
		case (ProtocolIE_ID_id_RICrequestID):
				_failureIEs->set_ricRequestorID(memb_ptr->value.choice.RICrequestID.ricRequestorID);
    			_failureIEs->set_ricInstanceID(memb_ptr->value.choice.RICrequestID.ricInstanceID);
    	break;
    	//2.
		case (ProtocolIE_ID_id_RANfunctionID):
				_failureIEs->set_ranFunctionID(memb_ptr->value.choice.RANfunctionID);

		break;
		//3.
		case (ProtocolIE_ID_id_Cause):

				_failureIEs->set_ricCause(memb_ptr->value.choice.Cause.present);
			switch(memb_ptr->value.choice.Cause.present){
			case  Cause_PR_ricService :
				_failureIEs->set_ricSubCause(memb_ptr->value.choice.Cause.choice.ricService);
				break;
			case Cause_PR_transport :
				_failureIEs->set_ricSubCause(memb_ptr->value.choice.Cause.choice.transport);
				break;
			case  Cause_PR_protocol :
				_failureIEs->set_ricSubCause(memb_ptr->value.choice.Cause.choice.protocol);
				break;
			case Cause_PR_misc :
				_failureIEs->set_ricSubCause(memb_ptr->value.choice.Cause.choice.misc);
				break;
			case Cause_PR_ricRequest :
				_failureIEs->set_ricSubCause(memb_ptr->value.choice.Cause.choice.ricRequest);
				break;
			default:
				_failureIEs->set_ricSubCause(-1);
				break;
			}


	break;
    //4.
	case (ProtocolIE_ID_id_CriticalityDiagnostics):

		_cdWrapperIEs.set_procedureCode(*(memb_ptr->value.choice.CriticalityDiagnostics.procedureCode));
		_cdWrapperIEs.set_procedureCriticality(*(memb_ptr->value.choice.CriticalityDiagnostics.procedureCriticality));
		_cdWrapperIEs.set_triggeringMessage(*(memb_ptr->value.choice.CriticalityDiagnostics.triggeringMessage));


		RICrequestID_t* ricReq =  (RICrequestID_t*)memb_ptr->value.choice.CriticalityDiagnostics.ricRequestorID;
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
   // xer_fprint(stdout, &asn_DEF_E2AP_PDU, _e2ap_pdu_obj);
	return true;
}


bool E2APSubscriptionDeleteFailure::encode(unsigned char *buf, size_t *size){

  _e2ap_pdu_obj->choice.unsuccessfulOutcome = _unsuccessMsg;
  _e2ap_pdu_obj->present = E2AP_PDU_PR_unsuccessfulOutcome;
  bool result = setfields( _unsuccessMsg);
  if(!result){
	  return result;
  }
  _unsuccessMsg->procedureCode = ProcedureCode_id_RICsubscriptionDelete;
  _unsuccessMsg->criticality = Criticality_reject;
  _unsuccessMsg->value.present = UnsuccessfulOutcome__value_PR_RICsubscriptionDeleteFailure;

  int ret_constr = asn_check_constraints(&asn_DEF_E2AP_PDU, (void *) _e2ap_pdu_obj, _errbuf, &_errbuf_len);
  if(ret_constr){
    _error_string.assign(_errbuf, _errbuf_len);
    _error_string = "Constraints failed for encoding subscription delete request. Reason = " + _error_string;
    return false;
  }

  asn_enc_rval_t res = asn_encode_to_buffer(0, ATS_ALIGNED_BASIC_PER, &asn_DEF_E2AP_PDU, _e2ap_pdu_obj, buf, *size);

  if(res.encoded == -1){
    _error_string.assign(strerror(errno));
    _error_string = "Error encoding Subscription Delete Request. Reason = " + _error_string;
    return false;
  }
  else {
    if(*size < res.encoded){
      std::stringstream ss;
      ss  <<"Error encoding Subscription Delete Request . Reason =  encoded pdu size " << res.encoded << " exceeds buffer size " << *size << std::endl;
      _error_string = ss.str();
      res.encoded = -1;
      return false;
    }
  }

  *size = res.encoded;
  xer_fprint(stdout, &asn_DEF_E2AP_PDU, _e2ap_pdu_obj);

  return true;

}


bool  E2APSubscriptionDeleteFailure::setfields( UnsuccessfulOutcome_t *_unsuccessMsg){
  unsigned int ie_index;

  ie_index = 0;
  RICsubscriptionDeleteFailure_IEs_t *ies_ricreq = &IE_array[ie_index];
  ies_ricreq->criticality = Criticality_reject;
  ies_ricreq->id = ProtocolIE_ID_id_RICrequestID;
  ies_ricreq->value.present = RICsubscriptionDeleteFailure_IEs__value_PR_RICrequestID;
  RICrequestID_t *ricrequest_ie = &ies_ricreq->value.choice.RICrequestID;
  ricrequest_ie->ricRequestorID = this->getIEs().get_ricRequestorID();
  ricrequest_ie->ricInstanceID = this->getIEs().get_ricInstanceID();


  ie_index++;
  RICsubscriptionDeleteFailure_IEs_t *ies_ranfunc = &IE_array[ie_index];
  ies_ranfunc->criticality = Criticality_reject;
  ies_ranfunc->id = ProtocolIE_ID_id_RANfunctionID;
  ies_ranfunc->value.present = RICsubscriptionDeleteFailure_IEs__value_PR_RANfunctionID;
  RANfunctionID_t *ranfunction_ie = &ies_ranfunc->value.choice.RANfunctionID;
  *ranfunction_ie = this->getIEs().get_ranFunctionID();

  ie_index++;
  RICsubscriptionDeleteFailure_IEs_t *ies_ric_cause = &IE_array[ie_index];
  	ies_ric_cause->criticality = Criticality_ignore;
  	ies_ric_cause->id = ProtocolIE_ID_id_Cause;
  	ies_ric_cause->value.present = RICsubscriptionDeleteFailure_IEs__value_PR_Cause;
  	Cause_t * ric_cause = &(ies_ric_cause->value.choice.Cause);
  	ric_cause->present = (Cause_PR)this->getIEs().get_ricCause();
  	switch(this->getIEs().get_ricCause()){
  	case Cause_PR_ricService:
  		ric_cause->choice.ricService = this->getIEs().get_ricSubCause();
  		break;
  	case Cause_PR_transport:
  		ric_cause->choice.transport = this->getIEs().get_ricSubCause();
  		break;
  	case Cause_PR_protocol:
  		ric_cause->choice.protocol= this->getIEs().get_ricSubCause();
  		break;
  	case Cause_PR_misc:
  		ric_cause->choice.misc = this->getIEs().get_ricSubCause();
  		break;
  	case Cause_PR_ricRequest:
  		ric_cause->choice.ricRequest = this->getIEs().get_ricSubCause();
  		break;
  	default:
  		_error_string = _error_string + "Error ! Illegal cause enum " + std::to_string(this->getIEs().get_ricCause());
  		return false;
  	}

  if(this->getIEs().get_is_criticality_diagnostic()) {
  ie_index++;
  RICsubscriptionDeleteFailure_IEs_t *ies_criticdiag = &IE_array[ie_index];
  ies_criticdiag->criticality =  Criticality_ignore;
  ies_criticdiag->id = ProtocolIE_ID_id_CriticalityDiagnostics;
  ies_criticdiag->value.present = RICsubscriptionDeleteFailure_IEs__value_PR_CriticalityDiagnostics;
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
  }

  return true;
};





