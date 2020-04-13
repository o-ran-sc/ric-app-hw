/*
==================================================================================

        Copyright (c) 2018-2019 AT&T Intellectual Property.

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
 *//*
 * xapp.hpp
 *
 *  Modified: Mar, 2020 (Shraboni Jana)
 *
 */

#pragma once

#ifndef SRC_XAPP_HPP_
#define SRC_XAPP_HPP_

#include <iostream>
#include <string>
#include <memory>
#include <csignal>
#include <stdio.h>
#include <pthread.h>
#include <unordered_map>
#include "xapp_rmr.hpp"
#include "xapp_sdl.hpp"
#include "rapidjson/writer.h"
#include "rapidjson/document.h"
#include "msgs_proc.hpp"
#include "subs_mgmt.hpp"
#include "xapp_config.hpp"
extern "C" {
#include "rnib/rnibreader_old.h"
}
using namespace std;
using namespace std::placeholders;
using namespace rapidjson;

using callback_type = std::function< void(rmr_mbuf_t*,bool*) > ;

class Xapp{
public:

  Xapp(XappSettings &, XappRmr &);
  ~Xapp(void);
  void stop(void);
  void startup();
  void shutdown(void);
  void init(void);
  void start_xapp_receiver(XappMsgHandler &);
  void sdl_data(void);

  Xapp(Xapp const &)=delete;
  Xapp& operator=(Xapp const &) = delete;

  template<typename FunctionObject>
  void register_handler(FunctionObject fn){
	  _callbacks.emplace_back(fn);
  }

  void callback_handler(){

  }

  void set_rnib_gnblist(void);
  //getters/setters.
  std::vector<std::string> get_rnib_gnblist(){ return rnib_gnblist; }

private:
  void startup_subscribe_requests(void );
  void shutdown_subscribe_deletes(void);
  void startup_get_policies(void );


  XappRmr * rmr_ref;
  XappSettings * config_ref;

  std::mutex *xapp_mutex;
  std::vector<std::thread> xapp_rcv_thread;
  std::vector<std::string> rnib_gnblist;

  std::vector<callback_type> _callbacks;

};


#endif /* SRC_XAPP_HPP_ */
