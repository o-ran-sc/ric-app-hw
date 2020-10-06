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
#ifndef TEST_TEST_SUBS_H_
#define TEST_TEST_SUBS_H_

#include<iostream>
#include<gtest/gtest.h>
#include "xapp.hpp"
#define BUFFER_SIZE 1024

using namespace std;
//generating a E2AP Subscription Message
TEST(SUBSCRIPTION, Request){


	subscription_helper  din;
	subscription_helper  dout;

	subscription_request sub_req;
	subscription_request sub_recv;

	unsigned char buf[BUFFER_SIZE];
	size_t buf_size = BUFFER_SIZE;
	bool res;


	//Random Data  for request
	int request_id = 1;
	int function_id = 0;
	std::string event_def = "HelloWorld Event Definition";

	din.set_request(request_id);
	din.set_function_id(function_id);
	din.set_event_def(event_def.c_str(), event_def.length());

	std::string act_def = "HelloWorld Action Definition";

	din.add_action(1,1,(void*)act_def.c_str(), act_def.length(), 0);

	res = sub_req.encode_e2ap_subscription(&buf[0], &buf_size, din);
	ASSERT_TRUE(res);



}

//create a MOck e2term
TEST (MOCK, E2TERM){

	 const char* meid = "test1";
	 const char* sub_id = "sub1";
	 //Send the Subscription Response.
	 xapp_rmr_header hdr;
	 hdr.message_type = RIC_SUB_RESP;
	 clock_gettime(CLOCK_REALTIME, &(hdr.ts));
	 const char* strMsg = "Subscription Response for MEID: test1";
	 hdr.payload_length = strlen(strMsg);
	 strcpy((char*)hdr.meid, meid);
	 strcpy((char*)hdr.sid,sub_id);
	 int total_num_msgs = 2;
	 int num_attempts = 10;

	 std::unique_ptr<XappRmr> rmr;
	 rmr = std::make_unique<XappRmr>("4591",num_attempts);
	 rmr->xapp_rmr_init(true);

	 XappSettings config;

	 std::unique_ptr<Xapp> hw_xapp = std::make_unique<Xapp>(std::ref(config),std::ref(*rmr));
	 while(1){
		 bool res_msg = rmr->xapp_rmr_send(&hdr,(void*)strMsg);
		 if(res_msg){
			 mdclog_write(MDCLOG_INFO, "Message Sent Successfully");
			 break;
		 }
		 sleep(10);
	 }
}

#endif /* TEST_TEST_SUBS_H_ */
