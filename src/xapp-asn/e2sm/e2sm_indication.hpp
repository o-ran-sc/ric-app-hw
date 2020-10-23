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
 * e2sm_indication.hpp
 *
 *  Created on: Apr, 2020
 *      Author: Shraboni Jana
 */

/* Classes to handle E2 service model based on e2sm-HelloWorld-v001.asn */
#ifndef SRC_XAPP_ASN_E2SM_E2SM_INDICATION_HPP_
#define SRC_XAPP_ASN_E2SM_E2SM_INDICATION_HPP_
#include <sstream>
#include <mdclog/mdclog.h>
#include <vector>
#include<iostream>
#include <E2SM-HelloWorld-IndicationHeader.h>
#include <E2SM-HelloWorld-IndicationMessage.h>
#include <E2SM-HelloWorld-IndicationHeader-Format1.h>
#include <E2SM-HelloWorld-IndicationMessage-Format1.h>
#include <HW-Header.h>
#include <HW-Message.h>
class HWIndicationHeader {
public:
	HWIndicationHeader(void);
	HWIndicationHeader(unsigned char*, size_t *);
  ~HWIndicationHeader(void);
   std::string  get_error (void) const {return _error_string ;};

  long int get_hw_header(){return this->_hw_header;};

  HWIndicationHeader& set_ricIndicationHeader(int param_header){
  				_hw_header = param_header; return *this;
  			}
  bool decode(unsigned char*, size_t *);
  bool encode(unsigned char*, size_t *);


private:

  long int _hw_header;
  E2SM_HelloWorld_IndicationHeader_t * _header; // used for encoding
  E2SM_HelloWorld_IndicationHeader_Format1_t _header_fmt1;

  bool setfields(E2SM_HelloWorld_IndicationHeader_t *);

  size_t _errbuf_len = 128;
  char _errbuf[128];
  std::string _error_string;
};

class HWIndicationMessage {
public:
	HWIndicationMessage(void);
	HWIndicationMessage(unsigned char*, size_t *);
  ~HWIndicationMessage(void);

  std::string  get_error (void) const {return _error_string ;};
  bool decode(unsigned char*, size_t *);
  bool encode(unsigned char*, size_t *);

  size_t get_hw_message_size(){return this->_hw_msg_size;};
  unsigned char* get_hw_message(){return this->_hw_msg;};
  HWIndicationMessage& set_hw_message(std::string msg)
  {
	  _hw_msg_size=strlen(msg.c_str());
	   strncpy((char*)_hw_msg,msg.c_str(),_hw_msg_size);
	   return *this;
  }
private:

  size_t _hw_msg_size;
  unsigned char _hw_msg[30];

  E2SM_HelloWorld_IndicationMessage_t* _message;
  E2SM_HelloWorld_IndicationMessage_Format1_t _message_fmt1;

  bool setfields(E2SM_HelloWorld_IndicationMessage_t *);

  size_t _errbuf_len = 128;
  char _errbuf[128];
  std::string _error_string;
};

#endif


