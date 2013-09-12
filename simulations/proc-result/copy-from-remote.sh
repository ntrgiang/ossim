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
REMOTE=152:/home/giang/sim/release/so/simulations
LOCAL=/dos/research/experiments/scheduling-diversification/potential-damage

STORE_DIR=$LOCAL/$1

CONFIG_NAME=Donet_oneRouter_overlayAttack_new
CONFIG_FILE=Donet-overlayAttack


# -- Preparation
#
#rm -rf $STORE_DIR

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
#CUR_DIR=$pwd

#cd $STORE_DIR
#tar \
#   -zcvpf /home/giang/Downloads/result-to-Mathias--$1.tar.gz \
#   *.sca \
#   *.vci \
#   *.vec \

cd $CUR_DIR
