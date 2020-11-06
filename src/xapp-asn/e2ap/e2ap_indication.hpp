
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
 * e2ap_indication.hpp
 *
 *  Created on: Sep 18, 2020
 *      Author: Shraboni Jana
 */
/*-- **************************************************************
--
-- RIC Indication Elementary Procedure
--
-- **************************************************************
-- **************************************************************
--
-- RIC INDICATION
--
-- **************************************************************
RICindication ::= SEQUENCE {
        protocolIEs                                     ProtocolIE-Container    {{RICindication-IEs}},
        ...
}

RICindication-IEs E2AP-PROTOCOL-IES ::= {
        { ID id-RICrequestID                            CRITICALITY reject      TYPE RICrequestID                                       PRESENCE mandatory      }|
        { ID id-RANfunctionID                           CRITICALITY reject      TYPE RANfunctionID                                      PRESENCE mandatory      }|
        { ID id-RICactionID                                     CRITICALITY reject      TYPE RICactionID                                        PRESENCE mandatory      }|
        { ID id-RICindicationSN                         CRITICALITY reject      TYPE RICindicationSN                            PRESENCE optional       }|
        { ID id-RICindicationType                       CRITICALITY reject      TYPE RICindicationType                          PRESENCE mandatory      }|
        { ID id-RICindicationHeader                     CRITICALITY reject      TYPE RICindicationHeader                        PRESENCE mandatory      }|
        { ID id-RICindicationMessage            CRITICALITY reject      TYPE RICindicationMessage                       PRESENCE mandatory      }|
        { ID id-RICcallProcessID                        CRITICALITY reject      TYPE RICcallProcessID                           PRESENCE optional       },
        ...
}*/


#ifndef SRC_XAPP_ASN_E2AP_E2AP_INDICATION_HPP_
#define SRC_XAPP_ASN_E2AP_E2AP_INDICATION_HPP_

#include <string>
#include <E2AP-PDU.h>
#include <InitiatingMessage.h>
#include <RICindication.h>
#include <ProtocolIE-Field.h>
#include <iostream>
#include <errno.h>
#include <mdclog/mdclog.h>
#include <sstream>
#include <memory>

#include "e2ap_consts.hpp"

template <typename E2SMIndicationHeader, typename E2SMIndicationMessage>
class E2APIndication{

public:

	class IndicationIEs{
		private:
			long int ricRequestorID, ranFunctionID, ricActionID, ricIndicationSN, ricIndicationType;
			size_t ricIndicationHeader_size = IE_SIZE;
			unsigned char ricIndicationHeader[IE_SIZE];

			size_t ricIndicationMessage_size = IE_SIZE;
			unsigned char ricIndicationMessage[IE_SIZE];

			unsigned char ricCallProcessId[IE_SIZE];
			size_t ricCallProcessId_size = IE_SIZE;

			bool is_callProcessID, is_ricIndicationSN;

		public:
			IndicationIEs(void) : ricRequestorID(0), ranFunctionID(0), ricActionID(0),ricIndicationSN(0), ricIndicationType(0),is_callProcessID(false),is_ricIndicationSN(false){};
			void* get_ricIndicationMessage(){return this->ricIndicationMessage; };
			void* get_ricIndicationHeader(){return this->ricIndicationHeader; };
			void* get_ricCallProcessId(){return this->ricCallProcessId;};

			size_t get_ricIndicationMessage_size(){return this->ricIndicationMessage_size; };
			size_t get_ricIndicationHeader_size(){return this->ricIndicationHeader_size; };
			size_t get_ricCallProcessId_size(){return this->ricCallProcessId_size;};

			long int get_ricRequestorID(){return this->ricRequestorID;};
			long int get_ranFunctionID(){return this->ranFunctionID;};
			long int get_ricActionID(){return this->ricActionID;};
			long int get_ricIndicationType(){return this->ricIndicationType;}
			long int get_ricIndicationSN(){return this->ricIndicationSN;};
			bool get_is_ricIndicationSN(){return this->is_ricIndicationSN;};
			bool get_is_callProcessID(){return this->is_callProcessID;};

