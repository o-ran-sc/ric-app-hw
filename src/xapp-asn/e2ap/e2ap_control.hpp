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
 * control.hpp
 *
 *  Created on: Oct 10, 2020
 *      Author: sjana
 */
/*RICcontrolRequest ::= SEQUENCE {
        protocolIEs                                     ProtocolIE-Container    {{RICcontrolRequest-IEs}},
        ...
}

RICcontrolRequest-IEs E2AP-PROTOCOL-IES ::= {
        { ID id-RICrequestID                            CRITICALITY reject      TYPE RICrequestID                                       PRESENCE mandatory      }|
        { ID id-RANfunctionID                           CRITICALITY reject      TYPE RANfunctionID                                      PRESENCE mandatory      }|
        { ID id-RICcallProcessID                        CRITICALITY reject      TYPE RICcallProcessID                           PRESENCE optional       }|
        { ID id-RICcontrolHeader                        CRITICALITY reject      TYPE RICcontrolHeader                           PRESENCE mandatory      }|
        { ID id-RICcontrolMessage                       CRITICALITY reject      TYPE RICcontrolMessage                          PRESENCE mandatory      }|
        { ID id-RICcontrolAckRequest            CRITICALITY reject      TYPE RICcontrolAckRequest                       PRESENCE optional       },
        ...
}
*/

#ifndef SRC_XAPP_ASN_E2AP_E2AP_CONTROL_HPP_
#define SRC_XAPP_ASN_E2AP_E2AP_CONTROL_HPP_
#include <iostream>
#include <errno.h>
#include <mdclog/mdclog.h>
#include <sstream>
#include <memory>
#include <E2AP-PDU.h>
#include <InitiatingMessage.h>
#include <RICcontrolRequest.h>
#include <ProtocolIE-Field.h>

#define NUM_CONTROL_REQUEST_IES 6
#define IE_SIZE ((int)128)

template <typename E2SMControlHeader, typename E2SMControlMessage>
class E2APControlMessage{

public:
  class ControlRequestIEs{
  private:
	    long int ricRequestorID , ranFunctionID, ricCallProcessID,  ricControlAckRequest;
	    unsigned char ricControlHeader[IE_SIZE];
	    size_t ricControlHeader_size=IE_SIZE;
	    unsigned char ricControlMessage[IE_SIZE];
	    size_t ricControlMessage_size=IE_SIZE;
	    unsigned char ricCallProcessId[IE_SIZE];
	    size_t ricCallProcessId_size = IE_SIZE;
	    bool is_ricCallProcessId;

  	  public:
	    ControlRequestIEs(void):ricRequestorID(1), ranFunctionID(1), ricCallProcessID(0), ricControlAckRequest(-1),is_ricCallProcessId(false){};
	    void* get_ricControlMessage(){return this->ricControlMessage; };
	    void* get_ricControlHeader(){return this->ricControlHeader; };
	    void* get_ricCallProcessId(){return this->ricCallProcessId;};

	    size_t get_ricControlMessageSize(){return this->ricControlMessage_size; };
	    size_t get_ricControlHeaderSize(){return this->ricControlHeader_size; };
	    size_t get_ricCallProcessIdSize(){return this->ricCallProcessId_size;};

	    long int get_ricRequestorID(){return this->ricRequestorID;};
	    long int get_ranFunctionID(){return this->ranFunctionID;};
	    long int get_ricControlAckRequest(){return this->ricControlAckRequest;};
	    bool get_is_ricCallProcessId(){return is_ricCallProcessId;};
	    ControlRequestIEs& set_ricControlHeader(E2SMControlHeader headerObj){
	    	bool res = headerObj.encode(&(this->ricControlHeader)[0],&this->ricControlHeader_size);
	        if(!res){
	    			mdclog_write(MDCLOG_ERR, "Failed to encode: %s","RIC Control Header");
	    			mdclog_write(MDCLOG_ERR, "Error during encode: %s",headerObj.get_error());
	    	} else {
	    			mdclog_write(MDCLOG_INFO, "Successfully encoded: %s","RIC Control Header");
	    	}

	        return *this;
	    }
	    ControlRequestIEs& set_ricControlMessage(E2SMControlMessage msgObj){
	    	bool res = msgObj.encode(&(this->ricControlMessage)[0],&this->ricControlMessage_size);
	    	if(!res){
	    		     mdclog_write(MDCLOG_ERR, "Failed to encode: %s","RIC Control Message");
	    		     mdclog_write(MDCLOG_ERR, "Error during encode: %s",msgObj.get_error());
	    		} else {
	    		     mdclog_write(MDCLOG_INFO, "Successfully encoded: %s","RIC Control Message");
	    	}
	    		return *this;
	    }

	    ControlRequestIEs& set_ricCallProcessID(unsigned char* callproc, size_t callproc_size){
	    	is_ricCallProcessId = true;
	    	memcpy(ricCallProcessId, callproc, callproc_size); ricCallProcessId_size = callproc_size;
	    	return *this;
	    }

