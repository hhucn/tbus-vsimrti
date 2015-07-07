#
# OMNeT++/OMNEST Makefile for libtbus-vsimrti
#
# This file was generated with the command:
#  opp_makemake -f --deep --make-so -O out -I/home/bialon/workspace/omnetpp-federate/src/mob -I../../inetmanet/src/networklayer/arp -I../../inetmanet/src/mobility -I../../inetmanet/src/transport/sctp -I../../inetmanet/src/world -I../../inetmanet/src/transport/contract -I../../inetmanet/src/linklayer/mfcore -I../../inetmanet/src/applications/udpapp -I../../inetmanet/src/linklayer/ethernet -I../../inetmanet/src/networklayer/ted -I../../inetmanet/src/util -I../../inetmanet/src/linklayer/ieee80211/mac -I../../inetmanet/src/networklayer/queue -I../../inetmanet/src/networklayer/ipv6 -I../../inetmanet/src/networklayer/common -I../../inetmanet/src/applications/pingapp -I../../inetmanet/src/networklayer/ldp -I../../inetmanet/src/linklayer/radio/errormodel -I../../inetmanet/src/transport/tcp -I../../inetmanet/src/util/headerserializers -I../../inetmanet/src/applications/tcpapp -I../../inetmanet/src -I../../inetmanet/src/networklayer/rsvp_te -I../../inetmanet/src/transport/udp -I../../inetmanet/src/networklayer/ipv4 -I../../inetmanet/src/networklayer/icmpv6 -I../../inetmanet/src/base -I../../inetmanet/src/networklayer/contract -I../../inetmanet/src/linklayer/ieee80211/mgmt -I../../inetmanet/src/networklayer/manetrouting/base -I../../inetmanet/src/networklayer/mpls -I../../inetmanet/src/linklayer/contract -I../../omnetpp-federate/src/msg -L../../inetmanet/out/$$\(CONFIGNAME\)/src -lproj -lsqlite3 -linet -KINETMANET_PROJ=../../inetmanet -KOMNETPP_FEDERATE_PROJ=../../omnetpp-federate
#

# Name of target to be created (-o option)
TARGET = libtbus-vsimrti$(SHARED_LIB_SUFFIX)

# C++ include paths (with -I)
INCLUDE_PATH = \
    -I$(OMNETPP_FEDERATE_PROJ)/src/mob \
    -I../../inetmanet/src/networklayer/arp \
    -I../../inetmanet/src/mobility \
    -I../../inetmanet/src/transport/sctp \
    -I../../inetmanet/src/world \
    -I../../inetmanet/src/transport/contract \
    -I../../inetmanet/src/linklayer/mfcore \
    -I../../inetmanet/src/applications/udpapp \
    -I../../inetmanet/src/linklayer/ethernet \
    -I../../inetmanet/src/networklayer/ted \
    -I../../inetmanet/src/util \
    -I../../inetmanet/src/linklayer/ieee80211/mac \
    -I../../inetmanet/src/networklayer/queue \
    -I../../inetmanet/src/networklayer/ipv6 \
    -I../../inetmanet/src/networklayer/common \
    -I../../inetmanet/src/applications/pingapp \
    -I../../inetmanet/src/networklayer/ldp \
    -I../../inetmanet/src/linklayer/radio/errormodel \
    -I../../inetmanet/src/transport/tcp \
    -I../../inetmanet/src/util/headerserializers \
    -I../../inetmanet/src/applications/tcpapp \
    -I../../inetmanet/src \
    -I../../inetmanet/src/networklayer/rsvp_te \
    -I../../inetmanet/src/transport/udp \
    -I../../inetmanet/src/networklayer/ipv4 \
    -I../../inetmanet/src/networklayer/icmpv6 \
    -I../../inetmanet/src/base \
    -I../../inetmanet/src/networklayer/contract \
    -I../../inetmanet/src/linklayer/ieee80211/mgmt \
    -I../../inetmanet/src/networklayer/manetrouting/base \
    -I../../inetmanet/src/networklayer/mpls \
    -I../../inetmanet/src/linklayer/contract \
    -I../../omnetpp-federate/src/msg \
    -I. \
    -Idatabase \
    -Ilinklayer \
    -Ilinklayer/helper \
    -Ilinklayer/radio \
    -Ilinklayer/radio/inet \
    -Ilinklayer/radio/mobile \
    -Ined \
    -Inodes \
    -Inodes/inet \
    -Inodes/mobile \
    -Iqueues \
    -Iqueues/download \
    -Iqueues/implementation \
    -Iqueues/implementation/base \
    -Iqueues/implementation/control_info \
    -Iqueues/implementation/datarate \
    -Iqueues/implementation/delay \
    -Iqueues/implementation/packetqueue \
    -Iqueues/implementation/test \
    -Iqueues/implementation/test/base \
    -Iqueues/implementation/test/datarate \
    -Iqueues/implementation/test/delay \
    -Iqueues/implementation/test/ned \
    -Iqueues/upload \
    -Iworld \
    -Iworld/cellshare \
    -Iworld/worldview \
    -Iworld/worldview/helper