			IndicationIEs& set_ricIndicationHeader(E2SMIndicationHeader e2smObj){
				bool res = e2smObj.encode(&(this->ricIndicationHeader)[0],&this->ricIndicationHeader_size);
				if(!res){
						mdclog_write(MDCLOG_ERR, "Failed to encode: %s","RIC Indication Header");
						mdclog_write(MDCLOG_ERR, "Error during encode: %s",e2smObj.get_error());

					} else {
						 mdclog_write(MDCLOG_INFO, "Successfully encoded: %s","RIC Indication Header");
				}
					return *this;
			}
			IndicationIEs& set_ricIndicationMessage(E2SMIndicationMessage e2smObj){
				bool res = e2smObj.encode(&(this->ricIndicationMessage)[0],&this->ricIndicationMessage_size);
				if(!res){
							mdclog_write(MDCLOG_ERR, "Failed to encode: %s","RIC Indication Message");
							mdclog_write(MDCLOG_ERR, "Error during encode: %s",e2smObj.get_error());
						} else {
   						    mdclog_write(MDCLOG_INFO, "Successfully encoded: %s","RIC Indication Message");
						}

				return *this;

			}

			IndicationIEs& set_ricIndicationHeader(unsigned char* header, size_t header_size){
							memcpy(ricIndicationHeader,header,header_size); ricIndicationHeader_size = header_size; return *this;
						}
			IndicationIEs& set_ricIndicationMessage(unsigned char* message, size_t message_size){
							memcpy(ricIndicationHeader,message,message_size); ricIndicationMessage_size = message_size; return *this;}

			IndicationIEs& set_ricCallProcessID(unsigned char* callproc, size_t callproc_size){
				is_callProcessID = true;
				memcpy(ricCallProcessId, callproc, callproc_size); ricCallProcessId_size = callproc_size;
				return *this;
			}

			IndicationIEs& set_ricRequestorID(long int reqID){this->ricRequestorID = reqID; return *this;}
			IndicationIEs& set_ranFunctionID(long int funcID){this->ranFunctionID = funcID; return *this;}
			IndicationIEs& set_ricActionID(long int actID){ this->ricActionID = actID; return *this;}
			IndicationIEs& set_ricIndicationType(long int typ){ this->ricIndicationType = typ; return *this;}
			IndicationIEs& set_ricIndicationSN(long int sn){ this->ricIndicationSN = sn; is_ricIndicationSN=true; return *this;}


	};

   E2APIndication(IndicationIEs&);
   E2APIndication(unsigned char *, size_t *);
  ~E2APIndication(void);
  IndicationIEs getIndicationIEs(){ return *_indicationIEs.get(); }

  std::string get_error(void) const {return _error_string ; };
  bool encode(unsigned char *buf, size_t *size);
  bool decode(unsigned char *buf, size_t *size);

private:

  E2AP_PDU_t * e2ap_pdu_obj;
  InitiatingMessage_t *initMsg;
  RICindication_IEs_t *IE_array;
  std::unique_ptr<IndicationIEs> _indicationIEs;

  std::string _error_string;
  char _errbuf[128];
  size_t _errbuf_len = 128;

  bool setfields( InitiatingMessage_t *);
};

template<typename T1, typename T2>
E2APIndication<T1,T2>::E2APIndication(IndicationIEs& ieObj){

  e2ap_pdu_obj = 0;
  e2ap_pdu_obj = (E2AP_PDU_t * )calloc(1, sizeof(E2AP_PDU_t));
  assert(e2ap_pdu_obj != 0);

  initMsg = 0;
  initMsg = (InitiatingMessage_t * )calloc(1, sizeof(InitiatingMessage_t));
  assert(initMsg != 0);

  IE_array = 0;
  IE_array = (RICindication_IEs_t *)calloc(RIC_INDICATION_IES_COUNT, sizeof(RICindication_IEs_t));
  assert(IE_array != 0);

  e2ap_pdu_obj->present = E2AP_PDU_PR_initiatingMessage;
  e2ap_pdu_obj->choice.initiatingMessage = initMsg;

  _indicationIEs = std::make_unique<IndicationIEs>();
  *_indicationIEs = ieObj;

};
template<typename T1, typename T2>
E2APIndication<T1,T2>::E2APIndication(unsigned char *buf, size_t *size){
	  e2ap_pdu_obj = 0;
	  initMsg = 0;
	  IE_array = 0;

	  _indicationIEs = std::make_unique<IndicationIEs>();
	   bool status =  this->decode(buf, size);
	   if(!status)
		   throw "E2AP Indication Decode Failed: "+this->get_error();
}


