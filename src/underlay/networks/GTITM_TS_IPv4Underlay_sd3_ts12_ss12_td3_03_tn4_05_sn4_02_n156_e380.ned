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
module GTITM_TS_IPv4Underlay_sd3_ts12_ss12_td3_03_tn4_05_sn4_02_n156_e380 
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
		rt[156]: StatisticRouter {
				parameters:
				@display("i=abstract/router");
				isTransit = (index==0 || index==1 || index==2 || index==3 || index==4 || index==5 || index==78 || index==79 || index==104 || index==105 || index==106 || index==107 ? true : false);
		}
  connections:
rt[0].out++ --> { delay = 0.55ms ; datarate = 6.146283172664899E8bps; } --> rt[2].in++;
rt[2].out++ --> { delay = 0.55ms ; datarate = 6.146283172664899E8bps; } --> rt[0].in++;
rt[0].out++ --> { delay = 0.01ms ; datarate = 1.0127722794364535E9bps; } --> rt[6].in++;
rt[6].out++ --> { delay = 0.01ms ; datarate = 1.0127722794364535E9bps; } --> rt[0].in++;
rt[0].out++ --> { delay = 0.42ms ; datarate = 7.659412725883111E8bps; } --> rt[11].in++;
rt[11].out++ --> { delay = 0.42ms ; datarate = 7.659412725883111E8bps; } --> rt[0].in++;
rt[0].out++ --> { delay = 0.34ms ; datarate = 4.0930037052439547E8bps; } --> rt[14].in++;
rt[14].out++ --> { delay = 0.34ms ; datarate = 4.0930037052439547E8bps; } --> rt[0].in++;
rt[0].out++ --> { delay = 0.39ms ; datarate = 5.1304101816166973E8bps; } --> rt[19].in++;
rt[19].out++ --> { delay = 0.39ms ; datarate = 5.1304101816166973E8bps; } --> rt[0].in++;
rt[0].out++ --> { delay = 0.37ms ; datarate = 5.327241035970487E8bps; } --> rt[35].in++;
rt[35].out++ --> { delay = 0.37ms ; datarate = 5.327241035970487E8bps; } --> rt[0].in++;
rt[0].out++ --> { delay = 0.22ms ; datarate = 5.436829944767674E8bps; } --> rt[105].in++;
rt[105].out++ --> { delay = 0.22ms ; datarate = 5.436829944767674E8bps; } --> rt[0].in++;
rt[1].out++ --> { delay = 0.35ms ; datarate = 1.0019954998431486E9bps; } --> rt[22].in++;
rt[22].out++ --> { delay = 0.35ms ; datarate = 1.0019954998431486E9bps; } --> rt[1].in++;
rt[1].out++ --> { delay = 0.35ms ; datarate = 5.945640397424593E8bps; } --> rt[5].in++;
rt[5].out++ --> { delay = 0.35ms ; datarate = 5.945640397424593E8bps; } --> rt[1].in++;
rt[1].out++ --> { delay = 0.41ms ; datarate = 1.0055404666066382E9bps; } --> rt[4].in++;
rt[4].out++ --> { delay = 0.41ms ; datarate = 1.0055404666066382E9bps; } --> rt[1].in++;
rt[1].out++ --> { delay = 0.16ms ; datarate = 1.792555131044105E7bps; } --> rt[27].in++;
rt[27].out++ --> { delay = 0.16ms ; datarate = 1.792555131044105E7bps; } --> rt[1].in++;
rt[1].out++ --> { delay = 0.39ms ; datarate = 7.518385131818849E8bps; } --> rt[31].in++;
rt[31].out++ --> { delay = 0.39ms ; datarate = 7.518385131818849E8bps; } --> rt[1].in++;
rt[1].out++ --> { delay = 0.43ms ; datarate = 7.628491077108641E8bps; } --> rt[35].in++;
rt[35].out++ --> { delay = 0.43ms ; datarate = 7.628491077108641E8bps; } --> rt[1].in++;
rt[2].out++ --> { delay = 0.2ms ; datarate = 9.26375021638721E8bps; } --> rt[41].in++;
rt[41].out++ --> { delay = 0.2ms ; datarate = 9.26375021638721E8bps; } --> rt[2].in++;
rt[2].out++ --> { delay = 0.33ms ; datarate = 4.9364171636731285E8bps; } --> rt[43].in++;
rt[43].out++ --> { delay = 0.33ms ; datarate = 4.9364171636731285E8bps; } --> rt[2].in++;
rt[2].out++ --> { delay = 0.37ms ; datarate = 6.375205866139511E8bps; } --> rt[3].in++;
rt[3].out++ --> { delay = 0.37ms ; datarate = 6.375205866139511E8bps; } --> rt[2].in++;
rt[2].out++ --> { delay = 0.33ms ; datarate = 9.442389132902261E8bps; } --> rt[4].in++;
rt[4].out++ --> { delay = 0.33ms ; datarate = 9.442389132902261E8bps; } --> rt[2].in++;
rt[3].out++ --> { delay = 0.04ms ; datarate = 9.188194297696207E8bps; } --> rt[49].in++;
rt[49].out++ --> { delay = 0.04ms ; datarate = 9.188194297696207E8bps; } --> rt[3].in++;
rt[3].out++ --> { delay = 0.48ms ; datarate = 7.593818787250745E7bps; } --> rt[4].in++;
rt[4].out++ --> { delay = 0.48ms ; datarate = 7.593818787250745E7bps; } --> rt[3].in++;
rt[3].out++ --> { delay = 0.41ms ; datarate = 4.8460418811407524E8bps; } --> rt[83].in++;
rt[83].out++ --> { delay = 0.41ms ; datarate = 4.8460418811407524E8bps; } --> rt[3].in++;
rt[3].out++ --> { delay = 0.37ms ; datarate = 3.625362541679374E7bps; } --> rt[60].in++;
rt[60].out++ --> { delay = 0.37ms ; datarate = 3.625362541679374E7bps; } --> rt[3].in++;
rt[4].out++ --> { delay = 0.06ms ; datarate = 7.224960113437759E8bps; } --> rt[55].in++;
rt[55].out++ --> { delay = 0.06ms ; datarate = 7.224960113437759E8bps; } --> rt[4].in++;
rt[4].out++ --> { delay = 0.11ms ; datarate = 7.04392045178258E8bps; } --> rt[61].in++;
rt[61].out++ --> { delay = 0.11ms ; datarate = 7.04392045178258E8bps; } --> rt[4].in++;
rt[4].out++ --> { delay = 0.11ms ; datarate = 9.101233355992512E8bps; } --> rt[5].in++;
rt[5].out++ --> { delay = 0.11ms ; datarate = 9.101233355992512E8bps; } --> rt[4].in++;
rt[4].out++ --> { delay = 0.69ms ; datarate = 3.570891334854855E7bps; } --> rt[18].in++;
rt[18].out++ --> { delay = 0.69ms ; datarate = 3.570891334854855E7bps; } --> rt[4].in++;
rt[4].out++ --> { delay = 0.13ms ; datarate = 8.357088702819408E7bps; } --> rt[51].in++;
rt[51].out++ --> { delay = 0.13ms ; datarate = 8.357088702819408E7bps; } --> rt[4].in++;
rt[4].out++ --> { delay = 0.47ms ; datarate = 3.3301203831603163E8bps; } --> rt[106].in++;
rt[106].out++ --> { delay = 0.47ms ; datarate = 3.3301203831603163E8bps; } --> rt[4].in++;
rt[4].out++ --> { delay = 0.14ms ; datarate = 7.922662006114987E8bps; } --> rt[124].in++;
rt[124].out++ --> { delay = 0.14ms ; datarate = 7.922662006114987E8bps; } --> rt[4].in++;
rt[5].out++ --> { delay = 0.22ms ; datarate = 1.162459704992192E8bps; } --> rt[66].in++;
rt[66].out++ --> { delay = 0.22ms ; datarate = 1.162459704992192E8bps; } --> rt[5].in++;
rt[5].out++ --> { delay = 0.32ms ; datarate = 6.498007977584777E8bps; } --> rt[71].in++;
rt[71].out++ --> { delay = 0.32ms ; datarate = 6.498007977584777E8bps; } --> rt[5].in++;
rt[5].out++ --> { delay = 0.25ms ; datarate = 1.2530552108532743E8bps; } --> rt[74].in++;
rt[74].out++ --> { delay = 0.25ms ; datarate = 1.2530552108532743E8bps; } --> rt[5].in++;
rt[5].out++ --> { delay = 0.39ms ; datarate = 7.301692134500754E8bps; } --> rt[64].in++;
rt[64].out++ --> { delay = 0.39ms ; datarate = 7.301692134500754E8bps; } --> rt[5].in++;
rt[7].out++ --> { delay = 0.38ms ; datarate = 9.837204320781097E7bps; } --> rt[10].in++;
rt[10].out++ --> { delay = 0.38ms ; datarate = 9.837204320781097E7bps; } --> rt[7].in++;
rt[8].out++ --> { delay = 0.51ms ; datarate = 4.66467223532387E8bps; } --> rt[12].in++;
rt[12].out++ --> { delay = 0.51ms ; datarate = 4.66467223532387E8bps; } --> rt[8].in++;
rt[8].out++ --> { delay = 0.01ms ; datarate = 9.87059797957968E7bps; } --> rt[11].in++;
rt[11].out++ --> { delay = 0.01ms ; datarate = 9.87059797957968E7bps; } --> rt[8].in++;
rt[8].out++ --> { delay = 0.41ms ; datarate = 6.068639682648492E8bps; } --> rt[10].in++;
rt[10].out++ --> { delay = 0.41ms ; datarate = 6.068639682648492E8bps; } --> rt[8].in++;
rt[9].out++ --> { delay = 0.12ms ; datarate = 1.4746943637483424E8bps; } --> rt[10].in++;
rt[10].out++ --> { delay = 0.12ms ; datarate = 1.4746943637483424E8bps; } --> rt[9].in++;
rt[11].out++ --> { delay = 0.31ms ; datarate = 5.379035906009154E8bps; } --> rt[54].in++;
rt[54].out++ --> { delay = 0.31ms ; datarate = 5.379035906009154E8bps; } --> rt[11].in++;
rt[13].out++ --> { delay = 0.09ms ; datarate = 2.9255448813734674E8bps; } --> rt[15].in++;
rt[15].out++ --> { delay = 0.09ms ; datarate = 2.9255448813734674E8bps; } --> rt[13].in++;
rt[13].out++ --> { delay = 0.18ms ; datarate = 6.283304223201809E8bps; } --> rt[16].in++;
rt[16].out++ --> { delay = 0.18ms ; datarate = 6.283304223201809E8bps; } --> rt[13].in++;
rt[13].out++ --> { delay = 0.11ms ; datarate = 1.447697658024981E8bps; } --> rt[14].in++;
rt[14].out++ --> { delay = 0.11ms ; datarate = 1.447697658024981E8bps; } --> rt[13].in++;
rt[17].out++ --> { delay = 0.21ms ; datarate = 7.161104698141441E8bps; } --> rt[78].in++;
rt[78].out++ --> { delay = 0.21ms ; datarate = 7.161104698141441E8bps; } --> rt[17].in++;
rt[17].out++ --> { delay = 0.19ms ; datarate = 3.953185944178404E7bps; } --> rt[19].in++;
rt[19].out++ --> { delay = 0.19ms ; datarate = 3.953185944178404E7bps; } --> rt[17].in++;
rt[18].out++ --> { delay = 0.4ms ; datarate = 1.1770892825160217E8bps; } --> rt[19].in++;
rt[19].out++ --> { delay = 0.4ms ; datarate = 1.1770892825160217E8bps; } --> rt[18].in++;
rt[18].out++ --> { delay = 0.39ms ; datarate = 1.9574659974733004E8bps; } --> rt[20].in++;
rt[20].out++ --> { delay = 0.39ms ; datarate = 1.9574659974733004E8bps; } --> rt[18].in++;
rt[18].out++ --> { delay = 0.3ms ; datarate = 8.501402238462653E8bps; } --> rt[21].in++;
rt[21].out++ --> { delay = 0.3ms ; datarate = 8.501402238462653E8bps; } --> rt[18].in++;
rt[19].out++ --> { delay = 0.25ms ; datarate = 4.7919875396633494E8bps; } --> rt[78].in++;
rt[78].out++ --> { delay = 0.25ms ; datarate = 4.7919875396633494E8bps; } --> rt[19].in++;
rt[19].out++ --> { delay = 0.15ms ; datarate = 8.615970005257202E8bps; } --> rt[21].in++;
rt[21].out++ --> { delay = 0.15ms ; datarate = 8.615970005257202E8bps; } --> rt[19].in++;
rt[22].out++ --> { delay = 0.28ms ; datarate = 3.440452455943636E8bps; } --> rt[23].in++;
rt[23].out++ --> { delay = 0.28ms ; datarate = 3.440452455943636E8bps; } --> rt[22].in++;
rt[22].out++ --> { delay = 0.21ms ; datarate = 4.2164510709890693E8bps; } --> rt[26].in++;
rt[26].out++ --> { delay = 0.21ms ; datarate = 4.2164510709890693E8bps; } --> rt[22].in++;
rt[23].out++ --> { delay = 0.22ms ; datarate = 1.302203203729074E8bps; } --> rt[25].in++;
rt[25].out++ --> { delay = 0.22ms ; datarate = 1.302203203729074E8bps; } --> rt[23].in++;
rt[24].out++ --> { delay = 0.39ms ; datarate = 2.584012587503529E8bps; } --> rt[25].in++;
rt[25].out++ --> { delay = 0.39ms ; datarate = 2.584012587503529E8bps; } --> rt[24].in++;
rt[27].out++ --> { delay = 0.16ms ; datarate = 7.381213473548348E7bps; } --> rt[29].in++;
rt[29].out++ --> { delay = 0.16ms ; datarate = 7.381213473548348E7bps; } --> rt[27].in++;
rt[27].out++ --> { delay = 0.08ms ; datarate = 6.345725411558839E8bps; } --> rt[28].in++;
rt[28].out++ --> { delay = 0.08ms ; datarate = 6.345725411558839E8bps; } --> rt[27].in++;
rt[30].out++ --> { delay = 0.08ms ; datarate = 4.211629154814989E7bps; } --> rt[31].in++;
rt[31].out++ --> { delay = 0.08ms ; datarate = 4.211629154814989E7bps; } --> rt[30].in++;
rt[30].out++ --> { delay = 0.05ms ; datarate = 6.590830379689984E8bps; } --> rt[34].in++;
rt[34].out++ --> { delay = 0.05ms ; datarate = 6.590830379689984E8bps; } --> rt[30].in++;
rt[32].out++ --> { delay = 0.36ms ; datarate = 6.081722029728435E8bps; } --> rt[34].in++;
rt[34].out++ --> { delay = 0.36ms ; datarate = 6.081722029728435E8bps; } --> rt[32].in++;
rt[33].out++ --> { delay = 0.29ms ; datarate = 4.739085983268617E8bps; } --> rt[34].in++;
rt[34].out++ --> { delay = 0.29ms ; datarate = 4.739085983268617E8bps; } --> rt[33].in++;
rt[35].out++ --> { delay = 0.1ms ; datarate = 6.343741145570701E8bps; } --> rt[36].in++;
rt[36].out++ --> { delay = 0.1ms ; datarate = 6.343741145570701E8bps; } --> rt[35].in++;
rt[36].out++ --> { delay = 0.27ms ; datarate = 1.0060205546631141E9bps; } --> rt[37].in++;
rt[37].out++ --> { delay = 0.27ms ; datarate = 1.0060205546631141E9bps; } --> rt[36].in++;
rt[38].out++ --> { delay = 0.23ms ; datarate = 3.754550594359807E7bps; } --> rt[41].in++;
rt[41].out++ --> { delay = 0.23ms ; datarate = 3.754550594359807E7bps; } --> rt[38].in++;
rt[39].out++ --> { delay = 0.21ms ; datarate = 3.903724903976934E7bps; } --> rt[40].in++;
rt[40].out++ --> { delay = 0.21ms ; datarate = 3.903724903976934E7bps; } --> rt[39].in++;
rt[40].out++ --> { delay = 0.15ms ; datarate = 3.0701894498445606E8bps; } --> rt[41].in++;
rt[41].out++ --> { delay = 0.15ms ; datarate = 3.0701894498445606E8bps; } --> rt[40].in++;
rt[42].out++ --> { delay = 0.37ms ; datarate = 8.60740280548539E7bps; } --> rt[44].in++;
rt[44].out++ --> { delay = 0.37ms ; datarate = 8.60740280548539E7bps; } --> rt[42].in++;
rt[43].out++ --> { delay = 0.26ms ; datarate = 1.3212185237628092E8bps; } --> rt[45].in++;
rt[45].out++ --> { delay = 0.26ms ; datarate = 1.3212185237628092E8bps; } --> rt[43].in++;
rt[44].out++ --> { delay = 0.17ms ; datarate = 1.1709960619531694E8bps; } --> rt[45].in++;
rt[45].out++ --> { delay = 0.17ms ; datarate = 1.1709960619531694E8bps; } --> rt[44].in++;
rt[46].out++ --> { delay = 0.24ms ; datarate = 8.640722929189007E8bps; } --> rt[47].in++;
rt[47].out++ --> { delay = 0.24ms ; datarate = 8.640722929189007E8bps; } --> rt[46].in++;
rt[46].out++ --> { delay = 0.23ms ; datarate = 3.9059742901964784E8bps; } --> rt[48].in++;
rt[48].out++ --> { delay = 0.23ms ; datarate = 3.9059742901964784E8bps; } --> rt[46].in++;
rt[47].out++ --> { delay = 0.06ms ; datarate = 8.608983786761522E8bps; } --> rt[49].in++;
rt[49].out++ --> { delay = 0.06ms ; datarate = 8.608983786761522E8bps; } --> rt[47].in++;
rt[48].out++ --> { delay = 0.14ms ; datarate = 4.867450667855599E7bps; } --> rt[144].in++;
rt[144].out++ --> { delay = 0.14ms ; datarate = 4.867450667855599E7bps; } --> rt[48].in++;
rt[50].out++ --> { delay = 0.28ms ; datarate = 5.848961046945691E8bps; } --> rt[52].in++;
rt[52].out++ --> { delay = 0.28ms ; datarate = 5.848961046945691E8bps; } --> rt[50].in++;
rt[50].out++ --> { delay = 0.17ms ; datarate = 6.875422734015768E8bps; } --> rt[53].in++;
rt[53].out++ --> { delay = 0.17ms ; datarate = 6.875422734015768E8bps; } --> rt[50].in++;
rt[51].out++ --> { delay = 0.28ms ; datarate = 3.677565702818731E8bps; } --> rt[54].in++;
rt[54].out++ --> { delay = 0.28ms ; datarate = 3.677565702818731E8bps; } --> rt[51].in++;
rt[53].out++ --> { delay = 0.08ms ; datarate = 3.478694782386766E8bps; } --> rt[54].in++;
rt[54].out++ --> { delay = 0.08ms ; datarate = 3.478694782386766E8bps; } --> rt[53].in++;
rt[53].out++ --> { delay = 0.49ms ; datarate = 3.810243049616951E8bps; } --> rt[105].in++;
rt[105].out++ --> { delay = 0.49ms ; datarate = 3.810243049616951E8bps; } --> rt[53].in++;
rt[55].out++ --> { delay = 0.26ms ; datarate = 7.722246038505232E8bps; } --> rt[56].in++;
rt[56].out++ --> { delay = 0.26ms ; datarate = 7.722246038505232E8bps; } --> rt[55].in++;
rt[57].out++ --> { delay = 0.21ms ; datarate = 2.1586606738892522E8bps; } --> rt[59].in++;
rt[59].out++ --> { delay = 0.21ms ; datarate = 2.1586606738892522E8bps; } --> rt[57].in++;
rt[58].out++ --> { delay = 0.69ms ; datarate = 4.761439610296361E8bps; } --> rt[131].in++;
rt[131].out++ --> { delay = 0.69ms ; datarate = 4.761439610296361E8bps; } --> rt[58].in++;
rt[58].out++ --> { delay = 0.29ms ; datarate = 5.824428832547753E8bps; } --> rt[59].in++;
rt[59].out++ --> { delay = 0.29ms ; datarate = 5.824428832547753E8bps; } --> rt[58].in++;
rt[58].out++ --> { delay = 0.28ms ; datarate = 4.016086020515919E8bps; } --> rt[60].in++;
rt[60].out++ --> { delay = 0.28ms ; datarate = 4.016086020515919E8bps; } --> rt[58].in++;
rt[60].out++ --> { delay = 0.27ms ; datarate = 2.912648850913694E8bps; } --> rt[78].in++;
rt[78].out++ --> { delay = 0.27ms ; datarate = 2.912648850913694E8bps; } --> rt[60].in++;
rt[60].out++ --> { delay = 0.15ms ; datarate = 6.010974481298128E8bps; } --> rt[61].in++;
rt[61].out++ --> { delay = 0.15ms ; datarate = 6.010974481298128E8bps; } --> rt[60].in++;
rt[62].out++ --> { delay = 0.4ms ; datarate = 9.987900073347746E8bps; } --> rt[63].in++;
rt[63].out++ --> { delay = 0.4ms ; datarate = 9.987900073347746E8bps; } --> rt[62].in++;
rt[62].out++ --> { delay = 0.45ms ; datarate = 9.060280807022544E8bps; } --> rt[78].in++;
rt[78].out++ --> { delay = 0.45ms ; datarate = 9.060280807022544E8bps; } --> rt[62].in++;
rt[63].out++ --> { delay = 0.39ms ; datarate = 3.987919538571598E8bps; } --> rt[64].in++;
rt[64].out++ --> { delay = 0.39ms ; datarate = 3.987919538571598E8bps; } --> rt[63].in++;
rt[64].out++ --> { delay = 0.8ms ; datarate = 7.939105198721557E8bps; } --> rt[106].in++;
rt[106].out++ --> { delay = 0.8ms ; datarate = 7.939105198721557E8bps; } --> rt[64].in++;
rt[65].out++ --> { delay = 0.25ms ; datarate = 8.365415204139054E8bps; } --> rt[67].in++;
rt[67].out++ --> { delay = 0.25ms ; datarate = 8.365415204139054E8bps; } --> rt[65].in++;
rt[66].out++ --> { delay = 0.35ms ; datarate = 8.456221179589754E8bps; } --> rt[68].in++;
rt[68].out++ --> { delay = 0.35ms ; datarate = 8.456221179589754E8bps; } --> rt[66].in++;
rt[67].out++ --> { delay = 0.33ms ; datarate = 4.666566862482756E8bps; } --> rt[68].in++;
rt[68].out++ --> { delay = 0.33ms ; datarate = 4.666566862482756E8bps; } --> rt[67].in++;
rt[68].out++ --> { delay = 0.48ms ; datarate = 7.604900601257051E8bps; } --> rt[69].in++;
rt[69].out++ --> { delay = 0.48ms ; datarate = 7.604900601257051E8bps; } --> rt[68].in++;
rt[70].out++ --> { delay = 0.22ms ; datarate = 3.117145441793639E7bps; } --> rt[71].in++;
rt[71].out++ --> { delay = 0.22ms ; datarate = 3.117145441793639E7bps; } --> rt[70].in++;
rt[71].out++ --> { delay = 0.2ms ; datarate = 3.9135693892663026E8bps; } --> rt[123].in++;
rt[123].out++ --> { delay = 0.2ms ; datarate = 3.9135693892663026E8bps; } --> rt[71].in++;
rt[72].out++ --> { delay = 0.22ms ; datarate = 5.242385687432804E8bps; } --> rt[74].in++;
rt[74].out++ --> { delay = 0.22ms ; datarate = 5.242385687432804E8bps; } --> rt[72].in++;
rt[72].out++ --> { delay = 0.11ms ; datarate = 4.231522976159945E8bps; } --> rt[77].in++;
rt[77].out++ --> { delay = 0.11ms ; datarate = 4.231522976159945E8bps; } --> rt[72].in++;
rt[73].out++ --> { delay = 0.06ms ; datarate = 3.96433875761996E8bps; } --> rt[74].in++;
rt[74].out++ --> { delay = 0.06ms ; datarate = 3.96433875761996E8bps; } --> rt[73].in++;
rt[73].out++ --> { delay = 0.09ms ; datarate = 1.9606237741792142E8bps; } --> rt[75].in++;
rt[75].out++ --> { delay = 0.09ms ; datarate = 1.9606237741792142E8bps; } --> rt[73].in++;
rt[74].out++ --> { delay = 0.09ms ; datarate = 5.775431864706918E8bps; } --> rt[76].in++;
rt[76].out++ --> { delay = 0.09ms ; datarate = 5.775431864706918E8bps; } --> rt[74].in++;
rt[75].out++ --> { delay = 0.2ms ; datarate = 7.250987311679553E8bps; } --> rt[77].in++;
rt[77].out++ --> { delay = 0.2ms ; datarate = 7.250987311679553E8bps; } --> rt[75].in++;
rt[78].out++ --> { delay = 0.27ms ; datarate = 8.175425413048013E8bps; } --> rt[88].in++;
rt[88].out++ --> { delay = 0.27ms ; datarate = 8.175425413048013E8bps; } --> rt[78].in++;
rt[78].out++ --> { delay = 0.24ms ; datarate = 1.0075449176447427E9bps; } --> rt[86].in++;
rt[86].out++ --> { delay = 0.24ms ; datarate = 1.0075449176447427E9bps; } --> rt[78].in++;
rt[78].out++ --> { delay = 0.23ms ; datarate = 4.141602076090826E8bps; } --> rt[79].in++;
rt[79].out++ --> { delay = 0.23ms ; datarate = 4.141602076090826E8bps; } --> rt[78].in++;
rt[78].out++ --> { delay = 0.22ms ; datarate = 8.63015156618573E8bps; } --> rt[80].in++;
rt[80].out++ --> { delay = 0.22ms ; datarate = 8.63015156618573E8bps; } --> rt[78].in++;
rt[79].out++ --> { delay = 0.17ms ; datarate = 3.2292759382264286E8bps; } --> rt[103].in++;
rt[103].out++ --> { delay = 0.17ms ; datarate = 3.2292759382264286E8bps; } --> rt[79].in++;
rt[79].out++ --> { delay = 0.38ms ; datarate = 5.524361007665375E8bps; } --> rt[93].in++;
rt[93].out++ --> { delay = 0.38ms ; datarate = 5.524361007665375E8bps; } --> rt[79].in++;
rt[79].out++ --> { delay = 0.25ms ; datarate = 5.1707832647441365E7bps; } --> rt[92].in++;
rt[92].out++ --> { delay = 0.25ms ; datarate = 5.1707832647441365E7bps; } --> rt[79].in++;
rt[79].out++ --> { delay = 0.63ms ; datarate = 6.246596553925803E8bps; } --> rt[106].in++;
rt[106].out++ --> { delay = 0.63ms ; datarate = 6.246596553925803E8bps; } --> rt[79].in++;
rt[79].out++ --> { delay = 0.56ms ; datarate = 1.9356833715949732E8bps; } --> rt[105].in++;
rt[105].out++ --> { delay = 0.56ms ; datarate = 1.9356833715949732E8bps; } --> rt[79].in++;
rt[80].out++ --> { delay = 0.08ms ; datarate = 2.745630121045792E8bps; } --> rt[85].in++;
rt[85].out++ --> { delay = 0.08ms ; datarate = 2.745630121045792E8bps; } --> rt[80].in++;
rt[80].out++ --> { delay = 0.27ms ; datarate = 3.4570670772800064E8bps; } --> rt[84].in++;
rt[84].out++ --> { delay = 0.27ms ; datarate = 3.4570670772800064E8bps; } --> rt[80].in++;
rt[81].out++ --> { delay = 0.28ms ; datarate = 2.095028111100703E7bps; } --> rt[85].in++;
rt[85].out++ --> { delay = 0.28ms ; datarate = 2.095028111100703E7bps; } --> rt[81].in++;
rt[81].out++ --> { delay = 0.03ms ; datarate = 1.1752425287317532E8bps; } --> rt[82].in++;
rt[82].out++ --> { delay = 0.03ms ; datarate = 1.1752425287317532E8bps; } --> rt[81].in++;
rt[81].out++ --> { delay = 0.35ms ; datarate = 4.83733623645939E8bps; } --> rt[83].in++;
rt[83].out++ --> { delay = 0.35ms ; datarate = 4.83733623645939E8bps; } --> rt[81].in++;
rt[82].out++ --> { delay = 0.26ms ; datarate = 1.1457504054322681E8bps; } --> rt[85].in++;
rt[85].out++ --> { delay = 0.26ms ; datarate = 1.1457504054322681E8bps; } --> rt[82].in++;
rt[84].out++ --> { delay = 0.77ms ; datarate = 1.9490788911100218E8bps; } --> rt[146].in++;
rt[146].out++ --> { delay = 0.77ms ; datarate = 1.9490788911100218E8bps; } --> rt[84].in++;
rt[86].out++ --> { delay = 0.26ms ; datarate = 3.8313902216345173E8bps; } --> rt[87].in++;
rt[87].out++ --> { delay = 0.26ms ; datarate = 3.8313902216345173E8bps; } --> rt[86].in++;
rt[88].out++ --> { delay = 0.24ms ; datarate = 3.331613156448326E8bps; } --> rt[91].in++;
rt[91].out++ --> { delay = 0.24ms ; datarate = 3.331613156448326E8bps; } --> rt[88].in++;
rt[89].out++ --> { delay = 0.33ms ; datarate = 3.4433512514357835E8bps; } --> rt[90].in++;
rt[90].out++ --> { delay = 0.33ms ; datarate = 3.4433512514357835E8bps; } --> rt[89].in++;
rt[89].out++ --> { delay = 0.26ms ; datarate = 5.115772254711752E8bps; } --> rt[91].in++;
rt[91].out++ --> { delay = 0.26ms ; datarate = 5.115772254711752E8bps; } --> rt[89].in++;
rt[92].out++ --> { delay = 0.64ms ; datarate = 6.900787907807554E8bps; } --> rt[106].in++;
rt[106].out++ --> { delay = 0.64ms ; datarate = 6.900787907807554E8bps; } --> rt[92].in++;
rt[93].out++ --> { delay = 0.31ms ; datarate = 7.872501904336673E7bps; } --> rt[99].in++;
rt[99].out++ --> { delay = 0.31ms ; datarate = 7.872501904336673E7bps; } --> rt[93].in++;
rt[93].out++ --> { delay = 0.33ms ; datarate = 1.0779049808843797E8bps; } --> rt[96].in++;
rt[96].out++ --> { delay = 0.33ms ; datarate = 1.0779049808843797E8bps; } --> rt[93].in++;
rt[94].out++ --> { delay = 0.23ms ; datarate = 7.882635626280447E8bps; } --> rt[97].in++;
rt[97].out++ --> { delay = 0.23ms ; datarate = 7.882635626280447E8bps; } --> rt[94].in++;
rt[94].out++ --> { delay = 0.27ms ; datarate = 1.522518102142002E8bps; } --> rt[96].in++;
rt[96].out++ --> { delay = 0.27ms ; datarate = 1.522518102142002E8bps; } --> rt[94].in++;
rt[95].out++ --> { delay = 0.32ms ; datarate = 4.5977926459286666E8bps; } --> rt[97].in++;
rt[97].out++ --> { delay = 0.32ms ; datarate = 4.5977926459286666E8bps; } --> rt[95].in++;
rt[96].out++ --> { delay = 0.34ms ; datarate = 6.48529580031667E8bps; } --> rt[99].in++;
rt[99].out++ --> { delay = 0.34ms ; datarate = 6.48529580031667E8bps; } --> rt[96].in++;
rt[97].out++ --> { delay = 0.27ms ; datarate = 3.0182811025360113E8bps; } --> rt[98].in++;
rt[98].out++ --> { delay = 0.27ms ; datarate = 3.0182811025360113E8bps; } --> rt[97].in++;
rt[100].out++ --> { delay = 0.12ms ; datarate = 1.024248710356511E8bps; } --> rt[101].in++;
rt[101].out++ --> { delay = 0.12ms ; datarate = 1.024248710356511E8bps; } --> rt[100].in++;
rt[101].out++ --> { delay = 0.15ms ; datarate = 5.0825942991560364E8bps; } --> rt[102].in++;
rt[102].out++ --> { delay = 0.15ms ; datarate = 5.0825942991560364E8bps; } --> rt[101].in++;
rt[102].out++ --> { delay = 0.21ms ; datarate = 5.04648109636596E8bps; } --> rt[103].in++;
rt[103].out++ --> { delay = 0.21ms ; datarate = 5.04648109636596E8bps; } --> rt[102].in++;
rt[104].out++ --> { delay = 0.1ms ; datarate = 9.32795843096705E8bps; } --> rt[118].in++;
rt[118].out++ --> { delay = 0.1ms ; datarate = 9.32795843096705E8bps; } --> rt[104].in++;
rt[104].out++ --> { delay = 0.2ms ; datarate = 5.041732784271244E7bps; } --> rt[121].in++;
rt[121].out++ --> { delay = 0.2ms ; datarate = 5.041732784271244E7bps; } --> rt[104].in++;
rt[104].out++ --> { delay = 0.06ms ; datarate = 2.879394102573118E8bps; } --> rt[107].in++;
rt[107].out++ --> { delay = 0.06ms ; datarate = 2.879394102573118E8bps; } --> rt[104].in++;
rt[104].out++ --> { delay = 0.34ms ; datarate = 2.0251636530688187E8bps; } --> rt[108].in++;
rt[108].out++ --> { delay = 0.34ms ; datarate = 2.0251636530688187E8bps; } --> rt[104].in++;
rt[104].out++ --> { delay = 0.44ms ; datarate = 9.820104339053669E8bps; } --> rt[114].in++;
rt[114].out++ --> { delay = 0.44ms ; datarate = 9.820104339053669E8bps; } --> rt[104].in++;
rt[105].out++ --> { delay = 0.22ms ; datarate = 9.951009507578763E8bps; } --> rt[107].in++;
rt[107].out++ --> { delay = 0.22ms ; datarate = 9.951009507578763E8bps; } --> rt[105].in++;
rt[105].out++ --> { delay = 0.35ms ; datarate = 9.704335260949154E7bps; } --> rt[127].in++;
rt[127].out++ --> { delay = 0.35ms ; datarate = 9.704335260949154E7bps; } --> rt[105].in++;
rt[105].out++ --> { delay = 0.21ms ; datarate = 2.8836581118968433E8bps; } --> rt[129].in++;
rt[129].out++ --> { delay = 0.21ms ; datarate = 2.8836581118968433E8bps; } --> rt[105].in++;
rt[105].out++ --> { delay = 0.19ms ; datarate = 5.470988752467073E8bps; } --> rt[135].in++;
rt[135].out++ --> { delay = 0.19ms ; datarate = 5.470988752467073E8bps; } --> rt[105].in++;
rt[106].out++ --> { delay = 0.43ms ; datarate = 9.726240773199279E8bps; } --> rt[144].in++;
rt[144].out++ --> { delay = 0.43ms ; datarate = 9.726240773199279E8bps; } --> rt[106].in++;
rt[106].out++ --> { delay = 0.05ms ; datarate = 5.343471267943096E8bps; } --> rt[146].in++;
rt[146].out++ --> { delay = 0.05ms ; datarate = 5.343471267943096E8bps; } --> rt[106].in++;
rt[106].out++ --> { delay = 0.39ms ; datarate = 9.17358480064931E8bps; } --> rt[139].in++;
rt[139].out++ --> { delay = 0.39ms ; datarate = 9.17358480064931E8bps; } --> rt[106].in++;
rt[106].out++ --> { delay = 0.26ms ; datarate = 3.4668211010071695E8bps; } --> rt[107].in++;
rt[107].out++ --> { delay = 0.26ms ; datarate = 3.4668211010071695E8bps; } --> rt[106].in++;
rt[107].out++ --> { delay = 0.43ms ; datarate = 4.577283403589198E8bps; } --> rt[149].in++;
rt[149].out++ --> { delay = 0.43ms ; datarate = 4.577283403589198E8bps; } --> rt[107].in++;
rt[107].out++ --> { delay = 0.11ms ; datarate = 7.139099978326036E8bps; } --> rt[155].in++;
rt[155].out++ --> { delay = 0.11ms ; datarate = 7.139099978326036E8bps; } --> rt[107].in++;
rt[108].out++ --> { delay = 0.2ms ; datarate = 6.908123306251137E8bps; } --> rt[109].in++;
rt[109].out++ --> { delay = 0.2ms ; datarate = 6.908123306251137E8bps; } --> rt[108].in++;
rt[108].out++ --> { delay = 0.09ms ; datarate = 1.0134284356638137E9bps; } --> rt[110].in++;
rt[110].out++ --> { delay = 0.09ms ; datarate = 1.0134284356638137E9bps; } --> rt[108].in++;
rt[108].out++ --> { delay = 0.08ms ; datarate = 4.8326667866034496E8bps; } --> rt[111].in++;
rt[111].out++ --> { delay = 0.08ms ; datarate = 4.8326667866034496E8bps; } --> rt[108].in++;
rt[108].out++ --> { delay = 0.09ms ; datarate = 7.487886600008591E8bps; } --> rt[112].in++;
rt[112].out++ --> { delay = 0.09ms ; datarate = 7.487886600008591E8bps; } --> rt[108].in++;
rt[109].out++ --> { delay = 0.43ms ; datarate = 2.6577430691506037E8bps; } --> rt[152].in++;
rt[152].out++ --> { delay = 0.43ms ; datarate = 2.6577430691506037E8bps; } --> rt[109].in++;
rt[109].out++ --> { delay = 0.14ms ; datarate = 8.525985386461718E8bps; } --> rt[110].in++;
rt[110].out++ --> { delay = 0.14ms ; datarate = 8.525985386461718E8bps; } --> rt[109].in++;
rt[113].out++ --> { delay = 0.21ms ; datarate = 8.089854745636891E8bps; } --> rt[114].in++;
rt[114].out++ --> { delay = 0.21ms ; datarate = 8.089854745636891E8bps; } --> rt[113].in++;
rt[115].out++ --> { delay = 0.3ms ; datarate = 3.037343533011418E8bps; } --> rt[119].in++;
rt[119].out++ --> { delay = 0.3ms ; datarate = 3.037343533011418E8bps; } --> rt[115].in++;
rt[116].out++ --> { delay = 0.34ms ; datarate = 7.765823615525702E8bps; } --> rt[119].in++;
rt[119].out++ --> { delay = 0.34ms ; datarate = 7.765823615525702E8bps; } --> rt[116].in++;
rt[116].out++ --> { delay = 0.14ms ; datarate = 2.224655782526254E8bps; } --> rt[118].in++;
rt[118].out++ --> { delay = 0.14ms ; datarate = 2.224655782526254E8bps; } --> rt[116].in++;
rt[117].out++ --> { delay = 0.2ms ; datarate = 1.009441639624791E9bps; } --> rt[118].in++;
rt[118].out++ --> { delay = 0.2ms ; datarate = 1.009441639624791E9bps; } --> rt[117].in++;
rt[117].out++ --> { delay = 0.35ms ; datarate = 2.0686349209660694E8bps; } --> rt[127].in++;
rt[127].out++ --> { delay = 0.35ms ; datarate = 2.0686349209660694E8bps; } --> rt[117].in++;
rt[120].out++ --> { delay = 0.15ms ; datarate = 9.949515769769733E7bps; } --> rt[123].in++;
rt[123].out++ --> { delay = 0.15ms ; datarate = 9.949515769769733E7bps; } --> rt[120].in++;
rt[120].out++ --> { delay = 0.11ms ; datarate = 3.597119629864363E8bps; } --> rt[121].in++;
rt[121].out++ --> { delay = 0.11ms ; datarate = 3.597119629864363E8bps; } --> rt[120].in++;
rt[122].out++ --> { delay = 0.2ms ; datarate = 7.340750055597956E8bps; } --> rt[123].in++;
rt[123].out++ --> { delay = 0.2ms ; datarate = 7.340750055597956E8bps; } --> rt[122].in++;
rt[124].out++ --> { delay = 0.38ms ; datarate = 4.864137065608502E8bps; } --> rt[126].in++;
rt[126].out++ --> { delay = 0.38ms ; datarate = 4.864137065608502E8bps; } --> rt[124].in++;
rt[125].out++ --> { delay = 0.15ms ; datarate = 5.688935529179082E7bps; } --> rt[127].in++;
rt[127].out++ --> { delay = 0.15ms ; datarate = 5.688935529179082E7bps; } --> rt[125].in++;
rt[126].out++ --> { delay = 0.39ms ; datarate = 4.9665176301528925E8bps; } --> rt[128].in++;
rt[128].out++ --> { delay = 0.39ms ; datarate = 4.9665176301528925E8bps; } --> rt[126].in++;
rt[126].out++ --> { delay = 0.32ms ; datarate = 7.486878595377638E8bps; } --> rt[127].in++;
rt[127].out++ --> { delay = 0.32ms ; datarate = 7.486878595377638E8bps; } --> rt[126].in++;
rt[127].out++ --> { delay = 0.14ms ; datarate = 5.584254618419341E8bps; } --> rt[128].in++;
rt[128].out++ --> { delay = 0.14ms ; datarate = 5.584254618419341E8bps; } --> rt[127].in++;
rt[129].out++ --> { delay = 0.16ms ; datarate = 3.387579303059697E8bps; } --> rt[130].in++;
rt[130].out++ --> { delay = 0.16ms ; datarate = 3.387579303059697E8bps; } --> rt[129].in++;
rt[129].out++ --> { delay = 0.18ms ; datarate = 9.155580674768753E8bps; } --> rt[131].in++;
rt[131].out++ --> { delay = 0.18ms ; datarate = 9.155580674768753E8bps; } --> rt[129].in++;
rt[132].out++ --> { delay = 0.38ms ; datarate = 9.212199154737113E8bps; } --> rt[135].in++;
rt[135].out++ --> { delay = 0.38ms ; datarate = 9.212199154737113E8bps; } --> rt[132].in++;
rt[132].out++ --> { delay = 0.09ms ; datarate = 5.995727829967818E8bps; } --> rt[134].in++;
rt[134].out++ --> { delay = 0.09ms ; datarate = 5.995727829967818E8bps; } --> rt[132].in++;
rt[133].out++ --> { delay = 0.09ms ; datarate = 5.888953344829787E8bps; } --> rt[135].in++;
rt[135].out++ --> { delay = 0.09ms ; datarate = 5.888953344829787E8bps; } --> rt[133].in++;
rt[136].out++ --> { delay = 0.39ms ; datarate = 9.491397215743551E8bps; } --> rt[137].in++;
rt[137].out++ --> { delay = 0.39ms ; datarate = 9.491397215743551E8bps; } --> rt[136].in++;
rt[137].out++ --> { delay = 0.04ms ; datarate = 5.855154691622902E8bps; } --> rt[140].in++;
rt[140].out++ --> { delay = 0.04ms ; datarate = 5.855154691622902E8bps; } --> rt[137].in++;
rt[137].out++ --> { delay = 0.35ms ; datarate = 6.2134024517523356E7bps; } --> rt[138].in++;
rt[138].out++ --> { delay = 0.35ms ; datarate = 6.2134024517523356E7bps; } --> rt[137].in++;
rt[139].out++ --> { delay = 0.14ms ; datarate = 6.868657626177505E8bps; } --> rt[140].in++;
rt[140].out++ --> { delay = 0.14ms ; datarate = 6.868657626177505E8bps; } --> rt[139].in++;
rt[141].out++ --> { delay = 0.07ms ; datarate = 8.556606985338231E8bps; } --> rt[144].in++;
rt[144].out++ --> { delay = 0.07ms ; datarate = 8.556606985338231E8bps; } --> rt[141].in++;
rt[142].out++ --> { delay = 0.24ms ; datarate = 7.241629205434384E8bps; } --> rt[144].in++;
rt[144].out++ --> { delay = 0.24ms ; datarate = 7.241629205434384E8bps; } --> rt[142].in++;
rt[143].out++ --> { delay = 0.17ms ; datarate = 4.7108915441202205E8bps; } --> rt[144].in++;
rt[144].out++ --> { delay = 0.17ms ; datarate = 4.7108915441202205E8bps; } --> rt[143].in++;
rt[145].out++ --> { delay = 0.4ms ; datarate = 7.358310204462339E7bps; } --> rt[147].in++;
rt[147].out++ --> { delay = 0.4ms ; datarate = 7.358310204462339E7bps; } --> rt[145].in++;
rt[146].out++ --> { delay = 0.38ms ; datarate = 2.7837009062239283E8bps; } --> rt[147].in++;
rt[147].out++ --> { delay = 0.38ms ; datarate = 2.7837009062239283E8bps; } --> rt[146].in++;
rt[148].out++ --> { delay = 0.21ms ; datarate = 3.084677130723161E8bps; } --> rt[149].in++;
rt[149].out++ --> { delay = 0.21ms ; datarate = 3.084677130723161E8bps; } --> rt[148].in++;
rt[150].out++ --> { delay = 0.34ms ; datarate = 6.042574193782523E8bps; } --> rt[151].in++;
rt[151].out++ --> { delay = 0.34ms ; datarate = 6.042574193782523E8bps; } --> rt[150].in++;
rt[151].out++ --> { delay = 0.15ms ; datarate = 7.009860405298488E8bps; } --> rt[153].in++;
rt[153].out++ --> { delay = 0.15ms ; datarate = 7.009860405298488E8bps; } --> rt[151].in++;
rt[151].out++ --> { delay = 0.18ms ; datarate = 5.475370371083454E7bps; } --> rt[152].in++;
rt[152].out++ --> { delay = 0.18ms ; datarate = 5.475370371083454E7bps; } --> rt[151].in++;
rt[151].out++ --> { delay = 0.36ms ; datarate = 2.6644929936203584E8bps; } --> rt[154].in++;
rt[154].out++ --> { delay = 0.36ms ; datarate = 2.6644929936203584E8bps; } --> rt[151].in++;
rt[152].out++ --> { delay = 0.17ms ; datarate = 9.333254664151685E8bps; } --> rt[155].in++;
rt[155].out++ --> { delay = 0.17ms ; datarate = 9.333254664151685E8bps; } --> rt[152].in++;
rt[152].out++ --> { delay = 0.29ms ; datarate = 8.82614597517044E8bps; } --> rt[153].in++;
rt[153].out++ --> { delay = 0.29ms ; datarate = 8.82614597517044E8bps; } --> rt[152].in++;
}

//
network GTITM_TS_IPv4Network_sd3_ts12_ss12_td3_03_tn4_05_sn4_02_n156_e380 extends GTITM_TS_IPv4Underlay_sd3_ts12_ss12_td3_03_tn4_05_sn4_02_n156_e380 
{
	parameters:
}

