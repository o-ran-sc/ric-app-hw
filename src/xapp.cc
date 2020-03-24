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
 * xapp.cc
 *
 *  Created on: Mar, 2020
 *  Author: Shraboni Jana
 */

#include "xapp.hpp"

Xapp::Xapp(XappSettings &config, XappRmr &rmr){
	rmr_ref = &rmr;
	config_ref = &config;
	xapp_mutex = NULL;
	return;
}

Xapp::~Xapp(void){

	//Joining the threads
	int threadcnt = xapp_rcv_thread.size();
	for(int i=0; i<threadcnt; i++){
		if(xapp_rcv_thread[i].joinable())
			xapp_rcv_thread[i].join();
	}

	delete xapp_mutex;

};

void Xapp::init() {

	//get rnib information
	get_rnib_gnblist();


}
void Xapp::startup() {
	//send subscriptions and read A1 policies.
	startup_subscribe_requests();
	//startup_get_policies();
	return;
}

void Xapp::start_xapp_receiver(XappMsgHandler& mp_handler){
	//start a receiver thread. Can be multiple receiver threads for more than 1 listening port.

	xapp_mutex = new std::mutex();

	mdclog_write(MDCLOG_INFO,"Receiver Thread file= %s, line=%d",__FILE__,__LINE__);
	//std::unique_ptr<XappMsgHandler> mp_handler = std::make_unique<XappMsgHandler>();
	//auto mp_handler = _callbacks[0];
	std::lock_guard<std::mutex> guard(*xapp_mutex);
	std::thread th_recv([&](){ rmr_ref->xapp_rmr_receive(std::move(mp_handler), rmr_ref);});

	xapp_rcv_thread.push_back(std::move(th_recv));


	return;



}
void Xapp::shutdown(){

	return;

}


void Xapp::startup_subscribe_requests(void ){
   size_t data_size = ASN_BUFF_MAX_SIZE;
   unsigned char	data[data_size];

   for(auto &it: rnib_gnblist){
     int attempt = 0;
 	 XappMsgHandler msg;

 /*	 bool res_encode = msg.encode_subscription_request(data, &data_size);
 	 if(!res_encode) exit(0);*/
 	char *strMsg = "HelloWorld\0";
 	strncpy((char *)data,strMsg,strlen(strMsg));
 	data_size = sizeof(data);

 	 xapp_rmr_header rmr_header;
 	 rmr_header.message_type = RIC_SUB_RESP;
 	 rmr_header.payload_length = data_size;
     while(1){

     		 auto transmitter = std::bind(&XappRmr::xapp_rmr_send,rmr_ref, &rmr_header, (void*)data);
     		 transmitter(); //this will go to subscription manager.
     		 break;
     }
   }
}

void Xapp::startup_get_policies(void){

  int policy_id = HELLOWORLD_POLICY_ID;

  std::string policy_query = "{\"policy_id\":" + std::to_string(policy_id) + "}";
  unsigned char * message = (unsigned char *)calloc(policy_query.length(), sizeof(unsigned char));
  memcpy(message, policy_query.c_str(),  policy_query.length());
  xapp_rmr_header header;
  header.payload_length = policy_query.length();
  header.message_type = A1_POLICY_QUERY;
  mdclog_write(MDCLOG_INFO, "Sending request for policy id %d\n", policy_id);
  rmr_ref->xapp_rmr_send(&header, (void *)message);
  free(message);

}

void Xapp::set_rnib_gnblist(void) {

	   openSdl();
	   void *result = getListGnbIds();
	   if(result == NULL){
		    mdclog_write(MDCLOG_ERR, "ERROR: no data from getListGnbIds\n");
	        return;
	    }

	    mdclog_write(MDCLOG_INFO, "GNB List in R-NIB %s\n", (char*)result);

	    Document doc;
	    doc.Parse((char*)result);
	    assert(doc.HasMember("gnb_list"));
	    const Value& gnblist = doc["gnb_list"];
	    assert(gnblist.IsArray());
	    for (SizeType i = 0; i < gnblist.Size(); i++) // Uses SizeType instead of size_t
	    {
	    	assert(gnblist[i].IsObject());
	    	const Value& gnbobj = gnblist[i];
	    	assert(gnbobj.HasMember("inventory_name"));
	    	assert(gnbobj["inventory_name"].IsString());
	    	rnib_gnblist.push_back(gnbobj["inventory_name"].GetString());

	    }
	    closeSdl();
	    free(result);
	    return;

}






