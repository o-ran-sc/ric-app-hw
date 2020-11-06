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
 * e2ap_action.hpp
 *
 *  Created on: Jun 30, 2020
 *      Author: Shraboni Jana
 */

#ifndef XAPP_ASN_REFACTOR_E2AP_ACTION_HPP_
#define XAPP_ASN_REFACTOR_E2AP_ACTION_HPP_

#include <mdclog/mdclog.h>

#include <string>
#include <memory>
#include <sstream>
#include <vector>

#include "e2ap_consts.hpp"
/*
 RICaction-ToBeSetup-Item ::= SEQUENCE {
	ricActionID					RICactionID,
	ricActionType				RICactionType,
	ricActionDefinition			RICactionDefinition 	OPTIONAL,
	ricSubsequentAction			RICsubsequentAction 	OPTIONAL,
	...
}
RICsubsequentAction ::=SEQUENCE{
	ricSubsequentActionType		RICsubsequentActionType,
	ricTimeToWait				RICtimeToWait,
	...
}

*/


template<typename E2SMActionDefinition>
class E2APAction{

public:
	class ActionIEs{
	private:
			 bool is_ricSubsequentAction, is_ricActionDefinition;
			 unsigned int ricActionType, ricActionID,ricSubsequentActionType,ricTimeToWait;
			 unsigned char ricActionDefinition[IE_SIZE];
			 size_t ricActionDefinition_size = IE_SIZE;
	public:
			 ActionIEs():ricActionType(0),ricActionID(0),ricSubsequentActionType(0),ricTimeToWait(0),is_ricSubsequentAction(false),is_ricActionDefinition(false){ };
			 ActionIEs& set_ricSubsequentAction(int subsequentActionType, int timeToWait){
				 is_ricSubsequentAction = true;
				 ricSubsequentActionType = subsequentActionType;
				 ricTimeToWait = timeToWait;
				 return *this;
			 };

			 ActionIEs& set_ricActionDefinition(E2SMActionDefinition &e2smObj){
				 bool res = e2smObj.encode(&(this->ricActionDefinition)[0],&this->ricActionDefinition_size);
				 if(!res){
					 mdclog_write(MDCLOG_ERR, "Failed to encode: %s","RIC Action Definition");
					 mdclog_write(MDCLOG_ERR, "Error during encode: %s",e2smObj.get_error());

				 } else {
					 mdclog_write(MDCLOG_INFO, "Successfully encoded: %s","RIC Action Definition");
				 }
				 this->is_ricActionDefinition = true;
				 return *this;
			 };
			 ActionIEs& set_ricActionID(int actionID){ricActionID = actionID; return *this;};
			 ActionIEs& set_ricActionType(int actionType) {ricActionType = actionType; return *this;};

			 int    get_ricActionID(){return this->ricActionID;};
			 int    get_ricActionType() {return this->ricActionType;};
			 bool   get_is_ricSubsequentAction() { return this->is_ricSubsequentAction; };
			 int    get_ricSubsequentActionType(){return this->ricSubsequentActionType; }
			 int    get_ricTimeToWait(){ return this->ricTimeToWait; }
			 bool   get_is_ricActionDefinition(){return this->is_ricActionDefinition;};
			 void*  get_ricActionDefinition(){ return this->ricActionDefinition; };
			 size_t get_ricActionDefinition_size(){return this->ricActionDefinition_size; };

	};
	E2APAction(){ _ref_list = std::make_unique<std::vector<ActionIEs>>(); _count_list = 0;};

	std::vector<E2APAction<E2SMActionDefinition>::ActionIEs> * get_list() const {return _ref_list.get();};
	int get_list_count() {return _count_list;};

	void add(E2APAction<E2SMActionDefinition>::ActionIEs &actionObj) { _ref_list.get()->push_back(actionObj); _count_list++;};
private:

	std::unique_ptr<std::vector<ActionIEs>> _ref_list;
	int _count_list;
};

/*RICaction-Admitted-Item ::= SEQUENCE {
	ricActionID					RICactionID,
	...
}


RICaction-NotAdmitted-Item ::= SEQUENCE {
	ricActionID					RICactionID,
	cause						Cause,
	...
}*/

class RICactionAdmittedList{

public:
	class RICactionAdmittedItemIEs{
		private:
			long int ricActionID;
		public:
			RICactionAdmittedItemIEs(void):ricActionID(0){};
			RICactionAdmittedItemIEs& set_ricActionID(long int actID){ricActionID = actID; return *this;};
			long int get_ricActionID(){return this->ricActionID;};

	};
	RICactionAdmittedList(){ _ref_list = std::make_unique<std::vector<RICactionAdmittedItemIEs>>(); _count_list = 0;};

	std::vector<RICactionAdmittedItemIEs> * get_list() const {return _ref_list.get();};
	int get_list_count() {return _count_list;};

	void add(RICactionAdmittedItemIEs &actionObj) { _ref_list.get()->push_back(actionObj); _count_list++;};
private:

	std::unique_ptr<std::vector<RICactionAdmittedItemIEs>> _ref_list;
	int _count_list;

};

class RICactionNotAdmittedList{
 public:
	class RICactionNotAdmittedItemIEs{
		private:
			long int ricActionID;
			unsigned int ricCause, ricSubCause;
		public:
			RICactionNotAdmittedItemIEs(void):ricActionID(0),ricCause(0),ricSubCause(0){};
			RICactionNotAdmittedItemIEs& set_ricCause(long int cause){ricCause = cause; return *this;};
			RICactionNotAdmittedItemIEs& set_ricSubCause(long int subcause){ricSubCause = subcause; return *this;};
			RICactionNotAdmittedItemIEs& set_ricActionID(long int actID){ricActionID = actID; return *this;};

			long int get_ricCause(){return this->ricCause;};
			long int get_ricSubCause(){return this->ricSubCause;};
			long int get_ricActionID(){return this->ricActionID;};
	};
	RICactionNotAdmittedList(){ _ref_list = std::make_unique<std::vector<RICactionNotAdmittedItemIEs>>(); _count_list = 0;};

	std::vector<RICactionNotAdmittedItemIEs> * get_list() const {return _ref_list.get();};
	int get_list_count() {return _count_list;};

	void add(RICactionNotAdmittedItemIEs &actionObj) { _ref_list.get()->push_back(actionObj); _count_list++;};
private:

	std::unique_ptr<std::vector<RICactionNotAdmittedItemIEs>> _ref_list;
	int _count_list;

};


#endif /* XAPP_ASN_REFACTOR_E2AP_ACTION_HPP_ */
