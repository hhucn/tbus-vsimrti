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

#define TBUS_VEH_PREFIX "veh_"
#define TBUS_RSU_PREFIX "rsu_"

#define TBUS_CLEAR_FIRST_BYTE(X) ((uint32_t) (0x00ffffff & X))

/**
 * Helper class for ip address assigning.
 */
class IpAddressHelper {
	public:
		static IPAddress getNextVehicleIpAddress(const char* const name);
		static IPAddress getNextRsuIpAddress(const char* const name);
		static IPAddress getNextVehicleIpAddress();
		static IPAddress getNextRsuIpAddress();
		static IPRoute* getDefaultRoute(InterfaceEntry* interfaceEntry);
	private:
		IpAddressHelper();
		virtual ~IpAddressHelper();
};

#endif /* IPADDRESSHELPER_H_ */
