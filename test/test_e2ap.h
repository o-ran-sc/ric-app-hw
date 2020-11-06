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
#include "e2ap_control_ack.hpp"
#include "e2ap_subsdel_request.hpp"
#include "e2ap_subsdel_response.hpp"
#include "e2ap_subsdel_failure.hpp"
#include "e2ap_criticality_diagnostics.hpp"
#include "e2ap_subscription_response.hpp"
#include "e2ap_subscription_failure.hpp"


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
TEST(E2AP, ControlAcknowledgementDecode)
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
TEST(E2AP, ControlFailureDecode)
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

TEST(E2AP, SubscriptionDeleteRequest){

	unsigned char buff[1024];
	size_t buff_size = 1024;


	E2APSubscriptionDeleteRequest::SubscriptionDeleteRequestIEs dataObj;

	dataObj.set_ranFunctionID(1);
	dataObj.set_ricInstanceID(1);
	dataObj.set_ricRequestorID(3);

	E2APSubscriptionDeleteRequest requestObj(dataObj);
	bool res = requestObj.encode(buff, &buff_size);
	if(!res)
	{
		std::cout << requestObj.get_error() << std::endl;
	}
	ASSERT_TRUE(res);


}

TEST(E2AP, SubDelResponseEncode)
{

	unsigned char buff[1024];
	size_t buf_len = 1024;

	E2APSubscriptionDeleteResponse::SubscriptionDeleteResponseIEs  infoObj;
	infoObj.set_ranFunctionID(1);
	infoObj.set_ricRequestorID(1);
	infoObj.set_ricInstanceID(1);


	E2APSubscriptionDeleteResponse  subdelObj(infoObj);

	bool res = subdelObj.encode(buff, &buf_len);
	if(!res)
	{
		std::cout << subdelObj.get_error() << std::endl;
	}
	ASSERT_TRUE(res);
	FILE * pFile;
		pFile = fopen ("subdelresponse1.per","w");
		if (pFile!=NULL)
		 {
			  fwrite (buff , sizeof(char), buf_len, pFile);
		      sleep(2);
			  fclose (pFile);
		  }

}
TEST(E2AP, SubDelResponseDecode)
{
       sleep(10);
	   unsigned char e2ap_buff[1024];
       char filename[100] = "subdelresponse1.per";
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
        	  E2APSubscriptionDeleteResponse e2obj(&e2ap_buff[0],&e2ap_buff_size);


          } catch(const char* e){
          		decode_status = false;
          		std::cout << "Error Message: " << e << std::endl;
      	}


      	ASSERT_TRUE(decode_status);

}

TEST(E2AP, SubDelFailureEncode){
	unsigned char buff[1024];
	size_t buf_len = 1024;

	CriticalityDiagnosticsIEs crIEs1;
	crIEs1.set_ieID(1);
	crIEs1.set_typeOferror(1);
	crIEs1.set_iEcriticality(2);

	CriticalityDiagnosticsIEs crIEs2;
	crIEs2.set_ieID(1);
	crIEs2.set_typeOferror(1);
	crIEs2.set_iEcriticality(1);



	E2APCriticalityDiagnostics critObj;
	critObj.set_procedureCode(10);
	critObj.set_procedureCriticality(1);
	critObj.set_ricInstanceID(101);
	critObj.set_ricRequestorID(102);
	critObj.set_triggeringMessage(1);
	critObj.add(crIEs1);
	critObj.add(crIEs2);

	E2APSubscriptionDeleteFailure::SubscriptionDeleteFailureIEs  infoObj;
	infoObj.set_ranFunctionID(1);
	infoObj.set_ricRequestorID(1);
	infoObj.set_ricInstanceID(1);
	infoObj.set_ricCause(1);
	infoObj.set_ricSubCause(2);
	infoObj.set_critical_diagnostic(critObj);


	E2APSubscriptionDeleteFailure  subdelObj(infoObj);

	bool res = subdelObj.encode(buff, &buf_len);
	if(!res)
	{
		std::cout << "Subscribtion Delete Failure Error Message" << subdelObj.get_error() << std::endl;
	}
	ASSERT_TRUE(res);
	FILE * pFile;
	pFile = fopen ("subdelfailure2.per","w");
	if (pFile!=NULL)
	{
				  fwrite (buff , sizeof(char), buf_len, pFile);
			      sleep(2);
				  fclose (pFile);
	 }

}

