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
 * e2ap_control_ack.cc
 *
 *      Author: SJana, Ashwin Sridharan
 */

#include "e2ap_control_ack.hpp"


E2APControlAcknowledge::E2APControlAcknowledge(ControlAcknowledgeIEs &cntrlObj){
	_cntrlIEs = std::make_unique<ControlAcknowledgeIEs>();
	*_cntrlIEs = cntrlObj;

	_e2ap_pdu_obj = 0;
	_e2ap_pdu_obj = (E2AP_PDU_t * )calloc(1, sizeof(E2AP_PDU_t));
	assert(_e2ap_pdu_obj != 0);

	_successMsg = 0;
	_successMsg = (SuccessfulOutcome_t * )calloc(1, sizeof(SuccessfulOutcome_t));
	assert(_successMsg != 0);

	_successMsg->procedureCode = ProcedureCode_id_RICcontrol;
	_successMsg->criticality = Criticality_reject;
	_successMsg->value.present = SuccessfulOutcome__value_PR_RICcontrolAcknowledge;


	IE_array = 0;
	IE_array = (RICcontrolAcknowledge_IEs_t *)calloc(RIC_CONTROL_ACK_IES_COUNT, sizeof(RICcontrolAcknowledge_IEs_t));
	assert(IE_array != 0);

	RICcontrolAcknowledge_t * ric_acknowledge = &(_successMsg->value.choice.RICcontrolAcknowledge);
	for(int i = 0; i < RIC_CONTROL_ACK_IES_COUNT; i++){
		ASN_SEQUENCE_ADD(&(ric_acknowledge->protocolIEs), &(IE_array[i]));
	}

};

E2APControlAcknowledge::E2APControlAcknowledge(unsigned char *buf, size_t *size){
	  _e2ap_pdu_obj = 0;
	  _successMsg = 0;
	  IE_array = 0;

	  _cntrlIEs = std::make_unique<ControlAcknowledgeIEs>();
	   bool status =  this->decode(buf, size);
	   if(!status)
		   throw "E2AP Control Acknowledge Decode Failed: "+this->get_error();
}


// Clear assigned protocolIE list from RIC control_request IE container
E2APControlAcknowledge::~E2APControlAcknowledge(void){

	mdclog_write(MDCLOG_DEBUG, "Freeing E2AP Control Response object memory");

	RICcontrolAcknowledge_t * ric_acknowledge = &(_successMsg->value.choice.RICcontrolAcknowledge);
	for(int i  = 0; i < ric_acknowledge->protocolIEs.list.size; i++){
		ric_acknowledge->protocolIEs.list.array[i] = 0;
	}
	if (ric_acknowledge->protocolIEs.list.size > 0){
		free(ric_acknowledge->protocolIEs.list.array);
		ric_acknowledge->protocolIEs.list.array = 0;
		ric_acknowledge->protocolIEs.list.count = 0;
	}


	free(IE_array);
	free(_successMsg);

	_e2ap_pdu_obj->choice.successfulOutcome = 0;
	_e2ap_pdu_obj->present = E2AP_PDU_PR_successfulOutcome;

	ASN_STRUCT_FREE(asn_DEF_E2AP_PDU, _e2ap_pdu_obj);
	mdclog_write(MDCLOG_DEBUG, "Freed E2AP Control Response object memory");
}


