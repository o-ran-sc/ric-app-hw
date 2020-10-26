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
 * test_e2ap.h
 *
 *  Created on: Oct 23, 2020
 *  Author: Shraboni Jana
 */

#ifndef TEST_TEST_E2AP_H_
#define TEST_TEST_E2AP_H_
#include<iostream>
#include<gtest/gtest.h>

#include "e2ap_control_failure.hpp"
#include "xapp.hpp"
#include "e2ap_control_ack.hpp"
TEST(E2AP, ControlAcknowledgementEncode)
{

	unsigned char buff[1024];
	size_t buf_len = 1024;

	E2APControlAcknowledge::ControlAcknowledgeIEs infoObj;
	infoObj.set_ranFunctionID(1);
	infoObj.set_ricRequestorID(1);
	infoObj.set_ricInstanceID(1);
	infoObj.set_ricControlStatus(1);
	infoObj.set_ricCallProcessID("CallProcessID");
	infoObj.set_ricControlOutcome("ControlOutcome");


	E2APControlAcknowledge  cntrlObj(infoObj);

	bool res = cntrlObj.encode(buff, &buf_len);
	if(!res)
	{
		std::cout << cntrlObj.get_error() << std::endl;
	}
	ASSERT_TRUE(res);
	FILE * pFile;
		pFile = fopen ("controlack1.per","w");
		if (pFile!=NULL)
		 {
			  fwrite (buff , sizeof(char), buf_len, pFile);
		      sleep(2);
			  fclose (pFile);
		  }

}
TEST(E2SM, ControlAcknowledgementDecode)
{
       unsigned char e2ap_buff[4096];
       char filename[100] = "controlack1.per";
       FILE *fp;
       fp = fopen(filename,"rb");
       if(!fp) {
	             perror(filename);
	             exit(1); }

	   size_t e2ap_buff_size = fread(e2ap_buff, 1, 4096, fp);
	   fclose(fp);
       if(!e2ap_buff_size){
             fprintf(stderr, "%s: Empty or broken\n", filename);
              exit(1);
       	   } else {
              fprintf(stderr, "e2ap buffer size: %ld \n", e2ap_buff_size);
       }

       bool decode_status = true;
          try{
          	E2APControlAcknowledge e2obj(&e2ap_buff[0],&e2ap_buff_size);


          } catch(const char* e){
          		decode_status = false;
          		std::cout << "Error Message: " << e << std::endl;
      	}


      	ASSERT_TRUE(decode_status);

}
TEST(E2AP, ControlFailureEncode)
{

	unsigned char buff[1024];
	size_t buf_len = 1024;

	E2APControlFailure::ControlFailureIEs infoObj;
	infoObj.set_ranFunctionID(1);
	infoObj.set_ricRequestorID(1);
	infoObj.set_ricInstanceID(1);
	infoObj.set_ricCause(1);
	infoObj.set_ricSubCause(1);
	infoObj.set_ricCallProcessID("CallProcessID");
	infoObj.set_ricControlOutcome("ControlOutcome");


	E2APControlFailure  cntrlObj(infoObj);

	bool res = cntrlObj.encode(buff, &buf_len);
	if(!res)
	{
		std::cout << cntrlObj.get_error() << std::endl;
	}
	ASSERT_TRUE(res);
	FILE * pFile;
		pFile = fopen ("controlfail1.per","w");
		if (pFile!=NULL)
		 {
			  fwrite (buff , sizeof(char), buf_len, pFile);
		      sleep(2);
			  fclose (pFile);
		  }

}
TEST(E2SM, ControlFailureDecode)
{
       unsigned char e2ap_buff[1024];
       char filename[100] = "controlfail1.per";
       FILE *fp;
       fp = fopen(filename,"rb");
       if(!fp) {
	             perror(filename);
	             exit(1); }

	   size_t e2ap_buff_size = fread(e2ap_buff, 1, 1024, fp);
	   fclose(fp);
       if(!e2ap_buff_size){
             fprintf(stderr, "%s: Empty or broken\n", filename);
              exit(1);
       	   } else {
              fprintf(stderr, "e2ap buffer size: %ld \n", e2ap_buff_size);
       }

       bool decode_status = true;
          try{
          	E2APControlFailure e2obj(&e2ap_buff[0],&e2ap_buff_size);


          } catch(const char* e){
          		decode_status = false;
          		std::cout << "Error Message: " << e << std::endl;
      	}


      	ASSERT_TRUE(decode_status);

}

#endif /* TEST_TEST_E2AP_H_ */