# Additional object and library files to link with
EXTRA_OBJS =

# Additional libraries (-L, -l options)
LIBS = -L../../inetmanet/out/$(CONFIGNAME)/src  -lproj -lsqlite3 -linet
LIBS += -Wl,-rpath,`abspath ../../inetmanet/out/$(CONFIGNAME)/src`

# Output directory
PROJECT_OUTPUT_DIR = ../out
PROJECTRELATIVE_PATH = src
O = $(PROJECT_OUTPUT_DIR)/$(CONFIGNAME)/$(PROJECTRELATIVE_PATH)

# Object files for local .cc and .msg files
OBJS = \
    $O/database/TbusSqliteDatabaseHandler.o \
    $O/linklayer/helper/IpAddressHelper.o \
    $O/linklayer/radio/inet/TbusInetPHY.o \
    $O/linklayer/radio/inet/TbusInetMAC.o \
    $O/linklayer/radio/mobile/TbusMobilePHY.o \
    $O/linklayer/radio/mobile/TbusMobileMAC.o \
    $O/linklayer/radio/mobile/TbusQueueControl.o \
    $O/nodes/inet/TbusInetNode.o \
    $O/nodes/mobile/TbusMobileNode.o \
    $O/queues/download/TbusCDRQ.o \
    $O/queues/download/TbusCRRQ.o \
    $O/queues/implementation/base/TbusBaseQueue.o \
    $O/queues/implementation/control_info/TbusQueueControlInfo.o \
    $O/queues/implementation/datarate/TbusDatarateQueue.o \
    $O/queues/implementation/datarate/TbusQueueDatarateValue.o \
    $O/queues/implementation/delay/TbusDelayQueue.o \
    $O/queues/implementation/delay/TbusQueueDelayValue.o \
    $O/queues/implementation/packetqueue/TbusPacketQueue.o \
    $O/queues/implementation/test/base/TbusTestBase.o \
    $O/queues/implementation/test/datarate/TbusDatarateQueueTester.o \
    $O/queues/implementation/test/delay/TbusDelayQueueTester.o \
    $O/queues/upload/TbusCDSQ.o \
    $O/queues/upload/TbusCRSQ.o \
    $O/world/TbusChannelControl.o \
    $O/world/cellshare/TbusTrivialCellShare.o \
    $O/world/worldview/TbusWorldView.o \
    $O/queues/implementation/test/base/testPacket_m.o

# Message files
MSGFILES = \
    queues/implementation/test/base/testPacket.msg

# Other makefile variables (-K)
INETMANET_PROJ=../../inetmanet
OMNETPP_FEDERATE_PROJ=../../omnetpp-federate

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
OMNETPP_LIBS = -L"$(OMNETPP_LIB_SUBDIR)" -L"$(OMNETPP_LIB_DIR)" -loppenvir$D $(KERNEL_LIBS) $(SYS_LIBS)

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
	$(Q)$(LN) $O/$(TARGET) .

$O/$(TARGET): $(OBJS)  $(wildcard $(EXTRA_OBJS)) Makefile
	@$(MKPATH) $O
	@echo Creating shared library: $@
	$(Q)$(SHLIB_LD) -o $O/$(TARGET)  $(OBJS) $(EXTRA_OBJS) $(AS_NEEDED_OFF) $(WHOLE_ARCHIVE_ON) $(LIBS) $(WHOLE_ARCHIVE_OFF) $(OMNETPP_LIBS) $(LDFLAGS)
	$(Q)$(SHLIB_POSTPROCESS) $O/$(TARGET)

.PHONY: all clean cleanall depend msgheaders

.SUFFIXES: .cc

$O/%.o: %.cc $(COPTS_FILE)
	@$(MKPATH) $(dir $@)
	$(qecho) "$<"
	$(Q)$(CXX) -c $(CXXFLAGS) $(COPTS) -o $@ $<

%_m.cc %_m.h: %.msg
	$(qecho) MSGC: $<
	$(Q)$(MSGC) -s _m.cc $(MSGCOPTS) $?

msgheaders: $(MSGFILES:.msg=_m.h)

