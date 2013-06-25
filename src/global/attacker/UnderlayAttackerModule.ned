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
// UnderlayAttackerModule.ned
// -----------------------------------------------------------------------------
// (C) Copyright 2012-2013, by Giang Nguyen (P2P, TU Darmstadt) and Contributors
//
// Contributors: Mathias Fischer;
// Code Reviewers: Giang Nguyen;
// -----------------------------------------------------------------------------
//

// This is the global attacker module.

package so.global.attacker;

simple UnderlayAttackerModule
{
    parameters:
        bool active;
        int startAttack;
		int victimNumber;
		double truncateDamageMax = default(0.25);
		double truncateDamageStep = default(0.01);
		bool performDamageTest = default(true); //should be true to gather greedy damage
		double simTimeLimit; // variable that gives the end of the simulation

        // ----- Statistics -----
        @statistic[damage](source="Signal_Damage"; record=mean,min,max,vector);

        @statistic[remaining_nodes](source="Signal_remaining_nodes"; record=mean);
        @statistic[used_Links](source="Signal_used_Links"; record=mean);
        @statistic[used_Routers](source="Signal_used_Routers"; record=mean);
        @statistic[weighted_router_usage_gates](source="Signal_weighted_router_usage_gates"; record=mean);
        @statistic[average_router_usage](source="Signal_average_router_usage"; record=mean);

}
