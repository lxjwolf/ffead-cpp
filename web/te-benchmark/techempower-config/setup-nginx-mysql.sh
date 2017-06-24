#!/bin/bash

fw_depends ffead-cpp-nginx-mysql

export FFEAD_CPP_PATH=$IROOT/ffead-cpp-2.0
export LD_LIBRARY_PATH=$IROOT:$FFEAD_CPP_PATH/lib:$LD_LIBRARY_PATH
echo $FFEAD_CPP_PATH
echo $LD_LIBRARY_PATH
rm -f $FFEAD_CPP_PATH/*.cntrl
rm -f $FFEAD_CPP_PATH/tmp/*.sess
$IROOT/nginxfc/sbin/nginx > ffead.log 2>&1

