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
 * e2ap_subsdel_request.cc
 *
 * Author: SJana, Ashwin Sridharan
 */


#include "e2ap_subsdel_request.hpp"

E2APSubscriptionDeleteRequest::E2APSubscriptionDeleteRequest(SubscriptionDeleteRequestIEs& ieObj){



  _requestIEs = std::make_unique<SubscriptionDeleteRequestIEs>();
  *_requestIEs = ieObj;

  e2ap_pdu_obj = (E2AP_PDU_t * )calloc(1, sizeof(E2AP_PDU_t));
  assert(e2ap_pdu_obj != 0);

  initMsg = (InitiatingMessage_t * )calloc(1, sizeof(InitiatingMessage_t));
  assert(initMsg != 0);

  IE_array = (RICsubscriptionDeleteRequest_IEs_t *)calloc(RIC_SUBDEL_REQUEST_IES_COUNT, sizeof(RICsubscriptionDeleteRequest_IEs_t));
  assert(IE_array != 0);

  RICsubscriptionDeleteRequest_t * subDelrequest = &(initMsg->value.choice.RICsubscriptionDeleteRequest);
  for(int i = 0; i < RIC_SUBDEL_REQUEST_IES_COUNT; i++){
    ASN_SEQUENCE_ADD(&subDelrequest->protocolIEs, &(IE_array[i]));
  }

};



// Clear assigned protocolIE list from RIC indication IE container
E2APSubscriptionDeleteRequest::~E2APSubscriptionDeleteRequest(void){

  mdclog_write(MDCLOG_DEBUG, "Freeing subscription delete request object memory");
  RICsubscriptionDeleteRequest_t * E2APSubscriptionDeleteRequest = &(initMsg->value.choice.RICsubscriptionDeleteRequest);

 for(int i = 0; i < E2APSubscriptionDeleteRequest->protocolIEs.list.size; i++){
    E2APSubscriptionDeleteRequest->protocolIEs.list.array[i] = 0;
  }

  if (E2APSubscriptionDeleteRequest->protocolIEs.list.size > 0){
    free(E2APSubscriptionDeleteRequest->protocolIEs.list.array);
    E2APSubscriptionDeleteRequest->protocolIEs.list.count = 0;
    E2APSubscriptionDeleteRequest->protocolIEs.list.size = 0;
    E2APSubscriptionDeleteRequest->protocolIEs.list.array = 0;
  }

  free(IE_array);
  free(initMsg);
  e2ap_pdu_obj->choice.initiatingMessage = 0;

  ASN_STRUCT_FREE(asn_DEF_E2AP_PDU, e2ap_pdu_obj);
  mdclog_write(MDCLOG_DEBUG, "Freed subscription delete request object memory");


};


bool E2APSubscriptionDeleteRequest::encode(unsigned char *buf, size_t *size){

  e2ap_pdu_obj->choice.initiatingMessage = initMsg;
  e2ap_pdu_obj->present = E2AP_PDU_PR_initiatingMessage;
  bool result = setfields( initMsg);
  if(!result)
	  return result;

  initMsg->procedureCode = ProcedureCode_id_RICsubscriptionDelete;
  initMsg->criticality = Criticality_reject;
  initMsg->value.present = InitiatingMessage__value_PR_RICsubscriptionDeleteRequest;

  int ret_constr = asn_check_constraints(&asn_DEF_E2AP_PDU, (void *) e2ap_pdu_obj, _errbuf, &_errbuf_len);
  if(ret_constr){
    _error_string.assign(_errbuf, _errbuf_len);
    _error_string = "Constraints failed for encoding subscription delete request. Reason = " + _error_string;
    return false;
  }

  asn_enc_rval_t res = asn_encode_to_buffer(0, ATS_ALIGNED_BASIC_PER, &asn_DEF_E2AP_PDU, e2ap_pdu_obj, buf, *size);

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
  xer_fprint(stdout, &asn_DEF_E2AP_PDU, e2ap_pdu_obj);

  return true;

}


bool  E2APSubscriptionDeleteRequest::setfields( InitiatingMessage_t *initMsg){
  unsigned int ie_index;

  ie_index = 0;
  RICsubscriptionDeleteRequest_IEs_t *ies_ricreq = &IE_array[ie_index];
  ies_ricreq->criticality = Criticality_reject;
  ies_ricreq->id = ProtocolIE_ID_id_RICrequestID;
  ies_ricreq->value.present = RICsubscriptionDeleteRequest_IEs__value_PR_RICrequestID;
  RICrequestID_t *ricrequest_ie = &ies_ricreq->value.choice.RICrequestID;
  ricrequest_ie->ricRequestorID = this->getIEs().get_ricRequestorID();
  ricrequest_ie->ricInstanceID = this->getIEs().get_ricInstanceID();


  ie_index = 1;
  RICsubscriptionDeleteRequest_IEs_t *ies_ranfunc = &IE_array[ie_index];
  ies_ranfunc->criticality = Criticality_reject;
  ies_ranfunc->id = ProtocolIE_ID_id_RANfunctionID;
  ies_ranfunc->value.present = RICsubscriptionDeleteRequest_IEs__value_PR_RANfunctionID;
  RANfunctionID_t *ranfunction_ie = &ies_ranfunc->value.choice.RANfunctionID;
  *ranfunction_ie = this->getIEs().get_ranFunctionID();


  return true;
};