	    ControlRequestIEs& set_ricRequestorID(long int reqID){this->ricRequestorID = reqID; return *this;}
	    ControlRequestIEs& set_ranFunctionID(long int funcID){this->ranFunctionID = funcID; return *this;}

  };
  E2APControlMessage(ControlRequestIEs &);
 ~E2APControlMessage(void);
  bool encode(unsigned char *, size_t *);
  ControlRequestIEs& getIEs(){ return *_cntrlIEs.get();};
  std::string get_error(void) const {return _error_string ; };
private:

  E2AP_PDU_t * _e2ap_pdu_obj;
  InitiatingMessage_t *_initMsg;
  RICcontrolRequest_IEs_t *IE_array;
  std::string _error_string;
  bool setfields(InitiatingMessage_t *);
  std::unique_ptr<ControlRequestIEs> _cntrlIEs;
  char _errbuf[128];
  size_t _errbuf_len = 128;
};

template <typename T1, typename T2>
E2APControlMessage<T1,T2>::E2APControlMessage(ControlRequestIEs &controlObj){

  _cntrlIEs = std::make_unique<ControlRequestIEs>();
  *_cntrlIEs = controlObj;

  _e2ap_pdu_obj = 0;
  _e2ap_pdu_obj = (E2AP_PDU_t * )calloc(1, sizeof(E2AP_PDU_t));
  assert(_e2ap_pdu_obj != 0);

  _initMsg = 0;
  _initMsg = (InitiatingMessage_t * )calloc(1, sizeof(InitiatingMessage_t));
  assert(_initMsg != 0);

  IE_array = 0;
  IE_array = (RICcontrolRequest_IEs_t *)calloc(NUM_CONTROL_REQUEST_IES, sizeof(RICcontrolRequest_IEs_t));
  assert(IE_array != 0);

  _e2ap_pdu_obj->present = E2AP_PDU_PR_initiatingMessage;
  _e2ap_pdu_obj->choice.initiatingMessage = _initMsg;


};


// Clear assigned protocolIE list from RIC control_request IE container
template <typename T1, typename T2>
E2APControlMessage<T1,T2>::~E2APControlMessage(void){

  mdclog_write(MDCLOG_DEBUG, "Freeing E2AP Control Request object memory");

  RICcontrolRequest_t *ricControl_Request  = &(_initMsg->value.choice.RICcontrolRequest);
  for(int i = 0; i < ricControl_Request->protocolIEs.list.size; i++){
    ricControl_Request->protocolIEs.list.array[i] = 0;
  }

  if (ricControl_Request->protocolIEs.list.size > 0){
    free(ricControl_Request->protocolIEs.list.array);
    ricControl_Request->protocolIEs.list.size = 0;
    ricControl_Request->protocolIEs.list.count = 0;
  }

  free(IE_array);
  free(_initMsg);
  _e2ap_pdu_obj->choice.initiatingMessage = 0;

  ASN_STRUCT_FREE(asn_DEF_E2AP_PDU, _e2ap_pdu_obj);
  mdclog_write(MDCLOG_DEBUG, "Freed E2AP Control Request object memory");

}

template <typename T1, typename T2>
bool E2APControlMessage<T1,T2>::encode(unsigned char *buf, size_t *size){

  _initMsg->procedureCode = ProcedureCode_id_RICcontrol;
  _initMsg->criticality = Criticality_ignore;
  _initMsg->value.present = InitiatingMessage__value_PR_RICcontrolRequest;

  bool res;

  res = setfields(_initMsg);
  if (!res){
    return false;
  }

  int ret_constr = asn_check_constraints(&asn_DEF_E2AP_PDU, (void *) _e2ap_pdu_obj, _errbuf, &_errbuf_len);
  if(ret_constr){
    _error_string.assign(_errbuf, _errbuf_len);
    _error_string = "Constraints failed for encoding control . Reason = " + _error_string;
    return false;
  }

  xer_fprint(stdout, &asn_DEF_E2AP_PDU, _e2ap_pdu_obj);

  asn_enc_rval_t retval = asn_encode_to_buffer(0, ATS_ALIGNED_BASIC_PER, &asn_DEF_E2AP_PDU, _e2ap_pdu_obj, buf, *size);

  if(retval.encoded == -1){
    _error_string.assign(strerror(errno));
    return false;
  }
  else {
    if(*size < retval.encoded){
      std::stringstream ss;
      ss  <<"Error encoding event trigger definition. Reason =  encoded pdu size " << retval.encoded << " exceeds buffer size " << *size << std::endl;
      _error_string = ss.str();
      return false;
    }
  }

  *size = retval.encoded;
  return true;

}

