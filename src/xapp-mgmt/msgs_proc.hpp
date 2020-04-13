/*
==================================================================================
        Copyright (c) 2018-2019 AT&T Intellectual Property.

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
 * msgs_proc.hpp
 * Created on: 2019
 * Author: Ashwin Shridharan, Shraboni Jana
 */
#pragma once

#ifndef XAPP_MSG_XAPP_MSG_HPP_
#define XAPP_MSG_XAPP_MSG_HPP_

#include <iostream>
#include<rmr/rmr.h>
#include <rmr/RIC_message_types.h>
#include <mdclog/mdclog.h>

#include "e2ap_control.hpp"
#include "e2ap_control_response.hpp"
#include "e2ap_indication.hpp"
#include "subscription_delete_request.hpp"
#include "subscription_delete_response.hpp"
#include "subscription_helper.hpp"
#include "subscription_request.hpp"
#include "subscription_request.hpp"
#include "subscription_response.hpp"
#include "e2sm.hpp"
#include "format_helper.hpp"

#define MAX_RMR_RECV_SIZE 2<<15

class XappMsgHandler{

private:
	std::string xapp_id;
public:
	//constructor for xapp_id.
	 XappMsgHandler(std::string xid){xapp_id=xid;};
	 void operator() (rmr_mbuf_t *, bool*);

	 bool encode_subscription_request(unsigned char*, size_t* );
	 bool encode_subscription_delete_request(unsigned char*, size_t* );

	 bool decode_subscription_response(unsigned char*, size_t );
	 bool decode_subscription_delete_response(unsigned char*, size_t );
	 bool decode_subscription_response_failure(unsigned char*, size_t );
	 bool decode_subscription_delete_response_failure(unsigned char*, size_t );

	 bool a1_policy_handler(char *, int* , a1_policy_helper &);

};


#endif /* XAPP_MSG_XAPP_MSG_HPP_ */
