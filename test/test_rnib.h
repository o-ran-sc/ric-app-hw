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
 * test_rnib.h
 *
 *  Created on: Mar 23, 2020
 *  Author: Shraboni Jana
 */

#ifndef TEST_TEST_RNIB_H_
#define TEST_TEST_RNIB_H_

#include<iostream>
#include<gtest/gtest.h>
#include "xapp.hpp"

using namespace std;

TEST(Xapp, getGNBlist)
{
	XappSettings config;
	XappRmr rmr("4560");

	Xapp hw_xapp(std::ref(config),std::ref(rmr));
	hw_xapp.set_rnib_gnblist();

	auto gnblist = hw_xapp.get_rnib_gnblist();
	int sz = gnblist.size();
	EXPECT_GE(sz,0);
	std::cout << "************gnb ids retrieved using R-NIB**************" << std::endl;
	for(int i = 0; i<sz; i++){
		std::cout << gnblist[i] << std::endl;
	}

}





#endif /* TEST_TEST_RNIB_H_ */
