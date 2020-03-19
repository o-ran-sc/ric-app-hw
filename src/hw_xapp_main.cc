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
 * hw_xapp_main.cc
 * Created on: Dec, 2019
 * Author: Shraboni Jana
 */

#include "xapp.hpp"
#include "subscription_request.hpp"
#include "xapp_sdl.hpp"

void signalHandler( int signum ) {
   cout << "Interrupt signal (" << signum << ") received.\n";
   exit(signum);
}

int main(int argc, char *argv[]){

	//get configuration
	XappSettings config;
	//change the priority depending upon application requirement
	config.loadDefaultSettings();
	config.loadEnvVarSettings();
	config.loadCmdlineSettings(argc, argv);

	//getting the listening port and xapp name info
	std::string  port = config[XappSettings::SettingName::HW_PORTS];
	std::string  name = config[XappSettings::SettingName::XAPP_NAME];


	//initialize rmr
	std::unique_ptr<XappRmr> rmr;
	rmr = std::make_unique<XappRmr>(name,port);
	rmr->xapp_rmr_init();

	//Register signal handler to stop
	signal(SIGINT, signalHandler);
	signal(SIGTERM, signalHandler);

	//Test SDL.
	XappSDL sdl = XappSDL("hw-xapp");

	//Initiate the Xapp functionality
	std::unique_ptr<Xapp> hw_xapp = std::make_unique<Xapp>(std::ref(config), std::ref(*rmr),std::ref(sdl));


	//define the startup mode.
	hw_xapp->startup();

	//Register Callback Handlers
	//Register E2 Msg Handlers - Subscription/Indication.
	//Register A1 Msg Handlers.
	//Register Callback Handlers


	//start the receiver thread listening at HW_PORT
	//currently only one receiver thread. In total how many receiver threads depend on the xapp developer.
	//Register all the handlers required and start the receiver

	//register_msgproc(RIC_SUB_RESP, sub_handler);
	//register_msgproc(RIC_SUB_DEL_RESP, sub_handler);
	//register_msgproc(RIC_SUB_FAILURE, sub_handler);


	hw_xapp->start_xapp_receiver();
 	sleep(5);


	//Delete all subscriptions if any based on Xapp Mode.
	//xapp->shutdown();

	 while(1){
	 			sleep(1);
	 		 }

	return 0;
}



