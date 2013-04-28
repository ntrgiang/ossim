//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
//


package so.simulations;

//import p2pstreaming.modules.Statistic.Statistics;
//import p2pstreaming.modules.Attacker.AttackerModule;
//import p2pstreaming.modules.Attacker.UnderlayAttacker.UnderlayAttackerModule;
//import p2pstreaming.modules.CoordinateSystem.CoordinateLoader;
//import p2pstreaming.modules.Configuration.Configuration;
//import p2pstreaming.modules.Bootstrap.Bootstrap;
//import p2pstreaming.modules.Streaming.GlobalStreamTable;
//import p2pstreaming.underlay.StatisticHost;
//import p2pstreaming.underlay.StatisticRouter;

import inet.nodes.inet.Router;
import so.underlay.inet.InetUnderlayConfigurator;

//import inet.networklayer.autorouting.FlatNetworkConfigurator;

//import p2pstreaming.modules.ALM_RouterSupport.ARS_RouterManager;

channel fiberline extends ned.DatarateChannel
{
    parameters:
        delay = 0.01s;
        datarate = 1000*1000000bps;
}

channel ethernetline extends ned.DatarateChannel
{
    parameters:
        delay = 0.01s;
        datarate = 1000*1000000bps;
}

module RouterLineNetwork
{
    parameters:
        int nRouter;
        int nClient;
    submodules:
//        globalStreamTable: GlobalStreamTable {
//            @display("i=block/table");
//        }

        configurator: InetUnderlayConfigurator {
            networkAddress = "141.24.0.0";
            netmask = "255.255.0.0";
            clients = nClient;
            @display("i=block/cogwheel");
        }

//        configurator: FlatNetworkConfigurator {
//            @display("p=77,29;i=block/cogwheel");
//            networkAddress = "192.168.0.0";
//            netmask = "255.255.0.0";
//        }



//        routerManager: ARS_RouterManager {
//            parameters:
//                @display("i=block/uparrow");
//        }

//        statistics: Statistics {
//            parameters:
//                @display("i=block/table");
//        }
//        attacker: AttackerModule {
//            parameters:
//                @display("i=abstract/people");
//        }
//        underlayAttacker: UnderlayAttackerModule {
//            parameters:
//                @display("i=abstract/people");
//        }
//        coordLoader: CoordinateLoader {
//            parameters:
//                @display("i=device/receiverdish");
//        }
//        configuration: Configuration {
//            parameters:
//                @display("i=block/table");
//        }
//        bootstrap: Bootstrap {
//            parameters:
//                @display("i=block/table");
//        }

//        rt[r+1]: StatisticRouter {
//            parameters:
//                @display("i=abstract/router");
//        }

        router[nRouter+1]: Router {
            parameters:
                @display("i=abstract/router");
        }

//    connections allowunconnected:

//        router[nRouter].out++ --> ethernetline --> router[nRouter/2].in++;
//        router[nRouter].in++ <-- ethernetline <-- router[nRouter/2].out++;

//        for i=0..nRouter-2 {
//            router[i].out++ --> fiberline --> router[i+1].in++;
//            router[i].in++ <-- fiberline <-- router[i+1].out++;
//        }

        connections allowunconnected:

        router[nRouter].pppg$o++ --> ethernetline --> router[nRouter/2].pppg$i++;
        router[nRouter].pppg$i++ <-- ethernetline <-- router[nRouter/2].pppg$o++;

        for i=0..nRouter-2 {
            router[i].pppg$o++ --> fiberline --> router[i+1].pppg$i++;
            router[i].pppg$i++ <-- fiberline <-- router[i+1].pppg$o++;
        }
}


network RouterLine extends RouterLineNetwork
{
}
