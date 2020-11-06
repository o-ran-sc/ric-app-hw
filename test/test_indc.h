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
/*
 * test_e2sm.h
 *
 *  Created on: Apr, 2020
 *      Author: Shraboni Jana
 */

#ifndef TEST_TEST_ASN_H_
#define TEST_TEST_ASN_H_
#include<iostream>
#include<gtest/gtest.h>

#include "e2ap_indication.hpp"
#include "e2sm_indication.hpp"

#include "e2ap_subscription_request.hpp"
#include "e2sm_subscription.hpp"
using namespace std;
TEST(E2SM, IndicationMessageEncode)
{
	unsigned char buff[1024];
	size_t buf_len = 1024;


	HWIndicationHeader headerObj;
	headerObj.set_ricIndicationHeader(1);
	std::cout << headerObj.get_error() << std::endl;

	HWIndicationMessage messageObj;
	messageObj.set_hw_message("HelloWorld-E2SM");
	std::cout << messageObj.get_error() << std::endl;

	E2APIndication<HWIndicationHeader,HWIndicationMessage>::IndicationIEs infoObj;
	infoObj.set_ranFunctionID(1);
	infoObj.set_ricActionID(1);
	infoObj.set_ricRequestorID(1);
	infoObj.set_ricIndicationHeader(headerObj);
	infoObj.set_ricIndicationMessage(messageObj);

	E2APIndication<HWIndicationHeader,HWIndicationMessage> e2obj(infoObj);

	bool res = e2obj.encode(buff, &buf_len);
	if(!res)
	{
		std::cout << e2obj.get_error() << std::endl;
	}
	ASSERT_TRUE(res);


	FILE * pFile;
	pFile = fopen ("indication1.per","w");
	if (pFile!=NULL)
	 {
		  fwrite (buff , sizeof(char), buf_len, pFile);
	      sleep(2);
		  fclose (pFile);
		  sleep(2);
	 }

}
TEST(E2SM, IndicationMessageDecode)
{
       unsigned char e2ap_buff[4096];
       char filename[100] = "indication2.per";
       FILE *fp;
       fp = fopen(filename,"rb");
       if(!fp) {
	             perror(filename);
	             exit(1); }

	   size_t e2ap_buff_size = fread(e2ap_buff, 1, 4096, fp);
	   fclose(fp);
	   sleep(2);
       if(!e2ap_buff_size){
             fprintf(stderr, "%s: Empty or broken\n", filename);
              exit(1);
       	   } else {
              fprintf(stderr, "e2ap buffer size: %ld \n", e2ap_buff_size);
       }

    bool decode_status = true;
    try{
    	E2APIndication<HWIndicationHeader,HWIndicationMessage> e2obj(&e2ap_buff[0],&e2ap_buff_size);
    	E2APIndication<HWIndicationHeader,HWIndicationMessage>::IndicationIEs indIEs = e2obj.getIndicationIEs();
    	std::cout << indIEs.get_ricRequestorID() << std::endl;

    	size_t hsize = indIEs.get_ricIndicationHeader_size();
    	unsigned char* hvalue = (unsigned char*)indIEs.get_ricIndicationHeader();
    	HWIndicationHeader headerObj(&hvalue[0], &hsize);

    	  size_t msize = indIEs.get_ricIndicationMessage_size();
    	  auto mvalue = (unsigned char*)indIEs.get_ricIndicationMessage();
    	  HWIndicationMessage msgObj(&mvalue[0], &msize);

    } catch(const char* e){
    		decode_status = false;
    		std::cout << "Error Message: " << e << std::endl;
	}


	ASSERT_TRUE(decode_status);

}

/**/

#endif /* TEST_TEST_ASN_H_ */
