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
 * e2ap_control_failure.cc
 *
 * Author: SJana, Ashwin Sridharan
 */

#include "e2ap_control_failure.hpp"


E2APControlFailure::E2APControlFailure(ControlFailureIEs &cntrlObj){
	_cntrlIEs = std::make_unique<ControlFailureIEs>();
	*_cntrlIEs = cntrlObj;

	_e2ap_pdu_obj = 0;
	_e2ap_pdu_obj = (E2AP_PDU_t * )calloc(1, sizeof(E2AP_PDU_t));
	assert(_e2ap_pdu_obj != 0);

	_unsuccessMsg = 0;
	_unsuccessMsg = (UnsuccessfulOutcome_t * )calloc(1, sizeof(UnsuccessfulOutcome_t));
	assert(_unsuccessMsg != 0);

	_unsuccessMsg->procedureCode = ProcedureCode_id_RICcontrol;
	_unsuccessMsg->criticality = Criticality_reject;
	_unsuccessMsg->value.present = UnsuccessfulOutcome__value_PR_RICcontrolFailure;


	IE_array = 0;
	IE_array = (RICcontrolFailure_IEs_t *)calloc(RIC_CONTROL_FAILURE_IES_COUNT, sizeof(RICcontrolFailure_IEs_t));
	assert(IE_array != 0);

	RICcontrolFailure_t * ric_failure = &(_unsuccessMsg->value.choice.RICcontrolFailure);
	for(int i = 0; i < RIC_CONTROL_FAILURE_IES_COUNT; i++){
		ASN_SEQUENCE_ADD(&(ric_failure->protocolIEs), &(IE_array[i]));
	}

};

E2APControlFailure::E2APControlFailure(unsigned char *buf, size_t *size){
	  _e2ap_pdu_obj = 0;
	  _unsuccessMsg = 0;
	  IE_array = 0;

	  _cntrlIEs = std::make_unique<ControlFailureIEs>();
	   bool status =  this->decode(buf, size);
	   if(!status)
		   throw "E2AP Control Failure Decode Failed: "+this->get_error();
}


// Clear assigned protocolIE list from RIC control_request IE container
E2APControlFailure::~E2APControlFailure(void){

	mdclog_write(MDCLOG_DEBUG, "Freeing E2AP Control Response object memory");

	RICcontrolFailure_t * ric_failure = &(_unsuccessMsg->value.choice.RICcontrolFailure);
	for(int i  = 0; i < ric_failure->protocolIEs.list.size; i++){
		ric_failure->protocolIEs.list.array[i] = 0;
	}
	if (ric_failure->protocolIEs.list.size > 0){
		free(ric_failure->protocolIEs.list.array);
		ric_failure->protocolIEs.list.array = 0;
		ric_failure->protocolIEs.list.count = 0;
	}


	free(IE_array);
	free(_unsuccessMsg);

	_e2ap_pdu_obj->choice.unsuccessfulOutcome = 0;
	_e2ap_pdu_obj->present = E2AP_PDU_PR_unsuccessfulOutcome;

	ASN_STRUCT_FREE(asn_DEF_E2AP_PDU, _e2ap_pdu_obj);
	mdclog_write(MDCLOG_DEBUG, "Freed E2AP Control Response object memory");
}


bool E2APControlFailure::encode(unsigned char *buf, size_t *size){

	bool res;
	res = setfields(_unsuccessMsg);
	if (!res){
		return false;
	}
	_e2ap_pdu_obj->choice.unsuccessfulOutcome = _unsuccessMsg;
	_e2ap_pdu_obj->present = E2AP_PDU_PR_unsuccessfulOutcome ;

	int ret_constr = asn_check_constraints(&asn_DEF_E2AP_PDU, (void *) _e2ap_pdu_obj, _errbuf, &_errbuf_len);
	if(ret_constr){
		_error_string.assign(_errbuf, _errbuf_len);
		_error_string = "Constraints failed for encoding control response. Reason = " + _error_string;
		return false;
	}

	asn_enc_rval_t retval = asn_encode_to_buffer(0, ATS_ALIGNED_BASIC_PER, &asn_DEF_E2AP_PDU, _e2ap_pdu_obj, buf, *size);

	if(retval.encoded == -1){
		_error_string.assign(strerror(errno));
		return false;
	}
	else {
		if(*size < retval.encoded){
			std::stringstream ss;
			ss  <<"Error encoding E2AP Control response . Reason =  encoded pdu size " << retval.encoded << " exceeds buffer size " << *size << std::endl;
			_error_string = ss.str();
			return false;
		}
	}

	*size = retval.encoded;
	xer_fprint(stdout, &asn_DEF_E2AP_PDU, _e2ap_pdu_obj);

	return true;

}

