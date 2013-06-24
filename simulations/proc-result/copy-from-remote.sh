#!/bin/bash

#
# copy all the configuration files and result files to a separate folder 
# -- for backup and post-processing
#

# Syntax:
#
# copy-from-remote.sh <configuration>
#

# Example:
#
#$1=delays-500n-srate500kbps-upbw2500kbps-5partners
#
# ------------------------------------------------------------------------------

# -- Inputs
#
REMOTE=118:/home/giang/sim1/omnetpp/so/simulations
LOCAL=/nix/workspace/omnet422-inet118/store/sim-results/wMathias/damage

STORE_DIR=$LOCAL/$1

CONFIG_NAME=Donet_oneRouter_underlayAttack
CONFIG_FILE=Donet-underlayAttack


# -- Preparation
#
rm -rf $STORE_DIR

mkdir $STORE_DIR

# -- Copy
#
scp $REMOTE/results/$CONFIG_NAME-*.* $STORE_DIR

scp $REMOTE/$CONFIG_FILE.ini $STORE_DIR
scp $REMOTE/$CONFIG_FILE.ned $STORE_DIR

scp $REMOTE/include/churn.ini $STORE_DIR
scp $REMOTE/include/seed.ini $STORE_DIR


# -- Zip
#
CUR_DIR=$pwd

cd $STORE_DIR
tar \
   -zcvpf /home/giang/Downloads/result-to-Mathias--$1.tar.gz \
   *.sca \
   *.vci \
   *.vec \

cd $CUR_DIR
