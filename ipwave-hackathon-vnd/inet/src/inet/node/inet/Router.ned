//
// Copyright (C) OpenSim Ltd.
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program; if not, see <http://www.gnu.org/licenses/>.
//

package inet.node.inet;

import inet.applications.contract.IApp;
import inet.node.base.ApplicationLayerNodeBase;
import inet.routing.contract.IBgp;
import inet.routing.contract.IOspf;
import inet.routing.contract.IPim;

//
// IPv4 router that supports wireless, Ethernet, PPP and external interfaces.
// By default, no wireless and external interfaces are added; the number of
// Ethernet and PPP ports depends on the external connections.
//
// - Can be connected via ethernet interface to other nodes using
//   the ethg gate. By default full-duplex connections are supported
//   only (twisted pair). Set **.eth.typename="EthernetInterface" for
//   a full/half-duplex CSMA/CD implementation (coaxial cable)
// - By default contains no wireless cards, however it can be configured
//   by the numWlanInterfaces parameter. Wirless card type is configured by the
//   **.wlan.typename parameter. see:  inet.linklayer.ieee80211 or other
//   modules implementing ~IWirelessInterface
// - Also external interfaces can be configured for HW in the loop simulation
//   using the numExtInterfaces parameter and setting the type using
//   **.ext.typename in the INI file. see: ~ExtInterface and ~IExternalInterface
// - PPP queueing type can be set using **.ppp.queue.typename see: ~PppInterface
// - If wireless card is present, node mobility can be set using **.mobility.typename
//   see: inet.mobility and ~IMobility
//
// By default, dynamic routing is not supported. Specific routing protocols can
// be added by setting the hasOspf/hasRip/hasBgp parameters.
//
module Router extends ApplicationLayerNodeBase
{
    parameters:
        @display("i=abstract/router");
        @figure[submodules];
        forwarding = true;
//        isRouter = true;
        bool hasOspf = default(false);
        bool hasRip = default(false);
        bool hasBgp = default(false);
        bool hasPim = default(false);
        bool hasDhcp = default(false);
        hasUdp = default(hasRip || hasDhcp);
        hasTcp = default(hasBgp);
        *.routingTableModule = default("^.ipv4.routingTable");

    submodules:
        ospf: <"Ospf"> like IOspf if hasOspf {
            parameters:
                @display("p=975,226");
        }
        bgp: <"Bgp"> like IBgp if hasBgp {
            parameters:
                ospfRoutingModule = default(hasOspf ? "^.ospf" : "");
                @display("p=600,100");
        }
        rip: <"Rip"> like IApp if hasRip {
            parameters:
                @display("p=975,76");
        }
        pim: <"Pim"> like IPim if hasPim {
            parameters:
                @display("p=825,226");
        }
        dhcp: <"DhcpServer"> like IApp if hasDhcp {
            parameters:
                @display("p=1125,76");
        }

    connections allowunconnected:
        ospf.ipOut --> tn.in++ if hasOspf;
        ospf.ipIn <-- tn.out++ if hasOspf;

        bgp.socketOut --> at.in++ if hasBgp;
        bgp.socketIn <-- at.out++ if hasBgp;

        rip.socketOut --> at.in++ if hasRip;
        rip.socketIn <-- at.out++ if hasRip;

        pim.networkLayerOut --> tn.in++ if hasPim;
        pim.networkLayerIn <-- tn.out++ if hasPim;

        dhcp.socketOut --> at.in++ if hasDhcp;
        dhcp.socketIn <-- at.out++ if hasDhcp;
}
