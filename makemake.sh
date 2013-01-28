#!/bin/sh
cd ./src

# Make Makefiles
opp_makemake -f --deep -O out -I/usr/include/ -I/usr/include/c++/4.6 -I/nix/workspace/omnet422-inet118/omnetpp-4.2.2/include/ -I../../inet/src/networklayer/common -I../../inet/src/networklayer/rsvp_te -I../../inet/src/networklayer/icmpv6 -I../../inet/src/transport/tcp -I../../inet/src/networklayer/mpls -I../../inet/src/networklayer/ted -I../../inet/src/networklayer/contract -I../../inet/src/util -I../../inet/src/transport/contract -I../../inet/src/linklayer/mfcore -I../../inet/src/networklayer/ldp -I../../inet/src/applications/udpapp -I../../inet/src/networklayer/ipv4 -I../../inet/src/base -I../../inet/src/util/headerserializers -I../../inet/src/networklayer/ipv6 -I../../inet/src/transport/sctp -I../../inet/src/world -I../../inet/src/applications/pingapp -I../../inet/src/linklayer/contract -I../../inet/src/networklayer/arp -I../../inet/src/transport/udp -KINET_PROJ=../../inet -linet -L../../inet/out/gcc-debug/src

# Resolve dependencies
make depend


