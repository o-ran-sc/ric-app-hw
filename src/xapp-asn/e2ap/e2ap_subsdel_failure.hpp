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
 * e2ap_subsdel_failure.hpp
 *
 *  Created on: Oct 29, 2020
 *      Author: Shraboni Jana
 */
/*-- **************************************************************
--
-- RIC SUBSCRIPTION DELETE FAILURE
--
-- **************************************************************
RICsubscriptionDeleteFailure ::= SEQUENCE {
        protocolIEs                                     ProtocolIE-Container    {{RICsubscriptionDeleteFailure-IEs}},
        ...
}

RICsubscriptionDeleteFailure-IEs E2AP-PROTOCOL-IES ::= {
        { ID id-RICrequestID                            CRITICALITY reject      TYPE RICrequestID                                       PRESENCE mandatory      }|
        { ID id-RANfunctionID                           CRITICALITY reject      TYPE RANfunctionID                                      PRESENCE mandatory      }|
        { ID id-Cause                                           CRITICALITY ignore      TYPE Cause                                                      PRESENCE mandatory      }|
        { ID id-CriticalityDiagnostics          CRITICALITY ignore      TYPE CriticalityDiagnostics                     PRESENCE optional       },
        ...
}
*/
#ifndef SRC_XAPP_ASN_E2AP_E2AP_SUBSDEL_FAILURE_HPP_
#define SRC_XAPP_ASN_E2AP_E2AP_SUBSDEL_FAILURE_HPP_

#pragma once
#include <mdclog/mdclog.h>
#include <vector>
#include <sstream>
#include <memory>
#include <iostream>
#include <mdclog/mdclog.h>
#include <asn_application.h>
#include <E2AP-PDU.h>
#include <UnsuccessfulOutcome.h>
#include <RICsubscriptionDeleteFailure.h>
#include <CriticalityDiagnostics.h>
#include <CriticalityDiagnostics-IE-Item.h>
#include <CriticalityDiagnostics-IE-List.h>
#include <ProtocolIE-Field.h>
#include "e2ap_criticality_diagnostics.hpp"
#include "e2ap_consts.hpp"

class E2APSubscriptionDeleteFailure{
public:

	class SubscriptionDeleteFailureIEs{
	private:
		 long int ricRequestorID, ricInstanceID, ranFunctionID, ricCause, ricSubCause;
		 E2APCriticalityDiagnostics *critical_diagnostic;
		 bool is_criticality_diagnostic;
	public:
		 SubscriptionDeleteFailureIEs(void):ricRequestorID(0), ricInstanceID(0),ranFunctionID(0), ricCause(0), ricSubCause(0),critical_diagnostic(NULL), is_criticality_diagnostic(false){};
		 SubscriptionDeleteFailureIEs& set_ricRequestorID(long int req_id){ricRequestorID = req_id; return *this;};
		 SubscriptionDeleteFailureIEs& set_ranFunctionID(long int func_id){ranFunctionID = func_id; return *this;};
		 SubscriptionDeleteFailureIEs& set_ricInstanceID(long int inst_id){ricInstanceID = inst_id; return *this;};
		 SubscriptionDeleteFailureIEs& set_ricCause(long int cause){ricCause = cause; return *this;};
		 SubscriptionDeleteFailureIEs& set_ricSubCause(long int subcause){ricSubCause = subcause; return *this;};
		 SubscriptionDeleteFailureIEs& set_critical_diagnostic(E2APCriticalityDiagnostics &obj){this->is_criticality_diagnostic = true; critical_diagnostic = &obj; return *this;};

		 long int get_ricInstanceID(){return this->ricInstanceID;};
		 long int get_ricRequestorID(){return this->ricRequestorID;};
		 long int get_ranFunctionID(){return this->ranFunctionID;};
		 long int get_ricCause(){return this->ricCause;};
		 long int get_ricSubCause(){return this->ricSubCause;};
		 E2APCriticalityDiagnostics* get_critical_diagnostic(){return (this->critical_diagnostic);};
		 bool get_is_criticality_diagnostic(){return this->is_criticality_diagnostic;}

	};
	E2APSubscriptionDeleteFailure(SubscriptionDeleteFailureIEs&);
	E2APSubscriptionDeleteFailure(unsigned char *, size_t *);
	~E2APSubscriptionDeleteFailure();
	bool encode(unsigned char *, size_t * );
	bool decode(unsigned char*, size_t *);
	std::string  get_error (void) const {return _error_string ;};
	SubscriptionDeleteFailureIEs& getIEs(){ return *_failureIEs.get();};

private:

	bool setfields(UnsuccessfulOutcome_t *);
	UnsuccessfulOutcome_t * _unsuccessMsg;
	E2AP_PDU_t * _e2ap_pdu_obj;
	RICsubscriptionDeleteFailure_IEs_t * IE_array;
	CriticalityDiagnostics_IE_Item_t *CD_array;
	std::unique_ptr<SubscriptionDeleteFailureIEs> _failureIEs;
	E2APCriticalityDiagnostics _cdWrapperIEs;

	std::string _error_string;
    char _errbuf[ERR_LEN];
	size_t _errbuf_len = ERR_LEN;

};








#endif /* SRC_XAPP_ASN_E2AP_E2AP_SUBSDEL_FAILURE_HPP_ */
