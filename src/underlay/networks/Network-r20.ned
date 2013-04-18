//
// OMNeT++ network description -- exported from GT-ITM (alt) topology
// Generator Model Used:
//   Model (8 - Imported From format file /home/sdelling/workspace/gtitm-topology-generator/topologies/small_gtitm_ts_spec-0.gtts ): 156 380 380.0 transtub 380.0 0.0 0.0 3.0 3.0 12.0 12.0 12.0 12.0 12.0 3.0 3.0 112.0 112.0 3.0 3.0 0.3 0.3 0.0 0.0 0.0 0.0 0.0 0.0 4.0 4.0 56.0 56.0 3.0 3.0 0.5 0.5 0.0 0.0 0.0 0.0 0.0 0.0 4.0 4.0 46.0 46.0 3.0 3.0 0.2 0.2 0.0 0.0 0.0 0.0 0.0 112.0 3.0 4.0 2.0 
//

//This file must be placed under p2pstreaming/src/underlay/inetunderlay

//Package
package p2pstreaming.underlay.networks;

//
// Prototype for modules that can be used as node in a GTITM_TransitStub_Network
//

import p2pstreaming.modules.Statistic.Statistics;
import p2pstreaming.modules.Attacker.AttackerModule;
import p2pstreaming.modules.Attacker.UnderlayAttacker.UnderlayAttackerModule;
import p2pstreaming.modules.CoordinateSystem.CoordinateLoader;
import p2pstreaming.modules.Configuration.Configuration;
import p2pstreaming.modules.Bootstrap.Bootstrap;
import p2pstreaming.modules.ALM_RouterSupport.ARS_RouterManager;
import p2pstreaming.modules.Streaming.GlobalStreamTable;
import p2pstreaming.underlay.InetUnderlayConfigurator;
import p2pstreaming.underlay.StatisticHost;
import p2pstreaming.underlay.StatisticRouter;
//
// GT-ITM TS topology exported via BRITE.
//
module Network_r20
{
    parameters:
        double r;
        int clients;
    submodules:
        globalStreamTable: GlobalStreamTable {
            @display("p=498,194");
        }
        configurator: InetUnderlayConfigurator {
            parameters:
                networkAddress = "141.24.0.0";
                netmask = "255.255.0.0";
                clients = clients;
                @display("i=block/cogwheel");
        }
        routerManager: ARS_RouterManager {
            parameters:
                @display("i=block/uparrow");
        }
        statistics: Statistics {
            parameters:
                @display("i=block/table");
        }
        attacker: AttackerModule {
            parameters:
                @display("i=abstract/people");
        }
        underlayAttacker: UnderlayAttackerModule {
            parameters:
                @display("i=abstract/people");
        }
        coordLoader: CoordinateLoader {
            parameters:
                @display("i=device/receiverdish");
        }
        configuration: Configuration {
            parameters:
                @display("i=block/table");
        }
        bootstrap: Bootstrap {
            parameters:
                @display("i=block/table");
        }
        rt[16]: StatisticRouter {
            parameters:
                @display("i=abstract/router");
        }
    connections:
        rt[0].out++ --> {  delay = 0.01ms; datarate = 1.0127722794364535E9bps; } --> rt[1].in++;
        rt[1].out++ --> {  delay = 0.01ms; datarate = 1.0127722794364535E9bps; } --> rt[0].in++;
        
        rt[1].out++ --> {  delay = 0.55ms; datarate = 6.146283172664899E8bps; } --> rt[2].in++;
        rt[2].out++ --> {  delay = 0.55ms; datarate = 6.146283172664899E8bps; } --> rt[1].in++;
        
        rt[2].out++ --> {  delay = 0.01ms; datarate = 1.0127722794364535E9bps; } --> rt[3].in++;
        rt[3].out++ --> {  delay = 0.01ms; datarate = 1.0127722794364535E9bps; } --> rt[2].in++;
		
