#==================================================================================

#        Copyright (c) 2018-2019 AT&T Intellectual Property.
#
#   Licensed under the Apache License, Version 2.0 (the "License");
#   you may not use this file except in compliance with the License.
#   You may obtain a copy of the License at
#
#       http://www.apache.org/licenses/LICENSE-2.0
#
#   Unless required by applicable law or agreed to in writing, software
#   distributed under the License is distributed on an "AS IS" BASIS,
#   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
#   See the License for the specific language governing permissions and
#   limitations under the License.
#==================================================================================

#
# Author : Ashwin Sridharan
#   Date    : Feb 2019
#


# This initialization script reads in a json from the specified config map path
# to set up the initializations (route config map, variables etc) for the main
# xapp process

import json;
import sys;
import os;
import signal;
import time;


def signal_handler(signum, frame):
    print("Received signal {0}\n".format(signum));
    if(xapp_subprocess == None or xapp_pid == None):
        print("No xapp running. Quiting without sending signal to xapp\n");
    else:
        print("Sending signal {0} to xapp ...".format(signum));
        xapp_subprocess.send_signal(signum);
        

def parseConfigJson(config):
    for key in config:
        print("Processing ", key);
        if key in ParseSection:
            result = ParseSection[key](config);
            if result == False:
                return False;

        
def getRMRTable(config):
    myKey= "rmr";
    if myKey not in config:
        print(("Error ! No information found for {0} in config\n".format(myKey)));
        return False;

    # Get the rmr routing table
    if "file_path" not in config[myKey]:
        print(("Warning ! No file path specified to store seed routing table. Choosing default = {1}\n".format(default_routing_file)));
        route_file = default_routing_file;
    else:
        route_file = config[myKey]["file_path"];

    # Get the rmr routing table contents
    if "contents" not in config[myKey]:
        print("No contents for routing table found in config");
        return False;
    else:
        route_contents = config[myKey]["contents"];
        
    # Get directory : if not exists create it
    directory = os.path.dirname(route_file);
    if not os.path.exists(directory):
        # create directory
        try:
            os.mkdir(directory);
        except OSError as oe:
            print(("Error making directory {0}. Reason = {1}\n".format(directory, oe)));
            return False;

    # Write contents to file
    try:
        with open(route_file, "w") as f :
            f.write(config[myKey]["contents"]);
            f.close();
    except Exception as e:
        print(("Error writing contents to file {0}. Reason = {1}\n".format(route_file, e)));
        return False;

    # Set the environment variable
    os.environ["RMR_SEED_RT"] = route_file;

def getPort(config):
    myKey = "service_ports";
    if myKey not in config:
        print(("Error ! No information found for {0} in config\n".format(myKey)));
        return False;
    port_config = config[myKey];
    if "xapp_port" in port_config:
        try:
            xapp_port = int(port_config["xapp_port"]);
            if xapp_port < 1024:
                raise Exception("Port must be > 1024");
        except Exception as e:
            print(("Error processing xapp port {0}. Reason = {1}\n".format(port_config["xapp_port"], e)));
            return False;
    else:
        xapp_port = 0;
        

def getEnvs(config):
    myKey = "envs";
    if myKey not in config:
        print(("Error ! No information found for {0} in config\n".format(myKey)));
        return False;
    
    env_config = config[myKey];
    
    for env_key in env_config:
        os.environ[env_key] = env_config[env_key];
        print("Set environment variable {0} = {1}\n".format(env_key, os.environ[env_key]));

    return True;


# Global variables ...
xapp_subprocess = None;
xapp_pid = None;
ParseSection = {};
xapp_port = 0;
ParseSection["rmr"] = getRMRTable;
ParseSection["envs"] = getEnvs;

default_routing_file = "/tmp/routeinfo/routes.txt";


#================================================================
if __name__ == "__main__":

    import subprocess;
#    cmd = ["../src/hw_xapp_main"];
    cmd = ["/usr/local/bin/hw_xapp_main"];
    if xapp_port > 0:
        cmd.append("-p");
        cmd.append(xapp_port);
        
    if len(sys.argv) > 1:
        config_file = sys.argv[1];
    else:
        print("Error! No configuration file specified\n");
        sys.exit(1);
        
    if len(sys.argv) > 2:
        cmd[0] = sys.argv[2];

    with open(config_file, 'r') as f:
         try:
             config = json.load(f);
         except Exception as e:
             print(("Error loading json file from {0}. Reason = {1}\n".format(config_file, e)));
             sys.exit(1);
             
    result = parseConfigJson(config);

    if result == False:
        print("Error parsing json. Not executing xAPP");
        sys.exit(1);

    else:

        # Register signal handlers
        signal.signal(signal.SIGINT, signal_handler);
        signal.signal(signal.SIGTERM, signal_handler);

        # Start the xAPP
        #print("Executing xAPP ....");
        xapp_subprocess = subprocess.Popen(cmd, shell = False, stdin=None, stdout=None, stderr = None);
        xapp_pid = xapp_subprocess.pid;

        # Periodically poll the process every 5 seconds to check if still alive
        while(1):
            xapp_status = xapp_subprocess.poll();
            if xapp_status == None:
                time.sleep(5);
            else:
                print("XaPP terminated via signal {0}\n".format(-1 * xapp_status));
                break;
                
