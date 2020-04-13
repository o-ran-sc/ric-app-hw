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

void signalHandler( int signum ) {
   cout << "Interrupt signal (" << signum << ") received.\n";
   exit(signum);
}

int main(int argc, char *argv[]){

	// Get the thread id
	std::thread::id my_id = std::this_thread::get_id();
	std::stringstream thread_id;
	std::stringstream ss;

	thread_id << my_id;

	mdclog_write(MDCLOG_INFO, "Starting thread %s",  thread_id.str().c_str());

	//get configuration
	XappSettings config;
	//change the priority depending upon application requirement
	config.loadDefaultSettings();
	config.loadEnvVarSettings();
	config.loadCmdlineSettings(argc, argv);

	//Register signal handler to stop
	signal(SIGINT, signalHandler);
	signal(SIGTERM, signalHandler);

	//getting the listening port and xapp name info
	std::string  port = config[XappSettings::SettingName::HW_PORTS];
	std::string  name = config[XappSettings::SettingName::XAPP_NAME];

	//initialize rmr
	std::unique_ptr<XappRmr> rmr;
	rmr = std::make_unique<XappRmr>(port);
	rmr->xapp_rmr_init();


	std::unique_ptr<Xapp> hw_xapp = std::make_unique<Xapp>(std::ref(config),std::ref(*rmr));
	//hw_xapp->startup();

	std::unique_ptr<XappMsgHandler> mp_handler = std::make_unique<XappMsgHandler>(config[XappSettings::SettingName::XAPP_ID]);
	 //hw_xapp->register_handler(std::bind(&XappMsgHandler::operator (),mp_handler.get(),std::placeholders::_1,std::placeholders::_2));
	hw_xapp->start_xapp_receiver(std::ref(*mp_handler));

	//Delete all subscriptions if any based on Xapp Mode.
	//xapp->shutdown();

 	while(1){
	 			sleep(1);
	 		 }

	return 0;
}



