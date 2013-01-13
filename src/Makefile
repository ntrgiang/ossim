#
# OMNeT++/OMNEST Makefile for so
#
# This file was generated with the command:
#  opp_makemake -f --deep -I../../inet/src/networklayer/common -I../../inet/src/networklayer/rsvp_te -I../../inet/src/networklayer/icmpv6 -I../../inet/src/transport/tcp -I../../inet/src/networklayer/mpls -I../../inet/src/networklayer/ted -I../../inet/src/networklayer/contract -I../../inet/src/util -I../../inet/src/transport/contract -I../../inet/src/linklayer/mfcore -I../../inet/src/networklayer/ldp -I../../inet/src/applications/udpapp -I../../inet/src/networklayer/ipv4 -I../../inet/src/base -I../../inet/src/util/headerserializers -I../../inet/src/networklayer/ipv6 -I../../inet/src/transport/sctp -I../../inet/src/world -I../../inet/src/applications/pingapp -I../../inet/src/linklayer/contract -I../../inet/src/networklayer/arp -I../../inet/src/transport/udp -L../../inet/out/$(CONFIGNAME)/src -linet -KINET_PROJ=../../inet
#

# Name of target to be created (-o option)
TARGET = so$(EXE_SUFFIX)

# User interface (uncomment one) (-u option)
USERIF_LIBS = $(ALL_ENV_LIBS) # that is, $(TKENV_LIBS) $(CMDENV_LIBS)
#USERIF_LIBS = $(CMDENV_LIBS)
#USERIF_LIBS = $(TKENV_LIBS)

# C++ include paths (with -I)
INCLUDE_PATH = \
    -I../../inet/src/networklayer/common \
    -I../../inet/src/networklayer/rsvp_te \
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
    -I../../inet/src/networklayer/ipv6 \
    -I../../inet/src/transport/sctp \
    -I../../inet/src/world \
    -I../../inet/src/applications/pingapp \
    -I../../inet/src/linklayer/contract \
    -I../../inet/src/networklayer/arp \
    -I../../inet/src/transport/udp \
    -I. \
    -Icommon \
    -Iglobal \
    -Iglobal/churn \
    -Iglobal/logger \
    -Iglobal/observer \
    -Iglobal/statistic \
    -Imembership \
    -Imembership/dummy \
    -Imembership/newscast \
    -Imembership/oracle \
    -Imembership/scamp \
    -Imesh \
    -Imesh/coolstreaming \
    -Imesh/donet \
    -Ipayload \
    -Itree

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
    $O/common/CommBase.o \
    $O/common/Dispatcher.o \
    $O/global/ActivePeerTable.o \
    $O/global/ActivePeerInfo.o \
    $O/global/AppSettingDonet.o \
    $O/global/AppSetting.o \
    $O/global/churn/UniformChurn.o \
    $O/global/churn/StaticChurn.o \
    $O/global/churn/ExponentialChurnModel.o \
    $O/global/churn/PoissonChurnModel.o \
    $O/global/churn/VelosoChurnModel.o \
    $O/global/logger/Logger.o \
    $O/global/observer/MeshOverlayObserver.o \
    $O/global/statistic/GlobalStatistic.o \
    $O/global/statistic/Statistic.o \
    $O/membership/MCache.o \
    $O/membership/Contact.o \
    $O/membership/MembershipBase.o \
    $O/membership/MessageLogger.o \
    $O/membership/dummy/DummyMembership.o \
    $O/membership/newscast/NewscastStatistic.o \
    $O/membership/newscast/NewscastSource.o \
    $O/membership/newscast/GossipProtocol.o \
    $O/membership/newscast/GossipProtocolWithUserData.o \
    $O/membership/newscast/GossipUserData.o \
    $O/membership/newscast/NewscastCacheEntry.o \
    $O/membership/newscast/NewscastCache.o \
    $O/membership/newscast/NewscastPeer.o \
    $O/membership/newscast/NewscastBase.o \
    $O/membership/oracle/OracleMembership.o \
    $O/membership/scamp/ScampSource.o \
    $O/membership/scamp/ScampBase.o \
    $O/membership/scamp/ScampPeer.o \
    $O/mesh/BufferMap.o \
    $O/mesh/NeighborInfo.o \
    $O/mesh/PartnerList.o \
    $O/mesh/coolstreaming/CoolstreamingPeer.o \
    $O/mesh/coolstreaming/CoolstreamingPartner.o \
    $O/mesh/coolstreaming/CoolstreamingBase.o \
    $O/mesh/coolstreaming/CoolstreamingSource.o \
    $O/mesh/donet/DonetBase.o \
    $O/mesh/donet/DonetPeer.o \
    $O/mesh/donet/DonetSource.o \
    $O/payload/VideoBuffer.o \
    $O/payload/Player.o \
    $O/payload/VideoBufferListener.o \
    $O/payload/Forwarder.o \
    $O/payload/ChunkGenerator.o \
    $O/common/PeerStreamingPacket_m.o \
    $O/common/DpControlInfo_m.o \
    $O/membership/GossipMembershipPacket_m.o \
    $O/membership/newscast/NewscastPacket_m.o \
    $O/mesh/MeshPeerStreamingPacket_m.o \
    $O/mesh/coolstreaming/CoolstreamingPacket_m.o \
    $O/payload/VideoChunkPacket_m.o \
    $O/tree/TreePeerStreamingPacket_m.o

