

#ifndef E2SM_HELPER_
#define E2SM_HELPER_

#include <errno.h>
#include <iostream>
#include <vector>
#include <sstream>

 /* information holder for E2SM indication header */
typedef struct e2sm_header_helper e2sm_header_helper;
struct e2sm_header_helper {
  int egNB_id_type;
  
  std::string egNB_id;
  std::string plmn_id;
  
  long int interface_direction;
  unsigned char* timestamp;
};

/* information holder for E2SM indication message */
typedef struct e2sm_message_helper e2sm_message_helper;
struct e2sm_message_helper {
  unsigned char * x2ap_pdu;
  size_t x2ap_pdu_size;
};

  
/* information holder for E2SM Action Trigger Definition */
struct Item
{
  Item(long int id, long int test, long int val_type, int value):interface_id(id), test(test), val_type(val_type), value_n(value){};
  Item(long int id, long int test, long int val_type, std::string value):interface_id(id), test(test), val_type(val_type), value_s(value){};
    
  long int interface_id;
  long int test;
  long int  val_type;
  long int value_n;
  std::string value_s;
    
};
  
typedef struct e2sm_event_trigger_helper e2sm_event_trigger_helper;
struct e2sm_event_trigger_helper {
    
  int egNB_id_type;
  std::string egNB_id;
  std::string plmn_id;
    
  long int interface_direction;
  long int procedure_code;
    
  long int message_type;

    
  std::vector<struct Item> * get_list(void){ return &protocol_ie_list; };
  void add_protocol_ie_item(long int id, long int test , unsigned int val_type, long int value ){
    // into list 
    protocol_ie_list.emplace_back(id, test, val_type, value);
  };
  
  void add_protocol_ie_item(long int id, long int  test, unsigned  int val_type, std::string  value){
    //  into list 
    protocol_ie_list.emplace_back(id, test, val_type, value);    
  };
   
  void clear(void){
    protocol_ie_list.clear();
  }

  std::string get_string(void) const {
    std::stringstream ss;
    ss << "egNB_ID_type = " << egNB_id_type << std::endl;
    ss << "PLMN Id = " << plmn_id << std::endl;
    ss << "Procedure Code = " << procedure_code << std::endl;
    ss << "Message Type = " << message_type << std::endl;

    std::string info;
    info = ss.str();
    return info;
  }
  
    
    
private:
    
  std::vector<struct Item> protocol_ie_list;
    
};

  


#endif
