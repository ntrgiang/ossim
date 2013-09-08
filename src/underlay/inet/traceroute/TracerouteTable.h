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

// -----------------------------------------------------------------------------
// TracerouteTable.h
// -----------------------------------------------------------------------------
// (C) Copyright 2012-2013, by Giang Nguyen (P2P, TU Darmstadt) and Contributors
//
// Contributors: Mathias Fischer;
// Code Reviewers: Giang Nguyen;
// -----------------------------------------------------------------------------
//

#ifndef ROUTETABLE_H_
#define ROUTETABLE_H_

#include <omnetpp.h>
#include <IPvXAddress.h>

class TracerouteTable : public cSimpleModule
{
public:
	bool useLinksForOverlap;
	bool active;

   struct RouteInfo
   {
		int hop;
		IPvXAddress addr;
	};

	typedef std::vector<RouteInfo *> Route;
	typedef std::map<IPvXAddress, Route> RouteMap;
	typedef RouteMap::iterator Iterator;
	RouteMap routes;

public:
	void initialize();
	void finish();

protected:
	IPvXAddress referencePeer;

public:
	TracerouteTable() {};
	~TracerouteTable() {};
	int getSharedHops(IPvXAddress n1, IPvXAddress n2);
	int getSharedHops(const Route r1, const Route r2);
	int getSharedHopsForeignRoute(Route r1, IPvXAddress n2);
	double getWeightedSharedNodeBothHops(IPvXAddress n1, IPvXAddress n2);
	double getWeightedSharedHops(IPvXAddress n1, IPvXAddress n2);
	double getWeightedSharedHops(const Route r1, const Route r2);

	double getWeightedSharedNodeFirstHops2(IPvXAddress n1, IPvXAddress n2);
	Route getRoute(IPvXAddress addr);
	Route& getRouteRef(IPvXAddress addr);
	int getRouteSize(IPvXAddress addr);
	int getRouteSize2(IPvXAddress addr);
	RouteMap getRoutes() const;
	void setRoute(Route route, IPvXAddress addr);
	void printRoute(Route r);
	void printRoutes(Route r1, Route r2);
	bool isActive();
	void setActive(bool act);

	void setReferencePeer(IPvXAddress reference);
	IPvXAddress getReferencePeer();

   bool inTable(IPvXAddress addr);
};

#endif /* ROUTETABLE_H_ */
