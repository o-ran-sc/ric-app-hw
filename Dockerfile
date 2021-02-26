#/*
#==================================================================================
#
#        Copyright (c) 2020-2021 AT&T Intellectual Property.
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
# */

ARG SCHEMA_PATH=schemas
ARG STAGE_DIR=/tmp/helloworld-xapp

#==================================================================================
FROM nexus3.o-ran-sc.org:10001/ubuntu:18.04 as ricbuild

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
RUN wget -nv --content-disposition https://packagecloud.io/o-ran-sc/release/packages/debian/stretch/mdclog_${MDC_VER}_amd64.deb/download.deb
RUN wget -nv --content-disposition https://packagecloud.io/o-ran-sc/release/packages/debian/stretch/mdclog-dev_${MDC_VER}_amd64.deb/download.deb
RUN dpkg -i mdclog_${MDC_VER}_amd64.deb
RUN dpkg -i mdclog-dev_${MDC_VER}_amd64.deb

# Install RMr using debian package hosted at packagecloud.io
ARG RMR_VER=4.4.6
RUN wget -nv --content-disposition https://packagecloud.io/o-ran-sc/release/packages/debian/stretch/rmr_${RMR_VER}_amd64.deb/download.deb
RUN wget -nv --content-disposition https://packagecloud.io/o-ran-sc/release/packages/debian/stretch/rmr-dev_${RMR_VER}_amd64.deb/download.deb
RUN dpkg -i rmr_${RMR_VER}_amd64.deb
RUN dpkg -i rmr-dev_${RMR_VER}_amd64.deb

#Install ASN1C library package hosted at packagecloud.io
ARG ASN1C_VER=0.1.0
RUN wget --content-disposition https://packagecloud.io/o-ran-sc/staging/packages/debian/stretch/riclibe2ap_${ASN1C_VER}_amd64.deb/download.deb
RUN wget --content-disposition https://packagecloud.io/o-ran-sc/staging/packages/debian/stretch/riclibe2ap-dev_${ASN1C_VER}_amd64.deb/download.deb
RUN dpkg -i riclibe2ap_${ASN1C_VER}_amd64.deb
RUN dpkg -i riclibe2ap-dev_${ASN1C_VER}_amd64.deb


#Install RNIB libraries
ARG RNIB_VER=1.0.0
RUN wget -nv --content-disposition https://packagecloud.io/o-ran-sc/release/packages/debian/stretch/rnib_${RNIB_VER}_all.deb/download.deb
RUN dpkg -i rnib_${RNIB_VER}_all.deb

## Install SDL Libraries
WORKDIR ${STAGE_DIR}
RUN apt-get install -y cpputest
RUN apt-get remove -y libboost-all-dev
RUN apt-get install -y  libboost-all-dev
RUN apt-get install -y libhiredis-dev
#RUN apt-get install -y valgrind

RUN git clone https://gerrit.o-ran-sc.org/r/ric-plt/dbaas
RUN cd dbaas/redismodule && \
    ./autogen.sh && \
    ./configure && \
    make all && \
    make install

WORKDIR ${STAGE_DIR}
RUN git clone https://gerrit.o-ran-sc.org/r/ric-plt/sdl
RUN cd sdl && \
    ./autogen.sh && \
    ./configure && \
    make all && \
    make install


WORKDIR ${STAGE_DIR}
## Install rapidjson

RUN git clone https://github.com/Tencent/rapidjson && \
    cd rapidjson && \
    mkdir build && \
    cd build && \
    cmake -DCMAKE_INSTALL_PREFIX=/usr/local .. && \
    make install && \
    cd ${STAGE_DIR} && \
    rm -rf rapidjson

##-----------------------------------
# Now install the program
#------------------------------------
COPY ./ ${STAGE_DIR}
RUN ls -al


RUN export CPATH=$CPATH:/usr/local/include && \ 
    cd src && \
#    source ./xapp_env.sh \
    make clean && \
    make install 

COPY ${SCHEMA_PATH}/* /etc/xapp/ 
COPY init/init_script.py /etc/xapp/init_script.py
COPY init/routes.txt  /etc/xapp/routes.txt
#---------------------------------------------
# #Build the final version

FROM ubuntu:18.04
ARG SCHEMA_PATH
ARG STAGE_DIR

## copy just the needed libraries install it into the final image
COPY --from=ricbuild ${STAGE_DIR}/*.deb /tmp/
COPY --from=ricbuild /usr/local/lib/librmr_si* /usr/local/lib/
COPY --from=ricbuild /usr/local/lib/libsdl* /usr/local/lib/
COPY --from=ricbuild /usr/local/include/riclibe2ap /usr/local/include/
COPY --from=ricbuild /usr/local/libexec/redismodule/libredis* /usr/local/libexec/redismodule/
RUN dpkg -i /tmp/*.deb
RUN apt-get update && \
    apt-get install -y libcurl3 python3 && \
    apt-get install -y libboost-all-dev cpputest libhiredis-dev valgrind && \
    apt-get clean
COPY --from=ricbuild /etc/xapp/* /etc/xapp/
COPY --from=ricbuild /usr/local/bin/hw_xapp_main /usr/local/bin/hw_xapp_main
COPY --from=ricbuild /usr/local/include/rnib/*.h /usr/local/include/rnib/
COPY --from=ricbuild /usr/local/include/rnib/rnibreader.a /usr/local/include/rnib/


RUN ldconfig
RUN sysctl -w net.ipv6.conf.lo.disable_ipv6=1

##ENV  PYTHONHOME=/opt/python3 \
##     PYTHONPATH=/opt/python3 \
ENV  RMR_RTG_SVC="9999" \
     RMR_SEED_RT="/etc/xapp/routes.txt" \
     LD_LIBRARY_PATH="/usr/local/lib:/usr/local/libexec" \
     VERBOSE=0 \
     CONFIG_FILE=/opt/ric/config/config-file.json
     
      

CMD python3 /etc/xapp/init_script.py $CONFIG_FILE
