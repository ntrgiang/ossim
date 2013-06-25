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
// AttackerModule.ned
// -----------------------------------------------------------------------------
// (C) Copyright 2012-2013, by Giang Nguyen (P2P, TU Darmstadt) and Contributors
//
// Contributors: Mathias Fischer;
// Code Reviewers: Giang Nguyen;
// -----------------------------------------------------------------------------
//

// This is the global attacker module.
// It has a global view of the topology
// and is therefore capable of choosing
// and or the best node to drop.
// Its topology information is managed
// by the nodes themselves.

package so.global.attacker;

simple AttackerModule
{
    parameters:
        bool debug = default(false);
        bool active = default(false);

        volatile double start @unit(s) = default(1s);
        volatile double stop @unit(s) = default(1s);
        volatile double interval @unit(s) = default(1s);
        volatile double simTimeLimit @unit(s) = default(1s);

        volatile int numAttack = default(0);
        //volatile double percentage = default(0.1);
        //double rejoin = default(false);

        // ----- Statistics -----
        @statistic[damage](source="Signal_Damage"; record=mean,min,max,vector);
}

