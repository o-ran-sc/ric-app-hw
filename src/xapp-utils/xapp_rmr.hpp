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

#ifndef XAPP_RMR_XAPP_RMR_H_
#define XAPP_RMR_XAPP_RMR_H_


#ifdef __GNUC__
#define likely(x)  __builtin_expect((x), 1)
#define unlikely(x) __builtin_expect((x), 0)
#else
#define likely(x) (x)
#define unlikely(x) (x)
#endif

#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <error.h>
#include <assert.h>
#include <thread>
#include <functional>
#include <map>
#include <mutex>
#include <sys/epoll.h>
#include <rmr/rmr.h>
#include <rmr/RIC_message_types.h>
#include <mdclog/mdclog.h>
#include <vector>

typedef struct{
	struct timespec ts;
	int32_t message_type;
	int32_t state;
	int32_t payload_length;
	unsigned char xid[RMR_MAX_XID]; //space for user transaction id.
	unsigned char sid[RMR_MAX_SID]; //sender ID for return to sender needs.(ACKS required)
	unsigned char src[RMR_MAX_SRC]; //name of the sender (source)
}  xapp_rmr_header;


class XappRmr{
private:
	std::string _xapp_name;
	std::string _proto_port;
	int _nattempts;
	bool _rmr_is_ready;
	void* _xapp_rmr_ctx;
	rmr_mbuf_t*		_xapp_send_buff;					// send buffer
	rmr_mbuf_t*		_xapp_received_buff;					// received buffer


public:

	XappRmr(std::string, std::string, int rmrattempts=10);
	~XappRmr(void);
	void xapp_rmr_init(void);

	template <class MessageProcessor>
	void xapp_rmr_receive(MessageProcessor&&, XappRmr *parent);
	bool xapp_rmr_send(xapp_rmr_header*, void*);
	bool xapp_rmr_rts();

};

//RMR receive
template <class MessageProcessor>
void XappRmr::xapp_rmr_receive(MessageProcessor&& msgproc, XappRmr *parent){
		char*	listen_port;

		if( (listen_port = getenv( "RMR_RCV_PORT" )) == NULL ) {
			mdclog_write(MDCLOG_ERR,"No Listening port assigned, file= %s, line=%d",__FILE__,__LINE__);
		}

		if(!_rmr_is_ready){
			mdclog_write( MDCLOG_ERR, "RMR Shows Not Ready in RECEIVER, file= %s, line=%d ",__FILE__,__LINE__);
			return;
		}

		  while( 1 ) {
			  parent->_xapp_received_buff = rmr_rcv_msg( parent->_xapp_rmr_ctx, parent->_xapp_received_buff );						// block until one arrives
			if( parent->_xapp_received_buff->mtype < 0 || parent->_xapp_received_buff->state != RMR_OK ) {
				mdclog_write(MDCLOG_ERR, "bad msg:  state=%d  errno=%d, file= %s, line=%d", parent->_xapp_received_buff->state, errno, __FILE__,__LINE__ );
				return;
			} else {
				std::cout << "The Message Received is:" << (char*)parent->_xapp_received_buff->payload <<std::endl;
				std::cout << "The Message Received Type is:" << _xapp_received_buff->mtype <<std::endl;
				_xapp_send_buff = msgproc(_xapp_received_buff);
				if(_xapp_send_buff !=NULL)
					xapp_rmr_rts();
				//sleep(10);
				//_xapp_received_buff = NULL;

			}

		}
	    return;
}

#endif /* XAPP_RMR_XAPP_RMR_H_ */
