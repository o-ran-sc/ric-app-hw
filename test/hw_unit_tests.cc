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
 * hw_unit_tests.cc
 *
 *  Created on: Mar, 2020
 *  Author: Shraboni Jana
 */

#include<iostream>
#include <stdlib.h>
#include<gtest/gtest.h>

#include "test_rmr.h"
#include "test_rnib.h"
#include "test_sdl.h"

using namespace std;

int main(int argc, char* argv[])
{
	char rmr_seed[80]="RMR_SEED_RT=../src/routes.txt";
	putenv(rmr_seed);

	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