bool E2APControlFailure::setfields(UnsuccessfulOutcome_t *successMsg){
	unsigned int ie_index;

	if (successMsg == 0){
		_error_string = "Invalid reference for E2AP Control Failure in set_fields";
		return false;
	}

	RICcontrolFailure_t * ric_failure = &(successMsg->value.choice.RICcontrolFailure);
	ric_failure->protocolIEs.list.count = 0;

	ie_index = 0;
	RICcontrolFailure_IEs_t *ies_ricreq = &IE_array[ie_index];
	ies_ricreq->criticality = Criticality_reject;
	ies_ricreq->id = ProtocolIE_ID_id_RICrequestID;
	ies_ricreq->value.present = RICcontrolFailure_IEs__value_PR_RICrequestID;

	RICrequestID_t *ricrequest_ie = &ies_ricreq->value.choice.RICrequestID;
	ricrequest_ie->ricRequestorID = this->getIEs().get_ricRequestorID();
	ricrequest_ie->ricInstanceID = this->getIEs().get_ricInstanceID();
	ASN_SEQUENCE_ADD(&(ric_failure->protocolIEs), ies_ricreq);

	ie_index++;
	RICcontrolFailure_IEs_t *ies_ranfunc = &IE_array[ie_index];
	ies_ranfunc->criticality = Criticality_reject;
	ies_ranfunc->id = ProtocolIE_ID_id_RANfunctionID;
	ies_ranfunc->value.present = RICcontrolFailure_IEs__value_PR_RANfunctionID;

	RANfunctionID_t *ranfunction_ie = &ies_ranfunc->value.choice.RANfunctionID;
	*ranfunction_ie = this->getIEs().get_ranFunctionID();
	ASN_SEQUENCE_ADD(&(ric_failure->protocolIEs), ies_ranfunc);

	ie_index++;
	RICcontrolFailure_IEs_t *ies_ric_cause = &IE_array[ie_index];
	ies_ric_cause->criticality = Criticality_ignore;
	ies_ric_cause->id = ProtocolIE_ID_id_Cause;
	ies_ric_cause->value.present = RICcontrolFailure_IEs__value_PR_Cause;
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
		 mdclog_write(MDCLOG_INFO, "Error ! Illegal cause enum %ld",this->getIEs().get_ricCause());
		 return false;
	}
	ASN_SEQUENCE_ADD(&(ric_failure->protocolIEs), ies_ric_cause);

    if(this->getIEs().get_is_ricCallProcessId()){
    	ie_index++;
    	RICcontrolFailure_IEs_t *ies_riccallprocessid = &IE_array[ie_index];
    	ies_riccallprocessid->criticality = Criticality_reject;
    	ies_riccallprocessid->id = ProtocolIE_ID_id_RICcallProcessID;
    	ies_riccallprocessid->value.present = RICcontrolFailure_IEs__value_PR_RICcallProcessID;
    	RICcallProcessID_t *riccallprocessid_ie = &ies_riccallprocessid->value.choice.RICcallProcessID;
    	riccallprocessid_ie->buf = (uint8_t*)this->getIEs().get_ricCallProcessId();
    	riccallprocessid_ie->size = this->getIEs().get_ricCallProcessId_size();
    	ASN_SEQUENCE_ADD(&(ric_failure->protocolIEs), ies_riccallprocessid);

    }
    if(this->getIEs().get_is_ricControlOutcome()){
        	ie_index++;
        	RICcontrolFailure_IEs_t *ies_ricControlOutcome = &IE_array[ie_index];
        	ies_ricControlOutcome->criticality = Criticality_reject;
        	ies_ricControlOutcome->id = ProtocolIE_ID_id_RICcallProcessID;
        	ies_ricControlOutcome->value.present = RICcontrolFailure_IEs__value_PR_RICcontrolOutcome;
        	RICcontrolOutcome_t *ricControlOutcome_ie = &ies_ricControlOutcome->value.choice.RICcallProcessID;
        	ricControlOutcome_ie->buf = (uint8_t*)this->getIEs().get_ricControlOutcome();
        	ricControlOutcome_ie->size = this->getIEs().get_ricControlOutcome_size();
        	ASN_SEQUENCE_ADD(&(ric_failure->protocolIEs), ies_ricControlOutcome);

        }



	return true;

};


