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
// MultitreeBase.ned
// -----------------------------------------------------------------------------
// (C) Copyright 2012-2013, by Giang Nguyen (P2P, TU Darmstadt) and Contributors
//
// Contributors: Julian Wulfheide;
// Code Reviewers: Giang;
// -----------------------------------------------------------------------------
//

package so.tree.multitree;

import so.common.CommBase;

simple MultitreeBase extends CommBase
{
    parameters:
        @class(MultitreeBase);
        @display("i=block/app;bgb=250,410");

		double bwCapacity = default(1);

		double delayOptimization @unit(s) = default(1s);
		bool optimize = default(true);
		bool sendMissingChunks = default(true);

		double weightT = default(0.2);
		double weightK1 = default(0.2);
		double weightK2 = default(0.2);
		double weightK3 = default(0.2);
		double weightK4 = default(0.2);
}
