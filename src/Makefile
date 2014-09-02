#
# OMNeT++/OMNEST Makefile for libtbus-vsimrti
#
# This file was generated with the command:
#  opp_makemake -f --deep --make-so -O out -I../../inetmanet/src/networklayer/arp -I../../inetmanet/src/transport/sctp -I../../inetmanet/src/transport/contract -I../../inetmanet/src/world -I../../inetmanet/src/linklayer/mfcore -I../../inetmanet/src/linklayer/ethernet -I../../inetmanet/src/util -I../../inetmanet/src/networklayer/ted -I../../inetmanet/src/linklayer/ieee80211/mac -I../../inetmanet/src/networklayer/queue -I../../inetmanet/src/networklayer/common -I../../inetmanet/src/networklayer/ipv6 -I../../inetmanet/src/applications/pingapp -I../../inetmanet/src/networklayer/ldp -I../../inetmanet/src/linklayer/radio/errormodel -I../../inetmanet/src/transport/tcp -I../../inetmanet/src/util/headerserializers -I../../inetmanet/src/networklayer/rsvp_te -I../../inetmanet/src/transport/udp -I../../inetmanet/src/networklayer/ipv4 -I../../inetmanet/src/networklayer/icmpv6 -I../../inetmanet/src/base -I../../inetmanet/src/networklayer/contract -I../../inetmanet/src/linklayer/ieee80211/mgmt -I../../inetmanet/src/networklayer/manetrouting/base -I../../inetmanet/src/networklayer/mpls -I../../inetmanet/src/linklayer/contract -I../../inetmanet/src/applications/udpapp -L../../inetmanet/out/$$\(CONFIGNAME\)/src -linet -KINETMANET_PROJ=../../inetmanet
#

# Name of target to be created (-o option)
TARGET = libtbus-vsimrti$(SHARED_LIB_SUFFIX)

# C++ include paths (with -I)
INCLUDE_PATH = \
    -I../../inetmanet/src/networklayer/arp \
    -I../../inetmanet/src/transport/sctp \
    -I../../inetmanet/src/transport/contract \
    -I../../inetmanet/src/world \
    -I../../inetmanet/src/linklayer/mfcore \
    -I../../inetmanet/src/linklayer/ethernet \
    -I../../inetmanet/src/util \
    -I../../inetmanet/src/networklayer/ted \
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
    -I../../inetmanet/src/applications/udpapp \
    -I. \
    -Iapplications \
    -Iapplications/mobile \
    -Iapplications/router \
    -Ilinklayer \
    -Ilinklayer/airframe \
    -Ilinklayer/radio \
    -Inetworklayer \
    -Inetworklayer/message \
    -Inetworklayer/router \
    -Inodes \
    -Inodes/mobile \
    -Inodes/router \
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
    $O/applications/mobile/TbusMobileApplication.o \
    $O/applications/router/TbusRoutingApplication.o \
    $O/linklayer/radio/TbusRadioPHY.o \
    $O/linklayer/radio/TbusRadio.o \
    $O/linklayer/radio/TbusRadioMAC.o \
    $O/networklayer/router/TbusRouter.o \
    $O/nodes/mobile/TbusMobileNode.o \
    $O/nodes/router/TbusRouterNode.o \
    $O/world/TbusChannelControl.o \
    $O/linklayer/airframe/TbusAirFrame_m.o \
    $O/networklayer/message/TbusMessage_m.o