template <typename T1, typename T2>
bool E2APControlMessage<T1,T2>::setfields(InitiatingMessage_t *_initMsg){
  unsigned int ie_index;

  if (_initMsg == 0){
    _error_string = "Invalid reference for E2AP Control_Request message in set_fields";
    return false;
  }

  RICcontrolRequest_t * E2APControlMessage = &(_initMsg->value.choice.RICcontrolRequest);
  E2APControlMessage->protocolIEs.list.count = 0; // reset

  // for(i = 0; i < NUM_CONTROL_REQUEST_IES;i++){
  //   memset(&(IE_array[i]), 0, sizeof(RICcontrolRequest_IEs_t));
  // }

  // Mandatory IE
  ie_index = 0;
  RICcontrolRequest_IEs_t *ies_ricreq = &IE_array[ie_index];
  ies_ricreq->criticality = Criticality_reject;
  ies_ricreq->id = ProtocolIE_ID_id_RICrequestID;
  ies_ricreq->value.present = RICcontrolRequest_IEs__value_PR_RICrequestID;
  RICrequestID_t *ricrequest_ie = &ies_ricreq->value.choice.RICrequestID;
  ricrequest_ie->ricRequestorID = this->getIEs().get_ricRequestorID();
  //ricrequest_ie->ricRequestSequenceNumber = dinput.req_seq_no;
  ASN_SEQUENCE_ADD(&(E2APControlMessage->protocolIEs), &(IE_array[ie_index]));

  // Mandatory IE
  ie_index = 1;
  RICcontrolRequest_IEs_t *ies_ranfunc = &IE_array[ie_index];
  ies_ranfunc->criticality = Criticality_reject;
  ies_ranfunc->id = ProtocolIE_ID_id_RANfunctionID;
  ies_ranfunc->value.present = RICcontrolRequest_IEs__value_PR_RANfunctionID;
  RANfunctionID_t *ranfunction_ie = &ies_ranfunc->value.choice.RANfunctionID;
  *ranfunction_ie = this->getIEs().get_ranFunctionID();
  ASN_SEQUENCE_ADD(&(E2APControlMessage->protocolIEs), &(IE_array[ie_index]));


  // Mandatory IE
  ie_index = 2;
  RICcontrolRequest_IEs_t *ies_richead = &IE_array[ie_index];
  ies_richead->criticality = Criticality_reject;
  ies_richead->id = ProtocolIE_ID_id_RICcontrolHeader;
  ies_richead->value.present = RICcontrolRequest_IEs__value_PR_RICcontrolHeader;
  RICcontrolHeader_t *richeader_ie = &ies_richead->value.choice.RICcontrolHeader;
  richeader_ie->buf = (uint8_t*)this->getIEs().get_ricControlHeader();
  richeader_ie->size = this->getIEs().get_ricControlHeaderSize();
  ASN_SEQUENCE_ADD(&(E2APControlMessage->protocolIEs), &(IE_array[ie_index]));

  // Mandatory IE
  ie_index = 3;
  RICcontrolRequest_IEs_t *ies_indmsg = &IE_array[ie_index];
  ies_indmsg->criticality = Criticality_reject;
  ies_indmsg->id = ProtocolIE_ID_id_RICcontrolMessage;
  ies_indmsg->value.present = RICcontrolRequest_IEs__value_PR_RICcontrolMessage;
  RICcontrolMessage_t *ricmsg_ie = &ies_indmsg->value.choice.RICcontrolMessage;
  ricmsg_ie->buf = (uint8_t*)this->getIEs().get_ricControlMessage();
  ricmsg_ie->size = this->getIEs().get_ricControlMessageSize();
  ASN_SEQUENCE_ADD(&(E2APControlMessage->protocolIEs), &(IE_array[ie_index]));

  // Optional IE
  ie_index = 4;
  if (this->getIEs().get_ricControlAckRequest()>= 0){
    RICcontrolRequest_IEs_t *ies_indtyp = &IE_array[ie_index];
    ies_indtyp->criticality = Criticality_reject;
    ies_indtyp->id = ProtocolIE_ID_id_RICcontrolAckRequest;
    ies_indtyp->value.present = RICcontrolRequest_IEs__value_PR_RICcontrolAckRequest;
    RICcontrolAckRequest_t *ricackreq_ie = &ies_indtyp->value.choice.RICcontrolAckRequest;
    *ricackreq_ie = this->getIEs().get_ricControlAckRequest();
    ASN_SEQUENCE_ADD(&(E2APControlMessage->protocolIEs), &(IE_array[ie_index]));
  }

  // Optional IE
  ie_index = 5;
  if(this->getIEs().get_is_ricCallProcessId()){
    RICcontrolRequest_IEs_t *ies_callprocid = &IE_array[ie_index];
    ies_callprocid->criticality = Criticality_reject;
    ies_callprocid->id = ProtocolIE_ID_id_RICcallProcessID;
    ies_callprocid->value.present = RICcontrolRequest_IEs__value_PR_RICcallProcessID;
    RICcallProcessID_t *riccallprocessid_ie = &ies_callprocid->value.choice.RICcallProcessID;
    riccallprocessid_ie->buf = (uint8_t*)this->getIEs().get_ricCallProcessId();
    riccallprocessid_ie->size = this->getIEs().get_ricCallProcessIdSize();
    ASN_SEQUENCE_ADD(&(E2APControlMessage->protocolIEs), &(IE_array[ie_index]));

  }
  return true;

};

#endif /* SRC_XAPP_ASN_E2AP_E2AP_CONTROL_HPP_ */
