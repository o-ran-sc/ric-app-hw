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

TEST(XappRmr, RMRSender){

	int total_num_msgs = 10;
	int num_attempts = 5;

	std::unique_ptr<XappRmr> rmr;
	rmr = std::make_unique<XappRmr>("4560",num_attempts);
	rmr->xapp_rmr_init();

	xapp_rmr_header hdr;
 	hdr.message_type = RIC_HEALTH_CHECK_REQ;
    char *strMsg = (char*)malloc(HC_MSG_SIZE);

    for(int i = 0; i < total_num_msgs; i++){
       snprintf(strMsg,HC_MSG_SIZE, "HelloWorld: RMR Health Check %d", i);
       clock_gettime(CLOCK_REALTIME, &(hdr.ts));
       hdr.payload_length = strlen(strMsg);

       bool res = rmr->xapp_rmr_send(&hdr,(void*)strMsg);
       ASSERT_TRUE(res);

       usleep(10);
     }
    delete strMsg;

}
 TEST(XappRmr, RMRReceiver){
	 //initialize rmr
	 std::unique_ptr<XappRmr> rmr;
	 rmr = std::make_unique<XappRmr>("5551");
	 rmr->xapp_rmr_init();

	 XappSettings config;

	 std::unique_ptr<Xapp> hw_xapp = std::make_unique<Xapp>(std::ref(config),std::ref(*rmr));

	 std::unique_ptr<XappMsgHandler> mp_handler = std::make_unique<XappMsgHandler>();

	 rmr->set_listen(true);
	 hw_xapp->start_xapp_receiver(std::ref(*mp_handler));
	 sleep(2);
	 rmr->~XappRmr();

	 ASSERT_TRUE(true);
 };

 TEST(XappRmr, HC_ReturnToSender){

	 int total_num_msgs = 2;
	 int num_attempts = 10;

	 std::unique_ptr<XappRmr> rmr;
	 rmr = std::make_unique<XappRmr>("4560",num_attempts);
	 rmr->xapp_rmr_init();

	 XappSettings config;

	 std::unique_ptr<Xapp> hw_xapp = std::make_unique<Xapp>(std::ref(config),std::ref(*rmr));

	 std::unique_ptr<XappMsgHandler> mp_handler = std::make_unique<XappMsgHandler>();

	 rmr->set_listen(true);
	 hw_xapp->start_xapp_receiver(std::ref(*mp_handler));
	 sleep(5);

	 xapp_rmr_header hdr;
	 hdr.message_type = RIC_HEALTH_CHECK_REQ;
	 char strMsg[HC_MSG_SIZE];

	 for(int i = 0; i < total_num_msgs; i++){
		 snprintf(strMsg,HC_MSG_SIZE, "HelloWorld: RMR Health Check %d", i);
		 clock_gettime(CLOCK_REALTIME, &(hdr.ts));
		 hdr.payload_length = strlen(strMsg);

		 bool res = rmr->xapp_rmr_send(&hdr,(void*)strMsg);
		 usleep(10);
	 }


 };




#endif /* TEST_TEST_RMR_H_ */