TEST(E2AP, SubDelFailureDecode)
{
       sleep(10);
	   unsigned char e2ap_buff[1024];
       char filename[100] = "subdelfailure2.per";
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
        	  E2APSubscriptionDeleteFailure e2obj(&e2ap_buff[0],&e2ap_buff_size);

        	  std::cout << "Procedure Code: " << e2obj.getIEs().get_critical_diagnostic()->get_procedureCode() << std::endl;
        	  std::cout << "Procedure Criticality: " << e2obj.getIEs().get_critical_diagnostic()->get_procedureCriticality() << std::endl;
        	  std::cout << "Size OF LIST :" << e2obj.getIEs().get_critical_diagnostic()->get_list_count() << std::endl;

        	  int count = e2obj.getIEs().get_critical_diagnostic()->get_list_count() ;
        	  std::vector<CriticalityDiagnosticsIEs> *lst = e2obj.getIEs().get_critical_diagnostic()->get_list();
        	  for(int i=0; i< count; i++){
        		  std::cout<< "Typ Error:::" << (*lst)[i].get_typeOferror() << std::endl;
        		  std::cout<< "iEcriticality:::" << (*lst)[i].get_iEcriticality() << std::endl;
        		  std::cout<< "ieID:::" << (*lst)[i].get_ieID() << std::endl;
        	  }

          } catch(const char* e){
          		decode_status = false;
          		std::cout << "Error Message: " << e << std::endl;
      	}


      	ASSERT_TRUE(decode_status);

}

TEST(E2AP, SubResponseEncode){
	unsigned char buff[512];
	size_t buf_len = 512;

	RICactionAdmittedList::RICactionAdmittedItemIEs tmp1;
	tmp1.set_ricActionID(1);

	RICactionAdmittedList admitlst;
	admitlst.add(tmp1);
	admitlst.add(RICactionAdmittedList::RICactionAdmittedItemIEs().set_ricActionID(2));

	RICactionNotAdmittedList::RICactionNotAdmittedItemIEs tmp2;
	tmp2.set_ricActionID(2);
	tmp2.set_ricCause(1);
	tmp2.set_ricSubCause(3);
	RICactionNotAdmittedList notadmitlst;
	notadmitlst.add(tmp2);
	notadmitlst.add(RICactionNotAdmittedList::RICactionNotAdmittedItemIEs().set_ricActionID(2).set_ricCause(1).set_ricSubCause(1));

	E2APSubscriptionResponse::SubscriptionResponseIEs  infoObj;
	infoObj.set_ranFunctionID(1);
	infoObj.set_ricRequestorID(1);
	infoObj.set_ricInstanceID(1);
	infoObj.set_actionsNotAdmitted_list(notadmitlst);
	infoObj.set_actionsAdmitted_list(admitlst);

	E2APSubscriptionResponse  subObj(infoObj);

	bool res = subObj.encode(buff, &buf_len);
	if(!res)
	{
		std::cout << "Subscription Response Error Message" << subObj.get_error() << std::endl;
	}
	ASSERT_TRUE(res);
	FILE * pFile;
	pFile = fopen ("subresponse1.per","w");
	if (pFile!=NULL)
	{
				  fwrite (buff , sizeof(char), buf_len, pFile);
				  fclose (pFile);
	 }

}
TEST(E2AP, SubResponseDecode)
{
       sleep(10);
	   unsigned char e2ap_buff[1024];
       char filename[100] = "subresponse2.per";
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
        	  E2APSubscriptionResponse e2obj(&e2ap_buff[0],&e2ap_buff_size);

        	  std::cout << "RAN Function ID: " << e2obj.getIEs().get_ranFunctionID() << std::endl;
        	  std::cout << "Requestor ID: " << e2obj.getIEs().get_ricRequestorID() << std::endl;
        	  std::cout << "Instance ID :" << e2obj.getIEs().get_ricInstanceID() << std::endl;

        	  int count = e2obj.getIEs().get_actionsAdmitted_list().get_list_count() ;
        	  std::vector<RICactionAdmittedList::RICactionAdmittedItemIEs> *lst = e2obj.getIEs().get_actionsAdmitted_list().get_list();
        	  for(int i=0; i< count; i++){
        		  std::cout<< "Action ID:::" << (*lst)[i].get_ricActionID() << std::endl;

        	  }
        	  int count2 = e2obj.getIEs().get_actionsNotAdmitted_list().get_list_count() ;
        	  std::vector<RICactionNotAdmittedList::RICactionNotAdmittedItemIEs> *lst2 = e2obj.getIEs().get_actionsNotAdmitted_list().get_list();
        	  for(int i=0; i< count2; i++){
        	        std::cout<< "Action ID:::" << (*lst2)[i].get_ricActionID() << std::endl;
        	        std::cout<< "Cause:::" << (*lst2)[i].get_ricCause() << std::endl;
        	        std::cout<< "Sub Cause:::" << (*lst2)[i].get_ricSubCause() << std::endl;
        	   }

          } catch(const char* e){
          		decode_status = false;
          		std::cout << "Error Message: " << e << std::endl;
      	}


      	ASSERT_TRUE(decode_status);

}

