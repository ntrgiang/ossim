package so.simulations;

import so.global.ActivePeerTable;
import so.global.churn.IChurnModel;
import so.global.AppSettingDonet;
import so.global.statistic.IStreamingStatistic;
import so.global.observer.MeshOverlayObserver;
import so.global.logger.Logger;

import so.global.topology.OverlayTopology;
import so.global.attacker.UnderlayAttackerModule;

// -- TODO: Unify the NED interface for statistic module and the appSetting module

module BaseNetworkUnderlayAttackModule
{
    parameters:
        @display("bgb=488,389");
        string churnModel = default("StaticChurn");
        string statisticModule = default("DonetStatistic");

    submodules:
        activePeerTable: ActivePeerTable {
            @display("p=77,96");
        }

        churnModerator: <churnModel> like IChurnModel {
            @display("p=77,171");
        }

        globalStatistic: <statisticModule> like IStreamingStatistic {
            @display("p=77,248;i=device/disk");
        }

        appSetting: AppSettingDonet {
            @display("p=77,328;i=block/wheelbarrow");
        }

        meshObserver: MeshOverlayObserver {
            @display("p=77,430");
        }

        logger: Logger {
            @display("p=77,510");
        }
        topoObserver: OverlayTopology {
        }

        attacker: UnderlayAttackerModule {
        }
}

