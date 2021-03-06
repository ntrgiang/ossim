//
// =============================================================================
// OSSIM : A Generic Simulation Framework for Overlay Streaming
// =============================================================================
//
// (C) Copyright 2012-2013, by Giang Nguyen (P2P, TU Darmstadt) and Contributors
//
// Project Info: http://www.p2p.tu-darmstadt.de/research/ossim
//
// OSSIM is free software: you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the Free Software
// Foundation, either version 3 of the License, or (at your option) any later
// version.
//
// OSSIM is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
// A PARTICULAR PURPOSE. See the GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along with
// this program. If not, see <http://www.gnu.org/licenses/>.
//
// -----------------------------------------------------------------------------
// InetUnderlayConfigurator.ned
// -----------------------------------------------------------------------------
// (C) Copyright 2012-2013, by Giang Nguyen (P2P, TU Darmstadt) and Contributors
//
// Contributors: Mathias Fischer;
// Code Reviewers: Giang Nguyen;
// -----------------------------------------------------------------------------
//


package so.underlay.inet;

//
// Configures \IP addresses and routing tables for a "flat" network,
// "flat" meaning that all hosts and routers will have the same
// network address and will only differ in the host part.
//
// This module does't connect to any other modules (it has no gates),
// and should have only one instance in the whole model. The module
// will only run once, at the beginning of the simulation.
// When it runs, it will:
//
//   -#  assign \IP addresses to hosts and routers. All hosts and
//       routers will be in the same network (same network address).
//       For simplicity, it will assign the same address to all interfaces
//       of a router;
//   -#  then it'll discover the topology of the network (using OMNeT++'s
//       cTopology class), and calculate shortest paths;
//   -#  finally, it will add routes which correspond to the shortest
//       paths to the routing tables (see RoutingTable::addRoutingEntry()).
//
// How does it know which modules are routers, hosts, et.c that need to
// be configured, and what is the network topology? The configurator
// picks all modules which have a @node property and their connections,
// and builds a graph from it. Then it runs Dijstra's
// shortest path algorithm on it, and configures all modules which are
// IP nodes.
//
// It is assumed that the routing table (RoutingTable module) is the
// "routingTable" or "networkLayer.routingTable" submodule in all hosts
// and routers.
//
// To avoid interference with the above algorithm, it's recommended that
// no host or router should have its address set explicitly, and
// no routes are set up manually. Practically, routing files (.irt, .mrt)
// should be absent or empty.
//
// All the above takes place in initialization stage 2. (In stage 0,
// interfaces register themselves in the InterfaceTable modules, and
// in stage 1, routing files are read.)
//
simple InetUnderlayConfigurator
{
	parameters:
        string networkAddress = default("192.168.0.0");
        string netmask = default("255.255.0.0");
		@display("i=block/broadcast");
        int nPeers;
        int nSources = default(1);
		bool noneTransitNodesAccessRouters;
        // double simTimeLimit;
        double minAccessDelay;
        double maxAccessDelay;
}

