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
// Network-r20.ned
// -----------------------------------------------------------------------------
// (C) Copyright 2012-2013, by Giang Nguyen (P2P, TU Darmstadt) and Contributors
//
// Contributors: Mathias Fischer;
// Code Reviewers: Giang Nguyen;
// -----------------------------------------------------------------------------
//

//
// OMNeT++ network description -- exported from GT-ITM (alt) topology
// Generator Model Used:
//   Model (8 - Imported From format file /home/sdelling/workspace/gtitm-topology-generator/topologies/small_gtitm_ts_spec-0.gtts ): 156 380 380.0 transtub 380.0 0.0 0.0 3.0 3.0 12.0 12.0 12.0 12.0 12.0 3.0 3.0 112.0 112.0 3.0 3.0 0.3 0.3 0.0 0.0 0.0 0.0 0.0 0.0 4.0 4.0 56.0 56.0 3.0 3.0 0.5 0.5 0.0 0.0 0.0 0.0 0.0 0.0 4.0 4.0 46.0 46.0 3.0 3.0 0.2 0.2 0.0 0.0 0.0 0.0 0.0 112.0 3.0 4.0 2.0 
//

package so.simulations;

//
// Prototype for modules that can be used as node in a GTITM_TransitStub_Network
//
import so.simulations.BaseNetworkUnderlayAttackModule;
import so.simulations.BaseNetworkOverlayAttackModule;
import so.underlay.inet.GtitmRouter;
import so.underlay.inet.RouterTR;
import so.underlay.inet.InetUnderlayConfigurator;

//
// GT-ITM TS topology exported via BRITE.
//
//module Network_r20 extends BaseNetworkOverlayAttackModule
module Network_r20 extends BaseNetworkUnderlayAttackModule
{
    parameters:
        //int nRouters; // no impact
        int nPeers;
    submodules:
        configurator: InetUnderlayConfigurator {
            parameters:
                networkAddress = "192.168.0.0";
                netmask = "255.255.0.0";
                nPeers = nPeers;
                @display("i=block/cogwheel");
        }
        //rt[16]: GtitmRouter {
        rt[16]: RouterTR {
            parameters:
                @display("i=abstract/router");
        }
    connections:
        rt[0].pppg$o++ --> {  delay = 0.01ms; datarate = 1.0127722794364535E9bps; } --> rt[1].pppg$i++;
        rt[1].pppg$o++ --> {  delay = 0.01ms; datarate = 1.0127722794364535E9bps; } --> rt[0].pppg$i++;
        
        rt[1].pppg$o++ --> {  delay = 0.55ms; datarate = 6.146283172664899E8bps; } --> rt[2].pppg$i++;
        rt[2].pppg$o++ --> {  delay = 0.55ms; datarate = 6.146283172664899E8bps; } --> rt[1].pppg$i++;
        
        rt[2].pppg$o++ --> {  delay = 0.01ms; datarate = 1.0127722794364535E9bps; } --> rt[3].pppg$i++;
        rt[3].pppg$o++ --> {  delay = 0.01ms; datarate = 1.0127722794364535E9bps; } --> rt[2].pppg$i++;
		
		rt[3].pppg$o++ --> {  delay = 0.01ms; datarate = 1.0127722794364535E9bps; } --> rt[1].pppg$i++;
        rt[1].pppg$o++ --> {  delay = 0.01ms; datarate = 1.0127722794364535E9bps; } --> rt[3].pppg$i++;
        
		rt[4].pppg$o++ --> {  delay = 0.01ms; datarate = 1.0127722794364535E9bps; } --> rt[5].pppg$i++;
        rt[5].pppg$o++ --> {  delay = 0.01ms; datarate = 1.0127722794364535E9bps; } --> rt[4].pppg$i++;
        
        rt[1].pppg$o++ --> {  delay = 0.01ms; datarate = 1.0127722794364535E9bps; } --> rt[5].pppg$i++;
        rt[5].pppg$o++ --> {  delay = 0.01ms; datarate = 1.0127722794364535E9bps; } --> rt[1].pppg$i++;
        
        rt[2].pppg$o++ --> {  delay = 0.01ms; datarate = 1.0127722794364535E9bps; } --> rt[6].pppg$i++;
        rt[6].pppg$o++ --> {  delay = 0.01ms; datarate = 1.0127722794364535E9bps; } --> rt[2].pppg$i++;
        
        rt[6].pppg$o++ --> {  delay = 0.01ms; datarate = 1.0127722794364535E9bps; } --> rt[7].pppg$i++;
        rt[7].pppg$o++ --> {  delay = 0.01ms; datarate = 1.0127722794364535E9bps; } --> rt[6].pppg$i++;
        
        rt[6].pppg$o++ --> {  delay = 0.01ms; datarate = 1.0127722794364535E9bps; } --> rt[8].pppg$i++;
        rt[8].pppg$o++ --> {  delay = 0.01ms; datarate = 1.0127722794364535E9bps; } --> rt[6].pppg$i++;
        
        rt[2].pppg$o++ --> {  delay = 0.01ms; datarate = 1.0127722794364535E9bps; } --> rt[9].pppg$i++;
        rt[9].pppg$o++ --> {  delay = 0.01ms; datarate = 1.0127722794364535E9bps; } --> rt[2].pppg$i++;
        
        rt[9].pppg$o++ --> {  delay = 0.01ms; datarate = 1.0127722794364535E9bps; } --> rt[10].pppg$i++;
        rt[10].pppg$o++ --> {  delay = 0.01ms; datarate = 1.0127722794364535E9bps; } --> rt[9].pppg$i++;
       
       	rt[9].pppg$o++ --> {  delay = 0.01ms; datarate = 1.0127722794364535E9bps; } --> rt[11].pppg$i++;
        rt[11].pppg$o++ --> {  delay = 0.01ms; datarate = 1.0127722794364535E9bps; } --> rt[9].pppg$i++;
        
        rt[11].pppg$o++ --> {  delay = 0.01ms; datarate = 1.0127722794364535E9bps; } --> rt[12].pppg$i++;
        rt[12].pppg$o++ --> {  delay = 0.01ms; datarate = 1.0127722794364535E9bps; } --> rt[11].pppg$i++;
        
        rt[11].pppg$o++ --> {  delay = 0.01ms; datarate = 1.0127722794364535E9bps; } --> rt[13].pppg$i++;
        rt[13].pppg$o++ --> {  delay = 0.01ms; datarate = 1.0127722794364535E9bps; } --> rt[11].pppg$i++;
        
        rt[6].pppg$o++ --> {  delay = 0.01ms; datarate = 1.0127722794364535E9bps; } --> rt[14].pppg$i++;
        rt[14].pppg$o++ --> {  delay = 0.01ms; datarate = 1.0127722794364535E9bps; } --> rt[6].pppg$i++;
        
        rt[9].pppg$o++ --> {  delay = 0.01ms; datarate = 1.0127722794364535E9bps; } --> rt[3].pppg$i++;
        rt[3].pppg$o++ --> {  delay = 0.01ms; datarate = 1.0127722794364535E9bps; } --> rt[9].pppg$i++;
        
        rt[0].pppg$o++ --> {  delay = 0.01ms; datarate = 1.0127722794364535E9bps; } --> rt[15].pppg$i++;
        rt[15].pppg$o++ --> {  delay = 0.01ms; datarate = 1.0127722794364535E9bps; } --> rt[0].pppg$i++;
}

//
network Network20 extends Network_r20
{
    parameters:
}