bool E2APControlFailure::decode(unsigned char *buf, size_t *size)
{
	asn_dec_rval_t dec_res  = asn_decode(0,ATS_ALIGNED_BASIC_PER, &asn_DEF_E2AP_PDU, (void**)&(_e2ap_pdu_obj), buf, *size);
	if(dec_res.code != RC_OK){
				 mdclog_write(MDCLOG_ERR, "Failed to decode: %s","RIC Control Ack Message");
				 return false;
	} else {
				 mdclog_write(MDCLOG_INFO, "Successfully decoded: %s","RIC Control Ack Message");
	}


	  _unsuccessMsg = _e2ap_pdu_obj->choice.unsuccessfulOutcome;
	  //write the decoding code.


	if (_unsuccessMsg == 0){
		_error_string = "Invalid reference for E2AP Control Failure message in get_fields";
		return false;
	}


	for(int edx = 0; edx < _unsuccessMsg->value.choice.RICcontrolFailure.protocolIEs.list.count; edx++) {
		RICcontrolFailure_IEs_t *memb_ptr = _unsuccessMsg->value.choice.RICcontrolFailure.protocolIEs.list.array[edx];

		switch(memb_ptr->id)
		{

		case (ProtocolIE_ID_id_RICcallProcessID):
  		_cntrlIEs->set_ricCallProcessID(memb_ptr->value.choice.RICcallProcessID.buf, memb_ptr->value.choice.RICcallProcessID.size);
		break;

		case (ProtocolIE_ID_id_RICrequestID):
		_cntrlIEs->set_ricRequestorID(memb_ptr->value.choice.RICrequestID.ricRequestorID);
		_cntrlIEs->set_ricInstanceID(memb_ptr->value.choice.RICrequestID.ricInstanceID);
		break;

		case (ProtocolIE_ID_id_RANfunctionID):
		_cntrlIEs->set_ranFunctionID(memb_ptr->value.choice.RANfunctionID);
		break;

		case (ProtocolIE_ID_id_Cause):

		_cntrlIEs->set_ricCause(memb_ptr->value.choice.Cause.present);
				switch(memb_ptr->value.choice.Cause.present){
				case  Cause_PR_ricService :
					_cntrlIEs->set_ricSubCause(memb_ptr->value.choice.Cause.choice.ricService);
					break;

				case Cause_PR_transport :
					_cntrlIEs->set_ricSubCause(memb_ptr->value.choice.Cause.choice.transport);
					break;

				case  Cause_PR_protocol :
					_cntrlIEs->set_ricSubCause(memb_ptr->value.choice.Cause.choice.protocol);
					break;

				case Cause_PR_misc :
					_cntrlIEs->set_ricSubCause(memb_ptr->value.choice.Cause.choice.misc);
					break;

				case Cause_PR_ricRequest :
					_cntrlIEs->set_ricSubCause(memb_ptr->value.choice.Cause.choice.ricRequest);
					break;

				default:
					_cntrlIEs->set_ricSubCause(-1);
					break;
				}


		break;

		case (ProtocolIE_ID_id_RICcontrolOutcome):
		_cntrlIEs->set_ricControlOutcome(memb_ptr->value.choice.RICcontrolOutcome.buf, memb_ptr->value.choice.RICcontrolOutcome.size);
		break;

		}

	}
	//xer_fprint(stdout, &asn_DEF_E2AP_PDU, _e2ap_pdu_obj);

	return true;

}



