#/*
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
#*/
ARG SCHEMA_PATH=schemas
ARG STAGE_DIR=/tmp/helloworld-xapp

#==================================================================================
FROM nexus3.o-ran-sc.org:10004/bldr-ubuntu16-c-go@sha256:f4ebcc792b501f283c7848018b6f106bbcecf1f9e699035950614460b3c330f8 as ricbuild

# to override repo base, pass in repo argument when running docker build:
# docker build --build-arg REPOBASE=http://abc.def.org . ....
ARG REPOBASE=https://gerrit.oran-osc.org/r
ARG SCHEMA_FILE
ARG SCHEMA_PATH
ARG STAGE_DIR

# Install necessary packages
WORKDIR ${STAGE_DIR}
RUN apt-get update  \
     && apt-get install -y \
     libcurl4-openssl-dev \
     libcurl3 \
     cmake \
     git \
     build-essential \
     automake \
     autoconf-archive \
     autoconf \
     pkg-config \
     gawk \
     libtool \
     wget \
     zlib1g-dev \
     libffi-dev \
     && apt-get clean

# Install mdclog using debian package hosted at packagecloud.io
ARG MDC_VER=0.0.4-1
RUN wget -nv --content-disposition https://packagecloud.io/o-ran-sc/staging/packages/debian/stretch/mdclog_${MDC_VER}_amd64.deb/download.deb
RUN wget -nv --content-disposition https://packagecloud.io/o-ran-sc/staging/packages/debian/stretch/mdclog-dev_${MDC_VER}_amd64.deb/download.deb
RUN dpkg -i mdclog_${MDC_VER}_amd64.deb
RUN dpkg -i mdclog-dev_${MDC_VER}_amd64.deb

# Install RMr using debian package hosted at packagecloud.io
ARG RMR_VER=3.6.5
RUN wget -nv --content-disposition https://packagecloud.io/o-ran-sc/staging/packages/debian/stretch/rmr_${RMR_VER}_amd64.deb/download.deb
RUN wget -nv --content-disposition https://packagecloud.io/o-ran-sc/staging/packages/debian/stretch/rmr-dev_${RMR_VER}_amd64.deb/download.deb
RUN dpkg -i rmr_${RMR_VER}_amd64.deb
RUN dpkg -i rmr-dev_${RMR_VER}_amd64.deb

## Install SDL Libraries
WORKDIR ${STAGE_DIR}
RUN apt-get install -y cpputest
RUN apt-get remove -y libboost-all-dev
RUN apt-get install -y  libboost-all-dev
RUN apt-get install -y libhiredis-dev
RUN apt-get install -y valgrind

RUN git config --global http.proxy http://one.proxy.att.com:8080
RUN git clone https://gerrit.o-ran-sc.org/r/ric-plt/dbaas
RUN cd dbaas/redismodule && \
    ./autogen.sh && \
    ./configure && \
    make all && \
#    make test && \
    make install

WORKDIR ${STAGE_DIR}
RUN git clone https://gerrit.o-ran-sc.org/r/ric-plt/sdl
RUN cd sdl && \
    ./autogen.sh && \
    ./configure && \
    make all && \
#    make test && \
    make install

RUN git config --global --unset http.proxy

WORKDIR ${STAGE_DIR}
## Install rapidjson
    #git checkout tags/v1.1.0 && \

RUN git clone https://github.com/Tencent/rapidjson && \
    cd rapidjson && \
    mkdir build && \
    cd build && \
    cmake -DCMAKE_INSTALL_PREFIX=/usr/local .. && \
    make install && \
    cd ${STAGE_DIR} && \
    rm -rf rapidjson

### Copy RNIB library and include rnib.
RUN mkdir /usr/local/include/rnib

##-----------------------------------
# Now install the program
#------------------------------------
COPY ./ ${STAGE_DIR}
RUN ls -al
COPY ${STAGE}/rnib/*.h /usr/local/include/rnib/
COPY ${STAGE}/rnibreader_old.a /usr/local/lib/

RUN export CPATH=$CPATH:/usr/local/include && \ 
    cd src && \
    make clean && \
    make install 

COPY ${SCHEMA_PATH}/* /etc/xapp/ 
COPY init/init_script.py /etc/xapp/init_script.py

#---------------------------------------------
# #Build the final version

FROM ubuntu:16.04

ARG SCHEMA_PATH
ARG STAGE_DIR

## copy just the needed libraries install it into the final image
COPY --from=ricbuild ${STAGE_DIR}/*.deb /tmp/
#COPY --from=ricbuild /usr/local/libexec/. /usr/local/libexec/.
COPY --from=ricbuild /usr/local/lib/librmr_si* /usr/local/lib/
COPY --from=ricbuild /usr/local/lib/libsdl* /usr/local/lib/
COPY --from=ricbuild /usr/local/libexec/redismodule/libredis* /usr/local/libexec/redismodule/
RUN dpkg -i /tmp/*.deb
RUN apt-get update && \
    apt-get install -y libcurl3 python3 && \
    apt-get install -y libboost-all-dev cpputest libhiredis-dev valgrind && \
    apt-get clean
COPY --from=ricbuild /etc/xapp/* /etc/xapp/
COPY --from=ricbuild /usr/local/bin/hw_xapp_main /usr/local/bin/hw_xapp_main
COPY --from=ricbuild ${STAGE_DIR}/rnib/*.h /usr/local/include/rnib/
COPY --from=ricbuild ${STAGE_DIR}/rnibreader_old.a /usr/local/lib/

##COPY --from=ricbuild /usr/local/bin/e2e-test-client /usr/local/bin/e2e-test-client
##COPY --from=ricbuild /usr/local/bin/mock-e2term-server /usr/local/bin/mock-e2term-server
##COPY --from=ricbuild /usr/local/bin/e2e-perf-client /usr/local/bin/e2e-perf-client
##COPY --from=ricbuild /usr/local/bin/e2e-perf-server /usr/local/bin/e2e-perf-server
##COPY --from=ricbuild /usr/local/bin/mock-a1-server /usr/local/bin/mock-a1-server


RUN ldconfig
RUN sysctl -w net.ipv6.conf.lo.disable_ipv6=1

##ENV  PYTHONHOME=/opt/python3 \
##     PYTHONPATH=/opt/python3 \
ENV  RMR_RTG_SVC="9999" \
     LD_LIBRARY_PATH="/usr/local/lib:/usr/local/libexec" \
     NAME=ADM_CTRL_XAPP \
     PORT=tcp:4560 \
     THREADS=1\
     VERBOSE=0 \
     MESSAGE_TYPE=10002 \
     RATE=1 \
     CONFIG_FILE=/opt/ric/config/config-file.json
     
      

CMD python3 /etc/xapp/init_script.py $CONFIG_FILE
