/*
 * IpAddressHelper.cpp
 *
 *  Created on: 12.01.2015
 *      Author: bialon
 */

#include "IpAddressHelper.h"
#include <string>
#include <sstream>

/**
 * Empty constructor
 */
IpAddressHelper::IpAddressHelper() {}

/**
 * Empty destructor
 */
IpAddressHelper::~IpAddressHelper() {}

/**
 * Returns a default route for the subnet defined by TBUS_NET_ID and TBUS_NET_MASK on interface ie
 * @param ie The interface this route should act on
 * @return A default route for the defined subnet
 */
IPRoute* IpAddressHelper::getDefaultRoute(InterfaceEntry* ie) {
	IPRoute* route = new IPRoute();
	route->setHost(TBUS_NET_ID);
	route->setNetmask(TBUS_NET_MASK);
	route->setInterface(ie);
	route->setSource(IPRoute::MANUAL);

	return route;
}

IPAddress IpAddressHelper::getNextVehicleIpAddress() {
	#define TBUS_VEH_SUBNET ((uint32_t) 0x00 << 24)

	static uint32_t ip = 0;

	return IPAddress(TBUS_CLEAR_FIRST_BYTE(ip++) | TBUS_VEH_SUBNET);
}

IPAddress IpAddressHelper::getNextRsuIpAddress() {
	#define TBUS_RSU_SUBNET ((uint32_t) 0x01 << 24)

	static uint32_t ip = 0;

	return IPAddress(TBUS_CLEAR_FIRST_BYTE(ip++) | TBUS_RSU_SUBNET);
}

/**
 * Returns the next free vehicle ip address, starting from TBUS_IP_STARTBYTE.
 * According to VSimRTI, vehicles have IP addresses in range 0.0.0.0/8.
 * The last bytes are chosen from the vehicle id in the vehicle name "veh_X".
 * @param name Vehicle name
 * @return Next unassigned ip address in range
 * @deprecated VSimRTI has no correct mapping in OMNeT++ to its internal names used for ip address assignment.
 */
IPAddress IpAddressHelper::getNextVehicleIpAddress(const char* const name) {
	#define TBUS_VEH_SUBNET ((uint32_t) 0x00 << 24)
	std::string sName(name);
	std::size_t pos = sName.find(TBUS_VEH_PREFIX) + strlen(TBUS_VEH_PREFIX);
	uint32_t ip;

	std::istringstream(sName.substr(pos)) >> ip;

	return IPAddress(TBUS_CLEAR_FIRST_BYTE(ip) | TBUS_VEH_SUBNET);
}

/**
 * Returns the next free RSU ip address, starting from TBUS_IP_STARTBYTE.
 * According to VSimRTI, rsu have IP addresses in range 1.0.0.0/8.
 * The last bytes are chosen from the rsu id in the rsu name "rsu_X".
 * @param name RSU name
 * @return Next unassigned ip address in range
 * @deprecated VSimRTI has no correct mapping in OMNeT++ to its internal names used for ip address assignment.
 */
IPAddress IpAddressHelper::getNextRsuIpAddress(const char* const name) {
	#define TBUS_RSU_SUBNET ((uint32_t) 0x01 << 24)
	std::string sName(name);
	std::size_t pos = sName.find(TBUS_RSU_PREFIX) + strlen(TBUS_RSU_PREFIX);
	uint32_t ip;

	std::istringstream(sName.substr(pos)) >> ip;

	return IPAddress(TBUS_CLEAR_FIRST_BYTE(ip) | TBUS_RSU_SUBNET);
}