bool E2APControlAcknowledge::encode(unsigned char *buf, size_t *size){

	bool res;
	res = setfields(_successMsg);
	if (!res){
		return false;
	}
	_e2ap_pdu_obj->choice.successfulOutcome = _successMsg;
	_e2ap_pdu_obj->present = E2AP_PDU_PR_successfulOutcome ;

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

bool E2APControlAcknowledge::setfields(SuccessfulOutcome_t *successMsg){
	unsigned int ie_index;

	if (successMsg == 0){
		_error_string = "Invalid reference for E2AP Control Acknowledge in set_fields";
		return false;
	}

	RICcontrolAcknowledge_t * ric_acknowledge = &(successMsg->value.choice.RICcontrolAcknowledge);
	ric_acknowledge->protocolIEs.list.count = 0;

	ie_index = 0;
	RICcontrolAcknowledge_IEs_t *ies_ricreq = &IE_array[ie_index];
	ies_ricreq->criticality = Criticality_reject;
	ies_ricreq->id = ProtocolIE_ID_id_RICrequestID;
	ies_ricreq->value.present = RICcontrolAcknowledge_IEs__value_PR_RICrequestID;

	RICrequestID_t *ricrequest_ie = &ies_ricreq->value.choice.RICrequestID;
	ricrequest_ie->ricRequestorID = this->getIEs().get_ricRequestorID();
	ricrequest_ie->ricInstanceID = this->getIEs().get_ricInstanceID();
	ASN_SEQUENCE_ADD(&(ric_acknowledge->protocolIEs), ies_ricreq);

	ie_index = 1;
	RICcontrolAcknowledge_IEs_t *ies_ranfunc = &IE_array[ie_index];
	ies_ranfunc->criticality = Criticality_reject;
	ies_ranfunc->id = ProtocolIE_ID_id_RANfunctionID;
	ies_ranfunc->value.present = RICcontrolAcknowledge_IEs__value_PR_RANfunctionID;

	RANfunctionID_t *ranfunction_ie = &ies_ranfunc->value.choice.RANfunctionID;
	*ranfunction_ie = this->getIEs().get_ranFunctionID();
	ASN_SEQUENCE_ADD(&(ric_acknowledge->protocolIEs), ies_ranfunc);

	ie_index = 2;
	RICcontrolAcknowledge_IEs_t *ies_cntrlstatus = &IE_array[ie_index];
	ies_cntrlstatus->criticality = Criticality_reject;
	ies_cntrlstatus->id = ProtocolIE_ID_id_RICcontrolStatus;
	ies_cntrlstatus->value.present = RICcontrolAcknowledge_IEs__value_PR_RICcontrolStatus;
	RICcontrolStatus_t *ricstatus_ie = &ies_cntrlstatus->value.choice.RICcontrolStatus;
	*ricstatus_ie = this->getIEs().get_ricControlStatus();

	ASN_SEQUENCE_ADD(&(ric_acknowledge->protocolIEs), ies_cntrlstatus);

    if(this->getIEs().get_is_ricCallProcessId()){
    	ie_index++;
    	RICcontrolAcknowledge_IEs_t *ies_riccallprocessid = &IE_array[ie_index];
    	ies_riccallprocessid->criticality = Criticality_reject;
    	ies_riccallprocessid->id = ProtocolIE_ID_id_RICcallProcessID;
    	ies_riccallprocessid->value.present = RICcontrolAcknowledge_IEs__value_PR_RICcallProcessID;
    	RICcallProcessID_t *riccallprocessid_ie = &ies_riccallprocessid->value.choice.RICcallProcessID;
    	riccallprocessid_ie->buf = (uint8_t*)this->getIEs().get_ricCallProcessId();
    	riccallprocessid_ie->size = this->getIEs().get_ricCallProcessId_size();
    	ASN_SEQUENCE_ADD(&(ric_acknowledge->protocolIEs), ies_riccallprocessid);

    }
    if(this->getIEs().get_is_ricControlOutcome()){
        	ie_index++;
        	RICcontrolAcknowledge_IEs_t *ies_ricControlOutcome = &IE_array[ie_index];
        	ies_ricControlOutcome->criticality = Criticality_reject;
        	ies_ricControlOutcome->id = ProtocolIE_ID_id_RICcallProcessID;
        	ies_ricControlOutcome->value.present = RICcontrolAcknowledge_IEs__value_PR_RICcontrolOutcome;
        	RICcontrolOutcome_t *ricControlOutcome_ie = &ies_ricControlOutcome->value.choice.RICcallProcessID;
        	ricControlOutcome_ie->buf = (uint8_t*)this->getIEs().get_ricControlOutcome();
        	ricControlOutcome_ie->size = this->getIEs().get_ricControlOutcome_size();
        	ASN_SEQUENCE_ADD(&(ric_acknowledge->protocolIEs), ies_ricControlOutcome);

        }



	return true;

};


bool E2APControlAcknowledge::decode(unsigned char *buf, size_t *size)
{
	asn_dec_rval_t dec_res  = asn_decode(0,ATS_ALIGNED_BASIC_PER, &asn_DEF_E2AP_PDU, (void**)&(_e2ap_pdu_obj), buf, *size);
	if(dec_res.code != RC_OK){
				 mdclog_write(MDCLOG_ERR, "Failed to decode: %s","RIC Control Ack Message");
				 return false;
	} else {
				 mdclog_write(MDCLOG_INFO, "Successfully decoded: %s","RIC Control Ack Message");
	}

	  _successMsg = _e2ap_pdu_obj->choice.successfulOutcome;
	  //write the decoding code.


	if (_successMsg == 0){
		_error_string = "Invalid reference for E2AP Control Acknowledge message in get_fields";
		return false;
	}


	for(int edx = 0; edx < _successMsg->value.choice.RICcontrolAcknowledge.protocolIEs.list.count; edx++) {
		RICcontrolAcknowledge_IEs_t *memb_ptr = _successMsg->value.choice.RICcontrolAcknowledge.protocolIEs.list.array[edx];

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

		case (ProtocolIE_ID_id_RICcontrolStatus):
  			_cntrlIEs->set_ricControlStatus(memb_ptr->value.choice.RICcontrolStatus);
		break;

		case (ProtocolIE_ID_id_RICcontrolOutcome):
		_cntrlIEs->set_ricControlOutcome(memb_ptr->value.choice.RICcontrolOutcome.buf, memb_ptr->value.choice.RICcontrolOutcome.size);
		break;

		}

	}
//	  xer_fprint(stdout, &asn_DEF_E2AP_PDU, _e2ap_pdu_obj);

	return true;

}


