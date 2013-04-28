//
// OMNeT++ network description -- exported from GT-ITM (alt) topology
// Generator Model Used:
//   Model (8 - Imported From format file /home/sdelling/workspace/gtitm-topology-generator/topologies/small_gtitm_ts_spec-0.gtts ): 156 380 380.0 transtub 380.0 0.0 0.0 3.0 3.0 12.0 12.0 12.0 12.0 12.0 3.0 3.0 112.0 112.0 3.0 3.0 0.3 0.3 0.0 0.0 0.0 0.0 0.0 0.0 4.0 4.0 56.0 56.0 3.0 3.0 0.5 0.5 0.0 0.0 0.0 0.0 0.0 0.0 4.0 4.0 46.0 46.0 3.0 3.0 0.2 0.2 0.0 0.0 0.0 0.0 0.0 112.0 3.0 4.0 2.0 
//

//This file must be placed under p2pstreaming/src/underlay/inetunderlay

//Package
package so.simulations;

//
// Prototype for modules that can be used as node in a GTITM_TransitStub_Network
//
//import so.simulations.BaseNetworkModule;
import so.underlay.inet.GtitmRouter;
import so.underlay.inet.InetUnderlayConfigurator;

//
// GT-ITM TS topology exported via BRITE.
//
module Network_r20 extends BaseNetworkModule
//module Network_r20
{
    parameters:
		int nRouter;
		int nClient;
    submodules:
        configurator: InetUnderlayConfigurator {
            parameters:
                networkAddress = "141.24.0.0";
                netmask = "255.255.0.0";
                clients = nClient;
                @display("i=block/cogwheel");
        }
        rt[16]: GtitmRouter {
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

