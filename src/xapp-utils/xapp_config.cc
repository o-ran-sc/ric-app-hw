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
 * xapp_config.cc
 * Created on: 2019
 * Author: Ashwin Shridharan, Shraboni Jana
 */

#include "xapp_config.hpp"

string& XappSettings::operator[](const SettingName& theName){
    return theSettings[theName];
}

void XappSettings::loadCmdlineSettings(int argc, char **argv){

	   // Parse command line options to over ride
	  static struct option long_options[] =
	    {
	    		{"xappname", required_argument, 0, 'n'},
				{"xappid", required_argument, 0, 'x'},
				{"port", required_argument, 0, 'p'},
				{"threads", required_argument,    0, 't'},
				{"a1-schema", required_argument, 0, 'a'},
				{"ves-schema", required_argument, 0, 'v'},
				{"ves-url", required_argument, 0, 'u'},
				{"ves-interval", required_argument, 0, 'i'},
				{"gNodeB", required_argument, 0, 'g'},
				{"opmode", required_argument, 0, 'c'}

	    };


	   while(1) {

		int option_index = 0;
		char c = getopt_long(argc, argv, "n:p:t:s:g:a:v:u:i:c:x:", long_options, &option_index);

	        if(c == -1){
		    break;
	         }

		switch(c)
		  {

		  case 'n':
		    theSettings[XAPP_NAME].assign(optarg);
		    break;

		  case 'p':
		    theSettings[HW_PORTS].assign(optarg);
		    break;

		  case 't':
			theSettings[THREADS].assign(optarg);
		    mdclog_write(MDCLOG_INFO, "Number of threads set to %s from command line e\n", theSettings[THREADS].c_str());
		    break;

		  case 'a':
			theSettings[A1_SCHEMA_FILE].assign(optarg);
		    mdclog_write(MDCLOG_INFO, "Schema file set to %s from command line ", theSettings[A1_SCHEMA_FILE].c_str());
		    break;

		  case 'v':
		    theSettings[VES_SCHEMA_FILE].assign(optarg);
		    mdclog_write(MDCLOG_INFO, "VES Schema file set to %s from command line ", theSettings[VES_SCHEMA_FILE].c_str());
		    break;

		  case 'c':
		    theSettings[OPERATING_MODE].assign(optarg);
		    mdclog_write(MDCLOG_INFO, "Operating mode set from command line to %s\n", theSettings[OPERATING_MODE].c_str());
		    break;

		  case 'u':
		    theSettings[VES_COLLECTOR_URL].assign(optarg);
		    mdclog_write(MDCLOG_INFO, "VES collector url set to %s from command line ", theSettings[VES_COLLECTOR_URL].c_str());
		    break;

		  case 'x':
		    theSettings[XAPP_ID].assign(optarg);
		    mdclog_write(MDCLOG_INFO, "XAPP ID set to  %s from command line ", theSettings[XAPP_ID].c_str());
		    break;

		  case 'i':
			theSettings[VES_MEASUREMENT_INTERVAL].assign(optarg);
		    mdclog_write(MDCLOG_INFO, "Measurement interval set to %s from command line\n", theSettings[VES_MEASUREMENT_INTERVAL].c_str());
		    break;

		  case 'g':
		    theSettings[GNODEB].assign(optarg);
		    mdclog_write(MDCLOG_INFO, "gNodeB List set to %s from command line ", theSettings[GNODEB].c_str());
		    break;

		  case 'h':
		    usage(argv[0]);
		    exit(0);

		  default:
		    usage(argv[0]);
		    exit(1);
		  }
	   };

}

void XappSettings::loadDefaultSettings(){


		 if(theSettings[XAPP_NAME].empty()){
		  theSettings[XAPP_NAME] = DEFAULT_PORT;
		  }

	  	  if(theSettings[XAPP_ID].empty()){
	  		  theSettings[XAPP_ID] = DEFAULT_PORT;
	  	  }
	  	  if(theSettings[LOG_LEVEL].empty()){
	  		  theSettings[LOG_LEVEL] = DEFAULT_LOG_LEVEL;
	  	  }
	  	  if(theSettings[HW_PORTS].empty()){
	  		  theSettings[HW_PORTS] = DEFAULT_PORT;
	  	  }
	  	  if(theSettings[MSG_MAX_BUFFER].empty()){
	  		  theSettings[MSG_MAX_BUFFER] = DEFAULT_BUFFER;
	  	  }

	  	  if(theSettings[A1_SCHEMA_FILE].empty()){
	  		  theSettings[A1_SCHEMA_FILE] = DEFAULT_A1_SCHEMA_FILE;
	  	  }

	  	  if(theSettings[VES_SCHEMA_FILE].empty()){
	  		  theSettings[VES_SCHEMA_FILE] = DEFAULT_VES_SCHEMA_FILE;
	  	  }

	  	  if(theSettings[VES_COLLECTOR_URL].empty()){
	  		  theSettings[VES_COLLECTOR_URL] = DEFAULT_VES_COLLECTOR_URL;
	  	  }

	  	 if(theSettings[VES_MEASUREMENT_INTERVAL].empty()){
	  		  theSettings[VES_MEASUREMENT_INTERVAL] = DEFAULT_VES_MEASUREMENT_INTERVAL;
	  	  }

	 	 if(theSettings[GNODEB].empty()){
		  	  theSettings[GNODEB] = DEFAULT_GNODEB;
		  }

	 	 if(theSettings[OPERATING_MODE].empty()){
	 		  theSettings[OPERATING_MODE] = DEFAULT_OPERATING_MODE;
	 	  }

}

