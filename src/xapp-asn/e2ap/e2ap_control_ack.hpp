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
 * e2ap_control_ack.hpp
 *
 * Author: Shraboni Jana
 */

/*-- **************************************************************
--
-- RIC CONTROL ACKNOWLEDGE
--
-- **************************************************************
RICcontrolAcknowledge ::= SEQUENCE {
        protocolIEs                                     ProtocolIE-Container    {{RICcontrolAcknowledge-IEs}},
        ...
}

RICcontrolAcknowledge-IEs E2AP-PROTOCOL-IES ::= {
        { ID id-RICrequestID                            CRITICALITY reject      TYPE RICrequestID                                       PRESENCE mandatory      }|
        { ID id-RANfunctionID                           CRITICALITY reject      TYPE RANfunctionID                                      PRESENCE mandatory      }|
        { ID id-RICcallProcessID                        CRITICALITY reject      TYPE RICcallProcessID                           PRESENCE optional       }|
        { ID id-RICcontrolStatus                        CRITICALITY reject      TYPE RICcontrolStatus                           PRESENCE mandatory      } |
        { ID id-RICcontrolOutcome                       CRITICALITY reject      TYPE RICcontrolOutcome                          PRESENCE optional       },
        ...
}*/

#ifndef SRC_XAPP_ASN_E2AP_E2AP_CONTROL_ACK_HPP_
#define SRC_XAPP_ASN_E2AP_E2AP_CONTROL_ACK_HPP_
#include <iostream>
#include <errno.h>
#include <mdclog/mdclog.h>
#include <sstream>
#include <memory>
#include <E2AP-PDU.h>
#include <SuccessfulOutcome.h>
#include <RICcontrolAcknowledge.h>
#include <ProtocolIE-Field.h>
#include "e2ap_consts.hpp"

class E2APControlAcknowledge{

public:
  class ControlAcknowledgeIEs{
  private:
	    long int ricRequestorID , ricInstanceID, ranFunctionID, ricCallProcessID,  ricControlStatus;

	    unsigned char ricCallProcessId[IE_SIZE];
	    size_t ricCallProcessId_size = IE_SIZE;
	    bool is_ricCallProcessId;
	    unsigned char ricControlOutcome[IE_SIZE];
	    size_t ricControlOutcome_size = IE_SIZE;
	    bool is_ricControlOutcome;

  	  public:
	    ControlAcknowledgeIEs(void):ricRequestorID(1), ricInstanceID(1),ranFunctionID(1), ricCallProcessID(0),ricControlStatus(1),is_ricControlOutcome(false),is_ricCallProcessId(false){};
	    void* get_ricCallProcessId(){return this->ricCallProcessId;};
	    size_t get_ricCallProcessId_size(){return this->ricCallProcessId_size;};

	    void* get_ricControlOutcome(){return this->ricControlOutcome;};
	    size_t get_ricControlOutcome_size(){return this->ricControlOutcome_size;};

	    long int get_ricRequestorID(){return this->ricRequestorID;};
	    long int get_ricInstanceID(){return this->ricInstanceID;};
	    long int get_ranFunctionID(){return this->ranFunctionID;};
	    long int get_ricControlStatus(){return this->ricControlStatus;};

	    bool get_is_ricCallProcessId(){return is_ricCallProcessId;};
	    bool get_is_ricControlOutcome(){return is_ricControlOutcome;};


	    ControlAcknowledgeIEs& set_ricCallProcessID(std::string strCallProcID){
	    	is_ricCallProcessId = true; ricCallProcessId_size = strlen(strCallProcID.c_str());
	    	memcpy((char*)ricCallProcessId, strCallProcID.c_str(), ricCallProcessId_size);
	    	return *this;
	    }
	    ControlAcknowledgeIEs& set_ricCallProcessID(unsigned char* callproc, size_t callproc_size){
	  	    	is_ricCallProcessId = true;
	  	    	memcpy(ricCallProcessId, callproc, callproc_size); ricCallProcessId_size = callproc_size;
	  	    	return *this;
	  	    }


	    ControlAcknowledgeIEs& set_ricControlOutcome(std::string strOutcome){
	    	is_ricControlOutcome = true; ricControlOutcome_size = strlen(strOutcome.c_str());
	   	    memcpy((char*)ricControlOutcome, strOutcome.c_str(), ricControlOutcome_size);
	   	    return *this;
	   	}
	    ControlAcknowledgeIEs& set_ricControlOutcome(unsigned char* callproc, size_t callproc_size){
	    	is_ricControlOutcome = true;
	   	  	memcpy(ricControlOutcome, callproc, callproc_size); ricControlOutcome_size = callproc_size;
	   	  	return *this;
	   	}


	    ControlAcknowledgeIEs& set_ricRequestorID(long int reqID){this->ricRequestorID = reqID; return *this;}
	    ControlAcknowledgeIEs& set_ricControlStatus(long int stat){this->ricControlStatus = stat; return *this;}
	    ControlAcknowledgeIEs& set_ricInstanceID(long int reqID){this->ricInstanceID = reqID; return *this;}
	    ControlAcknowledgeIEs& set_ranFunctionID(long int funcID){this->ranFunctionID = funcID; return *this;}

  };
  E2APControlAcknowledge(ControlAcknowledgeIEs &);
  E2APControlAcknowledge(unsigned char *, size_t *);
 ~E2APControlAcknowledge(void);
  bool encode(unsigned char *, size_t *);
  bool decode(unsigned char *, size_t *);
  ControlAcknowledgeIEs& getIEs(){ return *_cntrlIEs.get();};
  std::string get_error(void) const {return _error_string ; };
private:

  E2AP_PDU_t * _e2ap_pdu_obj;
  SuccessfulOutcome_t * _successMsg;
  RICcontrolAcknowledge_IEs_t *IE_array;
  std::string _error_string;
  bool setfields(SuccessfulOutcome_t *);
  bool getfields(SuccessfulOutcome_t *);
  std::unique_ptr<ControlAcknowledgeIEs> _cntrlIEs;
  char _errbuf[128];
  size_t _errbuf_len = IE_SIZE;
};




#endif /* SRC_XAPP_ASN_E2AP_E2AP_CONTROL_ACK_HPP_ */
