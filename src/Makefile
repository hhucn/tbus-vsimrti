#
# OMNeT++/OMNEST Makefile for libtbus-vsimrti
#
# This file was generated with the command:
#  opp_makemake -f --deep --make-so -O out -I../../inetmanet/src/networklayer/arp -I../../inetmanet/src/transport/sctp -I../../inetmanet/src/world -I../../inetmanet/src/transport/contract -I../../inetmanet/src/linklayer/mfcore -I../../inetmanet/src/linklayer/ethernet -I../../inetmanet/src/networklayer/ted -I../../inetmanet/src/util -I../../inetmanet/src/linklayer/ieee80211/mac -I../../inetmanet/src/networklayer/queue -I../../inetmanet/src/networklayer/common -I../../inetmanet/src/networklayer/ipv6 -I../../inetmanet/src/applications/pingapp -I../../inetmanet/src/networklayer/ldp -I../../inetmanet/src/linklayer/radio/errormodel -I../../inetmanet/src/transport/tcp -I../../inetmanet/src/util/headerserializers -I../../inetmanet/src/networklayer/rsvp_te -I../../inetmanet/src/transport/udp -I../../inetmanet/src/networklayer/ipv4 -I../../inetmanet/src/networklayer/icmpv6 -I../../inetmanet/src/base -I../../inetmanet/src/networklayer/contract -I../../inetmanet/src/linklayer/ieee80211/mgmt -I../../inetmanet/src/networklayer/manetrouting/base -I../../inetmanet/src/networklayer/mpls -I../../inetmanet/src/linklayer/contract -L../../inetmanet/out/$$\(CONFIGNAME\)/src -linet -KINETMANET_PROJ=../../inetmanet
#

# Name of target to be created (-o option)
TARGET = libtbus-vsimrti$(SHARED_LIB_SUFFIX)

# C++ include paths (with -I)
INCLUDE_PATH = \
    -I../../inetmanet/src/networklayer/arp \
    -I../../inetmanet/src/transport/sctp \
    -I../../inetmanet/src/world \
    -I../../inetmanet/src/transport/contract \
    -I../../inetmanet/src/linklayer/mfcore \
    -I../../inetmanet/src/linklayer/ethernet \
    -I../../inetmanet/src/networklayer/ted \
    -I../../inetmanet/src/util \
    -I../../inetmanet/src/linklayer/ieee80211/mac \
    -I../../inetmanet/src/networklayer/queue \
    -I../../inetmanet/src/networklayer/common \
    -I../../inetmanet/src/networklayer/ipv6 \
    -I../../inetmanet/src/applications/pingapp \
    -I../../inetmanet/src/networklayer/ldp \
    -I../../inetmanet/src/linklayer/radio/errormodel \
    -I../../inetmanet/src/transport/tcp \
    -I../../inetmanet/src/util/headerserializers \
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
    -I. \
    -Ilinklayer \
    -Ilinklayer/radio \
    -Inodes \
    -Inodes/mobile \
    -Inodes/router \
    -Iqueues \
    -Iqueues/helper \
    -Iqueues/interfaces \
    -Iqueues/message \
    -Iqueues/ned \
    -Iqueues/network \
    -Iqueues/queues_download \
    -Iqueues/queues_upload \
    -Iqueues2 \
    -Iqueues2/control_info \
    -Iworld

# Additional object and library files to link with
EXTRA_OBJS =

# Additional libraries (-L, -l options)
LIBS = -L../../inetmanet/out/$(CONFIGNAME)/src  -linet
LIBS += -Wl,-rpath,`abspath ../../inetmanet/out/$(CONFIGNAME)/src`

# Output directory
PROJECT_OUTPUT_DIR = ../out
PROJECTRELATIVE_PATH = src
O = $(PROJECT_OUTPUT_DIR)/$(CONFIGNAME)/$(PROJECTRELATIVE_PATH)

# Object files for local .cc and .msg files
OBJS = \
    $O/linklayer/radio/TbusRadioPHY.o \
    $O/linklayer/radio/TbusQueueControl.o \
    $O/linklayer/radio/TbusRadio.o \
    $O/linklayer/radio/TbusRadioMAC.o \
    $O/nodes/router/TbusRouterNode.o \
    $O/queues/network/Datarate.o \
    $O/queues/network/Bbdelay.o \
    $O/queues/queues_download/CDRQ.o \
    $O/queues/queues_download/CRRQ.o \
    $O/queues/queues_upload/CRSQ.o \
    $O/queues/queues_upload/CDSQ.o \
    $O/queues2/TbusBaseQueue.o \
    $O/queues2/TbusDelayQueue.o \
    $O/queues2/TbusQueueDelayValue.o \
    $O/queues2/control_info/TbusQueueControlInfo.o \
    $O/world/TbusChannelControl.o \
    $O/queues/message/MyPacket_m.o \
    $O/queues/message/SendHeadAndDeletePacket_m.o

