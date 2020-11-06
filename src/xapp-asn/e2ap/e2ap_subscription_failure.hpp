/*
 * e2ap_subscription_failure.hpp
 *
 *  Created on: Nov 5, 2020
 *      Author: Shraboni Jana
 */

#ifndef SRC_XAPP_ASN_E2AP_E2AP_SUBSCRIPTION_FAILURE_HPP_
#define SRC_XAPP_ASN_E2AP_E2AP_SUBSCRIPTION_FAILURE_HPP_

/*
 * -- **************************************************************
--
-- RIC SUBSCRIPTION FAILURE
--
-- **************************************************************
RICsubscriptionFailure ::= SEQUENCE {
	protocolIEs					ProtocolIE-Container	{{RICsubscriptionFailure-IEs}},
	...
}

RICsubscriptionFailure-IEs E2AP-PROTOCOL-IES ::= {
	{ ID id-RICrequestID				CRITICALITY reject	TYPE RICrequestID					PRESENCE mandatory	}|
	{ ID id-RANfunctionID				CRITICALITY reject	TYPE RANfunctionID					PRESENCE mandatory	}|
	{ ID id-RICactions-NotAdmitted		CRITICALITY reject	TYPE RICaction-NotAdmitted-List		PRESENCE mandatory	}|
	{ ID id-CriticalityDiagnostics		CRITICALITY ignore	TYPE CriticalityDiagnostics			PRESENCE optional	},
	...
}
 */

#include <mdclog/mdclog.h>
#include <vector>
#include <iostream>
#include <sstream>
#include <memory>
#include <E2AP-PDU.h>
#include <UnsuccessfulOutcome.h>
#include <ProtocolIE-Field.h>
#include <ProcedureCode.h>
#include <CriticalityDiagnostics.h>
#include <CriticalityDiagnostics-IE-Item.h>
#include <CriticalityDiagnostics-IE-List.h>
#include "e2ap_consts.hpp"
#include "e2ap_action.hpp"
#include "e2ap_criticality_diagnostics.hpp"
class E2APSubscriptionFailure {
public:

	class SubscriptionFailureIEs{
		private:
			 long int ricRequestorID, ricInstanceID, ranFunctionID;
			 RICactionNotAdmittedList *ricActionNotAdmittedlst;
			 bool is_ricActionNotAdmitlst;
			 E2APCriticalityDiagnostics *critical_diagnostic;
			 bool is_criticality_diagnostic;
		public:
			 SubscriptionFailureIEs(void):ricRequestorID(0), ricInstanceID(0),ranFunctionID(0),ricActionNotAdmittedlst(0),is_ricActionNotAdmitlst(false),critical_diagnostic(NULL), is_criticality_diagnostic(false){};

			 SubscriptionFailureIEs& set_ricRequestorID(long int req_id){ricRequestorID = req_id; return *this;};
			 SubscriptionFailureIEs& set_ranFunctionID(long int func_id){ranFunctionID = func_id; return *this;};
			 SubscriptionFailureIEs& set_ricInstanceID(long int inst_id){ricInstanceID = inst_id; return *this;};
			 SubscriptionFailureIEs& set_actionsNotAdmitted_list(RICactionNotAdmittedList &lstobj){ricActionNotAdmittedlst= &lstobj; is_ricActionNotAdmitlst = true; return *this;};
			 SubscriptionFailureIEs& set_critical_diagnostic(E2APCriticalityDiagnostics &obj){this->is_criticality_diagnostic = true; critical_diagnostic = &obj; return *this;};

			 long int get_ricInstanceID(){return this->ricInstanceID;};
			 long int get_ricRequestorID(){return this->ricRequestorID;};
			 long int get_ranFunctionID(){return this->ranFunctionID;};
			 bool get_is_ricActionNotAdmitlst(){return this->is_ricActionNotAdmitlst;};
			 RICactionNotAdmittedList& get_actionsNotAdmitted_list()  {return *(this->ricActionNotAdmittedlst);};
			 E2APCriticalityDiagnostics* get_critical_diagnostic(){return (this->critical_diagnostic);};
			 bool get_is_criticality_diagnostic(){return this->is_criticality_diagnostic;}
	};

	E2APSubscriptionFailure(SubscriptionFailureIEs&);
	E2APSubscriptionFailure(unsigned char *, size_t *);
	~E2APSubscriptionFailure();
	bool encode(unsigned char *, size_t * );
	bool decode(unsigned char*, size_t *);
	std::string  get_error (void) const {return _error_string ;};
	SubscriptionFailureIEs& getIEs(){ return *_failureIEs.get();};


private:

	bool setfields(UnsuccessfulOutcome_t *);

	UnsuccessfulOutcome_t * _unsuccessMsg;
	E2AP_PDU_t * _e2ap_pdu_obj;
	RICsubscriptionFailure_IEs_t * IE_array;
	RICaction_NotAdmitted_ItemIEs_t * ie_not_admitted_list;

	RICactionNotAdmittedList _notadmitlst;
	E2APCriticalityDiagnostics _cdWrapperIEs;
	std::unique_ptr<SubscriptionFailureIEs> _failureIEs;
	CriticalityDiagnostics_IE_Item_t *CD_array;


	std::string _error_string;
    char _errbuf[ERR_LEN];
	size_t _errbuf_len = ERR_LEN;

};



#endif /* SRC_XAPP_ASN_E2AP_E2AP_SUBSCRIPTION_FAILURE_HPP_ */
