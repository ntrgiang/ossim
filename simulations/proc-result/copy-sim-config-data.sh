#!/bin/bash

#
# copy all the configuration files and result files to a separate folder for backup
#

STORE_DIR=store

mkdir $STORE_DIR

cp ../results/*-0.* $STORE_DIR
cp ../DonetNetworkConfig.ini $STORE_DIR
cp ../DonetNetwork.ned $STORE_DIR
cp ../include/churn.ini $STORE_DIR
cp ../include/seed.ini $STORE_DIR


