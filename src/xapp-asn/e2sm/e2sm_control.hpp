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
 * e2sm_control.hpp
 *
 *  Created on: Apr, 2020
 *      Author: Shraboni Jana
 */

/* Classes to handle E2 service model based on e2sm-HelloWorld-v001.asn */
#ifndef SRC_XAPP_ASN_E2SM_E2SM_CONTROL_HPP_
#define SRC_XAPP_ASN_E2SM_E2SM_CONTROL_HPP_

#include <sstream>
#include <mdclog/mdclog.h>
#include <vector>
#include <string>
#include <E2SM-HelloWorld-ControlHeader.h>
#include <E2SM-HelloWorld-ControlMessage.h>
#include <E2SM-HelloWorld-ControlHeader-Format1.h>
#include <E2SM-HelloWorld-ControlMessage-Format1.h>
#include <HW-Header.h>
#include <HW-Message.h>

class HWControlHeader {
public:
	HWControlHeader(void);
	HWControlHeader(unsigned char*, size_t *, bool&);
  ~HWControlHeader(void);
   std::string  get_error (void) const {return _error_string ;};

  long int get_hw_header(){return this->_hw_header;};

  HWControlHeader& set_ricControlHeader(int param_header){
  				_hw_header = param_header; return *this;
  			}


  bool decode(unsigned char*, size_t *);
  bool encode(unsigned char*, size_t *);


private:

  long int _hw_header;
  E2SM_HelloWorld_ControlHeader_t * _header; // used for encoding
  E2SM_HelloWorld_ControlHeader_Format1_t _header_fmt1;

  bool setfields(E2SM_HelloWorld_ControlHeader_t *);

  size_t _errbuf_len = 128;
  char _errbuf[128];
  std::string _error_string;
};


class HWControlMessage {
public:
	HWControlMessage(void);
	HWControlMessage(unsigned char*, size_t *, bool&);
  ~HWControlMessage(void);

  std::string  get_error (void) const {return _error_string ;};
  bool decode(unsigned char*, size_t *);
  bool encode(unsigned char*, size_t *);


  size_t get_hw_message_size(){return this->_hw_msg_size;};
  unsigned char* get_hw_message(){return this->_hw_msg;};

  HWControlMessage& set_ricControlMessage(std::string msg){
    				_hw_msg_size = strlen(msg.c_str());
    				strncpy((char*)_hw_msg,msg.c_str(),_hw_msg_size);
    				return *this;
    			};

private:

  size_t _hw_msg_size;
  unsigned char _hw_msg[128];

  E2SM_HelloWorld_ControlMessage_t* _message;
  E2SM_HelloWorld_ControlMessage_Format1_t _message_fmt1;

  bool setfields(E2SM_HelloWorld_ControlMessage_t *);

  size_t _errbuf_len = 128;
  char _errbuf[128];
  std::string _error_string;
};

#endif /* SRC_XAPP_ASN_E2SM_E2SM_INDICATION_HPP_ */
