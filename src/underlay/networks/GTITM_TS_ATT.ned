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
module GTITM_TS_IPv4Underlay_ATT
{
	parameters:
		double r;
		int clients;
	submodules:
		globalStreamTable: GlobalStreamTable {
            @display("i=block/table");
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
		rt[156]: StatisticRouter {
				parameters:
				@display("i=abstract/router");
				isTransit = (index==0 || index==1 || index==2 || index==3 || index==4 || index==5 || index==78 || index==79 || index==104 || index==105 || index==106 || index==107 ? true : false);
		}
  connections:
rt[0].out++ --> { delay = 0.55ms ; datarate = 6.815276769036564E8bps; } --> rt[2].in++;
rt[2].out++ --> { delay = 0.55ms ; datarate = 6.815276769036564E8bps; } --> rt[0].in++;
rt[0].out++ --> { delay = 0.01ms ; datarate = 7.396806392170885E8bps; } --> rt[6].in++;
rt[6].out++ --> { delay = 0.01ms ; datarate = 7.396806392170885E8bps; } --> rt[0].in++;
rt[0].out++ --> { delay = 0.42ms ; datarate = 5.674914601689454E7bps; } --> rt[11].in++;
rt[11].out++ --> { delay = 0.42ms ; datarate = 5.674914601689454E7bps; } --> rt[0].in++;
rt[0].out++ --> { delay = 0.34ms ; datarate = 1.9059554300464505E8bps; } --> rt[14].in++;
rt[14].out++ --> { delay = 0.34ms ; datarate = 1.9059554300464505E8bps; } --> rt[0].in++;
rt[0].out++ --> { delay = 0.39ms ; datarate = 6.626096179522302E8bps; } --> rt[19].in++;
rt[19].out++ --> { delay = 0.39ms ; datarate = 6.626096179522302E8bps; } --> rt[0].in++;
rt[0].out++ --> { delay = 0.37ms ; datarate = 3.055020300973818E8bps; } --> rt[35].in++;
rt[35].out++ --> { delay = 0.37ms ; datarate = 3.055020300973818E8bps; } --> rt[0].in++;
rt[0].out++ --> { delay = 0.22ms ; datarate = 7.366025742038646E8bps; } --> rt[105].in++;
rt[105].out++ --> { delay = 0.22ms ; datarate = 7.366025742038646E8bps; } --> rt[0].in++;
rt[1].out++ --> { delay = 0.35ms ; datarate = 1.358686958909359E8bps; } --> rt[22].in++;
rt[22].out++ --> { delay = 0.35ms ; datarate = 1.358686958909359E8bps; } --> rt[1].in++;
rt[1].out++ --> { delay = 0.35ms ; datarate = 7.897827910310513E8bps; } --> rt[5].in++;
rt[5].out++ --> { delay = 0.35ms ; datarate = 7.897827910310513E8bps; } --> rt[1].in++;
rt[1].out++ --> { delay = 0.41ms ; datarate = 9.555873566007029E7bps; } --> rt[4].in++;
rt[4].out++ --> { delay = 0.41ms ; datarate = 9.555873566007029E7bps; } --> rt[1].in++;
rt[1].out++ --> { delay = 0.16ms ; datarate = 9.046972185189947E8bps; } --> rt[27].in++;
rt[27].out++ --> { delay = 0.16ms ; datarate = 9.046972185189947E8bps; } --> rt[1].in++;
rt[1].out++ --> { delay = 0.39ms ; datarate = 3.071304928126895E8bps; } --> rt[31].in++;
rt[31].out++ --> { delay = 0.39ms ; datarate = 3.071304928126895E8bps; } --> rt[1].in++;
rt[1].out++ --> { delay = 0.43ms ; datarate = 4.201785785712573E8bps; } --> rt[35].in++;
rt[35].out++ --> { delay = 0.43ms ; datarate = 4.201785785712573E8bps; } --> rt[1].in++;
rt[2].out++ --> { delay = 0.2ms ; datarate = 8.917841802666233E8bps; } --> rt[41].in++;
rt[41].out++ --> { delay = 0.2ms ; datarate = 8.917841802666233E8bps; } --> rt[2].in++;
rt[2].out++ --> { delay = 0.33ms ; datarate = 4.282172373226103E8bps; } --> rt[43].in++;
rt[43].out++ --> { delay = 0.33ms ; datarate = 4.282172373226103E8bps; } --> rt[2].in++;
rt[2].out++ --> { delay = 0.37ms ; datarate = 2.1921448641072208E8bps; } --> rt[3].in++;
rt[3].out++ --> { delay = 0.37ms ; datarate = 2.1921448641072208E8bps; } --> rt[2].in++;
rt[2].out++ --> { delay = 0.33ms ; datarate = 1.180551016102919E8bps; } --> rt[4].in++;
rt[4].out++ --> { delay = 0.33ms ; datarate = 1.180551016102919E8bps; } --> rt[2].in++;
rt[3].out++ --> { delay = 0.04ms ; datarate = 5.531669403405408E8bps; } --> rt[49].in++;
rt[49].out++ --> { delay = 0.04ms ; datarate = 5.531669403405408E8bps; } --> rt[3].in++;
rt[3].out++ --> { delay = 0.48ms ; datarate = 3.3557353598262453E8bps; } --> rt[4].in++;
rt[4].out++ --> { delay = 0.48ms ; datarate = 3.3557353598262453E8bps; } --> rt[3].in++;
rt[3].out++ --> { delay = 0.41ms ; datarate = 3.1311194050578344E8bps; } --> rt[83].in++;
rt[83].out++ --> { delay = 0.41ms ; datarate = 3.1311194050578344E8bps; } --> rt[3].in++;
rt[3].out++ --> { delay = 0.37ms ; datarate = 2.008244762761233E8bps; } --> rt[60].in++;
rt[60].out++ --> { delay = 0.37ms ; datarate = 2.008244762761233E8bps; } --> rt[3].in++;
rt[4].out++ --> { delay = 0.06ms ; datarate = 7.451615780887772E8bps; } --> rt[55].in++;
rt[55].out++ --> { delay = 0.06ms ; datarate = 7.451615780887772E8bps; } --> rt[4].in++;
rt[4].out++ --> { delay = 0.11ms ; datarate = 1.911609767137245E8bps; } --> rt[61].in++;
rt[61].out++ --> { delay = 0.11ms ; datarate = 1.911609767137245E8bps; } --> rt[4].in++;
rt[4].out++ --> { delay = 0.11ms ; datarate = 6.903722728722874E8bps; } --> rt[5].in++;
rt[5].out++ --> { delay = 0.11ms ; datarate = 6.903722728722874E8bps; } --> rt[4].in++;
rt[4].out++ --> { delay = 0.69ms ; datarate = 5.558397770652115E8bps; } --> rt[18].in++;
rt[18].out++ --> { delay = 0.69ms ; datarate = 5.558397770652115E8bps; } --> rt[4].in++;
rt[4].out++ --> { delay = 0.13ms ; datarate = 4.019291362700778E7bps; } --> rt[51].in++;
rt[51].out++ --> { delay = 0.13ms ; datarate = 4.019291362700778E7bps; } --> rt[4].in++;
rt[4].out++ --> { delay = 0.47ms ; datarate = 7.531555669630804E8bps; } --> rt[106].in++;
rt[106].out++ --> { delay = 0.47ms ; datarate = 7.531555669630804E8bps; } --> rt[4].in++;
rt[4].out++ --> { delay = 0.14ms ; datarate = 5.649227074338037E8bps; } --> rt[124].in++;
rt[124].out++ --> { delay = 0.14ms ; datarate = 5.649227074338037E8bps; } --> rt[4].in++;
rt[5].out++ --> { delay = 0.22ms ; datarate = 5.3224613759214664E8bps; } --> rt[66].in++;
rt[66].out++ --> { delay = 0.22ms ; datarate = 5.3224613759214664E8bps; } --> rt[5].in++;
rt[5].out++ --> { delay = 0.32ms ; datarate = 9.636394770837667E8bps; } --> rt[71].in++;
rt[71].out++ --> { delay = 0.32ms ; datarate = 9.636394770837667E8bps; } --> rt[5].in++;
rt[5].out++ --> { delay = 0.25ms ; datarate = 2.3153158906672502E8bps; } --> rt[74].in++;
rt[74].out++ --> { delay = 0.25ms ; datarate = 2.3153158906672502E8bps; } --> rt[5].in++;
rt[5].out++ --> { delay = 0.39ms ; datarate = 9.415322382840385E7bps; } --> rt[64].in++;
rt[64].out++ --> { delay = 0.39ms ; datarate = 9.415322382840385E7bps; } --> rt[5].in++;
rt[7].out++ --> { delay = 0.38ms ; datarate = 5.384890794792687E8bps; } --> rt[10].in++;
rt[10].out++ --> { delay = 0.38ms ; datarate = 5.384890794792687E8bps; } --> rt[7].in++;
rt[8].out++ --> { delay = 0.51ms ; datarate = 3.948647055338139E8bps; } --> rt[12].in++;
rt[12].out++ --> { delay = 0.51ms ; datarate = 3.948647055338139E8bps; } --> rt[8].in++;
rt[8].out++ --> { delay = 0.01ms ; datarate = 1.7257073922058693E8bps; } --> rt[11].in++;
rt[11].out++ --> { delay = 0.01ms ; datarate = 1.7257073922058693E8bps; } --> rt[8].in++;
rt[8].out++ --> { delay = 0.41ms ; datarate = 6.606716398935751E8bps; } --> rt[10].in++;
rt[10].out++ --> { delay = 0.41ms ; datarate = 6.606716398935751E8bps; } --> rt[8].in++;
rt[9].out++ --> { delay = 0.12ms ; datarate = 3.518162038371009E8bps; } --> rt[10].in++;
rt[10].out++ --> { delay = 0.12ms ; datarate = 3.518162038371009E8bps; } --> rt[9].in++;
rt[11].out++ --> { delay = 0.31ms ; datarate = 3.71300080086345E8bps; } --> rt[54].in++;
rt[54].out++ --> { delay = 0.31ms ; datarate = 3.71300080086345E8bps; } --> rt[11].in++;
rt[13].out++ --> { delay = 0.09ms ; datarate = 2.4176509866522634E8bps; } --> rt[15].in++;
rt[15].out++ --> { delay = 0.09ms ; datarate = 2.4176509866522634E8bps; } --> rt[13].in++;
rt[13].out++ --> { delay = 0.18ms ; datarate = 9.221973667650944E8bps; } --> rt[16].in++;
rt[16].out++ --> { delay = 0.18ms ; datarate = 9.221973667650944E8bps; } --> rt[13].in++;
rt[13].out++ --> { delay = 0.11ms ; datarate = 8.078898116172956E8bps; } --> rt[14].in++;
rt[14].out++ --> { delay = 0.11ms ; datarate = 8.078898116172956E8bps; } --> rt[13].in++;
rt[17].out++ --> { delay = 0.21ms ; datarate = 4.777117257898726E7bps; } --> rt[78].in++;
rt[78].out++ --> { delay = 0.21ms ; datarate = 4.777117257898726E7bps; } --> rt[17].in++;
rt[17].out++ --> { delay = 0.19ms ; datarate = 8.761306048131837E8bps; } --> rt[19].in++;
rt[19].out++ --> { delay = 0.19ms ; datarate = 8.761306048131837E8bps; } --> rt[17].in++;
rt[18].out++ --> { delay = 0.4ms ; datarate = 5.107406019042312E8bps; } --> rt[19].in++;
rt[19].out++ --> { delay = 0.4ms ; datarate = 5.107406019042312E8bps; } --> rt[18].in++;
rt[18].out++ --> { delay = 0.39ms ; datarate = 8.061787743507932E8bps; } --> rt[20].in++;
rt[20].out++ --> { delay = 0.39ms ; datarate = 8.061787743507932E8bps; } --> rt[18].in++;
rt[18].out++ --> { delay = 0.3ms ; datarate = 7.514101616126188E8bps; } --> rt[21].in++;
rt[21].out++ --> { delay = 0.3ms ; datarate = 7.514101616126188E8bps; } --> rt[18].in++;
rt[19].out++ --> { delay = 0.25ms ; datarate = 2.615515227836016E8bps; } --> rt[78].in++;
rt[78].out++ --> { delay = 0.25ms ; datarate = 2.615515227836016E8bps; } --> rt[19].in++;
rt[19].out++ --> { delay = 0.15ms ; datarate = 1.6973573200291365E8bps; } --> rt[21].in++;
rt[21].out++ --> { delay = 0.15ms ; datarate = 1.6973573200291365E8bps; } --> rt[19].in++;
rt[22].out++ --> { delay = 0.28ms ; datarate = 3.8292521678216505E8bps; } --> rt[23].in++;
rt[23].out++ --> { delay = 0.28ms ; datarate = 3.8292521678216505E8bps; } --> rt[22].in++;
rt[22].out++ --> { delay = 0.21ms ; datarate = 6.994400091964056E8bps; } --> rt[26].in++;
rt[26].out++ --> { delay = 0.21ms ; datarate = 6.994400091964056E8bps; } --> rt[22].in++;
rt[23].out++ --> { delay = 0.22ms ; datarate = 5.90980609234972E8bps; } --> rt[25].in++;
rt[25].out++ --> { delay = 0.22ms ; datarate = 5.90980609234972E8bps; } --> rt[23].in++;
rt[24].out++ --> { delay = 0.39ms ; datarate = 2.987981471670442E8bps; } --> rt[25].in++;
rt[25].out++ --> { delay = 0.39ms ; datarate = 2.987981471670442E8bps; } --> rt[24].in++;
rt[27].out++ --> { delay = 0.16ms ; datarate = 8.72376745345079E8bps; } --> rt[29].in++;
rt[29].out++ --> { delay = 0.16ms ; datarate = 8.72376745345079E8bps; } --> rt[27].in++;
rt[27].out++ --> { delay = 0.08ms ; datarate = 7.403092748014405E8bps; } --> rt[28].in++;
rt[28].out++ --> { delay = 0.08ms ; datarate = 7.403092748014405E8bps; } --> rt[27].in++;
rt[30].out++ --> { delay = 0.08ms ; datarate = 9.077046959879419E7bps; } --> rt[31].in++;
rt[31].out++ --> { delay = 0.08ms ; datarate = 9.077046959879419E7bps; } --> rt[30].in++;
rt[30].out++ --> { delay = 0.05ms ; datarate = 4.275750956082587E8bps; } --> rt[34].in++;
rt[34].out++ --> { delay = 0.05ms ; datarate = 4.275750956082587E8bps; } --> rt[30].in++;
rt[32].out++ --> { delay = 0.36ms ; datarate = 5.144985949669128E8bps; } --> rt[34].in++;
rt[34].out++ --> { delay = 0.36ms ; datarate = 5.144985949669128E8bps; } --> rt[32].in++;
rt[33].out++ --> { delay = 0.29ms ; datarate = 5.234819553518031E8bps; } --> rt[34].in++;
rt[34].out++ --> { delay = 0.29ms ; datarate = 5.234819553518031E8bps; } --> rt[33].in++;
rt[35].out++ --> { delay = 0.1ms ; datarate = 5.809597578561503E8bps; } --> rt[36].in++;
rt[36].out++ --> { delay = 0.1ms ; datarate = 5.809597578561503E8bps; } --> rt[35].in++;
rt[36].out++ --> { delay = 0.27ms ; datarate = 9.331765126770493E8bps; } --> rt[37].in++;
rt[37].out++ --> { delay = 0.27ms ; datarate = 9.331765126770493E8bps; } --> rt[36].in++;
rt[38].out++ --> { delay = 0.23ms ; datarate = 7.547854731397784E8bps; } --> rt[41].in++;
rt[41].out++ --> { delay = 0.23ms ; datarate = 7.547854731397784E8bps; } --> rt[38].in++;
rt[39].out++ --> { delay = 0.21ms ; datarate = 7.293328250703926E8bps; } --> rt[40].in++;
rt[40].out++ --> { delay = 0.21ms ; datarate = 7.293328250703926E8bps; } --> rt[39].in++;
rt[40].out++ --> { delay = 0.15ms ; datarate = 5.751602166713163E8bps; } --> rt[41].in++;
rt[41].out++ --> { delay = 0.15ms ; datarate = 5.751602166713163E8bps; } --> rt[40].in++;
rt[42].out++ --> { delay = 0.37ms ; datarate = 1.3604734000967267E8bps; } --> rt[44].in++;
rt[44].out++ --> { delay = 0.37ms ; datarate = 1.3604734000967267E8bps; } --> rt[42].in++;
rt[43].out++ --> { delay = 0.26ms ; datarate = 5.627409511184257E8bps; } --> rt[45].in++;
rt[45].out++ --> { delay = 0.26ms ; datarate = 5.627409511184257E8bps; } --> rt[43].in++;
rt[44].out++ --> { delay = 0.17ms ; datarate = 1.0123798050799731E9bps; } --> rt[45].in++;
rt[45].out++ --> { delay = 0.17ms ; datarate = 1.0123798050799731E9bps; } --> rt[44].in++;
rt[46].out++ --> { delay = 0.24ms ; datarate = 8.122950698406799E8bps; } --> rt[47].in++;
rt[47].out++ --> { delay = 0.24ms ; datarate = 8.122950698406799E8bps; } --> rt[46].in++;
rt[46].out++ --> { delay = 0.23ms ; datarate = 7.233383355268213E8bps; } --> rt[48].in++;
rt[48].out++ --> { delay = 0.23ms ; datarate = 7.233383355268213E8bps; } --> rt[46].in++;
rt[47].out++ --> { delay = 0.06ms ; datarate = 1.0577188588838363E8bps; } --> rt[49].in++;
rt[49].out++ --> { delay = 0.06ms ; datarate = 1.0577188588838363E8bps; } --> rt[47].in++;
rt[48].out++ --> { delay = 0.14ms ; datarate = 8.844050043813648E8bps; } --> rt[144].in++;
rt[144].out++ --> { delay = 0.14ms ; datarate = 8.844050043813648E8bps; } --> rt[48].in++;
rt[50].out++ --> { delay = 0.28ms ; datarate = 1.6627786269655088E8bps; } --> rt[52].in++;
rt[52].out++ --> { delay = 0.28ms ; datarate = 1.6627786269655088E8bps; } --> rt[50].in++;
rt[50].out++ --> { delay = 0.17ms ; datarate = 4.5108864054993445E8bps; } --> rt[53].in++;
rt[53].out++ --> { delay = 0.17ms ; datarate = 4.5108864054993445E8bps; } --> rt[50].in++;
rt[51].out++ --> { delay = 0.28ms ; datarate = 5.15278095519799E8bps; } --> rt[54].in++;
rt[54].out++ --> { delay = 0.28ms ; datarate = 5.15278095519799E8bps; } --> rt[51].in++;
rt[53].out++ --> { delay = 0.08ms ; datarate = 8.360022031510426E8bps; } --> rt[54].in++;
rt[54].out++ --> { delay = 0.08ms ; datarate = 8.360022031510426E8bps; } --> rt[53].in++;
rt[53].out++ --> { delay = 0.49ms ; datarate = 9.202745969300902E8bps; } --> rt[105].in++;
rt[105].out++ --> { delay = 0.49ms ; datarate = 9.202745969300902E8bps; } --> rt[53].in++;
rt[55].out++ --> { delay = 0.26ms ; datarate = 4.561748203688861E8bps; } --> rt[56].in++;
rt[56].out++ --> { delay = 0.26ms ; datarate = 4.561748203688861E8bps; } --> rt[55].in++;
rt[57].out++ --> { delay = 0.21ms ; datarate = 6.148013667818806E8bps; } --> rt[59].in++;
rt[59].out++ --> { delay = 0.21ms ; datarate = 6.148013667818806E8bps; } --> rt[57].in++;
rt[58].out++ --> { delay = 0.69ms ; datarate = 9.493432390765066E8bps; } --> rt[131].in++;
rt[131].out++ --> { delay = 0.69ms ; datarate = 9.493432390765066E8bps; } --> rt[58].in++;
rt[58].out++ --> { delay = 0.29ms ; datarate = 7.082126726181002E7bps; } --> rt[59].in++;
rt[59].out++ --> { delay = 0.29ms ; datarate = 7.082126726181002E7bps; } --> rt[58].in++;
rt[58].out++ --> { delay = 0.28ms ; datarate = 1.7153673362835377E8bps; } --> rt[60].in++;
rt[60].out++ --> { delay = 0.28ms ; datarate = 1.7153673362835377E8bps; } --> rt[58].in++;
rt[60].out++ --> { delay = 0.27ms ; datarate = 3.743466666277392E8bps; } --> rt[78].in++;
rt[78].out++ --> { delay = 0.27ms ; datarate = 3.743466666277392E8bps; } --> rt[60].in++;
rt[60].out++ --> { delay = 0.15ms ; datarate = 9.804306111351366E8bps; } --> rt[61].in++;
rt[61].out++ --> { delay = 0.15ms ; datarate = 9.804306111351366E8bps; } --> rt[60].in++;
rt[62].out++ --> { delay = 0.4ms ; datarate = 5.719939616882563E8bps; } --> rt[63].in++;
rt[63].out++ --> { delay = 0.4ms ; datarate = 5.719939616882563E8bps; } --> rt[62].in++;
rt[62].out++ --> { delay = 0.45ms ; datarate = 5.774456833027645E8bps; } --> rt[78].in++;
rt[78].out++ --> { delay = 0.45ms ; datarate = 5.774456833027645E8bps; } --> rt[62].in++;
rt[63].out++ --> { delay = 0.39ms ; datarate = 3.3925231696523374E8bps; } --> rt[64].in++;
rt[64].out++ --> { delay = 0.39ms ; datarate = 3.3925231696523374E8bps; } --> rt[63].in++;
rt[64].out++ --> { delay = 0.8ms ; datarate = 1.5586243226945817E8bps; } --> rt[106].in++;
rt[106].out++ --> { delay = 0.8ms ; datarate = 1.5586243226945817E8bps; } --> rt[64].in++;
rt[65].out++ --> { delay = 0.25ms ; datarate = 4.4774042850087994E8bps; } --> rt[67].in++;
rt[67].out++ --> { delay = 0.25ms ; datarate = 4.4774042850087994E8bps; } --> rt[65].in++;
rt[66].out++ --> { delay = 0.35ms ; datarate = 4.6798212601998013E8bps; } --> rt[68].in++;
rt[68].out++ --> { delay = 0.35ms ; datarate = 4.6798212601998013E8bps; } --> rt[66].in++;
rt[67].out++ --> { delay = 0.33ms ; datarate = 7.374995944357929E8bps; } --> rt[68].in++;
rt[68].out++ --> { delay = 0.33ms ; datarate = 7.374995944357929E8bps; } --> rt[67].in++;
rt[68].out++ --> { delay = 0.48ms ; datarate = 5.443032906199548E8bps; } --> rt[69].in++;
rt[69].out++ --> { delay = 0.48ms ; datarate = 5.443032906199548E8bps; } --> rt[68].in++;
rt[70].out++ --> { delay = 0.22ms ; datarate = 3.221003218309229E8bps; } --> rt[71].in++;
rt[71].out++ --> { delay = 0.22ms ; datarate = 3.221003218309229E8bps; } --> rt[70].in++;
rt[71].out++ --> { delay = 0.2ms ; datarate = 4.548597309683705E8bps; } --> rt[123].in++;
rt[123].out++ --> { delay = 0.2ms ; datarate = 4.548597309683705E8bps; } --> rt[71].in++;
rt[72].out++ --> { delay = 0.22ms ; datarate = 7.64512631952142E8bps; } --> rt[74].in++;
rt[74].out++ --> { delay = 0.22ms ; datarate = 7.64512631952142E8bps; } --> rt[72].in++;
rt[72].out++ --> { delay = 0.11ms ; datarate = 5.495913759399525E7bps; } --> rt[77].in++;
rt[77].out++ --> { delay = 0.11ms ; datarate = 5.495913759399525E7bps; } --> rt[72].in++;
rt[73].out++ --> { delay = 0.06ms ; datarate = 9.878407264829754E8bps; } --> rt[74].in++;
rt[74].out++ --> { delay = 0.06ms ; datarate = 9.878407264829754E8bps; } --> rt[73].in++;
rt[73].out++ --> { delay = 0.09ms ; datarate = 5.7286460495066E8bps; } --> rt[75].in++;
rt[75].out++ --> { delay = 0.09ms ; datarate = 5.7286460495066E8bps; } --> rt[73].in++;
rt[74].out++ --> { delay = 0.09ms ; datarate = 7.153856260278542E8bps; } --> rt[76].in++;
rt[76].out++ --> { delay = 0.09ms ; datarate = 7.153856260278542E8bps; } --> rt[74].in++;
rt[75].out++ --> { delay = 0.2ms ; datarate = 1.864409754466692E7bps; } --> rt[77].in++;
rt[77].out++ --> { delay = 0.2ms ; datarate = 1.864409754466692E7bps; } --> rt[75].in++;
rt[78].out++ --> { delay = 0.27ms ; datarate = 6.655982541394387E8bps; } --> rt[88].in++;
rt[88].out++ --> { delay = 0.27ms ; datarate = 6.655982541394387E8bps; } --> rt[78].in++;
rt[78].out++ --> { delay = 0.24ms ; datarate = 7.464413895758055E8bps; } --> rt[86].in++;
rt[86].out++ --> { delay = 0.24ms ; datarate = 7.464413895758055E8bps; } --> rt[78].in++;
rt[78].out++ --> { delay = 0.23ms ; datarate = 3.1733159964081764E8bps; } --> rt[79].in++;
rt[79].out++ --> { delay = 0.23ms ; datarate = 3.1733159964081764E8bps; } --> rt[78].in++;
rt[78].out++ --> { delay = 0.22ms ; datarate = 7.971468946269957E8bps; } --> rt[80].in++;
rt[80].out++ --> { delay = 0.22ms ; datarate = 7.971468946269957E8bps; } --> rt[78].in++;
rt[79].out++ --> { delay = 0.17ms ; datarate = 6.071466890983144E8bps; } --> rt[103].in++;
rt[103].out++ --> { delay = 0.17ms ; datarate = 6.071466890983144E8bps; } --> rt[79].in++;
rt[79].out++ --> { delay = 0.38ms ; datarate = 1.2017995934982423E8bps; } --> rt[93].in++;
rt[93].out++ --> { delay = 0.38ms ; datarate = 1.2017995934982423E8bps; } --> rt[79].in++;
rt[79].out++ --> { delay = 0.25ms ; datarate = 9.073225969692771E8bps; } --> rt[92].in++;
rt[92].out++ --> { delay = 0.25ms ; datarate = 9.073225969692771E8bps; } --> rt[79].in++;
rt[79].out++ --> { delay = 0.63ms ; datarate = 3.324525037723139E8bps; } --> rt[106].in++;
rt[106].out++ --> { delay = 0.63ms ; datarate = 3.324525037723139E8bps; } --> rt[79].in++;
rt[79].out++ --> { delay = 0.56ms ; datarate = 9.297814364675995E8bps; } --> rt[105].in++;
rt[105].out++ --> { delay = 0.56ms ; datarate = 9.297814364675995E8bps; } --> rt[79].in++;
rt[80].out++ --> { delay = 0.08ms ; datarate = 3.0457768362437266E8bps; } --> rt[85].in++;
rt[85].out++ --> { delay = 0.08ms ; datarate = 3.0457768362437266E8bps; } --> rt[80].in++;
rt[80].out++ --> { delay = 0.27ms ; datarate = 5.878839487165709E8bps; } --> rt[84].in++;
rt[84].out++ --> { delay = 0.27ms ; datarate = 5.878839487165709E8bps; } --> rt[80].in++;
rt[81].out++ --> { delay = 0.28ms ; datarate = 2.0491240405679968E8bps; } --> rt[85].in++;
rt[85].out++ --> { delay = 0.28ms ; datarate = 2.0491240405679968E8bps; } --> rt[81].in++;
rt[81].out++ --> { delay = 0.03ms ; datarate = 8.329064875723038E8bps; } --> rt[82].in++;
rt[82].out++ --> { delay = 0.03ms ; datarate = 8.329064875723038E8bps; } --> rt[81].in++;
rt[81].out++ --> { delay = 0.35ms ; datarate = 5.981137265775187E8bps; } --> rt[83].in++;
rt[83].out++ --> { delay = 0.35ms ; datarate = 5.981137265775187E8bps; } --> rt[81].in++;
rt[82].out++ --> { delay = 0.26ms ; datarate = 9.992438028653862E7bps; } --> rt[85].in++;
rt[85].out++ --> { delay = 0.26ms ; datarate = 9.992438028653862E7bps; } --> rt[82].in++;
rt[84].out++ --> { delay = 0.77ms ; datarate = 1.4274485183901561E7bps; } --> rt[146].in++;
rt[146].out++ --> { delay = 0.77ms ; datarate = 1.4274485183901561E7bps; } --> rt[84].in++;
rt[86].out++ --> { delay = 0.26ms ; datarate = 8.268318131131428E8bps; } --> rt[87].in++;
rt[87].out++ --> { delay = 0.26ms ; datarate = 8.268318131131428E8bps; } --> rt[86].in++;
rt[88].out++ --> { delay = 0.24ms ; datarate = 1.826271077655224E7bps; } --> rt[91].in++;
rt[91].out++ --> { delay = 0.24ms ; datarate = 1.826271077655224E7bps; } --> rt[88].in++;
rt[89].out++ --> { delay = 0.33ms ; datarate = 5.590132451641178E8bps; } --> rt[90].in++;
rt[90].out++ --> { delay = 0.33ms ; datarate = 5.590132451641178E8bps; } --> rt[89].in++;
rt[89].out++ --> { delay = 0.26ms ; datarate = 6.0178867765425E8bps; } --> rt[91].in++;
rt[91].out++ --> { delay = 0.26ms ; datarate = 6.0178867765425E8bps; } --> rt[89].in++;
rt[92].out++ --> { delay = 0.64ms ; datarate = 5.2523577388895726E8bps; } --> rt[106].in++;
rt[106].out++ --> { delay = 0.64ms ; datarate = 5.2523577388895726E8bps; } --> rt[92].in++;
rt[93].out++ --> { delay = 0.31ms ; datarate = 5.426258974022827E8bps; } --> rt[99].in++;
rt[99].out++ --> { delay = 0.31ms ; datarate = 5.426258974022827E8bps; } --> rt[93].in++;
rt[93].out++ --> { delay = 0.33ms ; datarate = 5.998361091541778E8bps; } --> rt[96].in++;
rt[96].out++ --> { delay = 0.33ms ; datarate = 5.998361091541778E8bps; } --> rt[93].in++;
rt[94].out++ --> { delay = 0.23ms ; datarate = 7.236640832259208E8bps; } --> rt[97].in++;
rt[97].out++ --> { delay = 0.23ms ; datarate = 7.236640832259208E8bps; } --> rt[94].in++;
rt[94].out++ --> { delay = 0.27ms ; datarate = 1.9997402745685634E8bps; } --> rt[96].in++;
rt[96].out++ --> { delay = 0.27ms ; datarate = 1.9997402745685634E8bps; } --> rt[94].in++;
rt[95].out++ --> { delay = 0.32ms ; datarate = 5.16310543808723E8bps; } --> rt[97].in++;
rt[97].out++ --> { delay = 0.32ms ; datarate = 5.16310543808723E8bps; } --> rt[95].in++;
rt[96].out++ --> { delay = 0.34ms ; datarate = 7.346684899311401E8bps; } --> rt[99].in++;
rt[99].out++ --> { delay = 0.34ms ; datarate = 7.346684899311401E8bps; } --> rt[96].in++;
rt[97].out++ --> { delay = 0.27ms ; datarate = 2.850000540607654E8bps; } --> rt[98].in++;
rt[98].out++ --> { delay = 0.27ms ; datarate = 2.850000540607654E8bps; } --> rt[97].in++;
rt[100].out++ --> { delay = 0.12ms ; datarate = 7.316128670701712E8bps; } --> rt[101].in++;
rt[101].out++ --> { delay = 0.12ms ; datarate = 7.316128670701712E8bps; } --> rt[100].in++;
rt[101].out++ --> { delay = 0.15ms ; datarate = 9.265067858067402E7bps; } --> rt[102].in++;
rt[102].out++ --> { delay = 0.15ms ; datarate = 9.265067858067402E7bps; } --> rt[101].in++;
rt[102].out++ --> { delay = 0.21ms ; datarate = 3.29527851327612E8bps; } --> rt[103].in++;
rt[103].out++ --> { delay = 0.21ms ; datarate = 3.29527851327612E8bps; } --> rt[102].in++;
rt[104].out++ --> { delay = 0.1ms ; datarate = 6.795927014649636E7bps; } --> rt[118].in++;
rt[118].out++ --> { delay = 0.1ms ; datarate = 6.795927014649636E7bps; } --> rt[104].in++;
rt[104].out++ --> { delay = 0.2ms ; datarate = 2.2698808420176783E8bps; } --> rt[121].in++;
rt[121].out++ --> { delay = 0.2ms ; datarate = 2.2698808420176783E8bps; } --> rt[104].in++;
rt[104].out++ --> { delay = 0.06ms ; datarate = 1.1989341296930434E8bps; } --> rt[107].in++;
rt[107].out++ --> { delay = 0.06ms ; datarate = 1.1989341296930434E8bps; } --> rt[104].in++;
rt[104].out++ --> { delay = 0.34ms ; datarate = 4.240807599152455E8bps; } --> rt[108].in++;
rt[108].out++ --> { delay = 0.34ms ; datarate = 4.240807599152455E8bps; } --> rt[104].in++;
rt[104].out++ --> { delay = 0.44ms ; datarate = 5.470457931677161E8bps; } --> rt[114].in++;
rt[114].out++ --> { delay = 0.44ms ; datarate = 5.470457931677161E8bps; } --> rt[104].in++;
rt[105].out++ --> { delay = 0.22ms ; datarate = 3.948574106645731E7bps; } --> rt[107].in++;
rt[107].out++ --> { delay = 0.22ms ; datarate = 3.948574106645731E7bps; } --> rt[105].in++;
rt[105].out++ --> { delay = 0.35ms ; datarate = 7.929147642506113E8bps; } --> rt[127].in++;
rt[127].out++ --> { delay = 0.35ms ; datarate = 7.929147642506113E8bps; } --> rt[105].in++;
rt[105].out++ --> { delay = 0.21ms ; datarate = 8.070122721021278E8bps; } --> rt[129].in++;
rt[129].out++ --> { delay = 0.21ms ; datarate = 8.070122721021278E8bps; } --> rt[105].in++;
rt[105].out++ --> { delay = 0.19ms ; datarate = 7.354443895442734E8bps; } --> rt[135].in++;
rt[135].out++ --> { delay = 0.19ms ; datarate = 7.354443895442734E8bps; } --> rt[105].in++;
rt[106].out++ --> { delay = 0.43ms ; datarate = 2.957070442669078E8bps; } --> rt[144].in++;
rt[144].out++ --> { delay = 0.43ms ; datarate = 2.957070442669078E8bps; } --> rt[106].in++;
rt[106].out++ --> { delay = 0.05ms ; datarate = 6.295110875866902E8bps; } --> rt[146].in++;
rt[146].out++ --> { delay = 0.05ms ; datarate = 6.295110875866902E8bps; } --> rt[106].in++;
rt[106].out++ --> { delay = 0.39ms ; datarate = 9.830862640696723E8bps; } --> rt[139].in++;
rt[139].out++ --> { delay = 0.39ms ; datarate = 9.830862640696723E8bps; } --> rt[106].in++;
rt[106].out++ --> { delay = 0.26ms ; datarate = 7.46378658668106E8bps; } --> rt[107].in++;
rt[107].out++ --> { delay = 0.26ms ; datarate = 7.46378658668106E8bps; } --> rt[106].in++;
rt[107].out++ --> { delay = 0.43ms ; datarate = 1.5073206637349665E8bps; } --> rt[149].in++;
rt[149].out++ --> { delay = 0.43ms ; datarate = 1.5073206637349665E8bps; } --> rt[107].in++;
rt[107].out++ --> { delay = 0.11ms ; datarate = 8.197632201327541E8bps; } --> rt[155].in++;
rt[155].out++ --> { delay = 0.11ms ; datarate = 8.197632201327541E8bps; } --> rt[107].in++;
rt[108].out++ --> { delay = 0.2ms ; datarate = 4.958329326087329E8bps; } --> rt[109].in++;
rt[109].out++ --> { delay = 0.2ms ; datarate = 4.958329326087329E8bps; } --> rt[108].in++;
rt[108].out++ --> { delay = 0.09ms ; datarate = 8.00024341366887E8bps; } --> rt[110].in++;
rt[110].out++ --> { delay = 0.09ms ; datarate = 8.00024341366887E8bps; } --> rt[108].in++;
rt[108].out++ --> { delay = 0.08ms ; datarate = 1.4859227311985819E7bps; } --> rt[111].in++;
rt[111].out++ --> { delay = 0.08ms ; datarate = 1.4859227311985819E7bps; } --> rt[108].in++;
rt[108].out++ --> { delay = 0.09ms ; datarate = 6.03170379686761E8bps; } --> rt[112].in++;
rt[112].out++ --> { delay = 0.09ms ; datarate = 6.03170379686761E8bps; } --> rt[108].in++;
rt[109].out++ --> { delay = 0.43ms ; datarate = 2.1706318547263572E8bps; } --> rt[152].in++;
rt[152].out++ --> { delay = 0.43ms ; datarate = 2.1706318547263572E8bps; } --> rt[109].in++;
rt[109].out++ --> { delay = 0.14ms ; datarate = 5.504954121363193E8bps; } --> rt[110].in++;
rt[110].out++ --> { delay = 0.14ms ; datarate = 5.504954121363193E8bps; } --> rt[109].in++;
rt[113].out++ --> { delay = 0.21ms ; datarate = 1.1985981126262687E7bps; } --> rt[114].in++;
rt[114].out++ --> { delay = 0.21ms ; datarate = 1.1985981126262687E7bps; } --> rt[113].in++;
rt[115].out++ --> { delay = 0.3ms ; datarate = 5.495890723062046E8bps; } --> rt[119].in++;
rt[119].out++ --> { delay = 0.3ms ; datarate = 5.495890723062046E8bps; } --> rt[115].in++;
rt[116].out++ --> { delay = 0.34ms ; datarate = 1.4445834932228762E8bps; } --> rt[119].in++;
rt[119].out++ --> { delay = 0.34ms ; datarate = 1.4445834932228762E8bps; } --> rt[116].in++;
rt[116].out++ --> { delay = 0.14ms ; datarate = 6.439507872325095E8bps; } --> rt[118].in++;
rt[118].out++ --> { delay = 0.14ms ; datarate = 6.439507872325095E8bps; } --> rt[116].in++;
rt[117].out++ --> { delay = 0.2ms ; datarate = 5.6347586956334114E7bps; } --> rt[118].in++;
rt[118].out++ --> { delay = 0.2ms ; datarate = 5.6347586956334114E7bps; } --> rt[117].in++;
rt[117].out++ --> { delay = 0.35ms ; datarate = 4.688719485458797E8bps; } --> rt[127].in++;
rt[127].out++ --> { delay = 0.35ms ; datarate = 4.688719485458797E8bps; } --> rt[117].in++;
rt[120].out++ --> { delay = 0.15ms ; datarate = 7.303137311529944E8bps; } --> rt[123].in++;
rt[123].out++ --> { delay = 0.15ms ; datarate = 7.303137311529944E8bps; } --> rt[120].in++;
rt[120].out++ --> { delay = 0.11ms ; datarate = 7.830361199807928E8bps; } --> rt[121].in++;
rt[121].out++ --> { delay = 0.11ms ; datarate = 7.830361199807928E8bps; } --> rt[120].in++;
rt[122].out++ --> { delay = 0.2ms ; datarate = 1.521551899770519E8bps; } --> rt[123].in++;
rt[123].out++ --> { delay = 0.2ms ; datarate = 1.521551899770519E8bps; } --> rt[122].in++;
rt[124].out++ --> { delay = 0.38ms ; datarate = 4.604491691381634E7bps; } --> rt[126].in++;
rt[126].out++ --> { delay = 0.38ms ; datarate = 4.604491691381634E7bps; } --> rt[124].in++;
rt[125].out++ --> { delay = 0.15ms ; datarate = 5.852210494582641E8bps; } --> rt[127].in++;
rt[127].out++ --> { delay = 0.15ms ; datarate = 5.852210494582641E8bps; } --> rt[125].in++;
rt[126].out++ --> { delay = 0.39ms ; datarate = 6.001670248348191E8bps; } --> rt[128].in++;
rt[128].out++ --> { delay = 0.39ms ; datarate = 6.001670248348191E8bps; } --> rt[126].in++;
rt[126].out++ --> { delay = 0.32ms ; datarate = 3.894945583401933E8bps; } --> rt[127].in++;
rt[127].out++ --> { delay = 0.32ms ; datarate = 3.894945583401933E8bps; } --> rt[126].in++;
rt[127].out++ --> { delay = 0.14ms ; datarate = 7.782262229651552E8bps; } --> rt[128].in++;
rt[128].out++ --> { delay = 0.14ms ; datarate = 7.782262229651552E8bps; } --> rt[127].in++;
rt[129].out++ --> { delay = 0.16ms ; datarate = 8.557051795009238E7bps; } --> rt[130].in++;
rt[130].out++ --> { delay = 0.16ms ; datarate = 8.557051795009238E7bps; } --> rt[129].in++;
rt[129].out++ --> { delay = 0.18ms ; datarate = 1.8698230208487144E8bps; } --> rt[131].in++;
rt[131].out++ --> { delay = 0.18ms ; datarate = 1.8698230208487144E8bps; } --> rt[129].in++;
rt[132].out++ --> { delay = 0.38ms ; datarate = 4.101741819130501E8bps; } --> rt[135].in++;
rt[135].out++ --> { delay = 0.38ms ; datarate = 4.101741819130501E8bps; } --> rt[132].in++;
rt[132].out++ --> { delay = 0.09ms ; datarate = 7.168222910725421E8bps; } --> rt[134].in++;
rt[134].out++ --> { delay = 0.09ms ; datarate = 7.168222910725421E8bps; } --> rt[132].in++;
rt[133].out++ --> { delay = 0.09ms ; datarate = 9.873839226872977E8bps; } --> rt[135].in++;
rt[135].out++ --> { delay = 0.09ms ; datarate = 9.873839226872977E8bps; } --> rt[133].in++;
rt[136].out++ --> { delay = 0.39ms ; datarate = 4.758998258678986E8bps; } --> rt[137].in++;
rt[137].out++ --> { delay = 0.39ms ; datarate = 4.758998258678986E8bps; } --> rt[136].in++;
rt[137].out++ --> { delay = 0.04ms ; datarate = 7.075179697412851E8bps; } --> rt[140].in++;
rt[140].out++ --> { delay = 0.04ms ; datarate = 7.075179697412851E8bps; } --> rt[137].in++;
rt[137].out++ --> { delay = 0.35ms ; datarate = 3.675400607799795E8bps; } --> rt[138].in++;
rt[138].out++ --> { delay = 0.35ms ; datarate = 3.675400607799795E8bps; } --> rt[137].in++;
rt[139].out++ --> { delay = 0.14ms ; datarate = 8.338216519394741E8bps; } --> rt[140].in++;
rt[140].out++ --> { delay = 0.14ms ; datarate = 8.338216519394741E8bps; } --> rt[139].in++;
rt[141].out++ --> { delay = 0.07ms ; datarate = 9.968253608568937E8bps; } --> rt[144].in++;
rt[144].out++ --> { delay = 0.07ms ; datarate = 9.968253608568937E8bps; } --> rt[141].in++;
rt[142].out++ --> { delay = 0.24ms ; datarate = 9.026773424553094E8bps; } --> rt[144].in++;
rt[144].out++ --> { delay = 0.24ms ; datarate = 9.026773424553094E8bps; } --> rt[142].in++;
rt[143].out++ --> { delay = 0.17ms ; datarate = 6.500163733926264E8bps; } --> rt[144].in++;
rt[144].out++ --> { delay = 0.17ms ; datarate = 6.500163733926264E8bps; } --> rt[143].in++;
rt[145].out++ --> { delay = 0.4ms ; datarate = 7.058646759360738E8bps; } --> rt[147].in++;
rt[147].out++ --> { delay = 0.4ms ; datarate = 7.058646759360738E8bps; } --> rt[145].in++;
rt[146].out++ --> { delay = 0.38ms ; datarate = 9.35379491311771E8bps; } --> rt[147].in++;
rt[147].out++ --> { delay = 0.38ms ; datarate = 9.35379491311771E8bps; } --> rt[146].in++;
rt[148].out++ --> { delay = 0.21ms ; datarate = 8.752048131254585E8bps; } --> rt[149].in++;
rt[149].out++ --> { delay = 0.21ms ; datarate = 8.752048131254585E8bps; } --> rt[148].in++;
rt[150].out++ --> { delay = 0.34ms ; datarate = 5.2788077770923984E8bps; } --> rt[151].in++;
rt[151].out++ --> { delay = 0.34ms ; datarate = 5.2788077770923984E8bps; } --> rt[150].in++;
rt[151].out++ --> { delay = 0.15ms ; datarate = 6.849361947974372E8bps; } --> rt[153].in++;
rt[153].out++ --> { delay = 0.15ms ; datarate = 6.849361947974372E8bps; } --> rt[151].in++;
rt[151].out++ --> { delay = 0.18ms ; datarate = 6.081879630294397E8bps; } --> rt[152].in++;
rt[152].out++ --> { delay = 0.18ms ; datarate = 6.081879630294397E8bps; } --> rt[151].in++;
rt[151].out++ --> { delay = 0.36ms ; datarate = 7.246000422317615E7bps; } --> rt[154].in++;
rt[154].out++ --> { delay = 0.36ms ; datarate = 7.246000422317615E7bps; } --> rt[151].in++;
rt[152].out++ --> { delay = 0.17ms ; datarate = 4.785633758570045E8bps; } --> rt[155].in++;
rt[155].out++ --> { delay = 0.17ms ; datarate = 4.785633758570045E8bps; } --> rt[152].in++;
rt[152].out++ --> { delay = 0.29ms ; datarate = 8.714930012031215E8bps; } --> rt[153].in++;
rt[153].out++ --> { delay = 0.29ms ; datarate = 8.714930012031215E8bps; } --> rt[152].in++;
}

//
network GTITM_TS_IPv4Network_ATT extends GTITM_TS_IPv4Underlay_ATT
{
	parameters:
}