# Message files
MSGFILES = \
    common/PeerStreamingPacket.msg \
    common/DpControlInfo.msg \
    membership/GossipMembershipPacket.msg \
    membership/newscast/NewscastPacket.msg \
    mesh/MeshPeerStreamingPacket.msg \
    mesh/coolstreaming/CoolstreamingPacket.msg \
    payload/VideoChunkPacket.msg \
    tree/TreePeerStreamingPacket.msg

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
	-rm -f so so.exe libso.so libso.a libso.dll libso.dylib
	-rm -f ./*_m.cc ./*_m.h
	-rm -f common/*_m.cc common/*_m.h
	-rm -f global/*_m.cc global/*_m.h
	-rm -f global/churn/*_m.cc global/churn/*_m.h
	-rm -f global/logger/*_m.cc global/logger/*_m.h
	-rm -f global/observer/*_m.cc global/observer/*_m.h
	-rm -f global/statistic/*_m.cc global/statistic/*_m.h
	-rm -f membership/*_m.cc membership/*_m.h
	-rm -f membership/dummy/*_m.cc membership/dummy/*_m.h
	-rm -f membership/newscast/*_m.cc membership/newscast/*_m.h
	-rm -f membership/oracle/*_m.cc membership/oracle/*_m.h
	-rm -f membership/scamp/*_m.cc membership/scamp/*_m.h
	-rm -f mesh/*_m.cc mesh/*_m.h
	-rm -f mesh/coolstreaming/*_m.cc mesh/coolstreaming/*_m.h
	-rm -f mesh/donet/*_m.cc mesh/donet/*_m.h
	-rm -f payload/*_m.cc payload/*_m.h
	-rm -f tree/*_m.cc tree/*_m.h

cleanall: clean
	-rm -rf $(PROJECT_OUTPUT_DIR)

depend:
	$(MAKEDEPEND) $(INCLUDE_PATH) -f Makefile -P\$$O/ -- $(MSG_CC_FILES)  ./*.cc common/*.cc global/*.cc global/churn/*.cc global/logger/*.cc global/observer/*.cc global/statistic/*.cc membership/*.cc membership/dummy/*.cc membership/newscast/*.cc membership/oracle/*.cc membership/scamp/*.cc mesh/*.cc mesh/coolstreaming/*.cc mesh/donet/*.cc payload/*.cc tree/*.cc

# DO NOT DELETE THIS LINE -- make depend depends on it.
$O/common/CommBase.o: common/CommBase.cc \
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
	common/AppCommon.h \
	common/CommBase.h \
	common/DpControlInfo_m.h \
	global/ActivePeerInfo.h \
	global/ActivePeerTable.h \
	global/churn/IChurnGenerator.h \
	global/statistic/GlobalStatistic.h
$O/common/Dispatcher.o: common/Dispatcher.cc \
	$(INET_PROJ)/src/applications/udpapp/UDPAppBase.h \
	$(INET_PROJ)/src/base/INETDefs.h \
	$(INET_PROJ)/src/networklayer/contract/IPAddress.h \
	$(INET_PROJ)/src/networklayer/contract/IPv6Address.h \
	$(INET_PROJ)/src/networklayer/contract/IPvXAddress.h \
	$(INET_PROJ)/src/transport/contract/UDPControlInfo_m.h \
	common/Dispatcher.h \
	common/DpControlInfo_m.h \
	common/PeerStreamingPacket_m.h
$O/common/DpControlInfo_m.o: common/DpControlInfo_m.cc \
	$(INET_PROJ)/src/base/INETDefs.h \
	$(INET_PROJ)/src/networklayer/contract/IPAddress.h \
	$(INET_PROJ)/src/networklayer/contract/IPv6Address.h \
	$(INET_PROJ)/src/networklayer/contract/IPvXAddress.h \
	common/DpControlInfo_m.h
$O/common/PeerStreamingPacket_m.o: common/PeerStreamingPacket_m.cc \
	common/PeerStreamingPacket_m.h
$O/global/ActivePeerInfo.o: global/ActivePeerInfo.cc \
	$(INET_PROJ)/src/base/INETDefs.h \
	$(INET_PROJ)/src/networklayer/contract/IPAddress.h \
	$(INET_PROJ)/src/networklayer/contract/IPv6Address.h \
	$(INET_PROJ)/src/networklayer/contract/IPvXAddress.h \
	global/ActivePeerInfo.h
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
	common/AppCommon.h \
	global/ActivePeerInfo.h \
	global/ActivePeerTable.h \
	global/churn/IChurnGenerator.h \
	global/statistic/GlobalStatistic.h
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
$O/global/logger/Logger.o: global/logger/Logger.cc \
	$(INET_PROJ)/src/base/INETDefs.h \
	$(INET_PROJ)/src/networklayer/contract/IPAddress.h \
	$(INET_PROJ)/src/networklayer/contract/IPv6Address.h \
	$(INET_PROJ)/src/networklayer/contract/IPvXAddress.h \
	common/AppCommon.h \
	global/logger/Logger.h
$O/global/observer/MeshOverlayObserver.o: global/observer/MeshOverlayObserver.cc \
	$(INET_PROJ)/src/base/INETDefs.h \
	$(INET_PROJ)/src/networklayer/contract/IPAddress.h \
	$(INET_PROJ)/src/networklayer/contract/IPv6Address.h \
	$(INET_PROJ)/src/networklayer/contract/IPvXAddress.h \
	common/AppCommon.h \
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
	common/AppCommon.h \
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
$O/membership/Contact.o: membership/Contact.cc \
	$(INET_PROJ)/src/base/INETDefs.h \
	$(INET_PROJ)/src/networklayer/contract/IPAddress.h \
	$(INET_PROJ)/src/networklayer/contract/IPv6Address.h \
	$(INET_PROJ)/src/networklayer/contract/IPvXAddress.h \
	$(INET_PROJ)/src/transport/contract/UDPControlInfo_m.h \
	$(INET_PROJ)/src/transport/udp/UDP.h \
	membership/Contact.h
$O/membership/GossipMembershipPacket_m.o: membership/GossipMembershipPacket_m.cc \
	$(INET_PROJ)/src/base/INETDefs.h \
	$(INET_PROJ)/src/networklayer/contract/IPAddress.h \
	$(INET_PROJ)/src/networklayer/contract/IPv6Address.h \
	$(INET_PROJ)/src/networklayer/contract/IPvXAddress.h \
	common/PeerStreamingPacket_m.h \
	membership/GossipMembershipPacket_m.h
$O/membership/MCache.o: membership/MCache.cc \
	membership/MCache.h
$O/membership/MembershipBase.o: membership/MembershipBase.cc \
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
	common/AppCommon.h \
	common/CommBase.h \
	global/ActivePeerInfo.h \
	global/ActivePeerTable.h \
	global/churn/IChurnGenerator.h \
	global/statistic/GlobalStatistic.h \
	membership/MembershipBase.h
$O/membership/MessageLogger.o: membership/MessageLogger.cc \
	$(INET_PROJ)/src/base/INETDefs.h \
	$(INET_PROJ)/src/networklayer/contract/IPAddress.h \
	$(INET_PROJ)/src/networklayer/contract/IPv6Address.h \
	$(INET_PROJ)/src/networklayer/contract/IPvXAddress.h \
	common/AppCommon.h \
	common/PeerStreamingPacket_m.h \
	membership/GossipMembershipPacket_m.h \
	membership/MessageLogger.h
$O/membership/dummy/DummyMembership.o: membership/dummy/DummyMembership.cc \
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
	common/AppCommon.h \
	common/CommBase.h \
	global/ActivePeerInfo.h \
	global/ActivePeerTable.h \
	global/churn/IChurnGenerator.h \
	global/statistic/GlobalStatistic.h \
	membership/MembershipBase.h \
	membership/dummy/DummyMembership.h
$O/membership/newscast/GossipProtocol.o: membership/newscast/GossipProtocol.cc \
	$(INET_PROJ)/src/base/INETDefs.h \
	$(INET_PROJ)/src/networklayer/contract/IPAddress.h \
	$(INET_PROJ)/src/networklayer/contract/IPv6Address.h \
	$(INET_PROJ)/src/networklayer/contract/IPvXAddress.h \
	membership/newscast/GossipProtocol.h
$O/membership/newscast/GossipProtocolWithUserData.o: membership/newscast/GossipProtocolWithUserData.cc \
	$(INET_PROJ)/src/base/INETDefs.h \
	$(INET_PROJ)/src/networklayer/contract/IPAddress.h \
	$(INET_PROJ)/src/networklayer/contract/IPv6Address.h \
	$(INET_PROJ)/src/networklayer/contract/IPvXAddress.h \
	membership/newscast/GossipProtocol.h \
	membership/newscast/GossipProtocolWithUserData.h \
	membership/newscast/GossipUserData.h
$O/membership/newscast/GossipUserData.o: membership/newscast/GossipUserData.cc \
	membership/newscast/GossipUserData.h
$O/membership/newscast/NewscastBase.o: membership/newscast/NewscastBase.cc \
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
	common/AppCommon.h \
	common/Dispatcher.h \
	common/DpControlInfo_m.h \
	common/PeerStreamingPacket_m.h \
	global/ActivePeerInfo.h \
	global/ActivePeerTable.h \
	global/churn/IChurnGenerator.h \
	global/statistic/GlobalStatistic.h \
	membership/newscast/GossipProtocol.h \
	membership/newscast/GossipProtocolWithUserData.h \
	membership/newscast/GossipUserData.h \
	membership/newscast/NewscastAgentInterface.h \
	membership/newscast/NewscastBase.h \
	membership/newscast/NewscastCache.h \
	membership/newscast/NewscastCacheEntry.h \
	membership/newscast/NewscastPacket_m.h \
	membership/newscast/NewscastStatistic.h
$O/membership/newscast/NewscastCache.o: membership/newscast/NewscastCache.cc \
	$(INET_PROJ)/src/base/INETDefs.h \
	$(INET_PROJ)/src/networklayer/contract/IPAddress.h \
	$(INET_PROJ)/src/networklayer/contract/IPv6Address.h \
	$(INET_PROJ)/src/networklayer/contract/IPvXAddress.h \
	membership/newscast/GossipUserData.h \
	membership/newscast/NewscastCache.h \
	membership/newscast/NewscastCacheEntry.h
$O/membership/newscast/NewscastCacheEntry.o: membership/newscast/NewscastCacheEntry.cc \
	$(INET_PROJ)/src/base/INETDefs.h \
	$(INET_PROJ)/src/networklayer/contract/IPAddress.h \
	$(INET_PROJ)/src/networklayer/contract/IPv6Address.h \
	$(INET_PROJ)/src/networklayer/contract/IPvXAddress.h \
	membership/newscast/GossipUserData.h \
	membership/newscast/NewscastCacheEntry.h
$O/membership/newscast/NewscastPacket_m.o: membership/newscast/NewscastPacket_m.cc \
	$(INET_PROJ)/src/base/INETDefs.h \
	$(INET_PROJ)/src/networklayer/contract/IPAddress.h \
	$(INET_PROJ)/src/networklayer/contract/IPv6Address.h \
	$(INET_PROJ)/src/networklayer/contract/IPvXAddress.h \
	common/PeerStreamingPacket_m.h \
	membership/newscast/GossipUserData.h \
	membership/newscast/NewscastCache.h \
	membership/newscast/NewscastCacheEntry.h \
	membership/newscast/NewscastPacket_m.h
$O/membership/newscast/NewscastPeer.o: membership/newscast/NewscastPeer.cc \
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
	common/AppCommon.h \
	common/Dispatcher.h \
	global/ActivePeerInfo.h \
	global/ActivePeerTable.h \
	global/churn/IChurnGenerator.h \
	global/statistic/GlobalStatistic.h \
	membership/newscast/GossipProtocol.h \
	membership/newscast/GossipProtocolWithUserData.h \
	membership/newscast/GossipUserData.h \
	membership/newscast/NewscastAgentInterface.h \
	membership/newscast/NewscastBase.h \
	membership/newscast/NewscastCache.h \
	membership/newscast/NewscastCacheEntry.h \
	membership/newscast/NewscastPeer.h \
	membership/newscast/NewscastStatistic.h
$O/membership/newscast/NewscastSource.o: membership/newscast/NewscastSource.cc \
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
	common/AppCommon.h \
	common/Dispatcher.h \
	global/ActivePeerInfo.h \
	global/ActivePeerTable.h \
	global/churn/IChurnGenerator.h \
	global/statistic/GlobalStatistic.h \
	membership/newscast/GossipProtocol.h \
	membership/newscast/GossipProtocolWithUserData.h \
	membership/newscast/GossipUserData.h \
	membership/newscast/NewscastAgentInterface.h \
	membership/newscast/NewscastBase.h \
	membership/newscast/NewscastCache.h \
	membership/newscast/NewscastCacheEntry.h \
	membership/newscast/NewscastSource.h \
	membership/newscast/NewscastStatistic.h
$O/membership/newscast/NewscastStatistic.o: membership/newscast/NewscastStatistic.cc \
	$(INET_PROJ)/src/base/INETDefs.h \
	$(INET_PROJ)/src/networklayer/contract/IPAddress.h \
	$(INET_PROJ)/src/networklayer/contract/IPv6Address.h \
	$(INET_PROJ)/src/networklayer/contract/IPvXAddress.h \
	membership/newscast/NewscastStatistic.h
$O/membership/oracle/OracleMembership.o: membership/oracle/OracleMembership.cc \
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
	common/AppCommon.h \
	common/CommBase.h \
	global/ActivePeerInfo.h \
	global/ActivePeerTable.h \
	global/churn/IChurnGenerator.h \
	global/statistic/GlobalStatistic.h \
	membership/MembershipBase.h \
	membership/oracle/OracleMembership.h
$O/membership/scamp/ScampBase.o: membership/scamp/ScampBase.cc \
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
	common/AppCommon.h \
	common/CommBase.h \
	common/Dispatcher.h \
	common/DpControlInfo_m.h \
	common/PeerStreamingPacket_m.h \
	global/ActivePeerInfo.h \
	global/ActivePeerTable.h \
	global/churn/IChurnGenerator.h \
	global/statistic/GlobalStatistic.h \
	membership/Contact.h \
	membership/GossipMembershipPacket_m.h \
	membership/MembershipBase.h \
	membership/MessageLogger.h \
	membership/scamp/ScampBase.h
$O/membership/scamp/ScampPeer.o: membership/scamp/ScampPeer.cc \
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
	common/AppCommon.h \
	common/CommBase.h \
	common/Dispatcher.h \
	common/DpControlInfo_m.h \
	common/PeerStreamingPacket_m.h \
	global/ActivePeerInfo.h \
	global/ActivePeerTable.h \
	global/churn/IChurnGenerator.h \
	global/statistic/GlobalStatistic.h \
	membership/Contact.h \
	membership/GossipMembershipPacket_m.h \
	membership/MembershipBase.h \
	membership/MessageLogger.h \
	membership/scamp/ScampBase.h \
	membership/scamp/ScampPeer.h \
	mesh/MeshPeerStreamingPacket_m.h
$O/membership/scamp/ScampSource.o: membership/scamp/ScampSource.cc \
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
	common/AppCommon.h \
	common/CommBase.h \
	common/Dispatcher.h \
	common/PeerStreamingPacket_m.h \
	global/ActivePeerInfo.h \
	global/ActivePeerTable.h \
	global/churn/IChurnGenerator.h \
	global/statistic/GlobalStatistic.h \
	membership/Contact.h \
	membership/GossipMembershipPacket_m.h \
	membership/MembershipBase.h \
	membership/MessageLogger.h \
	membership/scamp/ScampBase.h \
	membership/scamp/ScampSource.h \
	mesh/MeshPeerStreamingPacket_m.h
$O/mesh/BufferMap.o: mesh/BufferMap.cc \
	$(INET_PROJ)/src/base/INETDefs.h \
	$(INET_PROJ)/src/networklayer/contract/IPAddress.h \
	$(INET_PROJ)/src/networklayer/contract/IPv6Address.h \
	$(INET_PROJ)/src/networklayer/contract/IPvXAddress.h \
	common/AppCommon.h \
	common/PeerStreamingPacket_m.h \
	mesh/BufferMap.h \
	mesh/MeshPeerStreamingPacket_m.h
$O/mesh/MeshPeerStreamingPacket_m.o: mesh/MeshPeerStreamingPacket_m.cc \
	common/PeerStreamingPacket_m.h \
	mesh/MeshPeerStreamingPacket_m.h
$O/mesh/NeighborInfo.o: mesh/NeighborInfo.cc \
	$(INET_PROJ)/src/base/INETDefs.h \
	$(INET_PROJ)/src/networklayer/contract/IPAddress.h \
	$(INET_PROJ)/src/networklayer/contract/IPv6Address.h \
	$(INET_PROJ)/src/networklayer/contract/IPvXAddress.h \
	common/AppCommon.h \
	common/PeerStreamingPacket_m.h \
	mesh/BufferMap.h \
	mesh/MeshPeerStreamingPacket_m.h \
	mesh/NeighborInfo.h
$O/mesh/PartnerList.o: mesh/PartnerList.cc \
	$(INET_PROJ)/src/base/INETDefs.h \
	$(INET_PROJ)/src/networklayer/contract/IPAddress.h \
	$(INET_PROJ)/src/networklayer/contract/IPv6Address.h \
	$(INET_PROJ)/src/networklayer/contract/IPvXAddress.h \
	common/AppCommon.h \
	common/PeerStreamingPacket_m.h \
	global/AppSetting.h \
	global/AppSettingDonet.h \
	mesh/BufferMap.h \
	mesh/MeshPeerStreamingPacket_m.h \
	mesh/NeighborInfo.h \
	mesh/PartnerList.h
$O/mesh/coolstreaming/CoolstreamingBase.o: mesh/coolstreaming/CoolstreamingBase.cc \
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
	common/AppCommon.h \
	common/CommBase.h \
	common/Dispatcher.h \
	common/DpControlInfo_m.h \
	common/PeerStreamingPacket_m.h \
	global/ActivePeerInfo.h \
	global/ActivePeerTable.h \
	global/AppSetting.h \
	global/AppSettingDonet.h \
	global/churn/IChurnGenerator.h \
	global/logger/Logger.h \
	global/observer/MeshOverlayObserver.h \
	global/statistic/GlobalStatistic.h \
	membership/newscast/GossipProtocol.h \
	mesh/BufferMap.h \
	mesh/MeshPeerStreamingPacket_m.h \
	mesh/NeighborInfo.h \
	mesh/PartnerList.h \
	mesh/coolstreaming/CoolstreamingBase.h \
	mesh/coolstreaming/CoolstreamingPacket_m.h \
	mesh/coolstreaming/CoolstreamingPartner.h \
	payload/Forwarder.h \
	payload/VideoBuffer.h \
	payload/VideoBufferListener.h \
	payload/VideoChunkPacket_m.h
$O/mesh/coolstreaming/CoolstreamingPacket_m.o: mesh/coolstreaming/CoolstreamingPacket_m.cc \
	common/PeerStreamingPacket_m.h \
	mesh/MeshPeerStreamingPacket_m.h \
	mesh/coolstreaming/CoolstreamingPacket_m.h
$O/mesh/coolstreaming/CoolstreamingPartner.o: mesh/coolstreaming/CoolstreamingPartner.cc \
	$(INET_PROJ)/src/base/INETDefs.h \
	$(INET_PROJ)/src/networklayer/contract/IPAddress.h \
	$(INET_PROJ)/src/networklayer/contract/IPv6Address.h \
	$(INET_PROJ)/src/networklayer/contract/IPvXAddress.h \
	common/PeerStreamingPacket_m.h \
	mesh/MeshPeerStreamingPacket_m.h \
	mesh/coolstreaming/CoolstreamingPacket_m.h \
	mesh/coolstreaming/CoolstreamingPartner.h \
	payload/VideoChunkPacket_m.h
$O/mesh/coolstreaming/CoolstreamingPeer.o: mesh/coolstreaming/CoolstreamingPeer.cc \
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
	common/AppCommon.h \
	common/CommBase.h \
	common/Dispatcher.h \
	common/PeerStreamingPacket_m.h \
	global/ActivePeerInfo.h \
	global/ActivePeerTable.h \
	global/AppSetting.h \
	global/AppSettingDonet.h \
	global/churn/IChurnGenerator.h \
	global/logger/Logger.h \
	global/observer/MeshOverlayObserver.h \
	global/statistic/GlobalStatistic.h \
	membership/newscast/GossipProtocol.h \
	mesh/BufferMap.h \
	mesh/MeshPeerStreamingPacket_m.h \
	mesh/NeighborInfo.h \
	mesh/PartnerList.h \
	mesh/coolstreaming/CoolstreamingBase.h \
	mesh/coolstreaming/CoolstreamingPacket_m.h \
	mesh/coolstreaming/CoolstreamingPartner.h \
	mesh/coolstreaming/CoolstreamingPeer.h \
	payload/Forwarder.h \
	payload/VideoBuffer.h \
	payload/VideoBufferListener.h \
	payload/VideoChunkPacket_m.h
$O/mesh/coolstreaming/CoolstreamingSource.o: mesh/coolstreaming/CoolstreamingSource.cc \
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
	common/AppCommon.h \
	common/CommBase.h \
	common/Dispatcher.h \
	common/PeerStreamingPacket_m.h \
	global/ActivePeerInfo.h \
	global/ActivePeerTable.h \
	global/AppSetting.h \
	global/AppSettingDonet.h \
	global/churn/IChurnGenerator.h \
	global/logger/Logger.h \
	global/observer/MeshOverlayObserver.h \
	global/statistic/GlobalStatistic.h \
	membership/newscast/GossipProtocol.h \
	mesh/BufferMap.h \
	mesh/MeshPeerStreamingPacket_m.h \
	mesh/NeighborInfo.h \
	mesh/PartnerList.h \
	mesh/coolstreaming/CoolstreamingBase.h \
	mesh/coolstreaming/CoolstreamingPacket_m.h \
	mesh/coolstreaming/CoolstreamingPartner.h \
	mesh/coolstreaming/CoolstreamingSource.h \
	payload/Forwarder.h \
	payload/VideoBuffer.h \
	payload/VideoBufferListener.h \
	payload/VideoChunkPacket_m.h
$O/mesh/donet/DonetBase.o: mesh/donet/DonetBase.cc \
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
	common/AppCommon.h \
	common/CommBase.h \
	common/Dispatcher.h \
	common/DpControlInfo_m.h \
	common/PeerStreamingPacket_m.h \
	global/ActivePeerInfo.h \
	global/ActivePeerTable.h \
	global/AppSetting.h \
	global/AppSettingDonet.h \
	global/churn/IChurnGenerator.h \
	global/logger/Logger.h \
	global/observer/MeshOverlayObserver.h \
	global/statistic/GlobalStatistic.h \
	mesh/BufferMap.h \
	mesh/MeshPeerStreamingPacket_m.h \
	mesh/NeighborInfo.h \
	mesh/PartnerList.h \
	mesh/donet/DonetBase.h \
	payload/Forwarder.h \
	payload/VideoBuffer.h \
	payload/VideoBufferListener.h \
	payload/VideoChunkPacket_m.h
$O/mesh/donet/DonetPeer.o: mesh/donet/DonetPeer.cc \
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
	common/AppCommon.h \
	common/CommBase.h \
	common/Dispatcher.h \
	common/DpControlInfo_m.h \
	common/PeerStreamingPacket_m.h \
	global/ActivePeerInfo.h \
	global/ActivePeerTable.h \
	global/AppSetting.h \
	global/AppSettingDonet.h \
	global/churn/IChurnGenerator.h \
	global/logger/Logger.h \
	global/observer/MeshOverlayObserver.h \
	global/statistic/GlobalStatistic.h \
	mesh/BufferMap.h \
	mesh/MeshPeerStreamingPacket_m.h \
	mesh/NeighborInfo.h \
	mesh/PartnerList.h \
	mesh/donet/DonetBase.h \
	mesh/donet/DonetPeer.h \
	payload/Forwarder.h \
	payload/Player.h \
	payload/VideoBuffer.h \
	payload/VideoBufferListener.h \
	payload/VideoChunkPacket_m.h
$O/mesh/donet/DonetSource.o: mesh/donet/DonetSource.cc \
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
	common/AppCommon.h \
	common/CommBase.h \
	common/Dispatcher.h \
	common/DpControlInfo_m.h \
	common/PeerStreamingPacket_m.h \
	global/ActivePeerInfo.h \
	global/ActivePeerTable.h \
	global/AppSetting.h \
	global/AppSettingDonet.h \
	global/churn/IChurnGenerator.h \
	global/logger/Logger.h \
	global/observer/MeshOverlayObserver.h \
	global/statistic/GlobalStatistic.h \
	mesh/BufferMap.h \
	mesh/MeshPeerStreamingPacket_m.h \
	mesh/NeighborInfo.h \
	mesh/PartnerList.h \
	mesh/donet/DonetBase.h \
	mesh/donet/DonetSource.h \
	payload/Forwarder.h \
	payload/VideoBuffer.h \
	payload/VideoBufferListener.h \
	payload/VideoChunkPacket_m.h
$O/payload/ChunkGenerator.o: payload/ChunkGenerator.cc \
	$(INET_PROJ)/src/base/INETDefs.h \
	$(INET_PROJ)/src/base/INotifiable.h \
	$(INET_PROJ)/src/base/ModuleAccess.h \
	$(INET_PROJ)/src/base/NotificationBoard.h \
	$(INET_PROJ)/src/base/NotifierConsts.h \
	$(INET_PROJ)/src/networklayer/contract/IPAddress.h \
	$(INET_PROJ)/src/networklayer/contract/IPv6Address.h \
	$(INET_PROJ)/src/networklayer/contract/IPvXAddress.h \
	common/AppCommon.h \
	common/PeerStreamingPacket_m.h \
	global/AppSetting.h \
	global/AppSettingDonet.h \
	global/churn/IChurnGenerator.h \
	global/statistic/GlobalStatistic.h \
	mesh/BufferMap.h \
	mesh/MeshPeerStreamingPacket_m.h \
	payload/ChunkGenerator.h \
	payload/VideoBuffer.h \
	payload/VideoBufferListener.h \
	payload/VideoChunkPacket_m.h
$O/payload/Forwarder.o: payload/Forwarder.cc \
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
	common/AppCommon.h \
	common/CommBase.h \
	common/Dispatcher.h \
	common/DpControlInfo_m.h \
	common/PeerStreamingPacket_m.h \
	global/ActivePeerInfo.h \
	global/ActivePeerTable.h \
	global/churn/IChurnGenerator.h \
	global/statistic/GlobalStatistic.h \
	mesh/BufferMap.h \
	mesh/MeshPeerStreamingPacket_m.h \
	payload/Forwarder.h \
	payload/VideoBuffer.h \
	payload/VideoBufferListener.h \
	payload/VideoChunkPacket_m.h
$O/payload/Player.o: payload/Player.cc \
	$(INET_PROJ)/src/base/INETDefs.h \
	$(INET_PROJ)/src/base/INotifiable.h \
	$(INET_PROJ)/src/base/ModuleAccess.h \
	$(INET_PROJ)/src/base/NotificationBoard.h \
	$(INET_PROJ)/src/base/NotifierConsts.h \
	$(INET_PROJ)/src/networklayer/contract/IPAddress.h \
	$(INET_PROJ)/src/networklayer/contract/IPv6Address.h \
	$(INET_PROJ)/src/networklayer/contract/IPvXAddress.h \
	common/AppCommon.h \
	common/PeerStreamingPacket_m.h \
	global/AppSetting.h \
	global/AppSettingDonet.h \
	global/churn/IChurnGenerator.h \
	global/statistic/GlobalStatistic.h \
	mesh/BufferMap.h \
	mesh/MeshPeerStreamingPacket_m.h \
	payload/Player.h \
	payload/VideoBuffer.h \
	payload/VideoBufferListener.h \
	payload/VideoChunkPacket_m.h
$O/payload/VideoBuffer.o: payload/VideoBuffer.cc \
	$(INET_PROJ)/src/base/INETDefs.h \
	$(INET_PROJ)/src/base/INotifiable.h \
	$(INET_PROJ)/src/base/ModuleAccess.h \
	$(INET_PROJ)/src/base/NotificationBoard.h \
	$(INET_PROJ)/src/base/NotifierConsts.h \
	$(INET_PROJ)/src/networklayer/contract/IPAddress.h \
	$(INET_PROJ)/src/networklayer/contract/IPv6Address.h \
	$(INET_PROJ)/src/networklayer/contract/IPvXAddress.h \
	common/AppCommon.h \
	common/PeerStreamingPacket_m.h \
	global/AppSetting.h \
	global/AppSettingDonet.h \
	global/churn/IChurnGenerator.h \
	global/statistic/GlobalStatistic.h \
	mesh/BufferMap.h \
	mesh/MeshPeerStreamingPacket_m.h \
	payload/VideoBuffer.h \
	payload/VideoBufferListener.h \
	payload/VideoChunkPacket_m.h
$O/payload/VideoBufferListener.o: payload/VideoBufferListener.cc \
	payload/VideoBufferListener.h
$O/payload/VideoChunkPacket_m.o: payload/VideoChunkPacket_m.cc \
	common/PeerStreamingPacket_m.h \
	payload/VideoChunkPacket_m.h
$O/tree/TreePeerStreamingPacket_m.o: tree/TreePeerStreamingPacket_m.cc \
	common/PeerStreamingPacket_m.h \
	tree/TreePeerStreamingPacket_m.h