# Message files
MSGFILES = \
    queues/message/MyPacket.msg \
    queues/message/SendHeadAndDeletePacket.msg

# Other makefile variables (-K)
INETMANET_PROJ=../../inetmanet

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
	$(Q)-rm -f linklayer/*_m.cc linklayer/*_m.h
	$(Q)-rm -f linklayer/radio/*_m.cc linklayer/radio/*_m.h
	$(Q)-rm -f nodes/*_m.cc nodes/*_m.h
	$(Q)-rm -f nodes/mobile/*_m.cc nodes/mobile/*_m.h
	$(Q)-rm -f nodes/router/*_m.cc nodes/router/*_m.h
	$(Q)-rm -f queues/*_m.cc queues/*_m.h
	$(Q)-rm -f queues/helper/*_m.cc queues/helper/*_m.h
	$(Q)-rm -f queues/interfaces/*_m.cc queues/interfaces/*_m.h
	$(Q)-rm -f queues/message/*_m.cc queues/message/*_m.h
	$(Q)-rm -f queues/ned/*_m.cc queues/ned/*_m.h
	$(Q)-rm -f queues/network/*_m.cc queues/network/*_m.h
	$(Q)-rm -f queues/queues_download/*_m.cc queues/queues_download/*_m.h
	$(Q)-rm -f queues/queues_upload/*_m.cc queues/queues_upload/*_m.h
	$(Q)-rm -f queues2/*_m.cc queues2/*_m.h
	$(Q)-rm -f queues2/control_info/*_m.cc queues2/control_info/*_m.h
	$(Q)-rm -f world/*_m.cc world/*_m.h

cleanall: clean
	$(Q)-rm -rf $(PROJECT_OUTPUT_DIR)

depend:
	$(qecho) Creating dependencies...
	$(Q)$(MAKEDEPEND) $(INCLUDE_PATH) -f Makefile -P\$$O/ -- $(MSG_CC_FILES)  ./*.cc linklayer/*.cc linklayer/radio/*.cc nodes/*.cc nodes/mobile/*.cc nodes/router/*.cc queues/*.cc queues/helper/*.cc queues/interfaces/*.cc queues/message/*.cc queues/ned/*.cc queues/network/*.cc queues/queues_download/*.cc queues/queues_upload/*.cc queues2/*.cc queues2/control_info/*.cc world/*.cc

# DO NOT DELETE THIS LINE -- make depend depends on it.
$O/linklayer/radio/TbusQueueControl.o: linklayer/radio/TbusQueueControl.cc \
	$(INETMANET_PROJ)/src/base/Coord.h \
	$(INETMANET_PROJ)/src/base/INETDefs.h \
	$(INETMANET_PROJ)/src/base/ModuleAccess.h \
	$(INETMANET_PROJ)/src/util/FWMath.h \
	linklayer/radio/TbusQueueControl.h \
	queues/helper/Preamble.h \
	queues/interfaces/IBbdelayQueue.h \
	queues/interfaces/IDatarateQueue.h \
	queues/interfaces/IQueue.h \
	queues/message/MyPacket_m.h \
	queues/message/SendHeadAndDeletePacket_m.h \
	queues/network/Bbdelay.h \
	queues/network/Datarate.h \
	queues/queues_download/CDRQ.h \
	queues/queues_download/CRRQ.h \
	queues/queues_upload/CDSQ.h \
	queues/queues_upload/CRSQ.h
$O/linklayer/radio/TbusRadio.o: linklayer/radio/TbusRadio.cc \
	linklayer/radio/TbusRadio.h
$O/linklayer/radio/TbusRadioMAC.o: linklayer/radio/TbusRadioMAC.cc \
	$(INETMANET_PROJ)/src/base/INETDefs.h \
	$(INETMANET_PROJ)/src/base/INotifiable.h \
	$(INETMANET_PROJ)/src/base/ModuleAccess.h \
	$(INETMANET_PROJ)/src/base/NotificationBoard.h \
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
	linklayer/radio/TbusRadioMAC.h
$O/linklayer/radio/TbusRadioPHY.o: linklayer/radio/TbusRadioPHY.cc \
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
	linklayer/radio/TbusQueueControl.h \
	linklayer/radio/TbusRadioPHY.h \
	queues/helper/Preamble.h \
	queues/interfaces/IBbdelayQueue.h \
	queues/interfaces/IDatarateQueue.h \
	queues/interfaces/IQueue.h \
	queues/message/MyPacket_m.h \
	queues/message/SendHeadAndDeletePacket_m.h \
	queues/network/Bbdelay.h \
	queues/network/Datarate.h \
	queues/queues_download/CDRQ.h \
	queues/queues_download/CRRQ.h \
	queues/queues_upload/CDSQ.h \
	queues/queues_upload/CRSQ.h \
	world/TbusChannelControl.h
$O/nodes/router/TbusRouterNode.o: nodes/router/TbusRouterNode.cc \
	nodes/router/TbusRouterNode.h
$O/queues/message/MyPacket_m.o: queues/message/MyPacket_m.cc \
	queues/message/MyPacket_m.h \
	queues/network/Bbdelay.h \
	queues/network/Datarate.h
$O/queues/message/SendHeadAndDeletePacket_m.o: queues/message/SendHeadAndDeletePacket_m.cc \
	queues/message/SendHeadAndDeletePacket_m.h
$O/queues/network/Bbdelay.o: queues/network/Bbdelay.cc \
	queues/network/Bbdelay.h
$O/queues/network/Datarate.o: queues/network/Datarate.cc \
	queues/network/Datarate.h
$O/queues/queues_download/CDRQ.o: queues/queues_download/CDRQ.cc \
	queues/helper/Preamble.h \
	queues/interfaces/IBbdelayQueue.h \
	queues/interfaces/IQueue.h \
	queues/message/MyPacket_m.h \
	queues/message/SendHeadAndDeletePacket_m.h \
	queues/network/Bbdelay.h \
	queues/network/Datarate.h \
	queues/queues_download/CDRQ.h \
	queues2/control_info/TbusQueueControlInfo.h
$O/queues/queues_download/CRRQ.o: queues/queues_download/CRRQ.cc \
	queues/helper/Preamble.h \
	queues/interfaces/IDatarateQueue.h \
	queues/interfaces/IQueue.h \
	queues/message/MyPacket_m.h \
	queues/message/SendHeadAndDeletePacket_m.h \
	queues/network/Bbdelay.h \
	queues/network/Datarate.h \
	queues/queues_download/CRRQ.h
$O/queues/queues_upload/CDSQ.o: queues/queues_upload/CDSQ.cc \
	queues/helper/Preamble.h \
	queues/interfaces/IBbdelayQueue.h \
	queues/interfaces/IQueue.h \
	queues/message/MyPacket_m.h \
	queues/message/SendHeadAndDeletePacket_m.h \
	queues/network/Bbdelay.h \
	queues/network/Datarate.h \
	queues/queues_upload/CDSQ.h
$O/queues/queues_upload/CRSQ.o: queues/queues_upload/CRSQ.cc \
	queues/helper/Preamble.h \
	queues/interfaces/IDatarateQueue.h \
	queues/interfaces/IQueue.h \
	queues/message/MyPacket_m.h \
	queues/message/SendHeadAndDeletePacket_m.h \
	queues/network/Bbdelay.h \
	queues/network/Datarate.h \
	queues/queues_upload/CRSQ.h
$O/queues2/TbusBaseQueue.o: queues2/TbusBaseQueue.cc \
	queues2/TbusBaseQueue.h \
	queues2/TbusQueueValue.h \
	queues2/control_info/TbusQueueControlInfo.h
$O/queues2/TbusDelayQueue.o: queues2/TbusDelayQueue.cc \
	queues2/TbusBaseQueue.h \
	queues2/TbusDelayQueue.h \
	queues2/TbusQueueDelayValue.h \
	queues2/TbusQueueValue.h \
	queues2/control_info/TbusQueueControlInfo.h
$O/queues2/TbusQueueDelayValue.o: queues2/TbusQueueDelayValue.cc \
	queues2/TbusQueueDelayValue.h \
	queues2/TbusQueueValue.h
$O/queues2/control_info/TbusQueueControlInfo.o: queues2/control_info/TbusQueueControlInfo.cc \
	queues2/control_info/TbusQueueControlInfo.h
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
	$(INETMANET_PROJ)/src/world/ChannelControl.h \
	linklayer/radio/TbusQueueControl.h \
	linklayer/radio/TbusRadioPHY.h \
	queues/helper/Preamble.h \
	queues/interfaces/IBbdelayQueue.h \
	queues/interfaces/IDatarateQueue.h \
	queues/interfaces/IQueue.h \
	queues/message/MyPacket_m.h \
	queues/message/SendHeadAndDeletePacket_m.h \
	queues/network/Bbdelay.h \
	queues/network/Datarate.h \
	queues/queues_download/CDRQ.h \
	queues/queues_download/CRRQ.h \
	queues/queues_upload/CDSQ.h \
	queues/queues_upload/CRSQ.h \
	world/TbusChannelControl.h

