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


#include "xapp_rmr.hpp"

XappRmr::XappRmr(std::string xname, std::string port, int rmrattempts){

	_proto_port = port;
	_xapp_name = xname;
	_nattempts = rmrattempts;
	_xapp_rmr_ctx = NULL;
	_xapp_received_buff = NULL;
	_xapp_send_buff =NULL;
	_rmr_is_ready = false;

};

XappRmr::~XappRmr(void){

	// free memory
	if(_xapp_received_buff)
		rmr_free_msg(_xapp_received_buff);

	if(_xapp_send_buff)
		rmr_free_msg(_xapp_send_buff);

	if (_xapp_rmr_ctx){
		rmr_close(_xapp_rmr_ctx);
	}
};

//Get RMR Context.
void XappRmr::xapp_rmr_init(){

	// Initialize the RMR context
	_xapp_rmr_ctx = rmr_init(const_cast<char*>(_proto_port.c_str()), RMR_MAX_RCV_BYTES, RMRFL_NONE);

	if ( _xapp_rmr_ctx == NULL){
		mdclog_write(MDCLOG_ERR,"Error Initializing RMR, file= %s, line=%d",__FILE__,__LINE__);
	}
	while( ! rmr_ready(_xapp_rmr_ctx) ) {
		mdclog_write(MDCLOG_INFO,">>> waiting for RMR, file= %s, line=%d",__FILE__,__LINE__);
		sleep(1);
	}
	_rmr_is_ready = true;
	mdclog_write(MDCLOG_INFO,"RMR Context is Ready, file= %s, line=%d",__FILE__,__LINE__);

	return;

}

bool XappRmr::xapp_rmr_rts()
{
	_xapp_send_buff = rmr_realloc_payload( _xapp_send_buff, 128, false, false );  // ensure payload is large enough
	strncpy( (char*)_xapp_send_buff->payload, "OK\n", rmr_payload_size( _xapp_send_buff) );
	rmr_rts_msg(_xapp_rmr_ctx, _xapp_send_buff );
	_xapp_send_buff = NULL;
	return true;
}

//RMR Send with payload and header.
bool XappRmr::xapp_rmr_send(xapp_rmr_header *hdr, void *payload){

	if( _xapp_send_buff == NULL ) {
		_xapp_send_buff = rmr_alloc_msg(_xapp_rmr_ctx, RMR_DEF_SIZE);
	}


	_xapp_send_buff->mtype  = hdr->message_type;

	memcpy(_xapp_send_buff->payload, payload, hdr->payload_length);

	_xapp_send_buff->len = hdr->payload_length;
	if(!_rmr_is_ready) {
		mdclog_write(MDCLOG_ERR,"RMR Context is Not Ready in SENDER, file= %s, line=%d",__FILE__,__LINE__);
		return false;
	}
	if( _xapp_send_buff == NULL ) {
		return false;
	}


	while(_nattempts > 0){
		_xapp_send_buff = rmr_send_msg(_xapp_rmr_ctx,_xapp_send_buff);

		if(!_xapp_send_buff) {
			mdclog_write(MDCLOG_ERR,"Error In Sending Message , file= %s, line=%d",__FILE__,__LINE__);
			_nattempts--;
		}
		else if (_xapp_send_buff->state == RMR_OK){
			mdclog_write(MDCLOG_INFO,"The okay message is %d, file= %s, line=%d", RMR_OK, __FILE__,__LINE__);
			_nattempts = 0;
			return true;
		}
		else
		{
			mdclog_write(MDCLOG_INFO,"Need to retry RMR MSG NUM %d, file= %s, line=%d",_xapp_send_buff->state, __FILE__,__LINE__);
			_nattempts--;
		}
		sleep(1);
	}
	return false;
}

