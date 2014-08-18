//
// (c) 2014 Raphael Bialon <Raphael.Bialon@hhu.de>
// 
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

package tbus.nodes.router;

import tbus.applications.router.TbusRoutingApplication;
import tbus.networklayer.router.TbusRouter;

module TbusRouterNode {
    parameters:
        @display("i=device/router");
        
    gates:
        input channelInGate;
        output channelOutGate;
        input inetInGate @loose;
        output inetOutGate @loose;
        
	submodules:
	    routingApp: TbusRoutingApplication;
	    router: TbusRouter;
	    
	connections allowunconnected:
	    routingApp.lowerOutGate --> router.upperInGate;
	    routingApp.lowerInGate  <-- router.upperOutGate;
	    
	    router.channelInGate <-- channelInGate;
	    router.channelOutGate --> channelOutGate;
	    
	    router.inetInGate <-- inetInGate;
	    router.inetOutGate --> inetOutGate;
}