// Clear assigned protocolIE list from RIC indication IE container
template<typename T1, typename T2>
E2APIndication<T1,T2>::~E2APIndication(void){

  mdclog_write(MDCLOG_DEBUG, "Freeing E2AP Indication object memory");
  RICindication_t *ricIndication  = &(initMsg->value.choice.RICindication);
  for(int i = 0; i < ricIndication->protocolIEs.list.size; i++){
    ricIndication->protocolIEs.list.array[i] = 0;
  }
  if (ricIndication->protocolIEs.list.size > 0){
    free(ricIndication->protocolIEs.list.array);
    ricIndication->protocolIEs.list.array = 0;
    ricIndication->protocolIEs.list.count = 0;
    ricIndication->protocolIEs.list.size = 0;
  }

  free(IE_array);
  free(initMsg);
  ASN_STRUCT_FREE(asn_DEF_E2AP_PDU, e2ap_pdu_obj);
  mdclog_write(MDCLOG_DEBUG, "Freed E2AP Indication object memory");
}

template<typename T1, typename T2>
bool E2APIndication<T1, T2>::decode(unsigned char *buf, size_t *size)
{

	asn_dec_rval_t dec_res  = asn_decode(0,ATS_ALIGNED_BASIC_PER, &asn_DEF_E2AP_PDU, (void**)&(e2ap_pdu_obj), buf, *size);
	if(dec_res.code != RC_OK){
			 mdclog_write(MDCLOG_ERR, "Failed to decode: %s","RIC Indication Message");
			 return false;
	} else {
			 mdclog_write(MDCLOG_INFO, "Successfully decoded: %s","RIC Indication Message");
	}

  initMsg = e2ap_pdu_obj->choice.initiatingMessage;
  //write the decoding code.
  if (initMsg == 0){
    _error_string = "Invalid reference for E2AP Indication message in get_fields";
    return false;
  }


  for(int edx = 0; edx < initMsg->value.choice.RICindication.protocolIEs.list.count; edx++) {
	RICindication_IEs_t *memb_ptr = initMsg->value.choice.RICindication.protocolIEs.list.array[edx];
    switch(memb_ptr->id)
     {
      case (ProtocolIE_ID_id_RICindicationHeader):
  		_indicationIEs->set_ricIndicationHeader(memb_ptr->value.choice.RICindicationHeader.buf, memb_ptr->value.choice.RICindicationHeader.size);
      break;

      case (ProtocolIE_ID_id_RICindicationMessage):
		_indicationIEs->set_ricIndicationMessage(memb_ptr->value.choice.RICindicationMessage.buf, memb_ptr->value.choice.RICindicationMessage.size);
      break;

      case (ProtocolIE_ID_id_RICrequestID):
		_indicationIEs->set_ricRequestorID(memb_ptr->value.choice.RICrequestID.ricRequestorID);
      break;

      case (ProtocolIE_ID_id_RANfunctionID):
		_indicationIEs->set_ranFunctionID(memb_ptr->value.choice.RANfunctionID);
   	  break;

      case (ProtocolIE_ID_id_RICindicationSN):
		_indicationIEs->set_ricIndicationSN(memb_ptr->value.choice.RICindicationSN);
   	  break;

      case (ProtocolIE_ID_id_RICindicationType):
		_indicationIEs->set_ricIndicationType(memb_ptr->value.choice.RICindicationType);
  	  break;

      case (ProtocolIE_ID_id_RICactionID):
		_indicationIEs->set_ricActionID(memb_ptr->value.choice.RICactionID);
  	  break;

      case (ProtocolIE_ID_id_RICcallProcessID):
		_indicationIEs->set_ricCallProcessID(memb_ptr->value.choice.RICcallProcessID.buf,memb_ptr->value.choice.RICcallProcessID.size);
	  break;

      default:
      break;
      }

  }
  //  xer_fprint(stdout, &asn_DEF_E2AP_PDU, e2ap_pdu_obj);

  return true;

}

