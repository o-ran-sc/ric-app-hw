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
 * test_subs.h
 *
 *  Created on: Apr, 2020
 *      Author: Shraboni Jana
 */

#ifndef TEST_TEST_SUBS_H_
#define TEST_TEST_SUBS_H_

#include<iostream>
#include<gtest/gtest.h>

#include "e2ap_subscription_request.hpp"
#include "xapp.hpp"
#define BUFFER_SIZE 1024

using namespace std;

//generating a E2AP Subscription Message
TEST(E2SMSubREQ, MultipleRANParameters)
{
	unsigned char buff[1024];
	size_t buff_size = 1024;


	//creating Action Definition
	HWActionDefinition e2sm_actdefn1;
	HWActionDefinition::RANParamIEs rparam1;

	rparam1.set_param_id(1);
	rparam1.set_param_name("ENodeBID");
	rparam1.set_param_test(1);
	rparam1.set_param_value("SR123");

	e2sm_actdefn1.add(rparam1);

	HWActionDefinition::RANParamIEs rparam2;

	rparam2.set_param_id(1);
	rparam2.set_param_name("UEID");
	rparam2.set_param_test(2);
	rparam2.set_param_value("UE123");

	e2sm_actdefn1.add(rparam2);


	//first Action Object

	E2APAction<HWActionDefinition> actionObj;
	E2APAction<HWActionDefinition>::ActionIEs ieobj;
	ieobj.set_ricActionID(1);
	ieobj.set_ricActionType(1);
	ieobj.set_ricActionDefinition(e2sm_actdefn1);

	actionObj.add(ieobj);

	HWEventTriggerDefinition eventObj;
	eventObj.set_triggerNature(0);

	//Now form the subscription.

	E2APSubscriptionRequest<HWEventTriggerDefinition, HWActionDefinition>::SubscriptionRequestIEs infoObj;

	infoObj.set_ranFunctionID(1);
	infoObj.set_ricInstanceID(1);
	infoObj.set_ricRequestorID(3);
	infoObj.set_ricAction_ToBeSetup_List(actionObj);
	infoObj.set_ricEventTriggerDefinition(eventObj);


	E2APSubscriptionRequest<HWEventTriggerDefinition, HWActionDefinition> requestObj(infoObj);

	//Alternate way of implementation
	//requestObj.add(E2APSubscriptionRequest<HWEventTriggerDefinition, HWActionDefinition>::SubscriptionRequestIEs().set_ranFunctionID(1).set_ricInstanceID(2).set_ricRequestorID(3).set_ricAction_ToBeSetup_List(actionObj));

	bool res = requestObj.encode(buff, &buff_size);
	ASSERT_TRUE(res);
}

TEST(E2SMSubREQ, SubRespMultipleActions)
{
	unsigned char buff[1024];
	size_t buff_size = 1024;


	//creating Action Definition 1
	HWActionDefinition e2sm_actdefn1;
	HWActionDefinition::RANParamIEs rparam1;

	rparam1.set_param_id(1);
	rparam1.set_param_name("ENodeBID");
	rparam1.set_param_test(1);
	rparam1.set_param_value("SR123");

	e2sm_actdefn1.add(rparam1);

	HWActionDefinition e2sm_actdefn2;
	HWActionDefinition::RANParamIEs rparam2;

	rparam2.set_param_id(1);
	rparam2.set_param_name("UEID");
	rparam2.set_param_test(2);
	rparam2.set_param_value("UE123");

	e2sm_actdefn2.add(rparam2);


	//first Action Object

	E2APAction<HWActionDefinition> actionObj;
	E2APAction<HWActionDefinition>::ActionIEs ieobj;
	ieobj.set_ricActionID(1);
	ieobj.set_ricActionType(1);
	ieobj.set_ricActionDefinition(e2sm_actdefn1);
	ieobj.set_ricSubsequentAction(1,1);
	actionObj.add(ieobj);

	//Second Action object.
	actionObj.add(E2APAction<HWActionDefinition>::ActionIEs().set_ricActionID(4).set_ricActionType(2).set_ricActionDefinition(e2sm_actdefn2));


	HWEventTriggerDefinition eventObj;
	eventObj.set_triggerNature(1);



	E2APSubscriptionRequest<HWEventTriggerDefinition, HWActionDefinition>::SubscriptionRequestIEs dataObj;

	dataObj.set_ranFunctionID(1);
	dataObj.set_ricInstanceID(1);
	dataObj.set_ricRequestorID(3);
	dataObj.set_ricAction_ToBeSetup_List(actionObj);
	dataObj.set_ricEventTriggerDefinition(eventObj);

	E2APSubscriptionRequest<HWEventTriggerDefinition, HWActionDefinition> requestObj(dataObj);
	bool res = requestObj.encode(buff, &buff_size);
	if(!res)
	{
		std::cout << requestObj.get_error() << std::endl;
	}
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
