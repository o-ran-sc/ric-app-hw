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
 * action_e2ap.hpp
 *
 *  Created on: Jun 30, 2020
 *      Author: sjana
 */

#ifndef XAPP_ASN_REFACTOR_E2AP_ACTION_HPP_
#define XAPP_ASN_REFACTOR_E2AP_ACTION_HPP_
#define E2SM_SIZE ((int)128)

#include <mdclog/mdclog.h>

#include <string>
#include <memory>
#include <sstream>
#include <vector>
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
			 bool is_ricSubsequentAction;
			 unsigned int ricActionType, ricActionID,ricSubsequentActionType,ricTimeToWait;
			 unsigned char ricActionDefinition[E2SM_SIZE];
			 size_t ricActionDefinition_size = E2SM_SIZE;
	public:
			 ActionIEs():ricActionType(0),ricActionID(0),ricSubsequentActionType(0),ricTimeToWait(0),is_ricSubsequentAction(false){ };
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
				 return *this;
			 };
			 ActionIEs& set_ricActionID(int actionID){ricActionID = actionID; return *this;};
			 ActionIEs& set_ricActionType(int actionType) {ricActionType = actionType; return *this;};

			 int    get_ricActionID(){return this->ricActionID;};
			 int    get_ricActionType() {return this->ricActionType;};
			 bool   get_is_ricSubsequentAction() { return this->is_ricSubsequentAction; };
			 int    get_ricSubsequentActionType(){return this->ricSubsequentActionType; }
			 int    get_ricTimeToWait(){ return this->ricTimeToWait; }
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


#endif /* XAPP_ASN_REFACTOR_E2AP_ACTION_HPP_ */
