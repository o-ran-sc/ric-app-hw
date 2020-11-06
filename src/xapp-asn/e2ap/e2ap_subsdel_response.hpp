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
 * e2ap_subsdel_response.hpp
 *
 *  Created on: Oct 28, 2020
 *      Author: Shraboni Jana
 */
/*-- **************************************************************
--
-- RIC SUBSCRIPTION DELETE RESPONSE
--
-- **************************************************************
RICsubscriptionDeleteResponse ::= SEQUENCE {
        protocolIEs                                     ProtocolIE-Container    {{RICsubscriptionDeleteResponse-IEs}},
        ...
}

RICsubscriptionDeleteResponse-IEs E2AP-PROTOCOL-IES ::= {
        { ID id-RICrequestID                            CRITICALITY reject      TYPE RICrequestID                                       PRESENCE mandatory      }|
        { ID id-RANfunctionID                           CRITICALITY reject      TYPE RANfunctionID                                      PRESENCE mandatory      },
        ...
}
*/
#ifndef SRC_XAPP_ASN_E2AP_E2AP_SUBSDEL_RESPONSE_HPP_
#define SRC_XAPP_ASN_E2AP_E2AP_SUBSDEL_RESPONSE_HPP_

#include <mdclog/mdclog.h>
#include <vector>
#include <sstream>
#include <memory>
#include <mdclog/mdclog.h>
#include <asn_application.h>
#include <E2AP-PDU.h>
#include <SuccessfulOutcome.h>
#include <RICsubscriptionDeleteResponse.h>
#include <ProtocolIE-Field.h>
#include "e2ap_consts.hpp"
class E2APSubscriptionDeleteResponse {
public:

	class SubscriptionDeleteResponseIEs{
	private:
		 long int ricRequestorID, ricInstanceID, ranFunctionID;

	public:
		 SubscriptionDeleteResponseIEs(void):ricRequestorID(0), ricInstanceID(0),ranFunctionID(0){};
		 SubscriptionDeleteResponseIEs& set_ricRequestorID(long int req_id){ricRequestorID = req_id; return *this;};
		 SubscriptionDeleteResponseIEs& set_ranFunctionID(long int func_id){ranFunctionID = func_id; return *this;};
		 SubscriptionDeleteResponseIEs& set_ricInstanceID(long int inst_id){ricInstanceID = inst_id; return *this;};

		 long int get_ricInstanceID(){return this->ricInstanceID;};
		 long int get_ricRequestorID(){return this->ricRequestorID;};
		 long int get_ranFunctionID(){return this->ranFunctionID;};


	};

	E2APSubscriptionDeleteResponse(SubscriptionDeleteResponseIEs&);
	E2APSubscriptionDeleteResponse(unsigned char *, size_t *);
	~E2APSubscriptionDeleteResponse();
	bool encode(unsigned char *, size_t * );
	bool decode(unsigned char*, size_t *);
	std::string  get_error (void) const {return _error_string ;};
	SubscriptionDeleteResponseIEs& getIEs(){ return *_responseIEs.get();};
private:

	bool setfields(SuccessfulOutcome_t *);

	SuccessfulOutcome_t * _successMsg;
	E2AP_PDU_t * _e2ap_pdu_obj;
	RICsubscriptionDeleteResponse_IEs_t * IE_array;

	std::unique_ptr<SubscriptionDeleteResponseIEs> _responseIEs;

	std::string _error_string;
    char _errbuf[ERR_LEN];
	size_t _errbuf_len = ERR_LEN;

};





#endif /* SRC_XAPP_ASN_E2AP_E2AP_SUBSDEL_RESPONSE_HPP_ */
