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
 * e2ap_criticality_diagnostics.hpp
 *
 *  Created on: Oct 29, 2020
 *      Author: Shraboni Jana
 */
/*
-- **************************************************************
-- copied from 3GPP 38.413 NGAP IEs v15.5.0
-- note: ie-Extensions removed
-- **************************************************************
CriticalityDiagnostics ::= SEQUENCE {
        procedureCode                                   ProcedureCode                                                   OPTIONAL,
        triggeringMessage                               TriggeringMessage                                               OPTIONAL,
        procedureCriticality                    Criticality                                                             OPTIONAL,
        ricRequestorID                                  RICrequestID                                                    OPTIONAL,
        iEsCriticalityDiagnostics               CriticalityDiagnostics-IE-List                  OPTIONAL,
        ...
}

CriticalityDiagnostics-IE-List ::= SEQUENCE (SIZE(1..maxnoofErrors)) OF CriticalityDiagnostics-IE-Item

CriticalityDiagnostics-IE-Item ::= SEQUENCE {
        iECriticality           Criticality,
        iE-ID                           ProtocolIE-ID,
        typeOfError                     TypeOfError,
        ...
}
 *
 */

#ifndef SRC_XAPP_ASN_E2AP_E2AP_CRITICALITY_DIAGNOSTICS_HPP_
#define SRC_XAPP_ASN_E2AP_E2AP_CRITICALITY_DIAGNOSTICS_HPP_

#include <mdclog/mdclog.h>

#include <string>
#include <memory>
#include <sstream>
#include <vector>

#include "e2ap_consts.hpp"


class CriticalityDiagnosticsIEs{
			unsigned int typeOferror,iEcriticality;
			long int ieID;


		public:
			CriticalityDiagnosticsIEs():typeOferror(0), ieID(0), iEcriticality(0){};
			CriticalityDiagnosticsIEs& set_typeOferror(unsigned int typ_err){this->typeOferror = typ_err; return *this;};
			CriticalityDiagnosticsIEs& set_ieID(long int ieID){this->ieID = ieID; return *this;};
			CriticalityDiagnosticsIEs& set_iEcriticality(unsigned int crit){this->iEcriticality =crit; return *this;}

			unsigned int get_typeOferror(){return this->typeOferror;};
			unsigned int get_iEcriticality(){return this->iEcriticality;};
			long int get_ieID(){return this->ieID;};


};

class E2APCriticalityDiagnostics{

	private:

	 	long int procedureCode,ricRequestorID, ricInstanceID;
		unsigned int triggeringMessage,procedureCriticality;
		bool is_criticalityDiagnostics_list;
		std::unique_ptr<std::vector<CriticalityDiagnosticsIEs>> _ref_list;
		int _count_list;

	public:

	E2APCriticalityDiagnostics():procedureCode(0),ricRequestorID(0),ricInstanceID(0),triggeringMessage(0), procedureCriticality(0),is_criticalityDiagnostics_list(false)
	{ _ref_list = std::make_unique<std::vector<CriticalityDiagnosticsIEs>>(); _count_list = 0;};

	std::vector<CriticalityDiagnosticsIEs> * get_list() const {return _ref_list.get();};
	int get_list_count() {return _count_list;};

	void add(CriticalityDiagnosticsIEs &ieObj) { _ref_list.get()->push_back(ieObj); _count_list++;};
	void add(std::vector<CriticalityDiagnosticsIEs> &obj){ *(_ref_list.get()) = obj; _count_list = obj.size();};

	long int get_procedureCode(){return this->procedureCode;};
	long int get_ricRequestorID(){return this->ricRequestorID;};
	long int get_ricInstanceID(){return this->ricInstanceID;};
	unsigned int get_triggeringMessage(){return this->triggeringMessage;};
	unsigned int get_procedureCriticality(){return this->procedureCriticality;};
	bool get_is_criticalityDiagnostics_list(){if(this->get_list_count()>0) {is_criticalityDiagnostics_list = true;} return this->is_criticalityDiagnostics_list; };

	E2APCriticalityDiagnostics& set_procedureCode(long int proc_code){this->procedureCode = proc_code; return *this;};
	E2APCriticalityDiagnostics& set_ricRequestorID(long int req_id){this->ricRequestorID = req_id; return *this;};
	E2APCriticalityDiagnostics& set_ricInstanceID(long int inst_id){this->ricInstanceID = inst_id; return *this;};

	E2APCriticalityDiagnostics& set_triggeringMessage(unsigned int trigMsg){this->triggeringMessage = trigMsg; return *this;};
	E2APCriticalityDiagnostics& set_procedureCriticality(unsigned int pvalue){this->triggeringMessage = pvalue; return *this;};
	E2APCriticalityDiagnostics& set_iEsCriticalityDiagnostics(unsigned int pvalue){this->triggeringMessage = pvalue; return *this;};


};




#endif /* SRC_XAPP_ASN_E2AP_E2AP_CRITICALITY_DIAGNOSTICS_HPP_ */
