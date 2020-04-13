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
/*
 * test_rmr_healthcheck.h
 *
 *  Created on: Mar, 2020
 *  Author: Shraboni Jana
 */

#ifndef TEST_TEST_RMR_H_
#define TEST_TEST_RMR_H_
#include<iostream>
#include<gtest/gtest.h>
#include "xapp.hpp"

#define HC_MSG_SIZE 512

using namespace std;

TEST(RMR, Sender){

	int total_num_msgs = 2;
	int num_attempts = 10;

	std::unique_ptr<XappRmr> rmr;
	rmr = std::make_unique<XappRmr>("4560",num_attempts);
	rmr->xapp_rmr_init();
	rmr->set_listen(false);

	xapp_rmr_header hdr;
 	hdr.message_type = RIC_HEALTH_CHECK_REQ;

    for(int i = 0; i < total_num_msgs; i++){
    	std::string temp="HelloWorld: RMR Health Check"+ std::to_string(i);
    	int n = temp.length();
    	char strMsg[n+1];
        std::strcpy(strMsg,temp.c_str());

        clock_gettime(CLOCK_REALTIME, &(hdr.ts));
        hdr.payload_length = n+1;

        bool res = rmr->xapp_rmr_send(&hdr,(void*)strMsg);
        ASSERT_TRUE(res);
        usleep(1);
     }
    ASSERT_TRUE(true);
}
 TEST(RMR, Receiver){
	 //initialize rmr
	 std::unique_ptr<XappMsgHandler> mp_handler = std::make_unique<XappMsgHandler>("HW-Xapp-id");
	 XappRmr *rmr = new XappRmr("4560");
	 rmr->xapp_rmr_init();
	 sleep(10);
	 rmr->set_listen(false);
	 rmr->xapp_rmr_receive(std::move(*mp_handler), rmr);
	 ASSERT_TRUE(true);
 };


#endif /* TEST_TEST_RMR_H_ */
