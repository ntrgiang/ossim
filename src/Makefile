#
# OMNeT++/OMNEST Makefile for scamp
#
# This file was generated with the command:
#  opp_makemake -f --deep -I/usr/include/ -I/usr/include/c++/4.5 -I../../inet/src/networklayer/rsvp_te -I../../inet/src/networklayer/common -I../../inet/src/networklayer/icmpv6 -I../../inet/src/transport/tcp -I../../inet/src/networklayer/mpls -I../../inet/src/networklayer/ted -I../../inet/src/networklayer/contract -I../../inet/src/util -I../../inet/src/transport/contract -I../../inet/src/linklayer/mfcore -I../../inet/src/networklayer/ldp -I../../inet/src/applications/udpapp -I../../inet/src/networklayer/ipv4 -I../../inet/src/base -I../../inet/src/util/headerserializers -I../../inet/src/transport/sctp -I../../inet/src/networklayer/ipv6 -I../../inet/src/world -I../../inet/src/applications/pingapp -I../../inet/src/linklayer/contract -I../../inet/src/networklayer/arp -I../../inet/src/transport/udp -L../../inet/out/$(CONFIGNAME)/src -linet -KINET_PROJ=../../inet
#

# Name of target to be created (-o option)
TARGET = scamp$(EXE_SUFFIX)

# User interface (uncomment one) (-u option)
USERIF_LIBS = $(ALL_ENV_LIBS) # that is, $(TKENV_LIBS) $(CMDENV_LIBS)
#USERIF_LIBS = $(CMDENV_LIBS)
#USERIF_LIBS = $(TKENV_LIBS)

# C++ include paths (with -I)
INCLUDE_PATH = \
    -I/usr/include/ \
    -I/usr/include/c++/4.5 \
    -I../../inet/src/networklayer/rsvp_te \
    -I../../inet/src/networklayer/common \
    -I../../inet/src/networklayer/icmpv6 \
    -I../../inet/src/transport/tcp \
    -I../../inet/src/networklayer/mpls \
    -I../../inet/src/networklayer/ted \
    -I../../inet/src/networklayer/contract \
    -I../../inet/src/util \
    -I../../inet/src/transport/contract \
    -I../../inet/src/linklayer/mfcore \
    -I../../inet/src/networklayer/ldp \
    -I../../inet/src/applications/udpapp \
    -I../../inet/src/networklayer/ipv4 \
    -I../../inet/src/base \
    -I../../inet/src/util/headerserializers \
    -I../../inet/src/transport/sctp \
    -I../../inet/src/networklayer/ipv6 \
    -I../../inet/src/world \
    -I../../inet/src/applications/pingapp \
    -I../../inet/src/linklayer/contract \
    -I../../inet/src/networklayer/arp \
    -I../../inet/src/transport/udp \
    -I. \
    -Iapplications \
    -Iapplications/common \
    -Iapplications/common/membership \
    -Iapplications/common/messages \
    -Iapplications/common/streaming \
    -Iapplications/peer \
    -Iapplications/peer/smc \
    -Iapplications/source \
    -Iglobal \
    -Iglobal/churn \
    -Iglobal/observer \
    -Iglobal/statistic \
    -Inodes

# Additional object and library files to link with
EXTRA_OBJS =

# Additional libraries (-L, -l options)
LIBS = -L../../inet/out/$(CONFIGNAME)/src  -linet
LIBS += -Wl,-rpath,`abspath ../../inet/out/$(CONFIGNAME)/src`

# Output directory
PROJECT_OUTPUT_DIR = ../out
PROJECTRELATIVE_PATH = src
O = $(PROJECT_OUTPUT_DIR)/$(CONFIGNAME)/$(PROJECTRELATIVE_PATH)

# Object files for local .cc and .msg files
OBJS = \
    $O/applications/DonetBase.o \
    $O/applications/DummyMembership.o \
    $O/applications/CommBase.o \
    $O/applications/MembershipBase.o \
    $O/applications/ScampBase.o \
    $O/applications/common/Dispatcher.o \
    $O/applications/common/Forwarder.o \
    $O/applications/common/membership/MCache.o \
    $O/applications/common/membership/Contact.o \
    $O/applications/common/membership/MessageLogger.o \
    $O/applications/common/streaming/VideoBuffer.o \
    $O/applications/common/streaming/BufferMap.o \
    $O/applications/common/streaming/NeighborInfo.o \
    $O/applications/common/streaming/PartnerList.o \
    $O/applications/peer/DonetPeer.o \
    $O/applications/peer/Player.o \
    $O/applications/peer/ScampPeer.o \
    $O/applications/source/DonetSource.o \
    $O/applications/source/ScampSource.o \
    $O/applications/source/ChunkGenerator.o \
    $O/global/ActivePeerTable.o \
    $O/global/AppSettingDonet.o \
    $O/global/AppSetting.o \
    $O/global/churn/UniformChurn.o \
    $O/global/churn/StaticChurn.o \
    $O/global/churn/ExponentialChurnModel.o \
    $O/global/churn/PoissonChurnModel.o \
    $O/global/churn/VelosoChurnModel.o \
    $O/global/observer/MeshOverlayObserver.o \
    $O/global/statistic/GlobalStatistic.o \
    $O/global/statistic/Statistic.o \
    $O/applications/common/messages/PeerStreamingPacket_m.o \
    $O/applications/common/messages/MeshPeerStreamingPacket_m.o \
    $O/applications/common/messages/TreePeerStreamingPacket_m.o \
    $O/applications/common/messages/GossipMembershipPacket_m.o \
    $O/applications/common/messages/VideoChunkPacket_m.o \
    $O/applications/common/messages/DpControlInfo_m.o

