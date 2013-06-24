#!/bin/sh
# Current directory should be at .../projectName/simulations/run-scripts/

#opp_runall -j1 ../../src/so -r 0 -c Donet_Homo_oneRouter_Network ../Donet.ini -n ../:../../src:../../../inet -l ../../../inet/src

opp_runall -j1 ../../src/so -u Cmdenv -r 0 -c Donet_Homo_oneRouter_Network ../Donet.ini -n ../:../../src:../../../inet/src -l ../../../inet/src/inet 
