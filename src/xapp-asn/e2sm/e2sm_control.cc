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
 * e2sm_control.cc
 *
 *  Created on: Apr, 2020
 *      Author: Shraboni Jana
 */
/* Classes to handle E2 service model based on e2sm-HelloWorld-v001.asn */
#include "e2sm_control.hpp"

 //initialize
 HWControlMessage::HWControlMessage(void){

	memset(&_message_fmt1, 0, sizeof(E2SM_HelloWorld_ControlMessage_Format1_t));

	_message = 0;
    _message = (E2SM_HelloWorld_ControlMessage_t*)calloc(1, sizeof(E2SM_HelloWorld_ControlMessage_t));
    assert(_message !=0);
    _hw_msg_size = 0;
  };


 //initialize
  HWControlHeader::HWControlHeader(void){

 	memset(&_header_fmt1, 0, sizeof(E2SM_HelloWorld_ControlHeader_Format1_t));

 	_header = 0;
     _header = ( E2SM_HelloWorld_ControlHeader_t *)calloc(1, sizeof( E2SM_HelloWorld_ControlHeader_t));
     assert(_header != 0);

     _hw_header = 0;
   };


 HWControlMessage::~HWControlMessage(void){

  mdclog_write(MDCLOG_DEBUG, "Freeing event trigger object memory");
  _message->choice.controlMessage_Format1 = 0;
  ASN_STRUCT_FREE(asn_DEF_E2SM_HelloWorld_ControlMessage, _message);


};
 HWControlHeader::~HWControlHeader(void){

   mdclog_write(MDCLOG_DEBUG, "Freeing event trigger object memory");
   _header->choice.controlHeader_Format1 = 0;
   ASN_STRUCT_FREE(asn_DEF_E2SM_HelloWorld_ControlHeader, _header);
 };

bool HWControlHeader::encode(unsigned char *buf, size_t *size){

  ASN_STRUCT_RESET(asn_DEF_E2SM_HelloWorld_ControlHeader, _header);

  bool res;
  res = setfields(_header);
  if (!res){

    return false;
  }

  int ret_constr = asn_check_constraints(&asn_DEF_E2SM_HelloWorld_ControlHeader, _header, _errbuf, &_errbuf_len);
  if(ret_constr){
    _error_string.assign(&_errbuf[0], _errbuf_len);
    return false;
  }

  xer_fprint(stdout, &asn_DEF_E2SM_HelloWorld_ControlHeader, _header);

  asn_enc_rval_t retval = asn_encode_to_buffer(0, ATS_ALIGNED_BASIC_PER, &asn_DEF_E2SM_HelloWorld_ControlHeader, _header, buf, *size);

  if(retval.encoded == -1){
    _error_string.assign(strerror(errno));
    return false;
  }
  else if (retval.encoded > *size){
    std::stringstream ss;
    ss  <<"Error encoding event trigger definition. Reason =  encoded pdu size " << retval.encoded << " exceeds buffer size " << *size << std::endl;
    _error_string = ss.str();
    return false;
  }
  else{
    *size = retval.encoded;
  }

  return true;
}

bool HWControlMessage::encode(unsigned char *buf, size_t *size){

  bool res;
  res = setfields(_message);
  if (!res){
    return false;
  }

  int ret_constr = asn_check_constraints(&asn_DEF_E2SM_HelloWorld_ControlMessage, _message, _errbuf, &_errbuf_len);
  if(ret_constr){
    _error_string.assign(&_errbuf[0], _errbuf_len);
    return false;
  }

  xer_fprint(stdout, &asn_DEF_E2SM_HelloWorld_ControlMessage, _message);

  asn_enc_rval_t retval = asn_encode_to_buffer(0, ATS_ALIGNED_BASIC_PER, &asn_DEF_E2SM_HelloWorld_ControlMessage, _message, buf, *size);

  if(retval.encoded == -1){
    _error_string.assign(strerror(errno));
    return false;
  }
  else if (retval.encoded > *size){
    std::stringstream ss;
    ss  <<"Error encoding action definition. Reason =  encoded pdu size " << retval.encoded << " exceeds buffer size " << *size << std::endl;
    _error_string = ss.str();
    return false;
  }
  else{
    *size = retval.encoded;
  }

  return true;
}

bool HWControlHeader::setfields(E2SM_HelloWorld_ControlHeader_t * _header){

 if(_header == 0){
    _error_string = "Invalid reference for Event Trigger Definition set fields";
    return false;
  }

 _header->present = E2SM_HelloWorld_ControlHeader_PR_controlHeader_Format1;
  _header_fmt1.controlHeaderParam = this->get_hw_header();
  _header->choice.controlHeader_Format1 = &_header_fmt1;

  return true;
};

bool HWControlMessage::setfields(E2SM_HelloWorld_ControlMessage_t * _message){

 if(_message == 0){
    _error_string = "Invalid reference for Event Action Definition set fields";
    return false;
  }
  _message->present = E2SM_HelloWorld_ControlMessage_PR_controlMessage_Format1;

  _message_fmt1.controlMsgParam.buf = this->get_hw_message();
  _message_fmt1.controlMsgParam.size = this->get_hw_message_size();
  _message->choice.controlMessage_Format1 = &_message_fmt1;

  return true;
};

