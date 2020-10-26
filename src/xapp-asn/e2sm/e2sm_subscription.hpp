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
 * e2sm_subscription.hpp
 *
 *  Created on: Jun 30, 2020
 *      Author: Shraboni Jana
 */

#ifndef XAPP_ASN_REFACTOR_E2SM_SUBSCRIPTION_HPP_
#define XAPP_ASN_REFACTOR_E2SM_SUBSCRIPTION_HPP_


#include <errno.h>
#include <iostream>
#include <vector>
#include <sstream>
#include <memory>
#include <mdclog/mdclog.h>

#include <E2SM-HelloWorld-EventTriggerDefinition.h>
#include <E2SM-HelloWorld-ActionDefinition.h>
#include <E2SM-HelloWorld-EventTriggerDefinition-Format1.h>
#include <E2SM-HelloWorld-ActionDefinition-Format1.h>
#include <HW-TriggerNature.h>
#include <RANparameter-Item.h>
#include <ProtocolIE-Field.h>

class HWEventTriggerDefinition {
public:
   HWEventTriggerDefinition(void);
  ~HWEventTriggerDefinition(void);

  bool encode(unsigned char *, size_t *);
  std::string  get_error (void) const {return _error_string ;};
  int get_triggerNature(){return this->_triggerNature;};
  HWEventTriggerDefinition& set_triggerNature(int trigger){ this->_triggerNature = trigger; return *this;};

private:

  E2SM_HelloWorld_EventTriggerDefinition_t * _event_trigger; // used for encoding
  E2SM_HelloWorld_EventTriggerDefinition_Format1_t _event_fmt1;
  int _triggerNature;

  size_t _errbuf_len = 128;
  char _errbuf[128];
  std::string _error_string;
};


class HWActionDefinition {

public:
  HWActionDefinition(void);
  ~HWActionDefinition(void);
  class RANParamIEs{

  private:
  		  long int _param_id;
  	      unsigned char _param_name[20];
  	      size_t _param_name_size;
  	      int _param_test;
  	      unsigned char _param_value[20];
  	      size_t _param_value_size;
  public:
  		  RANParamIEs(void):_param_id(0),_param_name_size(0), _param_test(0),_param_value_size(0){};
  		  RANParamIEs& set_param_id(int param_id) {_param_id = param_id; return *this;};
  		  RANParamIEs& set_param_name(std::string param_name) { _param_name_size = strlen(param_name.c_str());
  		  	  	  	  	  	  	  	  	  	  	  	  	  	   strncpy((char*)_param_name,param_name.c_str(),_param_name_size); return *this;};
  		  RANParamIEs& set_param_test(int param_test) {_param_test = param_test; return *this;};
  		  RANParamIEs& set_param_value(std::string param_value) {_param_value_size = strlen(param_value.c_str());
  		  	  	  	  	  	  	  	  	  	  	  	  	  	  	 strncpy((char*)_param_value,param_value.c_str(),_param_value_size); return *this;};

  		  int get_param_id(){return this->_param_id;};
  		  int get_param_test(){return this->_param_test;};
  		  unsigned char* get_param_name(){return this->_param_name;};
  		  int get_param_name_size(){return this->_param_name_size;};
  		  unsigned char* get_param_value(){return this->_param_value;};
  		  int get_param_value_size(){return this->_param_value_size;};


  };
  std::vector<RANParamIEs> * get_list() const {return _param_list.get();};
  int get_list_count() const {return _count_list;};
  bool encode(unsigned char *, size_t *);
  std::string  get_error (void) const {return _error_string ;};
  void add(RANParamIEs ie_obj){
	  _param_list.get()->push_back(ie_obj);
  	  _count_list ++;
  }

private:
  std::unique_ptr<std::vector<RANParamIEs>> _param_list;
  int _count_list;
  E2SM_HelloWorld_ActionDefinition_t* _action_defn;
  E2SM_HelloWorld_ActionDefinition_Format1_t _action_fmt1;
  RANparameter_Item_t* _ranparam_ie;

  size_t _errbuf_len = 128;
  char _errbuf[128];
  std::string _error_string;

  bool setfields(E2SM_HelloWorld_ActionDefinition_t *);


};


#endif /* XAPP_ASN_REFACTOR_E2SM_SUBSCRIPTION_HPP_ */