clean:
	$(qecho) Cleaning...
	$(Q)-rm -rf $O
	$(Q)-rm -f tbus-vsimrti tbus-vsimrti.exe libtbus-vsimrti.so libtbus-vsimrti.a libtbus-vsimrti.dll libtbus-vsimrti.dylib
	$(Q)-rm -f ./*_m.cc ./*_m.h
	$(Q)-rm -f database/*_m.cc database/*_m.h
	$(Q)-rm -f linklayer/*_m.cc linklayer/*_m.h
	$(Q)-rm -f linklayer/helper/*_m.cc linklayer/helper/*_m.h
	$(Q)-rm -f linklayer/radio/*_m.cc linklayer/radio/*_m.h
	$(Q)-rm -f linklayer/radio/inet/*_m.cc linklayer/radio/inet/*_m.h
	$(Q)-rm -f linklayer/radio/mobile/*_m.cc linklayer/radio/mobile/*_m.h
	$(Q)-rm -f ned/*_m.cc ned/*_m.h
	$(Q)-rm -f nodes/*_m.cc nodes/*_m.h
	$(Q)-rm -f nodes/inet/*_m.cc nodes/inet/*_m.h
	$(Q)-rm -f nodes/mobile/*_m.cc nodes/mobile/*_m.h
	$(Q)-rm -f queues/*_m.cc queues/*_m.h
	$(Q)-rm -f queues/download/*_m.cc queues/download/*_m.h
	$(Q)-rm -f queues/implementation/*_m.cc queues/implementation/*_m.h
	$(Q)-rm -f queues/implementation/base/*_m.cc queues/implementation/base/*_m.h
	$(Q)-rm -f queues/implementation/control_info/*_m.cc queues/implementation/control_info/*_m.h
	$(Q)-rm -f queues/implementation/datarate/*_m.cc queues/implementation/datarate/*_m.h
	$(Q)-rm -f queues/implementation/delay/*_m.cc queues/implementation/delay/*_m.h
	$(Q)-rm -f queues/implementation/packetqueue/*_m.cc queues/implementation/packetqueue/*_m.h
	$(Q)-rm -f queues/implementation/test/*_m.cc queues/implementation/test/*_m.h
	$(Q)-rm -f queues/implementation/test/base/*_m.cc queues/implementation/test/base/*_m.h
	$(Q)-rm -f queues/implementation/test/datarate/*_m.cc queues/implementation/test/datarate/*_m.h
	$(Q)-rm -f queues/implementation/test/delay/*_m.cc queues/implementation/test/delay/*_m.h
	$(Q)-rm -f queues/implementation/test/ned/*_m.cc queues/implementation/test/ned/*_m.h
	$(Q)-rm -f queues/upload/*_m.cc queues/upload/*_m.h
	$(Q)-rm -f world/*_m.cc world/*_m.h
	$(Q)-rm -f world/cellshare/*_m.cc world/cellshare/*_m.h
	$(Q)-rm -f world/worldview/*_m.cc world/worldview/*_m.h
	$(Q)-rm -f world/worldview/helper/*_m.cc world/worldview/helper/*_m.h

cleanall: clean
	$(Q)-rm -rf $(PROJECT_OUTPUT_DIR)

depend:
	$(qecho) Creating dependencies...
	$(Q)$(MAKEDEPEND) $(INCLUDE_PATH) -f Makefile -P\$$O/ -- $(MSG_CC_FILES)  ./*.cc database/*.cc linklayer/*.cc linklayer/helper/*.cc linklayer/radio/*.cc linklayer/radio/inet/*.cc linklayer/radio/mobile/*.cc ned/*.cc nodes/*.cc nodes/inet/*.cc nodes/mobile/*.cc queues/*.cc queues/download/*.cc queues/implementation/*.cc queues/implementation/base/*.cc queues/implementation/control_info/*.cc queues/implementation/datarate/*.cc queues/implementation/delay/*.cc queues/implementation/packetqueue/*.cc queues/implementation/test/*.cc queues/implementation/test/base/*.cc queues/implementation/test/datarate/*.cc queues/implementation/test/delay/*.cc queues/implementation/test/ned/*.cc queues/upload/*.cc world/*.cc world/cellshare/*.cc world/worldview/*.cc world/worldview/helper/*.cc

# DO NOT DELETE THIS LINE -- make depend depends on it.
$O/database/TbusSqliteDatabaseHandler.o: database/TbusSqliteDatabaseHandler.cc \
	database/TbusDatabaseHandler.h \
	database/TbusSqliteDatabaseHandler.h \
	linklayer/radio/mobile/TbusQueueControlCallback.h \
	queues/implementation/base/TbusQueueSelection.h \
	queues/implementation/base/TbusQueueStatus.h \
	queues/implementation/base/TbusQueueValue.h \
	queues/implementation/datarate/TbusQueueDatarateValue.h \
	queues/implementation/delay/TbusQueueDelayValue.h \
	world/cellshare/TbusCellShareTypes.h
$O/linklayer/helper/IpAddressHelper.o: linklayer/helper/IpAddressHelper.cc \
	$(INETMANET_PROJ)/src/base/INETDefs.h \
	$(INETMANET_PROJ)/src/base/NotifierConsts.h \
	$(INETMANET_PROJ)/src/linklayer/contract/MACAddress.h \
	$(INETMANET_PROJ)/src/networklayer/common/InterfaceEntry.h \
	$(INETMANET_PROJ)/src/networklayer/common/InterfaceToken.h \
	$(INETMANET_PROJ)/src/networklayer/contract/IPAddress.h \
	$(INETMANET_PROJ)/src/networklayer/ipv4/IPRoute.h \
	linklayer/helper/IpAddressHelper.h
$O/linklayer/radio/inet/TbusInetMAC.o: linklayer/radio/inet/TbusInetMAC.cc \
	$(INETMANET_PROJ)/src/base/INETDefs.h \
	$(INETMANET_PROJ)/src/base/INotifiable.h \
	$(INETMANET_PROJ)/src/base/ModuleAccess.h \
	$(INETMANET_PROJ)/src/base/NotificationBoard.h \
	$(INETMANET_PROJ)/src/base/NotifierConsts.h \
	$(INETMANET_PROJ)/src/linklayer/contract/MACAddress.h \
	$(INETMANET_PROJ)/src/networklayer/common/IInterfaceTable.h \
	$(INETMANET_PROJ)/src/networklayer/common/InterfaceEntry.h \
	$(INETMANET_PROJ)/src/networklayer/common/InterfaceTable.h \
	$(INETMANET_PROJ)/src/networklayer/common/InterfaceTableAccess.h \
	$(INETMANET_PROJ)/src/networklayer/common/InterfaceToken.h \
	$(INETMANET_PROJ)/src/networklayer/contract/IPAddress.h \
	$(INETMANET_PROJ)/src/networklayer/contract/IPProtocolId_m.h \
	$(INETMANET_PROJ)/src/networklayer/ipv4/IPRoute.h \
	$(INETMANET_PROJ)/src/networklayer/ipv4/IPRouteRule.h \
	$(INETMANET_PROJ)/src/networklayer/ipv4/IPv4InterfaceData.h \
	$(INETMANET_PROJ)/src/networklayer/ipv4/IRoutingTable.h \
	$(INETMANET_PROJ)/src/networklayer/ipv4/RoutingTableAccess.h \
	linklayer/helper/IpAddressHelper.h \
	linklayer/radio/inet/TbusInetMAC.h
$O/linklayer/radio/inet/TbusInetPHY.o: linklayer/radio/inet/TbusInetPHY.cc \
	$(INETMANET_PROJ)/src/base/BasicModule.h \
	$(INETMANET_PROJ)/src/base/Coord.h \
	$(INETMANET_PROJ)/src/base/INETDefs.h \
	$(INETMANET_PROJ)/src/base/INotifiable.h \
	$(INETMANET_PROJ)/src/base/ModuleAccess.h \
	$(INETMANET_PROJ)/src/base/NotificationBoard.h \
	$(INETMANET_PROJ)/src/base/NotifierConsts.h \
	$(INETMANET_PROJ)/src/linklayer/mfcore/AirFrame_m.h \
	$(INETMANET_PROJ)/src/linklayer/mfcore/ModulationType.h \
	$(INETMANET_PROJ)/src/networklayer/contract/IPAddress.h \
	$(INETMANET_PROJ)/src/util/FWMath.h \
	$(INETMANET_PROJ)/src/world/ChannelAccess.h \
	$(INETMANET_PROJ)/src/world/ChannelControl.h \
	$(INETMANET_PROJ)/src/world/ChannelControlExtended.h \
	linklayer/radio/inet/TbusInetPHY.h \
	world/TbusChannelControl.h
$O/linklayer/radio/mobile/TbusMobileMAC.o: linklayer/radio/mobile/TbusMobileMAC.cc \
	$(INETMANET_PROJ)/src/base/INETDefs.h \
	$(INETMANET_PROJ)/src/base/ModuleAccess.h \
	$(INETMANET_PROJ)/src/base/NotifierConsts.h \
	$(INETMANET_PROJ)/src/linklayer/contract/MACAddress.h \
	$(INETMANET_PROJ)/src/networklayer/common/IInterfaceTable.h \
	$(INETMANET_PROJ)/src/networklayer/common/InterfaceEntry.h \
	$(INETMANET_PROJ)/src/networklayer/common/InterfaceTableAccess.h \
	$(INETMANET_PROJ)/src/networklayer/common/InterfaceToken.h \
	$(INETMANET_PROJ)/src/networklayer/contract/IPAddress.h \
	$(INETMANET_PROJ)/src/networklayer/contract/IPProtocolId_m.h \
	$(INETMANET_PROJ)/src/networklayer/ipv4/IPRoute.h \
	$(INETMANET_PROJ)/src/networklayer/ipv4/IPRouteRule.h \
	$(INETMANET_PROJ)/src/networklayer/ipv4/IPv4InterfaceData.h \
	$(INETMANET_PROJ)/src/networklayer/ipv4/IRoutingTable.h \
	$(INETMANET_PROJ)/src/networklayer/ipv4/RoutingTableAccess.h \
	$(INETMANET_PROJ)/src/util/opp_utils.h \
	linklayer/helper/IpAddressHelper.h \
	linklayer/radio/mobile/TbusMobileMAC.h \
	queues/implementation/control_info/TbusQueueControlInfo.h
$O/linklayer/radio/mobile/TbusMobilePHY.o: linklayer/radio/mobile/TbusMobilePHY.cc \
	$(INETMANET_PROJ)/src/base/BasicModule.h \
	$(INETMANET_PROJ)/src/base/Coord.h \
	$(INETMANET_PROJ)/src/base/INETDefs.h \
	$(INETMANET_PROJ)/src/base/INotifiable.h \
	$(INETMANET_PROJ)/src/base/ModuleAccess.h \
	$(INETMANET_PROJ)/src/base/NotificationBoard.h \
	$(INETMANET_PROJ)/src/base/NotifierConsts.h \
	$(INETMANET_PROJ)/src/linklayer/mfcore/AirFrame_m.h \
	$(INETMANET_PROJ)/src/linklayer/mfcore/ModulationType.h \
	$(INETMANET_PROJ)/src/networklayer/contract/IPAddress.h \
	$(INETMANET_PROJ)/src/util/FWMath.h \
	$(INETMANET_PROJ)/src/world/ChannelAccess.h \
	$(INETMANET_PROJ)/src/world/ChannelAccessExtended.h \
	$(INETMANET_PROJ)/src/world/ChannelControl.h \
	$(INETMANET_PROJ)/src/world/ChannelControlExtended.h \
	database/TbusDatabaseHandler.h \
	linklayer/radio/mobile/TbusMobilePHY.h \
	linklayer/radio/mobile/TbusQueueControl.h \
	linklayer/radio/mobile/TbusQueueControlCallback.h \
	queues/download/TbusCDRQ.h \
	queues/download/TbusCRRQ.h \
	queues/implementation/base/TbusBaseQueue.h \
	queues/implementation/base/TbusQueueSelection.h \
	queues/implementation/base/TbusQueueStatus.h \
	queues/implementation/base/TbusQueueValue.h \
	queues/implementation/control_info/TbusQueueControlInfo.h \
	queues/implementation/datarate/TbusDatarateQueue.h \
	queues/implementation/datarate/TbusQueueDatarateValue.h \
	queues/implementation/delay/TbusDelayQueue.h \
	queues/implementation/delay/TbusQueueDelayValue.h \
	queues/implementation/packetqueue/TbusPacketQueue.h \
	queues/upload/TbusCDSQ.h \
	queues/upload/TbusCRSQ.h \
	world/TbusChannelControl.h \
	world/cellshare/TbusCellShareTypes.h
$O/linklayer/radio/mobile/TbusQueueControl.o: linklayer/radio/mobile/TbusQueueControl.cc \
	$(INETMANET_PROJ)/src/base/INETDefs.h \
	$(INETMANET_PROJ)/src/base/ModuleAccess.h \
	$(OMNETPP_FEDERATE_PROJ)/src/mob/VSimRTIExtendedMobilityNode.h \
	database/TbusDatabaseHandler.h \
	database/TbusSqliteDatabaseHandler.h \
	linklayer/radio/mobile/TbusQueueControl.h \
	linklayer/radio/mobile/TbusQueueControlCallback.h \
	nodes/mobile/TbusMobileNode.h \
	queues/download/TbusCDRQ.h \
	queues/download/TbusCRRQ.h \
	queues/implementation/base/TbusBaseQueue.h \
	queues/implementation/base/TbusQueueSelection.h \
	queues/implementation/base/TbusQueueStatus.h \
	queues/implementation/base/TbusQueueValue.h \
	queues/implementation/control_info/TbusQueueControlInfo.h \
	queues/implementation/datarate/TbusDatarateQueue.h \
	queues/implementation/datarate/TbusQueueDatarateValue.h \
	queues/implementation/delay/TbusDelayQueue.h \
	queues/implementation/delay/TbusQueueDelayValue.h \
	queues/implementation/packetqueue/TbusPacketQueue.h \
	queues/upload/TbusCDSQ.h \
	queues/upload/TbusCRSQ.h \
	world/cellshare/TbusCellShare.h \
	world/cellshare/TbusCellShareTypes.h \
	world/cellshare/TbusTrivialCellShare.h \
	world/worldview/TbusWorldView.h
$O/nodes/inet/TbusInetNode.o: nodes/inet/TbusInetNode.cc \
	nodes/inet/TbusInetNode.h
$O/nodes/mobile/TbusMobileNode.o: nodes/mobile/TbusMobileNode.cc \
	$(INETMANET_PROJ)/src/base/INETDefs.h \
	$(INETMANET_PROJ)/src/base/ModuleAccess.h \
	$(OMNETPP_FEDERATE_PROJ)/src/mob/VSimRTIExtendedMobilityNode.h \
	database/TbusDatabaseHandler.h \
	linklayer/radio/mobile/TbusQueueControl.h \
	linklayer/radio/mobile/TbusQueueControlCallback.h \
	nodes/mobile/TbusMobileNode.h \
	queues/download/TbusCDRQ.h \
	queues/download/TbusCRRQ.h \
	queues/implementation/base/TbusBaseQueue.h \
	queues/implementation/base/TbusQueueSelection.h \
	queues/implementation/base/TbusQueueStatus.h \
	queues/implementation/base/TbusQueueValue.h \
	queues/implementation/control_info/TbusQueueControlInfo.h \
	queues/implementation/datarate/TbusDatarateQueue.h \
	queues/implementation/datarate/TbusQueueDatarateValue.h \
	queues/implementation/delay/TbusDelayQueue.h \
	queues/implementation/delay/TbusQueueDelayValue.h \
	queues/implementation/packetqueue/TbusPacketQueue.h \
	queues/upload/TbusCDSQ.h \
	queues/upload/TbusCRSQ.h \
	world/cellshare/TbusCellShareTypes.h \
	world/worldview/TbusWorldView.h
$O/queues/download/TbusCDRQ.o: queues/download/TbusCDRQ.cc \
	linklayer/radio/mobile/TbusQueueControlCallback.h \
	queues/download/TbusCDRQ.h \
	queues/implementation/base/TbusBaseQueue.h \
	queues/implementation/base/TbusQueueSelection.h \
	queues/implementation/base/TbusQueueStatus.h \
	queues/implementation/base/TbusQueueValue.h \
	queues/implementation/control_info/TbusQueueControlInfo.h \
	queues/implementation/delay/TbusDelayQueue.h \
	queues/implementation/delay/TbusQueueDelayValue.h \
	queues/implementation/packetqueue/TbusPacketQueue.h
$O/queues/download/TbusCRRQ.o: queues/download/TbusCRRQ.cc \
	linklayer/radio/mobile/TbusQueueControlCallback.h \
	queues/download/TbusCRRQ.h \
	queues/implementation/base/TbusBaseQueue.h \
	queues/implementation/base/TbusQueueSelection.h \
	queues/implementation/base/TbusQueueStatus.h \
	queues/implementation/base/TbusQueueValue.h \
	queues/implementation/control_info/TbusQueueControlInfo.h \
	queues/implementation/datarate/TbusDatarateQueue.h \
	queues/implementation/datarate/TbusQueueDatarateValue.h \
	queues/implementation/packetqueue/TbusPacketQueue.h
$O/queues/implementation/base/TbusBaseQueue.o: queues/implementation/base/TbusBaseQueue.cc \
	linklayer/radio/mobile/TbusQueueControlCallback.h \
	queues/implementation/base/TbusBaseQueue.h \
	queues/implementation/base/TbusQueueSelection.h \
	queues/implementation/base/TbusQueueStatus.h \
	queues/implementation/base/TbusQueueValue.h \
	queues/implementation/control_info/TbusQueueControlInfo.h \
	queues/implementation/datarate/TbusQueueDatarateValue.h \
	queues/implementation/delay/TbusQueueDelayValue.h \
	queues/implementation/packetqueue/TbusPacketQueue.h
$O/queues/implementation/control_info/TbusQueueControlInfo.o: queues/implementation/control_info/TbusQueueControlInfo.cc \
	queues/implementation/control_info/TbusQueueControlInfo.h
$O/queues/implementation/datarate/TbusDatarateQueue.o: queues/implementation/datarate/TbusDatarateQueue.cc \
	linklayer/radio/mobile/TbusQueueControlCallback.h \
	queues/implementation/base/TbusBaseQueue.h \
	queues/implementation/base/TbusQueueSelection.h \
	queues/implementation/base/TbusQueueStatus.h \
	queues/implementation/base/TbusQueueValue.h \
	queues/implementation/control_info/TbusQueueControlInfo.h \
	queues/implementation/datarate/TbusDatarateQueue.h \
	queues/implementation/datarate/TbusQueueDatarateValue.h \
	queues/implementation/packetqueue/TbusPacketQueue.h
$O/queues/implementation/datarate/TbusQueueDatarateValue.o: queues/implementation/datarate/TbusQueueDatarateValue.cc \
	queues/implementation/base/TbusQueueValue.h \
	queues/implementation/datarate/TbusQueueDatarateValue.h
$O/queues/implementation/delay/TbusDelayQueue.o: queues/implementation/delay/TbusDelayQueue.cc \
	linklayer/radio/mobile/TbusQueueControlCallback.h \
	queues/implementation/base/TbusBaseQueue.h \
	queues/implementation/base/TbusQueueSelection.h \
	queues/implementation/base/TbusQueueStatus.h \
	queues/implementation/base/TbusQueueValue.h \
	queues/implementation/control_info/TbusQueueControlInfo.h \
	queues/implementation/delay/TbusDelayQueue.h \
	queues/implementation/delay/TbusQueueDelayValue.h \
	queues/implementation/packetqueue/TbusPacketQueue.h
$O/queues/implementation/delay/TbusQueueDelayValue.o: queues/implementation/delay/TbusQueueDelayValue.cc \
	queues/implementation/base/TbusQueueValue.h \
	queues/implementation/delay/TbusQueueDelayValue.h
$O/queues/implementation/packetqueue/TbusPacketQueue.o: queues/implementation/packetqueue/TbusPacketQueue.cc \
	queues/implementation/control_info/TbusQueueControlInfo.h \
	queues/implementation/packetqueue/TbusPacketQueue.h
$O/queues/implementation/test/base/TbusTestBase.o: queues/implementation/test/base/TbusTestBase.cc \
	linklayer/radio/mobile/TbusQueueControlCallback.h \
	queues/implementation/base/TbusBaseQueue.h \
	queues/implementation/base/TbusQueueSelection.h \
	queues/implementation/base/TbusQueueStatus.h \
	queues/implementation/base/TbusQueueValue.h \
	queues/implementation/control_info/TbusQueueControlInfo.h \
	queues/implementation/datarate/TbusDatarateQueue.h \
	queues/implementation/datarate/TbusQueueDatarateValue.h \
	queues/implementation/delay/TbusDelayQueue.h \
	queues/implementation/delay/TbusQueueDelayValue.h \
	queues/implementation/packetqueue/TbusPacketQueue.h \
	queues/implementation/test/base/TbusTestBase.h \
	queues/implementation/test/datarate/TbusDatarateQueueTester.h \
	queues/implementation/test/delay/TbusDelayQueueTester.h
$O/queues/implementation/test/base/testPacket_m.o: queues/implementation/test/base/testPacket_m.cc \
	queues/implementation/test/base/testPacket_m.h
$O/queues/implementation/test/datarate/TbusDatarateQueueTester.o: queues/implementation/test/datarate/TbusDatarateQueueTester.cc \
	$(INETMANET_PROJ)/src/base/INETDefs.h \
	$(INETMANET_PROJ)/src/base/ModuleAccess.h \
	linklayer/radio/mobile/TbusQueueControlCallback.h \
	queues/implementation/base/TbusBaseQueue.h \
	queues/implementation/base/TbusQueueSelection.h \
	queues/implementation/base/TbusQueueStatus.h \
	queues/implementation/base/TbusQueueValue.h \
	queues/implementation/control_info/TbusQueueControlInfo.h \
	queues/implementation/datarate/TbusDatarateQueue.h \
	queues/implementation/datarate/TbusQueueDatarateValue.h \
	queues/implementation/packetqueue/TbusPacketQueue.h \
	queues/implementation/test/base/TbusTestBase.h \
	queues/implementation/test/base/testPacket_m.h \
	queues/implementation/test/datarate/TbusDatarateQueueTester.h
$O/queues/implementation/test/delay/TbusDelayQueueTester.o: queues/implementation/test/delay/TbusDelayQueueTester.cc \
	$(INETMANET_PROJ)/src/base/INETDefs.h \
	$(INETMANET_PROJ)/src/base/ModuleAccess.h \
	linklayer/radio/mobile/TbusQueueControlCallback.h \
	queues/implementation/base/TbusBaseQueue.h \
	queues/implementation/base/TbusQueueSelection.h \
	queues/implementation/base/TbusQueueStatus.h \
	queues/implementation/base/TbusQueueValue.h \
	queues/implementation/control_info/TbusQueueControlInfo.h \
	queues/implementation/delay/TbusDelayQueue.h \
	queues/implementation/delay/TbusQueueDelayValue.h \
	queues/implementation/packetqueue/TbusPacketQueue.h \
	queues/implementation/test/base/TbusTestBase.h \
	queues/implementation/test/base/testPacket_m.h \
	queues/implementation/test/delay/TbusDelayQueueTester.h
$O/queues/upload/TbusCDSQ.o: queues/upload/TbusCDSQ.cc \
	linklayer/radio/mobile/TbusQueueControlCallback.h \
	queues/implementation/base/TbusBaseQueue.h \
	queues/implementation/base/TbusQueueSelection.h \
	queues/implementation/base/TbusQueueStatus.h \
	queues/implementation/base/TbusQueueValue.h \
	queues/implementation/control_info/TbusQueueControlInfo.h \
	queues/implementation/delay/TbusDelayQueue.h \
	queues/implementation/delay/TbusQueueDelayValue.h \
	queues/implementation/packetqueue/TbusPacketQueue.h \
	queues/upload/TbusCDSQ.h
$O/queues/upload/TbusCRSQ.o: queues/upload/TbusCRSQ.cc \
	linklayer/radio/mobile/TbusQueueControlCallback.h \
	queues/implementation/base/TbusBaseQueue.h \
	queues/implementation/base/TbusQueueSelection.h \
	queues/implementation/base/TbusQueueStatus.h \
	queues/implementation/base/TbusQueueValue.h \
	queues/implementation/control_info/TbusQueueControlInfo.h \
	queues/implementation/datarate/TbusDatarateQueue.h \
	queues/implementation/datarate/TbusQueueDatarateValue.h \
	queues/implementation/delay/TbusDelayQueue.h \
	queues/implementation/delay/TbusQueueDelayValue.h \
	queues/implementation/packetqueue/TbusPacketQueue.h \
	queues/upload/TbusCDSQ.h \
	queues/upload/TbusCRSQ.h
$O/world/TbusChannelControl.o: world/TbusChannelControl.cc \
	$(INETMANET_PROJ)/src/base/BasicModule.h \
	$(INETMANET_PROJ)/src/base/Coord.h \
	$(INETMANET_PROJ)/src/base/INETDefs.h \
	$(INETMANET_PROJ)/src/base/INotifiable.h \
	$(INETMANET_PROJ)/src/base/ModuleAccess.h \
	$(INETMANET_PROJ)/src/base/NotificationBoard.h \
	$(INETMANET_PROJ)/src/base/NotifierConsts.h \
	$(INETMANET_PROJ)/src/linklayer/contract/MACAddress.h \
	$(INETMANET_PROJ)/src/linklayer/mfcore/AirFrame_m.h \
	$(INETMANET_PROJ)/src/linklayer/mfcore/ModulationType.h \
	$(INETMANET_PROJ)/src/networklayer/common/IInterfaceTable.h \
	$(INETMANET_PROJ)/src/networklayer/common/InterfaceEntry.h \
	$(INETMANET_PROJ)/src/networklayer/common/InterfaceToken.h \
	$(INETMANET_PROJ)/src/networklayer/contract/IPAddress.h \
	$(INETMANET_PROJ)/src/networklayer/contract/IPAddressResolver.h \
	$(INETMANET_PROJ)/src/networklayer/contract/IPProtocolId_m.h \
	$(INETMANET_PROJ)/src/networklayer/contract/IPv6Address.h \
	$(INETMANET_PROJ)/src/networklayer/contract/IPvXAddress.h \
	$(INETMANET_PROJ)/src/networklayer/ipv4/IPDatagram_m.h \
	$(INETMANET_PROJ)/src/networklayer/ipv4/IPv4InterfaceData.h \
	$(INETMANET_PROJ)/src/util/FWMath.h \
	$(INETMANET_PROJ)/src/world/ChannelAccess.h \
	$(INETMANET_PROJ)/src/world/ChannelAccessExtended.h \
	$(INETMANET_PROJ)/src/world/ChannelControl.h \
	$(INETMANET_PROJ)/src/world/ChannelControlExtended.h \
	$(OMNETPP_FEDERATE_PROJ)/src/msg/VSimRTIAppPacket_m.h \
	database/TbusDatabaseHandler.h \
	linklayer/radio/mobile/TbusMobilePHY.h \
	linklayer/radio/mobile/TbusQueueControl.h \
	linklayer/radio/mobile/TbusQueueControlCallback.h \
	queues/download/TbusCDRQ.h \
	queues/download/TbusCRRQ.h \
	queues/implementation/base/TbusBaseQueue.h \
	queues/implementation/base/TbusQueueSelection.h \
	queues/implementation/base/TbusQueueStatus.h \
	queues/implementation/base/TbusQueueValue.h \
	queues/implementation/control_info/TbusQueueControlInfo.h \
	queues/implementation/datarate/TbusDatarateQueue.h \
	queues/implementation/datarate/TbusQueueDatarateValue.h \
	queues/implementation/delay/TbusDelayQueue.h \
	queues/implementation/delay/TbusQueueDelayValue.h \
	queues/implementation/packetqueue/TbusPacketQueue.h \
	queues/upload/TbusCDSQ.h \
	queues/upload/TbusCRSQ.h \
	world/TbusChannelControl.h \
	world/cellshare/TbusCellShareTypes.h \
	world/worldview/TbusWorldView.h
$O/world/cellshare/TbusTrivialCellShare.o: world/cellshare/TbusTrivialCellShare.cc \
	linklayer/radio/mobile/TbusQueueControlCallback.h \
	queues/implementation/base/TbusQueueSelection.h \
	queues/implementation/base/TbusQueueStatus.h \
	queues/implementation/base/TbusQueueValue.h \
	queues/implementation/datarate/TbusQueueDatarateValue.h \
	queues/implementation/delay/TbusQueueDelayValue.h \
	world/cellshare/TbusCellShare.h \
	world/cellshare/TbusCellShareTypes.h \
	world/cellshare/TbusTrivialCellShare.h
$O/world/worldview/TbusWorldView.o: world/worldview/TbusWorldView.cc \
	database/TbusDatabaseHandler.h \
	linklayer/radio/mobile/TbusQueueControl.h \
	linklayer/radio/mobile/TbusQueueControlCallback.h \
	queues/download/TbusCDRQ.h \
	queues/download/TbusCRRQ.h \
	queues/implementation/base/TbusBaseQueue.h \
	queues/implementation/base/TbusQueueSelection.h \
	queues/implementation/base/TbusQueueStatus.h \
	queues/implementation/base/TbusQueueValue.h \
	queues/implementation/control_info/TbusQueueControlInfo.h \
	queues/implementation/datarate/TbusDatarateQueue.h \
	queues/implementation/datarate/TbusQueueDatarateValue.h \
	queues/implementation/delay/TbusDelayQueue.h \
	queues/implementation/delay/TbusQueueDelayValue.h \
	queues/implementation/packetqueue/TbusPacketQueue.h \
	queues/upload/TbusCDSQ.h \
	queues/upload/TbusCRSQ.h \
	world/cellshare/TbusCellShareTypes.h \
	world/worldview/TbusWorldView.h

