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
 * e2ap_subsdel_response.cc
 *
 * Author: SJana, Ashwin Sridharan
 */


#include "e2ap_subsdel_response.hpp"

E2APSubscriptionDeleteResponse::E2APSubscriptionDeleteResponse(SubscriptionDeleteResponseIEs& ieObj){



  _responseIEs = std::make_unique<SubscriptionDeleteResponseIEs>();
  *_responseIEs = ieObj;

  _e2ap_pdu_obj = (E2AP_PDU_t * )calloc(1, sizeof(E2AP_PDU_t));
  assert(_e2ap_pdu_obj != 0);

  _successMsg = (SuccessfulOutcome_t * )calloc(1, sizeof(SuccessfulOutcome_t));
  assert(_successMsg != 0);

  IE_array = (RICsubscriptionDeleteResponse_IEs_t *)calloc(RIC_SUBDEL_RESPONSE_IES_COUNT, sizeof(RICsubscriptionDeleteResponse_IEs_t));
  assert(IE_array != 0);

  RICsubscriptionDeleteResponse_t * subDelresponse = &(_successMsg->value.choice.RICsubscriptionDeleteResponse);
  for(int i = 0; i < RIC_SUBDEL_RESPONSE_IES_COUNT; i++){
    ASN_SEQUENCE_ADD(&subDelresponse->protocolIEs, &(IE_array[i]));
  }

};

E2APSubscriptionDeleteResponse::E2APSubscriptionDeleteResponse(unsigned char *buf, size_t *size){
	  _e2ap_pdu_obj = 0;
	  _successMsg = 0;
	  IE_array = 0;

	  _responseIEs = std::make_unique<SubscriptionDeleteResponseIEs>();
	   bool status =  this->decode(buf, size);
	   if(!status)
		   throw "E2AP Subscription Delete Response Decode Failed: "+this->get_error();
}

bool E2APSubscriptionDeleteResponse::decode(unsigned char *buf, size_t *size)
{

	asn_dec_rval_t dec_res  = asn_decode(0,ATS_ALIGNED_BASIC_PER, &asn_DEF_E2AP_PDU, (void**)&(_e2ap_pdu_obj), buf, *size);
	if(dec_res.code != RC_OK){
				 mdclog_write(MDCLOG_ERR, "Failed to decode: %s","RIC Subscription Delete Response");
				 return false;
	} else {
				 mdclog_write(MDCLOG_INFO, "Successfully decoded: %s","RIC Subscription Delete Response");
	}

    _successMsg = _e2ap_pdu_obj->choice.successfulOutcome;
    //write the decoding code.

    if (_successMsg == 0){
    	_error_string = "Invalid reference to success message in get fields subscription delete response";
    	return false;
    }


	for(int edx = 0; edx < _successMsg->value.choice.RICsubscriptionDeleteResponse.protocolIEs.list.count; edx++) {
		RICsubscriptionDeleteResponse_IEs_t *memb_ptr = _successMsg->value.choice.RICsubscriptionDeleteResponse.protocolIEs.list.array[edx];

  switch(memb_ptr->id)
    {
    case (ProtocolIE_ID_id_RICrequestID):
		_responseIEs->set_ricRequestorID(memb_ptr->value.choice.RICrequestID.ricRequestorID);
    	_responseIEs->set_ricInstanceID(memb_ptr->value.choice.RICrequestID.ricInstanceID);

	break;

    case (ProtocolIE_ID_id_RANfunctionID):
		_responseIEs->set_ranFunctionID(memb_ptr->value.choice.RANfunctionID);

	break;
    }

}
//    xer_fprint(stdout, &asn_DEF_E2AP_PDU, _e2ap_pdu_obj);

return true;
}

// Clear assigned protocolIE list from RIC indication IE container
E2APSubscriptionDeleteResponse::~E2APSubscriptionDeleteResponse(void){

  mdclog_write(MDCLOG_DEBUG, "Freeing subscription delete request object memory");
  RICsubscriptionDeleteResponse_t * E2APSubscriptionDeleteResponse = &(_successMsg->value.choice.RICsubscriptionDeleteResponse);

 for(int i = 0; i < E2APSubscriptionDeleteResponse->protocolIEs.list.size; i++){
    E2APSubscriptionDeleteResponse->protocolIEs.list.array[i] = 0;
  }

  if (E2APSubscriptionDeleteResponse->protocolIEs.list.size > 0){
    free(E2APSubscriptionDeleteResponse->protocolIEs.list.array);
    E2APSubscriptionDeleteResponse->protocolIEs.list.count = 0;
    E2APSubscriptionDeleteResponse->protocolIEs.list.size = 0;
    E2APSubscriptionDeleteResponse->protocolIEs.list.array = 0;
  }

  free(IE_array);
  free(_successMsg);
  _e2ap_pdu_obj->choice.successfulOutcome = 0;

  ASN_STRUCT_FREE(asn_DEF_E2AP_PDU, _e2ap_pdu_obj);
  mdclog_write(MDCLOG_DEBUG, "Freed subscription delete request object memory");


};


bool E2APSubscriptionDeleteResponse::encode(unsigned char *buf, size_t *size){

  _e2ap_pdu_obj->choice.successfulOutcome = _successMsg;
  _e2ap_pdu_obj->present = E2AP_PDU_PR_successfulOutcome;
  bool eres = setfields( _successMsg);
  if(!eres)
	  return eres;

  _successMsg->procedureCode = ProcedureCode_id_RICsubscriptionDelete;
  _successMsg->criticality = Criticality_reject;
  _successMsg->value.present = SuccessfulOutcome__value_PR_RICsubscriptionDeleteResponse;

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


bool  E2APSubscriptionDeleteResponse::setfields( SuccessfulOutcome_t *_successMsg){
  unsigned int ie_index;

  ie_index = 0;
  RICsubscriptionDeleteResponse_IEs_t *ies_ricresp = &IE_array[ie_index];
  ies_ricresp->criticality = Criticality_reject;
  ies_ricresp->id = ProtocolIE_ID_id_RICrequestID;
  ies_ricresp->value.present = RICsubscriptionDeleteResponse_IEs__value_PR_RICrequestID;
  RICrequestID_t *ricrequest_ie = &ies_ricresp->value.choice.RICrequestID;
  ricrequest_ie->ricRequestorID = this->getIEs().get_ricRequestorID();
  ricrequest_ie->ricInstanceID = this->getIEs().get_ricInstanceID();


  ie_index = 1;
  RICsubscriptionDeleteResponse_IEs_t *ies_ranfunc = &IE_array[ie_index];
  ies_ranfunc->criticality = Criticality_reject;
  ies_ranfunc->id = ProtocolIE_ID_id_RANfunctionID;
  ies_ranfunc->value.present = RICsubscriptionDeleteResponse_IEs__value_PR_RANfunctionID;
  RANfunctionID_t *ranfunction_ie = &ies_ranfunc->value.choice.RANfunctionID;
  *ranfunction_ie = this->getIEs().get_ranFunctionID();


  return true;
};





