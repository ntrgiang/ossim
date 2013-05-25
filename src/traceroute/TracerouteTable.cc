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

#include "TracerouteTable.h"
#include "TracerouteHandler.h"
#include <cassert>

Define_Module(TracerouteTable);


void TracerouteTable::initialize(){
	useLinksForOverlap = par("useLinksForOverlap");
}

void TracerouteTable::finish() {
	
}

bool TracerouteTable::isActive() {
	return active;
}

void TracerouteTable::setActive(bool act) {
	active = act;
}


/**
 * returns route to node
 *
 * @ param	addr	destination fro the route to be returned
 * @ return			route to the destination
 */
TracerouteTable::Route TracerouteTable::getRoute(IPvXAddress addr){
    bool started = false;

	if(!addr.isUnspecified()) {

		if (routes[addr].size() == 0) {
            PPIPvXAddressVector tracerouteStarted = check_and_cast<TracerouteHandler*>(getParentModule()->getSubmodule("tracerouteHandler"))->tracerouteStarted;

            //if traceroute already started -> return
            if (tracerouteStarted.size() > 0){
                for (int i = 0 ; i < (int)tracerouteStarted.size(); i++) {
					if (tracerouteStarted[i] == addr) started =  true;
                }
            }
        }

        if(!started) {
            check_and_cast<TracerouteHandler*>(getParentModule()->getSubmodule("tracerouteHandler"))->doHandleTraceroute(addr);
        }

        return routes[addr];

    }else{
        std::cout << "Routetable::getRoute: no node given";
		throw cRuntimeError("TracerouteTable::getRoute() addr not specified");
    }

}

TracerouteTable::Route& TracerouteTable::getRouteRef(IPvXAddress addr){
	bool started = false;
	if(!addr.isUnspecified()) {
		if (routes.find(addr) == routes.end() || routes[addr].size() == 0) {
                        PPIPvXAddressVector tracerouteStarted = check_and_cast<TracerouteHandler*>(getParentModule()->getSubmodule("tracerouteHandler"))->tracerouteStarted;

			//if traceroute already started -> return
			if (tracerouteStarted.size() > 0){
                for (int i = 0 ; i < (int)tracerouteStarted.size(); i++) {
					if (tracerouteStarted[i] == addr) {
						started =  true;
					}
				}
			}
		}

		if(!started) {
			check_and_cast<TracerouteHandler*>(getParentModule()->getSubmodule("tracerouteHandler"))->doHandleTraceroute(addr);
		}
		return routes[addr];
	}else{
		throw cRuntimeError("TracerouteTable::getRouteRef, IPvXAddress unspecified");
	}

}


/**
 * returns route size to node
 *
 * @ param	addr	destination from the route to be returned
 * @ return			route to the destination
 */
int TracerouteTable::getRouteSize2(IPvXAddress addr){
	if(routes.find(addr) != routes.end())
		return routes[addr].size();
	else
		return 0;
}


/**
 * returns route size to node
 *
 * @ param	addr	destination from the route to be returned
 * @ return	size of route to the destination
 */
int TracerouteTable::getRouteSize(IPvXAddress addr) {
	if(routes.find(addr) != routes.end())
		return getRouteRef(addr).size();
	else
		return 0;
}


/**
 * sets route entry in routetable to specified node
 *
 * @param route		The route to destination
 * @param addr		Destination address
 */
void TracerouteTable::setRoute(Route route, IPvXAddress addr) {

//	assert(!route.empty());
    routes[addr] = route;

    // Print received Route
    /*std::cout<< getParentModule()->getParentModule()->getParentModule()->getFullName() << " Dest: " << addr << endl;
	 int size = route.size();
	 for (int i = 0; i !=size; i++){
		 std::cout << route[i]->hop << " " << route[i]->addr << endl;
	 }*/

}


/**
 * returns number of shared nodes from this node to n1 and n2
 *
 * @param n1 	first destination
 * @param n2 	second destination
 * @return			Number of shared hops
 */
int TracerouteTable::getSharedHops(IPvXAddress n1, IPvXAddress n2){

	assert(routes.find(n1) != routes.end());
	assert(routes.find(n2) != routes.end());
	const Route r1 = routes[n1];
	const Route r2 = routes[n2];

	return getSharedHops(r1,r2);

} // int TracerouteTable::getSharedHops(IPvXAddress n1, IPvXAddress n2)


/**
 * returns number of shared nodes in between routes r1 and r2
 *
 * @param r1 	first route
 * @param r2 	second route
 * @return			Number of shared hops
 */
int TracerouteTable::getSharedHops(const Route r1, const Route r2) {

	assert(!r1.empty() && ! r2.empty());
	std::set<std::pair<IPvXAddress,IPvXAddress> > r1Links;
	std::set<std::pair<IPvXAddress,IPvXAddress> > r2Links;
	std::pair<IPvXAddress,IPvXAddress> addressPair;

	if (r1.size() > 1) {
		for(int i = 0; i < (int)r1.size() - 1; i++ ) {

			if(r1[i]->addr < r1[i+1]->addr) {
				addressPair.first = r1[i]->addr;
				addressPair.second = r1[i+1]->addr;
			} else {
				addressPair.first = r1[i+1]->addr;
				addressPair.second = r1[i]->addr;
			}
			r1Links.insert(addressPair);
		}
	}

	if (r2.size() > 1) {
		for(int i = 0; i < (int)r2.size() - 1; i++ ) {
			if(r2[i]->addr < r2[i+1]->addr) {
				addressPair.first = r2[i]->addr;
				addressPair.second = r2[i+1]->addr;
			} else {
				addressPair.first = r2[i+1]->addr;
				addressPair.second = r2[i]->addr;
			}
			r2Links.insert(addressPair);
		}
	}

	int sharedLinks = 0;

	foreach(addressPair, r1Links) {
		if (r2Links.count(addressPair) > 0)
			sharedLinks++;
	}

	int sharedRouters = 0;

	for(int i = 0; i < (int)r1.size();i++){
		for(int j = 0; j < (int)r2.size();j++){
			if(r1[i]->addr == r2[j]->addr) sharedRouters++;
		}
	}

	if (useLinksForOverlap) {
		return sharedLinks;
	} else {
		return sharedRouters;
	}
} // int TracerouteTable::getSharedHops(Route r1, Route r2)



