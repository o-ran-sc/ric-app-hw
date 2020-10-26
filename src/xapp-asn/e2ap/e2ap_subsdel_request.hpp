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
 * e2ap_subsdel_request.hpp
 *
 *  Created on: Oct 23, 2020
 *      Author: Shraboni Jana
 */

/*-- **************************************************************
--
-- RIC SUBSCRIPTION DELETE REQUEST
--
-- **************************************************************
RICsubscriptionDeleteRequest ::= SEQUENCE {
        protocolIEs                                     ProtocolIE-Container    {{RICsubscriptionDeleteRequest-IEs}},
        ...
}

RICsubscriptionDeleteRequest-IEs E2AP-PROTOCOL-IES ::= {
        { ID id-RICrequestID                            CRITICALITY reject      TYPE RICrequestID                                       PRESENCE mandatory      }|
        { ID id-RANfunctionID                           CRITICALITY reject      TYPE RANfunctionID                                      PRESENCE mandatory      },
        ...
}*/
#ifndef SRC_XAPP_ASN_E2AP_E2AP_SUBSDEL_REQUEST_HPP_
#define SRC_XAPP_ASN_E2AP_E2AP_SUBSDEL_REQUEST_HPP_


#include <mdclog/mdclog.h>
#include <vector>
#include <sstream>
#include <memory>
#include <mdclog/mdclog.h>
#include <asn_application.h>
#include <E2AP-PDU.h>
#include <InitiatingMessage.h>
#include <RICsubscriptionDeleteRequest.h>
#include <ProtocolIE-Field.h>
#include "e2ap_consts.hpp"
class E2APSubscriptionDeleteRequest {
public:

	class SubscriptionDeleteRequestIEs{
	private:
		 long int ricRequestorID, ricInstanceID, ranFunctionID;

	public:
		 SubscriptionDeleteRequestIEs(void):ricRequestorID(0), ricInstanceID(0),ranFunctionID(0){};
 		 SubscriptionDeleteRequestIEs& set_ricRequestorID(long int req_id){ricRequestorID = req_id; return *this;};
		 SubscriptionDeleteRequestIEs& set_ranFunctionID(long int func_id){ranFunctionID = func_id; return *this;};
		 SubscriptionDeleteRequestIEs& set_ricInstanceID(long int inst_id){ricInstanceID = inst_id; return *this;};

		 long int get_ricInstanceID(){return this->ricInstanceID;};
		 long int get_ricRequestorID(){return this->ricRequestorID;};
		 long int get_ranFunctionID(){return this->ranFunctionID;};


	};

	E2APSubscriptionDeleteRequest(SubscriptionDeleteRequestIEs&);
	~E2APSubscriptionDeleteRequest();
	bool encode(unsigned char *, size_t * );
	std::string  get_error (void) const {return _error_string ;};
	SubscriptionDeleteRequestIEs& getIEs(){ return *_requestIEs.get();};
private:

	InitiatingMessage_t *initMsg;
	E2AP_PDU_t * e2ap_pdu_obj;
	RICsubscriptionDeleteRequest_IEs_t * IE_array;

	std::unique_ptr<SubscriptionDeleteRequestIEs> _requestIEs;
	std::string _error_string;
    char _errbuf[128];
	size_t _errbuf_len = 128;

	bool setfields(InitiatingMessage_t *);

};




#endif /* SRC_XAPP_ASN_E2AP_E2AP_SUBSDEL_REQUEST_HPP_ */
