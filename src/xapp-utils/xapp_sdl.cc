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
 */

/*
 * xapp_sdl.cc
 *
 *  Created on: Mar, 2020
 *  Author: Shraboni Jana
 */
#include "xapp_sdl.hpp"
/*need to work on the SDL FLow.
As per Matti
An xApp can use the SDL for two things:
- persisting state for itself (in case it fails and recovers)
- making information available for other xApps. The xApp would typically write using SDL directly. The consumer of the data could also use SDL directly or use an access library like in the case of the R-NIB.
*/
void XappSDL::insert_data(){
	//connecting to the Redis and generating a random key for namespace "hwxapp"

	   	 DataMap dmap;
	   	 char key[4]={'a','b','c'};
	   	 std::cout << "KEY: "<< key << std::endl;
		 Key k = key;
		 Data d;
		 uint8_t num = 101;
		 d.push_back(num);
		 dmap.insert({k,d});
		 sdl->set(ns, dmap);
}