# Message files
MSGFILES = \
    applications/common/messages/PeerStreamingPacket.msg \
    applications/common/messages/MeshPeerStreamingPacket.msg \
    applications/common/messages/TreePeerStreamingPacket.msg \
    applications/common/messages/GossipMembershipPacket.msg \
    applications/common/messages/VideoChunkPacket.msg \
    applications/common/messages/DpControlInfo.msg

# Other makefile variables (-K)
INET_PROJ=../../inet

#------------------------------------------------------------------------------

# Pull in OMNeT++ configuration (Makefile.inc or configuser.vc)

ifneq ("$(OMNETPP_CONFIGFILE)","")
CONFIGFILE = $(OMNETPP_CONFIGFILE)
else
ifneq ("$(OMNETPP_ROOT)","")
CONFIGFILE = $(OMNETPP_ROOT)/Makefile.inc
else
CONFIGFILE = $(shell opp_configfilepath)
endif
endif

ifeq ("$(wildcard $(CONFIGFILE))","")
$(error Config file '$(CONFIGFILE)' does not exist -- add the OMNeT++ bin directory to the path so that opp_configfilepath can be found, or set the OMNETPP_CONFIGFILE variable to point to Makefile.inc)
endif

include $(CONFIGFILE)

# Simulation kernel and user interface libraries
OMNETPP_LIB_SUBDIR = $(OMNETPP_LIB_DIR)/$(TOOLCHAIN_NAME)
OMNETPP_LIBS = -L"$(OMNETPP_LIB_SUBDIR)" -L"$(OMNETPP_LIB_DIR)" -loppmain$D $(USERIF_LIBS) $(KERNEL_LIBS) $(SYS_LIBS)

COPTS = $(CFLAGS)  $(INCLUDE_PATH) -I$(OMNETPP_INCL_DIR)
MSGCOPTS = $(INCLUDE_PATH)

# we want to recompile everything if COPTS changes,
# so we store COPTS into $COPTS_FILE and have object
# files depend on it (except when "make depend" was called)
COPTS_FILE = $O/.last-copts
ifneq ($(MAKECMDGOALS),depend)
ifneq ("$(COPTS)","$(shell cat $(COPTS_FILE) 2>/dev/null || echo '')")
$(shell $(MKPATH) "$O" && echo "$(COPTS)" >$(COPTS_FILE))
endif
endif

#------------------------------------------------------------------------------
# User-supplied makefile fragment(s)
# >>>
# <<<
#------------------------------------------------------------------------------

# Main target
all: $O/$(TARGET)
	$(LN) $O/$(TARGET) .

$O/$(TARGET): $(OBJS)  $(wildcard $(EXTRA_OBJS)) Makefile
	@$(MKPATH) $O
	$(CXX) $(LDFLAGS) -o $O/$(TARGET)  $(OBJS) $(EXTRA_OBJS) $(AS_NEEDED_OFF) $(WHOLE_ARCHIVE_ON) $(LIBS) $(WHOLE_ARCHIVE_OFF) $(OMNETPP_LIBS)

.PHONY: all clean cleanall depend msgheaders

.SUFFIXES: .cc

$O/%.o: %.cc $(COPTS_FILE)
	@$(MKPATH) $(dir $@)
	$(CXX) -c $(COPTS) -o $@ $<

%_m.cc %_m.h: %.msg
	$(MSGC) -s _m.cc $(MSGCOPTS) $?

msgheaders: $(MSGFILES:.msg=_m.h)

