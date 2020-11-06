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
 * e2ap_subscription_response.hpp
 *
 *  Created on: Nov 1, 2020
 *      Author: Shraboni Jana
 */

#ifndef SRC_XAPP_ASN_E2AP_SUBSCRIPTION_RESPONSE_HPP_
#define SRC_XAPP_ASN_E2AP_SUBSCRIPTION_RESPONSE_HPP_
/*-- **************************************************************
--
-- RIC SUBSCRIPTION RESPONSE
--
-- **************************************************************
RICsubscriptionResponse ::= SEQUENCE {
	protocolIEs					ProtocolIE-Container{{RICsubscriptionResponse-IEs}},
	...
}

RICsubscriptionResponse-IEs E2AP-PROTOCOL-IES ::= {
	{ ID id-RICrequestID				CRITICALITY reject		TYPE RICrequestID				PRESENCE mandatory } |
	{ ID id-RANfunctionID				CRITICALITY reject		TYPE RANfunctionID				PRESENCE mandatory } |
	{ ID id-RICactions-Admitted			CRITICALITY reject		TYPE RICaction-Admitted-List	PRESENCE mandatory } |
	{ ID id-RICactions-NotAdmitted		CRITICALITY reject		TYPE RICaction-NotAdmitted-List	PRESENCE optional },
	...
}



RICaction-Admitted-List ::= SEQUENCE (SIZE(1..maxofRICactionID)) OF ProtocolIE-SingleContainer{{RICaction-Admitted-ItemIEs}}

RICaction-Admitted-ItemIEs E2AP-PROTOCOL-IES ::= {
	{ ID id-RICaction-Admitted-Item		CRITICALITY ignore		TYPE RICaction-Admitted-Item	PRESENCE mandatory },
	...
}

RICaction-Admitted-Item ::= SEQUENCE {
	ricActionID					RICactionID,
	...
}

RICaction-NotAdmitted-List ::= SEQUENCE (SIZE(0..maxofRICactionID)) OF ProtocolIE-SingleContainer { {RICaction-NotAdmitted-ItemIEs} }

RICaction-NotAdmitted-ItemIEs E2AP-PROTOCOL-IES ::= {
	{ ID id-RICaction-NotAdmitted-Item	CRITICALITY ignore	TYPE RICaction-NotAdmitted-Item		PRESENCE mandatory },
	...
}

RICaction-NotAdmitted-Item ::= SEQUENCE {
	ricActionID					RICactionID,
	cause						Cause,
	...
}*/
#include <mdclog/mdclog.h>
#include <vector>
#include <iostream>
#include <sstream>
#include <memory>
#include <E2AP-PDU.h>
#include <SuccessfulOutcome.h>
#include <ProtocolIE-Field.h>
#include <ProcedureCode.h>
#include "e2ap_consts.hpp"
#include "e2ap_action.hpp"
class E2APSubscriptionResponse {
public:

	class SubscriptionResponseIEs{
		private:
			 long int ricRequestorID, ricInstanceID, ranFunctionID;
			 RICactionNotAdmittedList *ricActionNotAdmittedlst;
			 RICactionAdmittedList *ricActionAdmittedlst;
			 bool is_ricActionAdmitlst;
			 bool is_ricActionNotAdmitlst;
		public:
			 SubscriptionResponseIEs(void):ricRequestorID(0), ricInstanceID(0),ranFunctionID(0),ricActionNotAdmittedlst(0),ricActionAdmittedlst(0),is_ricActionAdmitlst(false),is_ricActionNotAdmitlst(false){};

			 SubscriptionResponseIEs& set_ricRequestorID(long int req_id){ricRequestorID = req_id; return *this;};
			 SubscriptionResponseIEs& set_ranFunctionID(long int func_id){ranFunctionID = func_id; return *this;};
			 SubscriptionResponseIEs& set_ricInstanceID(long int inst_id){ricInstanceID = inst_id; return *this;};
			 SubscriptionResponseIEs& set_actionsAdmitted_list(RICactionAdmittedList &lstobj){ricActionAdmittedlst= &lstobj;is_ricActionAdmitlst=true;  return *this;};
			 SubscriptionResponseIEs& set_actionsNotAdmitted_list(RICactionNotAdmittedList &lstobj){ricActionNotAdmittedlst= &lstobj; is_ricActionNotAdmitlst = true; return *this;};

			 long int get_ricInstanceID(){return this->ricInstanceID;};
			 long int get_ricRequestorID(){return this->ricRequestorID;};
			 long int get_ranFunctionID(){return this->ranFunctionID;};
			 bool get_is_ricActionNotAdmitlst(){return this->is_ricActionNotAdmitlst;};
			 bool get_is_ricActionAdmitlst(){return this->is_ricActionAdmitlst;};
			 RICactionAdmittedList& get_actionsAdmitted_list()  {return *(this->ricActionAdmittedlst);};
			 RICactionNotAdmittedList& get_actionsNotAdmitted_list()  {return *(this->ricActionNotAdmittedlst);};

	};

	E2APSubscriptionResponse(SubscriptionResponseIEs&);
	E2APSubscriptionResponse(unsigned char *, size_t *);
	~E2APSubscriptionResponse();
	bool encode(unsigned char *, size_t * );
	bool decode(unsigned char*, size_t *);
	std::string  get_error (void) const {return _error_string ;};
	SubscriptionResponseIEs& getIEs(){ return *_responseIEs.get();};


private:

	bool setfields(SuccessfulOutcome_t *);

	SuccessfulOutcome_t * _successMsg;
	E2AP_PDU_t * _e2ap_pdu_obj;
	RICsubscriptionResponse_IEs_t * IE_array;
	RICaction_Admitted_ItemIEs_t * ie_admitted_list;
	RICaction_NotAdmitted_ItemIEs_t * ie_not_admitted_list;

	RICactionAdmittedList _admitlst;
	RICactionNotAdmittedList _notadmitlst;
	std::unique_ptr<SubscriptionResponseIEs> _responseIEs;


	std::string _error_string;
    char _errbuf[ERR_LEN];
	size_t _errbuf_len = ERR_LEN;

};




#endif /* SRC_XAPP_ASN_E2AP_SUBSCRIPTION_RESPONSE_HPP_ */