void XappSettings::loadEnvVarSettings(){

	  if (const char *env_xname = std::getenv("XAPP_NAME")){
		  theSettings[XAPP_NAME].assign(env_xname);
		  mdclog_write(MDCLOG_INFO,"Xapp Name set to %s from environment variable", theSettings[XAPP_NAME].c_str());
	  }
	  if (const char *env_xid = std::getenv("XAPP_ID")){
		   theSettings[XAPP_ID].assign(env_xid);
		   mdclog_write(MDCLOG_INFO,"Xapp ID set to %s from environment variable", theSettings[XAPP_ID].c_str());
	  }

	  if (const char *env_ports = std::getenv("HW_PORTS")){
		  theSettings[HW_PORTS].assign(env_ports);
	 	  mdclog_write(MDCLOG_INFO,"Ports set to %s from environment variable", theSettings[HW_PORTS].c_str());
	  }
	  if (const char *env_ports = std::getenv("MSG_MAX_BUFFER")){
	 		  theSettings[MSG_MAX_BUFFER].assign(env_ports);
	 	 	  mdclog_write(MDCLOG_INFO,"Ports set to %s from environment variable", theSettings[MSG_MAX_BUFFER].c_str());
	 	  }

	 if (const char *env_schema = std::getenv("A1_SCHEMA_FILE")){
		  theSettings[A1_SCHEMA_FILE].assign(env_schema);
		  mdclog_write(MDCLOG_INFO, "A1 Schema file set to %s from environment variable", theSettings[A1_SCHEMA_FILE].c_str());
	  }
	  if (const char *env_schema = std::getenv("VES_SCHEMA_FILE")){
		  theSettings[VES_SCHEMA_FILE].assign(env_schema);
		  mdclog_write(MDCLOG_INFO, "VES Schema file set to %s from environment variable", theSettings[VES_SCHEMA_FILE].c_str());
	  }
	  if (const char *env_schema = std::getenv("VES_COLLECTOR_URL")){
		  theSettings[VES_COLLECTOR_URL].assign(env_schema);
		  mdclog_write(MDCLOG_INFO, "VES Collector url set to %s from environment variable", theSettings[VES_COLLECTOR_URL].c_str());

	  }
	  if (const char *env_schema = std::getenv("VES_MEASUREMENT_INTERVAL")){
		  theSettings[VES_MEASUREMENT_INTERVAL].assign(env_schema);
		  mdclog_write(MDCLOG_INFO, "VES Measurement Interval set to %s from environment variable", theSettings[VES_MEASUREMENT_INTERVAL].c_str());
	  }

	  if (char *env_gnodeb = std::getenv("GNODEB")){
		  theSettings[GNODEB].assign(env_gnodeb);
		  mdclog_write(MDCLOG_INFO, "GNODEB file set to %s from environment variable", theSettings[GNODEB].c_str());
	  }


}

void XappSettings::usage(char *command){
	std::cout <<"Usage : " << command << " " << std::endl;
	std::cout <<" --name[-n] xapp_instance_name "<< std::endl;
    std::cout <<" --port[-p] port to listen on e.g tcp:4561  "<< std::endl;
    std::cout << "--threads[-t] number of listener threads "<< std::endl ;
    std::cout << "--a1-schema[-a] a1 schema file location" << std::endl;
    std::cout << "--ves-schema[-v] ves schema file location" << std::endl;
    std::cout << "--samples [-s]  samples file location with samples for all jsons" << std::endl;
    std::cout << "--ves-url [-u] ves collector url" << std::endl;
    std::cout << "--gNodeB[][-g] gNodeB" << std::endl;
    std::cout << "--interval[-i] measurement interval to send to ves collector (in seconds)" << std::endl;
    std::cout << "--opmode [-c] type of operatoring mode : either REPORT or CONTROL. In REPORT, does not send a control message back to gNodeB" << std::endl;
}
