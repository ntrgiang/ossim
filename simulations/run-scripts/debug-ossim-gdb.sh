#!/bin/bash

# Assuming that the file is executed within the run-script/ folder

# $1 = Configuration name
# $2 = INI file which stores the configuration

#config=Donet_oneRouter_underlayAttack 
#ini_file=../Donet-underlayAttack.ini

config=$1
ini_file=$2

rm /tmp/ossim.log

gdb --args ../../src/so -r 0 -u Cmdenv -c $config $ini_file -n /nix/workspace/omnet422-inet118/so/src:/nix/workspace/omnet422-inet118/so/simulations:/nix/workspace/omnet422-inet118/inet/src | tee /tmp/ossim.log
