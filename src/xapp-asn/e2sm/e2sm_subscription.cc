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
 * e2sm_subscription.cc
 *
 *  Created on: Jun 30, 2020
 *      Author: Shraboni Jana
 */

#include "e2sm_subscription.hpp"



HWEventTriggerDefinition::HWEventTriggerDefinition(void){

	memset(&_event_fmt1, 0, sizeof(E2SM_HelloWorld_EventTriggerDefinition_Format1_t));

	_triggerNature = 1; //Making first selection as default since enumerated type.

    _event_trigger = 0;
    _event_trigger = ( E2SM_HelloWorld_EventTriggerDefinition_t *)calloc(1, sizeof( E2SM_HelloWorld_EventTriggerDefinition_t));
    assert(_event_trigger != 0);

  };

HWEventTriggerDefinition::~HWEventTriggerDefinition(void){

  mdclog_write(MDCLOG_DEBUG, "Freeing event trigger object memory");
  _event_trigger->choice.eventDefinition_Format1 = 0;
  ASN_STRUCT_FREE(asn_DEF_E2SM_HelloWorld_EventTriggerDefinition, _event_trigger);

};

bool HWEventTriggerDefinition::encode(unsigned char *buf, size_t *size){

  ASN_STRUCT_RESET(asn_DEF_E2SM_HelloWorld_EventTriggerDefinition, _event_trigger);

  if(_event_trigger == 0){
      _error_string = "Invalid reference for Event Trigger Definition set fields";
      return false;
    }

  _event_trigger->present = E2SM_HelloWorld_EventTriggerDefinition_PR_eventDefinition_Format1;
  _event_fmt1.triggerNature = this->_triggerNature;
  _event_trigger->choice.eventDefinition_Format1 = &_event_fmt1;


  int ret_constr = asn_check_constraints(&asn_DEF_E2SM_HelloWorld_EventTriggerDefinition, _event_trigger, _errbuf, &_errbuf_len);
  if(ret_constr){
    _error_string.assign(&_errbuf[0], _errbuf_len);
    return false;
  }

  xer_fprint(stdout, &asn_DEF_E2SM_HelloWorld_EventTriggerDefinition, _event_trigger);

  asn_enc_rval_t retval = asn_encode_to_buffer(0, ATS_ALIGNED_BASIC_PER, &asn_DEF_E2SM_HelloWorld_EventTriggerDefinition, _event_trigger, buf, *size);

  if(retval.encoded == -1){
    _error_string.assign(strerror(errno));
    return false;
  }
  else if (retval.encoded > *size){
    std::stringstream ss;
    ss  <<"Error encoding event trigger definition. Reason =  encoded pdu size " << retval.encoded << " exceeds buffer size " << *size << std::endl;
    _error_string = ss.str();
    return false;
  }
  else{
    *size = retval.encoded;
  }

  return true;
}

HWActionDefinition::HWActionDefinition(void){

	_param_list = std::make_unique<std::vector<RANParamIEs>>(); _count_list = 0;

	memset(&_action_fmt1, 0, sizeof(E2SM_HelloWorld_ActionDefinition_Format1_t));

	int SEQOFRANPARAMS = 1;

	_ranparam_ie = 0;
	_ranparam_ie = (RANparameter_Item_t*)calloc(SEQOFRANPARAMS, sizeof(RANparameter_Item_t));
	assert(_ranparam_ie != 0);

	_action_defn = 0;
    _action_defn = (E2SM_HelloWorld_ActionDefinition_t*)calloc(1, sizeof(E2SM_HelloWorld_ActionDefinition_t));
    assert(_action_defn !=0);


};

HWActionDefinition::~HWActionDefinition(void){

  mdclog_write(MDCLOG_DEBUG, "Freeing Action Definition object memory");
  _action_defn->choice.actionDefinition_Format1 = 0;
  free(_ranparam_ie);
  ASN_STRUCT_FREE(asn_DEF_E2SM_HelloWorld_ActionDefinition, _action_defn);


};
bool HWActionDefinition::encode(unsigned char *buf, size_t *size){

  bool res;
  res = this->setfields(this->_action_defn);
  if (!res){
    return false;
  }

  int ret_constr = asn_check_constraints(&asn_DEF_E2SM_HelloWorld_ActionDefinition, this->_action_defn, _errbuf, &_errbuf_len);
  if(ret_constr){
    _error_string.assign(&_errbuf[0], _errbuf_len);
    return false;
  }

  xer_fprint(stdout, &asn_DEF_E2SM_HelloWorld_ActionDefinition, _action_defn);

  asn_enc_rval_t retval = asn_encode_to_buffer(0, ATS_ALIGNED_BASIC_PER, &asn_DEF_E2SM_HelloWorld_ActionDefinition, _action_defn, buf, *size);

  if(retval.encoded == -1){
    _error_string.assign(strerror(errno));
    return false;
  }
  else if (retval.encoded > *size){
    std::stringstream ss;
    ss  <<"Error encoding action definition. Reason =  encoded pdu size " << retval.encoded << " exceeds buffer size " << *size << std::endl;
    _error_string = ss.str();
    return false;
  }
  else{
    *size = retval.encoded;
  }

  return true;
}

bool HWActionDefinition::setfields(E2SM_HelloWorld_ActionDefinition_t *_action_defn){

 if(_action_defn == 0){
    _error_string = "Invalid reference for Event Action Definition set fields";
    return false;
  }
  _action_defn->present = E2SM_HelloWorld_ActionDefinition_PR_actionDefinition_Format1;

  _ranparam_ie = (RANparameter_Item_t*)calloc(this->_count_list, sizeof(RANparameter_Item_t));
   assert(_ranparam_ie != 0);

  auto paramList = this->get_list();
  int i = 0;
  for(RANParamIEs item:*paramList){

	  _ranparam_ie[i].ranParameter_ID = item.get_param_id();
	  _ranparam_ie[i].ranParameter_Name.buf = item.get_param_name();
	  _ranparam_ie[i].ranParameter_Name.size = item.get_param_name_size();
	  _ranparam_ie[i].ranParameter_Test = item.get_param_test();
	  _ranparam_ie[i].ranParameter_Value.buf = item.get_param_value();
	  _ranparam_ie[i].ranParameter_Value.size = item.get_param_value_size();

	 const int res = ASN_SEQUENCE_ADD(&_action_fmt1.ranParameter_List, &(_ranparam_ie[i]));
	 if (res!=0){
	       _error_string = "Error : Unable to assign memory to add RAN PARAM item to set up list";
	       return false;
	    }
	  i++;

  }

  _action_defn->choice.actionDefinition_Format1 = &_action_fmt1;


  return true;
};