/**
 * returns number of shared links/routers from the local node to n1 and n2
 *
 * @param r1 	route to destination
 * @param n2 	second destination
 */
int TracerouteTable::getSharedHopsForeignRoute(Route r1, IPvXAddress n2){

    assert(routes.find(n2) != routes.end());
	Route r2 = routes[n2];
	std::set<std::pair<IPvXAddress,IPvXAddress> > r1Links;
	std::set<std::pair<IPvXAddress,IPvXAddress> > r2Links;
	std::pair<IPvXAddress,IPvXAddress> addressPair;
	//TODO invert addressPair???
	
	// Create an edge-set for r1
	if (r1.size() > 1) {
        for(int i = 0; i < (int)r1.size() - 1; i++ ) {
			
			if(r1[i]->addr < r1[i+1]->addr) {
				addressPair.first = r1[i]->addr;
				addressPair.second = r1[i+1]->addr;
			} else {
				addressPair.first = r1[i+1]->addr;
				addressPair.second = r1[i]->addr;
			}				
			r1Links.insert(addressPair);	
		}
	}

	// Create an edge-set for r2
	if (r2.size() > 1) {	
        for(int i = 0; i < (int)r2.size() - 1; i++ ) {
			if(r2[i]->addr < r2[i+1]->addr) {
				addressPair.first = r2[i]->addr;
				addressPair.second = r2[i+1]->addr;
			} else {
				addressPair.first = r2[i+1]->addr;
				addressPair.second = r2[i]->addr;	
			}				
			r2Links.insert(addressPair);	
		}
	}
	
	// Calculate shared links
	if (useLinksForOverlap) {

		int sharedLinks = 0;
		foreach(addressPair, r1Links) {
			if (r2Links.count(addressPair) > 0) sharedLinks++;
		}
		return sharedLinks;

	// Calculate shared routers
	} else { // !useLinksForOverlap

		int sharedRouters = 0;
        for(int i = 0; i < (int)r1.size();i++){
			for(int j = 0; j < (int)r2.size();j++){
				if(r1[i]->addr == r2[j]->addr) sharedRouters++;
			}
		}
		return sharedRouters;
	}
	
} // END int TracerouteTable::getSharedHopsForeignRoute(Route r1, IPvXAddress n2)


/**
 * returns number of shared nodes from 2 paths, weighted by the path-length
 *
 * @param n1	first destination
 * @param n2	second destination
 */
double TracerouteTable::getWeightedSharedNodeBothHops(IPvXAddress n1, IPvXAddress n2){

	double sr = (double)getSharedHops(n1,n2);
    return 2*sr/(double)(routes[n1].size() + routes[n2].size());

}


/**
 * returns number of shared nodes from 2 paths, weighted by the path-length
 *
 * @param n1	first destination
 * @param n2	second destination
 */
double TracerouteTable::getWeightedSharedHops(IPvXAddress n1, IPvXAddress n2){

	double sr = (double)getSharedHops(n1,n2);
    return (sr / (double)(routes[n2].size()));

}


/**
 * returns number of shared nodes from 2 paths, weighted by the path-length
 *
 * @param r1	first destination
 * @param r2	second destination
 */
double TracerouteTable::getWeightedSharedHops(const Route r1, const Route r2) {
	double sr = (double) getSharedHops(r1,r2);
	return (sr / (double) r2.size());
}


/**
 * returns number of shared nodes from 2 paths, weighted by the path-length
 *
 * @param n1	first destination
 * @param n2	second destination
 */
double TracerouteTable::getWeightedSharedNodeFirstHops2(IPvXAddress n1, IPvXAddress n2){

	double sr = (double)getSharedHops(n1,n2);
    return ((double)(routes[n1].size()) + ((double)(routes[n1].size()) - sr));

}


/**
 *	returns all routes collected in this traceroutetable, used by the underlayattacker
 *
 *	@return	the map containing all routes from this traceroutetable
 */
TracerouteTable::RouteMap TracerouteTable::getRoutes() const {
    return routes;
}

/**
 *  prints the routes
 *  @param r1	route1
 *  @param r2	route2
 */
void TracerouteTable::printRoutes(Route r1,Route r2){
	
    std::cout << "Route1: " << endl;
	printRoute(r1);

    std::cout << "Route2: " << endl;
	printRoute(r2);
}

/**
 *  prints the routes
 *  @param r	route
 */
void TracerouteTable::printRoute(Route r) {
	for (int i = 0; i < (int)r.size() ; i++) {
		std::cout << r[i]->addr << " ->  ";
	}
	std::cout<<endl;
}

/**
 * Set reference peer that serves as common reference for topology construction
 */
void TracerouteTable::setReferencePeer(IPvXAddress reference) {
	referencePeer = reference;
}

/**
 * Obtain reference peer that serves as common reference for topology construction
 */
IPvXAddress TracerouteTable::getReferencePeer() {
	assert(!referencePeer.isUnspecified());
	return referencePeer;
}
