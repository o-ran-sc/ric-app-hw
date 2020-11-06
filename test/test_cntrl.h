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
 * test_cntrl.h
 *
 *  Created on: Nov 4, 2020
 *      Author: Shraboni Jana
 */

#ifndef TEST_TEST_CNTRL_H_
#define TEST_TEST_CNTRL_H_

#include<iostream>
#include<gtest/gtest.h>

#include "e2ap_control_request.hpp"
#include "e2sm_control.hpp"


TEST(E2SM, ControlMessage)
{

	unsigned char buff[1024];
	size_t buf_len = 1024;

	HWControlMessage msgObj;
	msgObj.set_ricControlMessage("ControlMessage");

	HWControlHeader headObj;
	headObj.set_ricControlHeader(1);

	E2APControlMessage<HWControlHeader,HWControlMessage>::ControlRequestIEs infoObj;
	infoObj.set_ranFunctionID(1);
	infoObj.set_ricRequestorID(1);
	infoObj.set_ricControlHeader(headObj);
	infoObj.set_ricControlMessage(msgObj);
//	infoObj.set_ricCallProcessID("ProcessID");

	E2APControlMessage<HWControlHeader,HWControlMessage>  cntrlObj(infoObj);

	bool res = cntrlObj.encode(buff, &buf_len);
		if(!res)
		{
			std::cout << cntrlObj.get_error() << std::endl;
		}
		ASSERT_TRUE(res);
}




#endif /* TEST_TEST_CNTRL_H_ */