template<typename T1, typename T2>
bool E2APIndication<T1,T2>::encode(unsigned char *buf, size_t *size){

  initMsg->procedureCode = ProcedureCode_id_RICindication;
  initMsg->criticality = Criticality_ignore;
  initMsg->value.present = InitiatingMessage__value_PR_RICindication;

  bool res;
  asn_enc_rval_t retval;

  res = setfields(initMsg);
  if (!res){
    return false;
  }

  int ret_constr = asn_check_constraints(&asn_DEF_E2AP_PDU, e2ap_pdu_obj, _errbuf, &_errbuf_len);
  if(ret_constr){
    _error_string.assign(&_errbuf[0], _errbuf_len);
    _error_string = "Error encoding E2AP Indication message. Reason = " + _error_string;
    return false;
  }

  retval = asn_encode_to_buffer(0, ATS_ALIGNED_BASIC_PER, &asn_DEF_E2AP_PDU, e2ap_pdu_obj, buf, *size);
  if(retval.encoded == -1){
    _error_string.assign(strerror(errno));
    return false;
  }

  else {
    if(*size < retval.encoded){
      std::stringstream ss;
      ss  <<"Error encoding E2AP Indication . Reason =  encoded pdu size " << retval.encoded << " exceeds buffer size " << *size << std::endl;
      _error_string = ss.str();
      return false;
    }
  }

  *size = retval.encoded;
  xer_fprint(stdout, &asn_DEF_E2AP_PDU, e2ap_pdu_obj);

  return true;

}
template<typename T1, typename T2>
bool E2APIndication<T1,T2>::setfields(InitiatingMessage_t *initMsg){
  unsigned int ie_index;

  if (initMsg == 0){
    _error_string = "Invalid reference for E2AP Indication message in set_fields";
    return false;
  }


  RICindication_t * ric_indication = &(initMsg->value.choice.RICindication);
  ric_indication->protocolIEs.list.count = 0;

  ie_index = 0;

  RICindication_IEs_t *ies_ricreq = &IE_array[ie_index];
  ies_ricreq->criticality = Criticality_reject;
  ies_ricreq->id = ProtocolIE_ID_id_RICrequestID;
  ies_ricreq->value.present = RICindication_IEs__value_PR_RICrequestID;
  RICrequestID_t *ricrequest_ie = &ies_ricreq->value.choice.RICrequestID;
  ricrequest_ie->ricRequestorID = _indicationIEs->get_ricRequestorID();
  ASN_SEQUENCE_ADD(&(ric_indication->protocolIEs), &(IE_array[ie_index]));

  ie_index++;
  RICindication_IEs_t *ies_ranfunc = &IE_array[ie_index];
  ies_ranfunc->criticality = Criticality_reject;
  ies_ranfunc->id = ProtocolIE_ID_id_RANfunctionID;
  ies_ranfunc->value.present = RICindication_IEs__value_PR_RANfunctionID;
  RANfunctionID_t *ranfunction_ie = &ies_ranfunc->value.choice.RANfunctionID;
  *ranfunction_ie = _indicationIEs->get_ranFunctionID();
  ASN_SEQUENCE_ADD(&(ric_indication->protocolIEs), &(IE_array[ie_index]));

  ie_index++;
  RICindication_IEs_t *ies_actid = &IE_array[ie_index];
  ies_actid->criticality = Criticality_reject;
  ies_actid->id = ProtocolIE_ID_id_RICactionID;
  ies_actid->value.present = RICindication_IEs__value_PR_RICactionID;
  RICactionID_t *ricaction_ie = &ies_actid->value.choice.RICactionID;
  *ricaction_ie = _indicationIEs->get_ricActionID();
  ASN_SEQUENCE_ADD(&(ric_indication->protocolIEs), &(IE_array[ie_index]));

  if(_indicationIEs->get_is_ricIndicationSN())
  {
	  ie_index++;
	  RICindication_IEs_t *ies_ricsn = &IE_array[ie_index];
	  ies_ricsn->criticality = Criticality_reject;
	  ies_ricsn->id = ProtocolIE_ID_id_RICindicationSN;
	  ies_ricsn->value.present = RICindication_IEs__value_PR_RICindicationSN;
	  RICindicationSN_t *ricsn_ie = &ies_ricsn->value.choice.RICindicationSN;
	  *ricsn_ie = _indicationIEs->get_ricIndicationSN();
	  ASN_SEQUENCE_ADD(&(ric_indication->protocolIEs), &(IE_array[ie_index]));
  }

  ie_index++;
  RICindication_IEs_t *ies_indtyp = &IE_array[ie_index];
  ies_indtyp->criticality = Criticality_reject;
  ies_indtyp->id = ProtocolIE_ID_id_RICindicationType;
  ies_indtyp->value.present = RICindication_IEs__value_PR_RICindicationType;
  RICindicationType_t *rictype_ie = &ies_indtyp->value.choice.RICindicationType;
  *rictype_ie = _indicationIEs->get_ricIndicationType();
  ASN_SEQUENCE_ADD(&(ric_indication->protocolIEs), &(IE_array[ie_index]));

  ie_index++;
  RICindication_IEs_t *ies_richead = &IE_array[ie_index];
  ies_richead->criticality = Criticality_reject;
  ies_richead->id = ProtocolIE_ID_id_RICindicationHeader;
  ies_richead->value.present = RICindication_IEs__value_PR_RICindicationHeader;
  RICindicationHeader_t *richeader_ie = &ies_richead->value.choice.RICindicationHeader;
  richeader_ie->buf = (uint8_t*)_indicationIEs->get_ricIndicationHeader();
  richeader_ie->size = _indicationIEs->get_ricIndicationHeader_size();
  ASN_SEQUENCE_ADD(&(ric_indication->protocolIEs), &(IE_array[ie_index]));

  ie_index++;
  RICindication_IEs_t *ies_indmsg = &IE_array[ie_index];
  ies_indmsg->criticality = Criticality_reject;
  ies_indmsg->id = ProtocolIE_ID_id_RICindicationMessage;
  ies_indmsg->value.present = RICindication_IEs__value_PR_RICindicationMessage;
  RICindicationMessage_t *ricmsg_ie = &ies_indmsg->value.choice.RICindicationMessage;
  ricmsg_ie->buf = (uint8_t*)_indicationIEs->get_ricIndicationMessage();
  ricmsg_ie->size = _indicationIEs->get_ricIndicationMessage_size();
  ASN_SEQUENCE_ADD(&(ric_indication->protocolIEs), &(IE_array[ie_index]));


  // optional call process id ..
  if (_indicationIEs->get_is_callProcessID()){
    ie_index++;
    RICindication_IEs_t *ies_ind_callprocessid = &IE_array[ie_index];
    ies_ind_callprocessid->criticality = Criticality_reject;
    ies_ind_callprocessid->id = ProtocolIE_ID_id_RICcallProcessID;
    ies_ind_callprocessid->value.present = RICindication_IEs__value_PR_RICcallProcessID;
    RICcallProcessID_t *riccallprocessid_ie = &ies_ind_callprocessid->value.choice.RICcallProcessID;
    riccallprocessid_ie->buf = (uint8_t*)_indicationIEs->get_ricCallProcessId();
    riccallprocessid_ie->size = _indicationIEs->get_ricCallProcessId_size();
    ASN_SEQUENCE_ADD(&(ric_indication->protocolIEs), &(IE_array[ie_index]));
  }

  return true;

};

#endif /* SRC_XAPP_ASN_E2AP_E2AP_INDICATION_HPP_ */