TEST(E2AP, SubFailureEncode){
	unsigned char buff[512];
	size_t buf_len = 512;

	CriticalityDiagnosticsIEs crIEs1;
	crIEs1.set_ieID(1);
	crIEs1.set_typeOferror(1);
	crIEs1.set_iEcriticality(2);

	CriticalityDiagnosticsIEs crIEs2;
	crIEs2.set_ieID(1);
	crIEs2.set_typeOferror(1);
	crIEs2.set_iEcriticality(1);



	E2APCriticalityDiagnostics critObj;
	critObj.set_procedureCode(10);
	critObj.set_procedureCriticality(1);
	critObj.set_ricInstanceID(101);
	critObj.set_ricRequestorID(102);
	critObj.set_triggeringMessage(1);
	critObj.add(crIEs1);
	critObj.add(crIEs2);

	RICactionNotAdmittedList::RICactionNotAdmittedItemIEs tmp2;
	tmp2.set_ricActionID(2);
	tmp2.set_ricCause(1);
	tmp2.set_ricSubCause(3);
	RICactionNotAdmittedList notadmitlst;
	notadmitlst.add(tmp2);
	notadmitlst.add(RICactionNotAdmittedList::RICactionNotAdmittedItemIEs().set_ricActionID(2).set_ricCause(1).set_ricSubCause(1));

	E2APSubscriptionFailure::SubscriptionFailureIEs  infoObj;
	infoObj.set_ranFunctionID(1);
	infoObj.set_ricRequestorID(1);
	infoObj.set_ricInstanceID(1);
	infoObj.set_actionsNotAdmitted_list(notadmitlst);
	infoObj.set_critical_diagnostic(critObj);


	E2APSubscriptionFailure  subObj(infoObj);

	bool res = subObj.encode(buff, &buf_len);
	if(!res)
	{
		std::cout << "Subscription Failure Error Message" << subObj.get_error() << std::endl;
	}
	ASSERT_TRUE(res);
	FILE * pFile;
	pFile = fopen ("subfailure1.per","w");
	if (pFile!=NULL)
	{
				  fwrite (buff , sizeof(char), buf_len, pFile);
				  fclose (pFile);
	 }

}

TEST(E2AP, SubFailureDecode)
{
       sleep(10);
	   unsigned char e2ap_buff[1024];
       char filename[100] = "subfailure1.per";
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
        	  E2APSubscriptionFailure e2obj(&e2ap_buff[0],&e2ap_buff_size);

        	  std::cout << "RAN Function ID: " << e2obj.getIEs().get_ranFunctionID() << std::endl;
        	  std::cout << "Requestor ID: " << e2obj.getIEs().get_ricRequestorID() << std::endl;
        	  std::cout << "Instance ID :" << e2obj.getIEs().get_ricInstanceID() << std::endl;
        	  int count = e2obj.getIEs().get_critical_diagnostic()->get_list_count() ;
        	  std::vector<CriticalityDiagnosticsIEs> *lst = e2obj.getIEs().get_critical_diagnostic()->get_list();
        	  for(int i=0; i< count; i++){
        	  		  std::cout<< "Typ Error:::" << (*lst)[i].get_typeOferror() << std::endl;
        	          std::cout<< "iEcriticality:::" << (*lst)[i].get_iEcriticality() << std::endl;
        	          std::cout<< "ieID:::" << (*lst)[i].get_ieID() << std::endl;
        	   }

        	  int count2 = e2obj.getIEs().get_actionsNotAdmitted_list().get_list_count() ;
        	  std::vector<RICactionNotAdmittedList::RICactionNotAdmittedItemIEs> *lst2 = e2obj.getIEs().get_actionsNotAdmitted_list().get_list();
        	  for(int i=0; i< count2; i++){
        	        std::cout<< "Action ID:::" << (*lst2)[i].get_ricActionID() << std::endl;
        	        std::cout<< "Cause:::" << (*lst2)[i].get_ricCause() << std::endl;
        	        std::cout<< "Sub Cause:::" << (*lst2)[i].get_ricSubCause() << std::endl;
        	   }

          } catch(const char* e){
          		decode_status = false;
          		std::cout << "Error Message: " << e << std::endl;
      	}


      	ASSERT_TRUE(decode_status);

}

#endif /* TEST_TEST_E2AP_H_ */
