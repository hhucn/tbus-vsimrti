/*
 * IpAddressHelper.h
 *
 *  Created on: 12.01.2015
 *      Author: bialon
 */

#ifndef IPADDRESSHELPER_H_
#define IPADDRESSHELPER_H_

#include "InterfaceEntry.h"
#include "IPAddress.h"
#include "IPRoute.h"

#define TBUS_NET_ID       IPAddress(0, 0, 0, 0) ///< VSimRTI only knows one Subnet, the WHOLE range
#define TBUS_NET_MASK     IPAddress(0, 0, 0, 0) ///< Netmask of given subnet (The WHOLE IPv4 address range)

#define TBUS_VEH_PREFIX "veh_" ///< Vehicle name prefix
#define TBUS_RSU_PREFIX "rsu_" ///< RSU name prefix

#define TBUS_CLEAR_FIRST_BYTE(X) ((uint32_t) (0x00ffffff & X)) ///< Clears the first byte of the 32bit IP address X

/**
 * Helper class for ip address assigning.
 */
class IpAddressHelper {
	public:
		static IPAddress getVehicleIpAddress(const uint32_t nodeid);
		static IPAddress getRsuIpAddress(const uint32_t nodeid);
		static IPRoute* getDefaultRoute(InterfaceEntry* interfaceEntry);

	private:
		IpAddressHelper();
		virtual ~IpAddressHelper();
        static uint32_t vehiclesJoined;
        static uint32_t rsusJoined;
};

#endif /* IPADDRESSHELPER_H_ */