# Message files
MSGFILES = \
    linklayer/airframe/TbusAirFrame.msg \
    networklayer/message/TbusMessage.msg

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
	$(Q)-rm -f applications/*_m.cc applications/*_m.h
	$(Q)-rm -f applications/mobile/*_m.cc applications/mobile/*_m.h
	$(Q)-rm -f applications/router/*_m.cc applications/router/*_m.h
	$(Q)-rm -f linklayer/*_m.cc linklayer/*_m.h
	$(Q)-rm -f linklayer/airframe/*_m.cc linklayer/airframe/*_m.h
	$(Q)-rm -f linklayer/radio/*_m.cc linklayer/radio/*_m.h
	$(Q)-rm -f networklayer/*_m.cc networklayer/*_m.h
	$(Q)-rm -f networklayer/message/*_m.cc networklayer/message/*_m.h
	$(Q)-rm -f networklayer/router/*_m.cc networklayer/router/*_m.h
	$(Q)-rm -f nodes/*_m.cc nodes/*_m.h
	$(Q)-rm -f nodes/mobile/*_m.cc nodes/mobile/*_m.h
	$(Q)-rm -f nodes/router/*_m.cc nodes/router/*_m.h
	$(Q)-rm -f world/*_m.cc world/*_m.h

cleanall: clean
	$(Q)-rm -rf $(PROJECT_OUTPUT_DIR)

depend:
	$(qecho) Creating dependencies...
	$(Q)$(MAKEDEPEND) $(INCLUDE_PATH) -f Makefile -P\$$O/ -- $(MSG_CC_FILES)  ./*.cc applications/*.cc applications/mobile/*.cc applications/router/*.cc linklayer/*.cc linklayer/airframe/*.cc linklayer/radio/*.cc networklayer/*.cc networklayer/message/*.cc networklayer/router/*.cc nodes/*.cc nodes/mobile/*.cc nodes/router/*.cc world/*.cc

# DO NOT DELETE THIS LINE -- make depend depends on it.
$O/applications/mobile/TbusMobileApplication.o: applications/mobile/TbusMobileApplication.cc \
	$(INETMANET_PROJ)/src/base/Coord.h \
	$(INETMANET_PROJ)/src/base/INETDefs.h \
	$(INETMANET_PROJ)/src/linklayer/mfcore/AirFrame_m.h \
	$(INETMANET_PROJ)/src/linklayer/mfcore/ModulationType.h \
	$(INETMANET_PROJ)/src/util/FWMath.h \
	$(INETMANET_PROJ)/src/world/ChannelControl.h \
	applications/mobile/TbusMobileApplication.h \
	linklayer/airframe/TbusAirFrame_m.h \
	networklayer/message/TbusMessage_m.h \
	world/TbusChannelControl.h
$O/applications/router/TbusRoutingApplication.o: applications/router/TbusRoutingApplication.cc \
	$(INETMANET_PROJ)/src/applications/udpapp/UDPAppBase.h \
	$(INETMANET_PROJ)/src/applications/udpapp/UDPBasicApp.h \
	$(INETMANET_PROJ)/src/base/INETDefs.h \
	$(INETMANET_PROJ)/src/networklayer/contract/IPAddress.h \
	$(INETMANET_PROJ)/src/networklayer/contract/IPv6Address.h \
	$(INETMANET_PROJ)/src/networklayer/contract/IPvXAddress.h \
	$(INETMANET_PROJ)/src/transport/contract/UDPControlInfo_m.h \
	applications/router/TbusRoutingApplication.h
$O/linklayer/airframe/TbusAirFrame_m.o: linklayer/airframe/TbusAirFrame_m.cc \
	linklayer/airframe/TbusAirFrame_m.h
$O/linklayer/radio/TbusRadio.o: linklayer/radio/TbusRadio.cc \
	linklayer/radio/TbusRadio.h
$O/linklayer/radio/TbusRadioMAC.o: linklayer/radio/TbusRadioMAC.cc \
	$(INETMANET_PROJ)/src/base/INETDefs.h \
	$(INETMANET_PROJ)/src/base/INotifiable.h \
	$(INETMANET_PROJ)/src/base/ModuleAccess.h \
	$(INETMANET_PROJ)/src/base/NotificationBoard.h \
	$(INETMANET_PROJ)/src/base/NotifierConsts.h \
	$(INETMANET_PROJ)/src/linklayer/contract/MACAddress.h \
	$(INETMANET_PROJ)/src/linklayer/contract/TxNotifDetails.h \
	$(INETMANET_PROJ)/src/networklayer/common/IInterfaceTable.h \
	$(INETMANET_PROJ)/src/networklayer/common/InterfaceEntry.h \
	$(INETMANET_PROJ)/src/networklayer/common/InterfaceTableAccess.h \
	$(INETMANET_PROJ)/src/networklayer/common/InterfaceToken.h \
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
	$(INETMANET_PROJ)/src/util/FWMath.h \
	$(INETMANET_PROJ)/src/world/ChannelAccess.h \
	$(INETMANET_PROJ)/src/world/ChannelControl.h \
	linklayer/airframe/TbusAirFrame_m.h \
	linklayer/radio/TbusRadioPHY.h \
	networklayer/message/TbusMessage_m.h \
	world/TbusChannelControl.h
$O/networklayer/message/TbusMessage_m.o: networklayer/message/TbusMessage_m.cc \
	networklayer/message/TbusMessage_m.h
$O/networklayer/router/TbusRouter.o: networklayer/router/TbusRouter.cc \
	networklayer/router/TbusRouter.h
$O/nodes/mobile/TbusMobileNode.o: nodes/mobile/TbusMobileNode.cc \
	$(INETMANET_PROJ)/src/base/INETDefs.h \
	$(INETMANET_PROJ)/src/base/ModuleAccess.h \
	$(INETMANET_PROJ)/src/base/NotifierConsts.h \
	$(INETMANET_PROJ)/src/linklayer/contract/MACAddress.h \
	$(INETMANET_PROJ)/src/networklayer/common/IInterfaceTable.h \
	$(INETMANET_PROJ)/src/networklayer/common/InterfaceEntry.h \
	$(INETMANET_PROJ)/src/networklayer/common/InterfaceTableAccess.h \
	$(INETMANET_PROJ)/src/networklayer/common/InterfaceToken.h \
	$(INETMANET_PROJ)/src/networklayer/contract/IPAddress.h \
	$(INETMANET_PROJ)/src/networklayer/contract/IPv6Address.h \
	$(INETMANET_PROJ)/src/networklayer/contract/IPvXAddress.h \
	$(INETMANET_PROJ)/src/networklayer/ipv4/IPv4InterfaceData.h \
	nodes/mobile/TbusMobileNode.h
$O/nodes/router/TbusRouterNode.o: nodes/router/TbusRouterNode.cc \
	nodes/router/TbusRouterNode.h
$O/world/TbusChannelControl.o: world/TbusChannelControl.cc \
	$(INETMANET_PROJ)/src/base/Coord.h \
	$(INETMANET_PROJ)/src/base/INETDefs.h \
	$(INETMANET_PROJ)/src/linklayer/mfcore/AirFrame_m.h \
	$(INETMANET_PROJ)/src/linklayer/mfcore/ModulationType.h \
	$(INETMANET_PROJ)/src/util/FWMath.h \
	$(INETMANET_PROJ)/src/world/ChannelControl.h \
	linklayer/airframe/TbusAirFrame_m.h \
	networklayer/message/TbusMessage_m.h \
	world/TbusChannelControl.h