		rt[3].out++ --> {  delay = 0.01ms; datarate = 1.0127722794364535E9bps; } --> rt[1].in++;
        rt[1].out++ --> {  delay = 0.01ms; datarate = 1.0127722794364535E9bps; } --> rt[3].in++;
        
		rt[4].out++ --> {  delay = 0.01ms; datarate = 1.0127722794364535E9bps; } --> rt[5].in++;
        rt[5].out++ --> {  delay = 0.01ms; datarate = 1.0127722794364535E9bps; } --> rt[4].in++;
        
        rt[1].out++ --> {  delay = 0.01ms; datarate = 1.0127722794364535E9bps; } --> rt[5].in++;
        rt[5].out++ --> {  delay = 0.01ms; datarate = 1.0127722794364535E9bps; } --> rt[1].in++;
        
        rt[2].out++ --> {  delay = 0.01ms; datarate = 1.0127722794364535E9bps; } --> rt[6].in++;
        rt[6].out++ --> {  delay = 0.01ms; datarate = 1.0127722794364535E9bps; } --> rt[2].in++;
        
        rt[6].out++ --> {  delay = 0.01ms; datarate = 1.0127722794364535E9bps; } --> rt[7].in++;
        rt[7].out++ --> {  delay = 0.01ms; datarate = 1.0127722794364535E9bps; } --> rt[6].in++;
        
        rt[6].out++ --> {  delay = 0.01ms; datarate = 1.0127722794364535E9bps; } --> rt[8].in++;
        rt[8].out++ --> {  delay = 0.01ms; datarate = 1.0127722794364535E9bps; } --> rt[6].in++;
        
        rt[2].out++ --> {  delay = 0.01ms; datarate = 1.0127722794364535E9bps; } --> rt[9].in++;
        rt[9].out++ --> {  delay = 0.01ms; datarate = 1.0127722794364535E9bps; } --> rt[2].in++;
        
        rt[9].out++ --> {  delay = 0.01ms; datarate = 1.0127722794364535E9bps; } --> rt[10].in++;
        rt[10].out++ --> {  delay = 0.01ms; datarate = 1.0127722794364535E9bps; } --> rt[9].in++;
       
       	rt[9].out++ --> {  delay = 0.01ms; datarate = 1.0127722794364535E9bps; } --> rt[11].in++;
        rt[11].out++ --> {  delay = 0.01ms; datarate = 1.0127722794364535E9bps; } --> rt[9].in++;
        
        rt[11].out++ --> {  delay = 0.01ms; datarate = 1.0127722794364535E9bps; } --> rt[12].in++;
        rt[12].out++ --> {  delay = 0.01ms; datarate = 1.0127722794364535E9bps; } --> rt[11].in++;
        
        rt[11].out++ --> {  delay = 0.01ms; datarate = 1.0127722794364535E9bps; } --> rt[13].in++;
        rt[13].out++ --> {  delay = 0.01ms; datarate = 1.0127722794364535E9bps; } --> rt[11].in++;
        
        rt[6].out++ --> {  delay = 0.01ms; datarate = 1.0127722794364535E9bps; } --> rt[14].in++;
        rt[14].out++ --> {  delay = 0.01ms; datarate = 1.0127722794364535E9bps; } --> rt[6].in++;
        
        rt[9].out++ --> {  delay = 0.01ms; datarate = 1.0127722794364535E9bps; } --> rt[3].in++;
        rt[3].out++ --> {  delay = 0.01ms; datarate = 1.0127722794364535E9bps; } --> rt[9].in++;
        
        rt[0].out++ --> {  delay = 0.01ms; datarate = 1.0127722794364535E9bps; } --> rt[15].in++;
        rt[15].out++ --> {  delay = 0.01ms; datarate = 1.0127722794364535E9bps; } --> rt[0].in++;
}

//
network Network20 extends Network_r20
{
    parameters:
}