clean:
	-rm -rf $O
	-rm -f scamp scamp.exe libscamp.so libscamp.a libscamp.dll libscamp.dylib
	-rm -f ./*_m.cc ./*_m.h
	-rm -f applications/*_m.cc applications/*_m.h
	-rm -f applications/common/*_m.cc applications/common/*_m.h
	-rm -f applications/common/membership/*_m.cc applications/common/membership/*_m.h
	-rm -f applications/common/messages/*_m.cc applications/common/messages/*_m.h
	-rm -f applications/common/streaming/*_m.cc applications/common/streaming/*_m.h
	-rm -f applications/peer/*_m.cc applications/peer/*_m.h
	-rm -f applications/peer/smc/*_m.cc applications/peer/smc/*_m.h
	-rm -f applications/source/*_m.cc applications/source/*_m.h
	-rm -f global/*_m.cc global/*_m.h
	-rm -f global/churn/*_m.cc global/churn/*_m.h
	-rm -f global/observer/*_m.cc global/observer/*_m.h
	-rm -f global/statistic/*_m.cc global/statistic/*_m.h
	-rm -f nodes/*_m.cc nodes/*_m.h

cleanall: clean
	-rm -rf $(PROJECT_OUTPUT_DIR)

depend:
	$(MAKEDEPEND) $(INCLUDE_PATH) -f Makefile -P\$$O/ -- $(MSG_CC_FILES)  ./*.cc applications/*.cc applications/common/*.cc applications/common/membership/*.cc applications/common/messages/*.cc applications/common/streaming/*.cc applications/peer/*.cc applications/peer/smc/*.cc applications/source/*.cc global/*.cc global/churn/*.cc global/observer/*.cc global/statistic/*.cc nodes/*.cc

# DO NOT DELETE THIS LINE -- make depend depends on it.
$O/applications/CommBase.o: applications/CommBase.cc \
	$(INET_PROJ)/src/base/INETDefs.h \
	$(INET_PROJ)/src/base/INotifiable.h \
	$(INET_PROJ)/src/base/ModuleAccess.h \
	$(INET_PROJ)/src/base/NotificationBoard.h \
	$(INET_PROJ)/src/base/NotifierConsts.h \
	$(INET_PROJ)/src/linklayer/contract/MACAddress.h \
	$(INET_PROJ)/src/networklayer/common/IInterfaceTable.h \
	$(INET_PROJ)/src/networklayer/common/InterfaceEntry.h \
	$(INET_PROJ)/src/networklayer/common/InterfaceTableAccess.h \
	$(INET_PROJ)/src/networklayer/common/InterfaceToken.h \
	$(INET_PROJ)/src/networklayer/contract/IPAddress.h \
	$(INET_PROJ)/src/networklayer/contract/IPv6Address.h \
	$(INET_PROJ)/src/networklayer/contract/IPvXAddress.h \
	$(INET_PROJ)/src/networklayer/ipv4/IPv4InterfaceData.h \
	applications/CommBase.h \
	applications/common/AppCommon.h \
	applications/common/messages/DpControlInfo_m.h \
	global/ActivePeerTable.h \
	global/churn/IChurnGenerator.h \
	global/statistic/GlobalStatistic.h
$O/applications/DonetBase.o: applications/DonetBase.cc \
	$(INET_PROJ)/src/applications/udpapp/UDPAppBase.h \
	$(INET_PROJ)/src/base/INETDefs.h \
	$(INET_PROJ)/src/base/INotifiable.h \
	$(INET_PROJ)/src/base/ModuleAccess.h \
	$(INET_PROJ)/src/base/NotificationBoard.h \
	$(INET_PROJ)/src/base/NotifierConsts.h \
	$(INET_PROJ)/src/linklayer/contract/MACAddress.h \
	$(INET_PROJ)/src/networklayer/common/IInterfaceTable.h \
	$(INET_PROJ)/src/networklayer/common/InterfaceEntry.h \
	$(INET_PROJ)/src/networklayer/common/InterfaceToken.h \
	$(INET_PROJ)/src/networklayer/contract/IPAddress.h \
	$(INET_PROJ)/src/networklayer/contract/IPv6Address.h \
	$(INET_PROJ)/src/networklayer/contract/IPvXAddress.h \
	applications/CommBase.h \
	applications/DonetBase.h \
	applications/common/AppCommon.h \
	applications/common/Dispatcher.h \
	applications/common/Forwarder.h \
	applications/common/messages/DpControlInfo_m.h \
	applications/common/messages/MeshPeerStreamingPacket_m.h \
	applications/common/messages/PeerStreamingPacket_m.h \
	applications/common/messages/VideoChunkPacket_m.h \
	applications/common/streaming/BufferMap.h \
	applications/common/streaming/NeighborInfo.h \
	applications/common/streaming/PartnerList.h \
	applications/common/streaming/VideoBuffer.h \
	global/ActivePeerTable.h \
	global/AppSetting.h \
	global/AppSettingDonet.h \
	global/churn/IChurnGenerator.h \
	global/observer/MeshOverlayObserver.h \
	global/statistic/GlobalStatistic.h
$O/applications/DummyMembership.o: applications/DummyMembership.cc \
	$(INET_PROJ)/src/base/INETDefs.h \
	$(INET_PROJ)/src/base/INotifiable.h \
	$(INET_PROJ)/src/base/ModuleAccess.h \
	$(INET_PROJ)/src/base/NotificationBoard.h \
	$(INET_PROJ)/src/base/NotifierConsts.h \
	$(INET_PROJ)/src/linklayer/contract/MACAddress.h \
	$(INET_PROJ)/src/networklayer/common/IInterfaceTable.h \
	$(INET_PROJ)/src/networklayer/common/InterfaceEntry.h \
	$(INET_PROJ)/src/networklayer/common/InterfaceToken.h \
	$(INET_PROJ)/src/networklayer/contract/IPAddress.h \
	$(INET_PROJ)/src/networklayer/contract/IPv6Address.h \
	$(INET_PROJ)/src/networklayer/contract/IPvXAddress.h \
	applications/CommBase.h \
	applications/DummyMembership.h \
	applications/MembershipBase.h \
	applications/common/AppCommon.h \
	global/ActivePeerTable.h \
	global/churn/IChurnGenerator.h \
	global/statistic/GlobalStatistic.h
$O/applications/MembershipBase.o: applications/MembershipBase.cc \
	$(INET_PROJ)/src/base/INETDefs.h \
	$(INET_PROJ)/src/base/INotifiable.h \
	$(INET_PROJ)/src/base/ModuleAccess.h \
	$(INET_PROJ)/src/base/NotificationBoard.h \
	$(INET_PROJ)/src/base/NotifierConsts.h \
	$(INET_PROJ)/src/linklayer/contract/MACAddress.h \
	$(INET_PROJ)/src/networklayer/common/IInterfaceTable.h \
	$(INET_PROJ)/src/networklayer/common/InterfaceEntry.h \
	$(INET_PROJ)/src/networklayer/common/InterfaceToken.h \
	$(INET_PROJ)/src/networklayer/contract/IPAddress.h \
	$(INET_PROJ)/src/networklayer/contract/IPv6Address.h \
	$(INET_PROJ)/src/networklayer/contract/IPvXAddress.h \
	applications/CommBase.h \
	applications/MembershipBase.h \
	applications/common/AppCommon.h \
	global/ActivePeerTable.h \
	global/churn/IChurnGenerator.h \
	global/statistic/GlobalStatistic.h
$O/applications/ScampBase.o: applications/ScampBase.cc \
	$(INET_PROJ)/src/applications/udpapp/UDPAppBase.h \
	$(INET_PROJ)/src/base/INETDefs.h \
	$(INET_PROJ)/src/base/INotifiable.h \
	$(INET_PROJ)/src/base/ModuleAccess.h \
	$(INET_PROJ)/src/base/NotificationBoard.h \
	$(INET_PROJ)/src/base/NotifierConsts.h \
	$(INET_PROJ)/src/linklayer/contract/MACAddress.h \
	$(INET_PROJ)/src/networklayer/common/IInterfaceTable.h \
	$(INET_PROJ)/src/networklayer/common/InterfaceEntry.h \
	$(INET_PROJ)/src/networklayer/common/InterfaceTableAccess.h \
	$(INET_PROJ)/src/networklayer/common/InterfaceToken.h \
	$(INET_PROJ)/src/networklayer/contract/IPAddress.h \
	$(INET_PROJ)/src/networklayer/contract/IPv6Address.h \
	$(INET_PROJ)/src/networklayer/contract/IPvXAddress.h \
	$(INET_PROJ)/src/networklayer/ipv4/IPv4InterfaceData.h \
	applications/CommBase.h \
	applications/ScampBase.h \
	applications/common/AppCommon.h \
	applications/common/Dispatcher.h \
	applications/common/membership/Contact.h \
	applications/common/membership/MessageLogger.h \
	applications/common/messages/DpControlInfo_m.h \
	applications/common/messages/GossipMembershipPacket_m.h \
	applications/common/messages/PeerStreamingPacket_m.h \
	global/ActivePeerTable.h \
	global/churn/IChurnGenerator.h \
	global/statistic/GlobalStatistic.h
$O/applications/common/Dispatcher.o: applications/common/Dispatcher.cc \
	$(INET_PROJ)/src/applications/udpapp/UDPAppBase.h \
	$(INET_PROJ)/src/base/INETDefs.h \
	$(INET_PROJ)/src/networklayer/contract/IPAddress.h \
	$(INET_PROJ)/src/networklayer/contract/IPv6Address.h \
	$(INET_PROJ)/src/networklayer/contract/IPvXAddress.h \
	$(INET_PROJ)/src/transport/contract/UDPControlInfo_m.h \
	applications/common/Dispatcher.h \
	applications/common/messages/DpControlInfo_m.h \
	applications/common/messages/PeerStreamingPacket_m.h
$O/applications/common/Forwarder.o: applications/common/Forwarder.cc \
	$(INET_PROJ)/src/applications/udpapp/UDPAppBase.h \
	$(INET_PROJ)/src/base/INETDefs.h \
	$(INET_PROJ)/src/base/INotifiable.h \
	$(INET_PROJ)/src/base/ModuleAccess.h \
	$(INET_PROJ)/src/base/NotificationBoard.h \
	$(INET_PROJ)/src/base/NotifierConsts.h \
	$(INET_PROJ)/src/linklayer/contract/MACAddress.h \
	$(INET_PROJ)/src/networklayer/common/IInterfaceTable.h \
	$(INET_PROJ)/src/networklayer/common/InterfaceEntry.h \
	$(INET_PROJ)/src/networklayer/common/InterfaceToken.h \
	$(INET_PROJ)/src/networklayer/contract/IPAddress.h \
	$(INET_PROJ)/src/networklayer/contract/IPv6Address.h \
	$(INET_PROJ)/src/networklayer/contract/IPvXAddress.h \
	applications/CommBase.h \
	applications/common/AppCommon.h \
	applications/common/Dispatcher.h \
	applications/common/Forwarder.h \
	applications/common/messages/MeshPeerStreamingPacket_m.h \
	applications/common/messages/PeerStreamingPacket_m.h \
	applications/common/messages/VideoChunkPacket_m.h \
	applications/common/streaming/BufferMap.h \
	applications/common/streaming/VideoBuffer.h \
	global/ActivePeerTable.h \
	global/churn/IChurnGenerator.h \
	global/statistic/GlobalStatistic.h
$O/applications/common/membership/Contact.o: applications/common/membership/Contact.cc \
	$(INET_PROJ)/src/base/INETDefs.h \
	$(INET_PROJ)/src/networklayer/contract/IPAddress.h \
	$(INET_PROJ)/src/networklayer/contract/IPv6Address.h \
	$(INET_PROJ)/src/networklayer/contract/IPvXAddress.h \
	$(INET_PROJ)/src/transport/contract/UDPControlInfo_m.h \
	$(INET_PROJ)/src/transport/udp/UDP.h \
	applications/common/membership/Contact.h
$O/applications/common/membership/MCache.o: applications/common/membership/MCache.cc \
	applications/common/membership/MCache.h
$O/applications/common/membership/MessageLogger.o: applications/common/membership/MessageLogger.cc \
	$(INET_PROJ)/src/base/INETDefs.h \
	$(INET_PROJ)/src/networklayer/contract/IPAddress.h \
	$(INET_PROJ)/src/networklayer/contract/IPv6Address.h \
	$(INET_PROJ)/src/networklayer/contract/IPvXAddress.h \
	applications/common/AppCommon.h \
	applications/common/membership/MessageLogger.h \
	applications/common/messages/GossipMembershipPacket_m.h \
	applications/common/messages/PeerStreamingPacket_m.h
$O/applications/common/messages/DpControlInfo_m.o: applications/common/messages/DpControlInfo_m.cc \
	$(INET_PROJ)/src/base/INETDefs.h \
	$(INET_PROJ)/src/networklayer/contract/IPAddress.h \
	$(INET_PROJ)/src/networklayer/contract/IPv6Address.h \
	$(INET_PROJ)/src/networklayer/contract/IPvXAddress.h \
	applications/common/messages/DpControlInfo_m.h
$O/applications/common/messages/GossipMembershipPacket_m.o: applications/common/messages/GossipMembershipPacket_m.cc \
	$(INET_PROJ)/src/base/INETDefs.h \
	$(INET_PROJ)/src/networklayer/contract/IPAddress.h \
	$(INET_PROJ)/src/networklayer/contract/IPv6Address.h \
	$(INET_PROJ)/src/networklayer/contract/IPvXAddress.h \
	applications/common/messages/GossipMembershipPacket_m.h \
	applications/common/messages/PeerStreamingPacket_m.h
$O/applications/common/messages/MeshPeerStreamingPacket_m.o: applications/common/messages/MeshPeerStreamingPacket_m.cc \
	applications/common/messages/MeshPeerStreamingPacket_m.h \
	applications/common/messages/PeerStreamingPacket_m.h
$O/applications/common/messages/PeerStreamingPacket_m.o: applications/common/messages/PeerStreamingPacket_m.cc \
	applications/common/messages/PeerStreamingPacket_m.h
$O/applications/common/messages/TreePeerStreamingPacket_m.o: applications/common/messages/TreePeerStreamingPacket_m.cc \
	applications/common/messages/PeerStreamingPacket_m.h \
	applications/common/messages/TreePeerStreamingPacket_m.h
$O/applications/common/messages/VideoChunkPacket_m.o: applications/common/messages/VideoChunkPacket_m.cc \
	applications/common/messages/PeerStreamingPacket_m.h \
	applications/common/messages/VideoChunkPacket_m.h
$O/applications/common/streaming/BufferMap.o: applications/common/streaming/BufferMap.cc \
	$(INET_PROJ)/src/base/INETDefs.h \
	$(INET_PROJ)/src/networklayer/contract/IPAddress.h \
	$(INET_PROJ)/src/networklayer/contract/IPv6Address.h \
	$(INET_PROJ)/src/networklayer/contract/IPvXAddress.h \
	applications/common/AppCommon.h \
	applications/common/messages/MeshPeerStreamingPacket_m.h \
	applications/common/messages/PeerStreamingPacket_m.h \
	applications/common/streaming/BufferMap.h
$O/applications/common/streaming/NeighborInfo.o: applications/common/streaming/NeighborInfo.cc \
	$(INET_PROJ)/src/base/INETDefs.h \
	$(INET_PROJ)/src/networklayer/contract/IPAddress.h \
	$(INET_PROJ)/src/networklayer/contract/IPv6Address.h \
	$(INET_PROJ)/src/networklayer/contract/IPvXAddress.h \
	applications/common/AppCommon.h \
	applications/common/messages/MeshPeerStreamingPacket_m.h \
	applications/common/messages/PeerStreamingPacket_m.h \
	applications/common/streaming/BufferMap.h \
	applications/common/streaming/NeighborInfo.h
$O/applications/common/streaming/PartnerList.o: applications/common/streaming/PartnerList.cc \
	$(INET_PROJ)/src/base/INETDefs.h \
	$(INET_PROJ)/src/networklayer/contract/IPAddress.h \
	$(INET_PROJ)/src/networklayer/contract/IPv6Address.h \
	$(INET_PROJ)/src/networklayer/contract/IPvXAddress.h \
	applications/common/AppCommon.h \
	applications/common/messages/MeshPeerStreamingPacket_m.h \
	applications/common/messages/PeerStreamingPacket_m.h \
	applications/common/streaming/BufferMap.h \
	applications/common/streaming/NeighborInfo.h \
	applications/common/streaming/PartnerList.h \
	global/AppSetting.h \
	global/AppSettingDonet.h
$O/applications/common/streaming/VideoBuffer.o: applications/common/streaming/VideoBuffer.cc \
	$(INET_PROJ)/src/base/INETDefs.h \
	$(INET_PROJ)/src/networklayer/contract/IPAddress.h \
	$(INET_PROJ)/src/networklayer/contract/IPv6Address.h \
	$(INET_PROJ)/src/networklayer/contract/IPvXAddress.h \
	applications/common/AppCommon.h \
	applications/common/messages/MeshPeerStreamingPacket_m.h \
	applications/common/messages/PeerStreamingPacket_m.h \
	applications/common/messages/VideoChunkPacket_m.h \
	applications/common/streaming/BufferMap.h \
	applications/common/streaming/VideoBuffer.h \
	global/AppSetting.h \
	global/AppSettingDonet.h
$O/applications/peer/DonetPeer.o: applications/peer/DonetPeer.cc \
	$(INET_PROJ)/src/applications/udpapp/UDPAppBase.h \
	$(INET_PROJ)/src/base/INETDefs.h \
	$(INET_PROJ)/src/base/INotifiable.h \
	$(INET_PROJ)/src/base/ModuleAccess.h \
	$(INET_PROJ)/src/base/NotificationBoard.h \
	$(INET_PROJ)/src/base/NotifierConsts.h \
	$(INET_PROJ)/src/linklayer/contract/MACAddress.h \
	$(INET_PROJ)/src/networklayer/common/IInterfaceTable.h \
	$(INET_PROJ)/src/networklayer/common/InterfaceEntry.h \
	$(INET_PROJ)/src/networklayer/common/InterfaceTableAccess.h \
	$(INET_PROJ)/src/networklayer/common/InterfaceToken.h \
	$(INET_PROJ)/src/networklayer/contract/IPAddress.h \
	$(INET_PROJ)/src/networklayer/contract/IPv6Address.h \
	$(INET_PROJ)/src/networklayer/contract/IPvXAddress.h \
	$(INET_PROJ)/src/networklayer/ipv4/IPv4InterfaceData.h \
	applications/CommBase.h \
	applications/DonetBase.h \
	applications/common/AppCommon.h \
	applications/common/Dispatcher.h \
	applications/common/Forwarder.h \
	applications/common/messages/DpControlInfo_m.h \
	applications/common/messages/MeshPeerStreamingPacket_m.h \
	applications/common/messages/PeerStreamingPacket_m.h \
	applications/common/messages/VideoChunkPacket_m.h \
	applications/common/streaming/BufferMap.h \
	applications/common/streaming/NeighborInfo.h \
	applications/common/streaming/PartnerList.h \
	applications/common/streaming/VideoBuffer.h \
	applications/peer/DonetPeer.h \
	applications/peer/Player.h \
	global/ActivePeerTable.h \
	global/AppSetting.h \
	global/AppSettingDonet.h \
	global/churn/IChurnGenerator.h \
	global/observer/MeshOverlayObserver.h \
	global/statistic/GlobalStatistic.h
$O/applications/peer/Player.o: applications/peer/Player.cc \
	$(INET_PROJ)/src/base/INETDefs.h \
	$(INET_PROJ)/src/base/INotifiable.h \
	$(INET_PROJ)/src/base/ModuleAccess.h \
	$(INET_PROJ)/src/base/NotificationBoard.h \
	$(INET_PROJ)/src/base/NotifierConsts.h \
	$(INET_PROJ)/src/networklayer/contract/IPAddress.h \
	$(INET_PROJ)/src/networklayer/contract/IPv6Address.h \
	$(INET_PROJ)/src/networklayer/contract/IPvXAddress.h \
	applications/common/AppCommon.h \
	applications/common/messages/MeshPeerStreamingPacket_m.h \
	applications/common/messages/PeerStreamingPacket_m.h \
	applications/common/messages/VideoChunkPacket_m.h \
	applications/common/streaming/BufferMap.h \
	applications/common/streaming/VideoBuffer.h \
	applications/peer/Player.h \
	global/AppSetting.h \
	global/AppSettingDonet.h \
	global/churn/IChurnGenerator.h \
	global/statistic/GlobalStatistic.h
$O/applications/peer/ScampPeer.o: applications/peer/ScampPeer.cc \
	$(INET_PROJ)/src/applications/udpapp/UDPAppBase.h \
	$(INET_PROJ)/src/applications/udpapp/UDPBasicApp.h \
	$(INET_PROJ)/src/base/INETDefs.h \
	$(INET_PROJ)/src/base/INotifiable.h \
	$(INET_PROJ)/src/base/ModuleAccess.h \
	$(INET_PROJ)/src/base/NotificationBoard.h \
	$(INET_PROJ)/src/base/NotifierConsts.h \
	$(INET_PROJ)/src/linklayer/contract/MACAddress.h \
	$(INET_PROJ)/src/networklayer/common/IInterfaceTable.h \
	$(INET_PROJ)/src/networklayer/common/InterfaceEntry.h \
	$(INET_PROJ)/src/networklayer/common/InterfaceTableAccess.h \
	$(INET_PROJ)/src/networklayer/common/InterfaceToken.h \
	$(INET_PROJ)/src/networklayer/contract/IPAddress.h \
	$(INET_PROJ)/src/networklayer/contract/IPAddressResolver.h \
	$(INET_PROJ)/src/networklayer/contract/IPv6Address.h \
	$(INET_PROJ)/src/networklayer/contract/IPvXAddress.h \
	$(INET_PROJ)/src/networklayer/ipv4/IPv4InterfaceData.h \
	$(INET_PROJ)/src/transport/udp/UDPPacket.h \
	$(INET_PROJ)/src/transport/udp/UDPPacket_m.h \
	applications/CommBase.h \
	applications/ScampBase.h \
	applications/common/AppCommon.h \
	applications/common/Dispatcher.h \
	applications/common/membership/Contact.h \
	applications/common/membership/MessageLogger.h \
	applications/common/messages/DpControlInfo_m.h \
	applications/common/messages/GossipMembershipPacket_m.h \
	applications/common/messages/MeshPeerStreamingPacket_m.h \
	applications/common/messages/PeerStreamingPacket_m.h \
	applications/peer/ScampPeer.h \
	global/ActivePeerTable.h \
	global/churn/IChurnGenerator.h \
	global/statistic/GlobalStatistic.h
$O/applications/source/ChunkGenerator.o: applications/source/ChunkGenerator.cc \
	$(INET_PROJ)/src/base/INETDefs.h \
	$(INET_PROJ)/src/networklayer/contract/IPAddress.h \
	$(INET_PROJ)/src/networklayer/contract/IPv6Address.h \
	$(INET_PROJ)/src/networklayer/contract/IPvXAddress.h \
	applications/common/AppCommon.h \
	applications/common/messages/MeshPeerStreamingPacket_m.h \
	applications/common/messages/PeerStreamingPacket_m.h \
	applications/common/messages/VideoChunkPacket_m.h \
	applications/common/streaming/BufferMap.h \
	applications/common/streaming/VideoBuffer.h \
	applications/source/ChunkGenerator.h \
	global/AppSetting.h \
	global/AppSettingDonet.h
$O/applications/source/DonetSource.o: applications/source/DonetSource.cc \
	$(INET_PROJ)/src/applications/udpapp/UDPAppBase.h \
	$(INET_PROJ)/src/base/INETDefs.h \
	$(INET_PROJ)/src/base/INotifiable.h \
	$(INET_PROJ)/src/base/ModuleAccess.h \
	$(INET_PROJ)/src/base/NotificationBoard.h \
	$(INET_PROJ)/src/base/NotifierConsts.h \
	$(INET_PROJ)/src/linklayer/contract/MACAddress.h \
	$(INET_PROJ)/src/networklayer/common/IInterfaceTable.h \
	$(INET_PROJ)/src/networklayer/common/InterfaceEntry.h \
	$(INET_PROJ)/src/networklayer/common/InterfaceTableAccess.h \
	$(INET_PROJ)/src/networklayer/common/InterfaceToken.h \
	$(INET_PROJ)/src/networklayer/contract/IPAddress.h \
	$(INET_PROJ)/src/networklayer/contract/IPv6Address.h \
	$(INET_PROJ)/src/networklayer/contract/IPvXAddress.h \
	$(INET_PROJ)/src/networklayer/ipv4/IPv4InterfaceData.h \
	applications/CommBase.h \
	applications/DonetBase.h \
	applications/common/AppCommon.h \
	applications/common/Dispatcher.h \
	applications/common/Forwarder.h \
	applications/common/messages/DpControlInfo_m.h \
	applications/common/messages/MeshPeerStreamingPacket_m.h \
	applications/common/messages/PeerStreamingPacket_m.h \
	applications/common/messages/VideoChunkPacket_m.h \
	applications/common/streaming/BufferMap.h \
	applications/common/streaming/NeighborInfo.h \
	applications/common/streaming/PartnerList.h \
	applications/common/streaming/VideoBuffer.h \
	applications/source/DonetSource.h \
	global/ActivePeerTable.h \
	global/AppSetting.h \
	global/AppSettingDonet.h \
	global/churn/IChurnGenerator.h \
	global/observer/MeshOverlayObserver.h \
	global/statistic/GlobalStatistic.h
$O/applications/source/ScampSource.o: applications/source/ScampSource.cc \
	$(INET_PROJ)/src/applications/udpapp/UDPAppBase.h \
	$(INET_PROJ)/src/base/INETDefs.h \
	$(INET_PROJ)/src/base/INotifiable.h \
	$(INET_PROJ)/src/base/ModuleAccess.h \
	$(INET_PROJ)/src/base/NotificationBoard.h \
	$(INET_PROJ)/src/base/NotifierConsts.h \
	$(INET_PROJ)/src/linklayer/contract/MACAddress.h \
	$(INET_PROJ)/src/networklayer/common/IInterfaceTable.h \
	$(INET_PROJ)/src/networklayer/common/InterfaceEntry.h \
	$(INET_PROJ)/src/networklayer/common/InterfaceTableAccess.h \
	$(INET_PROJ)/src/networklayer/common/InterfaceToken.h \
	$(INET_PROJ)/src/networklayer/contract/IPAddress.h \
	$(INET_PROJ)/src/networklayer/contract/IPv6Address.h \
	$(INET_PROJ)/src/networklayer/contract/IPvXAddress.h \
	$(INET_PROJ)/src/networklayer/ipv4/IPv4InterfaceData.h \
	$(INET_PROJ)/src/transport/contract/UDPControlInfo_m.h \
	applications/CommBase.h \
	applications/ScampBase.h \
	applications/common/AppCommon.h \
	applications/common/Dispatcher.h \
	applications/common/membership/Contact.h \
	applications/common/membership/MessageLogger.h \
	applications/common/messages/GossipMembershipPacket_m.h \
	applications/common/messages/MeshPeerStreamingPacket_m.h \
	applications/common/messages/PeerStreamingPacket_m.h \
	applications/source/ScampSource.h \
	global/ActivePeerTable.h \
	global/churn/IChurnGenerator.h \
	global/statistic/GlobalStatistic.h
$O/global/ActivePeerTable.o: global/ActivePeerTable.cc \
	$(INET_PROJ)/src/base/INETDefs.h \
	$(INET_PROJ)/src/base/INotifiable.h \
	$(INET_PROJ)/src/base/ModuleAccess.h \
	$(INET_PROJ)/src/base/NotificationBoard.h \
	$(INET_PROJ)/src/base/NotifierConsts.h \
	$(INET_PROJ)/src/linklayer/contract/MACAddress.h \
	$(INET_PROJ)/src/networklayer/common/IInterfaceTable.h \
	$(INET_PROJ)/src/networklayer/common/InterfaceEntry.h \
	$(INET_PROJ)/src/networklayer/common/InterfaceToken.h \
	$(INET_PROJ)/src/networklayer/contract/IPAddress.h \
	$(INET_PROJ)/src/networklayer/contract/IPv6Address.h \
	$(INET_PROJ)/src/networklayer/contract/IPvXAddress.h \
	global/ActivePeerTable.h
$O/global/AppSetting.o: global/AppSetting.cc \
	global/AppSetting.h
$O/global/AppSettingDonet.o: global/AppSettingDonet.cc \
	global/AppSetting.h \
	global/AppSettingDonet.h
$O/global/churn/ExponentialChurnModel.o: global/churn/ExponentialChurnModel.cc \
	$(INET_PROJ)/src/base/INETDefs.h \
	$(INET_PROJ)/src/base/INotifiable.h \
	$(INET_PROJ)/src/base/ModuleAccess.h \
	$(INET_PROJ)/src/base/NotificationBoard.h \
	$(INET_PROJ)/src/base/NotifierConsts.h \
	global/churn/ExponentialChurnModel.h \
	global/churn/IChurnGenerator.h
$O/global/churn/PoissonChurnModel.o: global/churn/PoissonChurnModel.cc \
	$(INET_PROJ)/src/base/INETDefs.h \
	$(INET_PROJ)/src/base/INotifiable.h \
	$(INET_PROJ)/src/base/ModuleAccess.h \
	$(INET_PROJ)/src/base/NotificationBoard.h \
	$(INET_PROJ)/src/base/NotifierConsts.h \
	global/churn/IChurnGenerator.h \
	global/churn/PoissonChurnModel.h
$O/global/churn/StaticChurn.o: global/churn/StaticChurn.cc \
	$(INET_PROJ)/src/base/INETDefs.h \
	$(INET_PROJ)/src/base/INotifiable.h \
	$(INET_PROJ)/src/base/ModuleAccess.h \
	$(INET_PROJ)/src/base/NotificationBoard.h \
	$(INET_PROJ)/src/base/NotifierConsts.h \
	global/churn/IChurnGenerator.h \
	global/churn/StaticChurn.h
$O/global/churn/UniformChurn.o: global/churn/UniformChurn.cc \
	$(INET_PROJ)/src/base/INETDefs.h \
	$(INET_PROJ)/src/base/INotifiable.h \
	$(INET_PROJ)/src/base/ModuleAccess.h \
	$(INET_PROJ)/src/base/NotificationBoard.h \
	$(INET_PROJ)/src/base/NotifierConsts.h \
	global/churn/IChurnGenerator.h \
	global/churn/UniformChurn.h
$O/global/churn/VelosoChurnModel.o: global/churn/VelosoChurnModel.cc \
	$(INET_PROJ)/src/base/INETDefs.h \
	$(INET_PROJ)/src/base/INotifiable.h \
	$(INET_PROJ)/src/base/ModuleAccess.h \
	$(INET_PROJ)/src/base/NotificationBoard.h \
	$(INET_PROJ)/src/base/NotifierConsts.h \
	global/churn/IChurnGenerator.h \
	global/churn/VelosoChurnModel.h
$O/global/observer/MeshOverlayObserver.o: global/observer/MeshOverlayObserver.cc \
	$(INET_PROJ)/src/base/INETDefs.h \
	$(INET_PROJ)/src/networklayer/contract/IPAddress.h \
	$(INET_PROJ)/src/networklayer/contract/IPv6Address.h \
	$(INET_PROJ)/src/networklayer/contract/IPvXAddress.h \
	applications/common/AppCommon.h \
	global/observer/MeshOverlayObserver.h
$O/global/statistic/GlobalStatistic.o: global/statistic/GlobalStatistic.cc \
	$(INET_PROJ)/src/base/INETDefs.h \
	$(INET_PROJ)/src/base/INotifiable.h \
	$(INET_PROJ)/src/base/ModuleAccess.h \
	$(INET_PROJ)/src/base/NotificationBoard.h \
	$(INET_PROJ)/src/base/NotifierConsts.h \
	$(INET_PROJ)/src/networklayer/contract/IPAddress.h \
	$(INET_PROJ)/src/networklayer/contract/IPv6Address.h \
	$(INET_PROJ)/src/networklayer/contract/IPvXAddress.h \
	applications/common/AppCommon.h \
	global/churn/IChurnGenerator.h \
	global/statistic/GlobalStatistic.h
$O/global/statistic/Statistic.o: global/statistic/Statistic.cc \
	$(INET_PROJ)/src/base/INETDefs.h \
	$(INET_PROJ)/src/base/INotifiable.h \
	$(INET_PROJ)/src/base/ModuleAccess.h \
	$(INET_PROJ)/src/base/NotificationBoard.h \
	$(INET_PROJ)/src/base/NotifierConsts.h \
	global/churn/IChurnGenerator.h \
	global/statistic/Statistic.h

