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
// CoolstreamingBase.ned
// -----------------------------------------------------------------------------
// (C) Copyright 2012-2013, by Giang Nguyen (P2P, TU Darmstadt) and Contributors
//
// Contributors: Thorsten Jacobi;
// Code Reviewers: Giang;
// -----------------------------------------------------------------------------
//


package so.mesh.coolstreaming;

import so.common.CommBase;

simple CoolstreamingBase extends CommBase
{
    parameters:
        @class(CoolstreamingBase);
        @display("i=block/app;bgb=250,410");
        
		int substreamCount = default(3); // Number of substreams 
       
        int maxNOP = default(4); // Max number of Partners
        int minNOP = default(3); // Min number of Partners
        
        double CheckPartnersIntervall @unit(s) = default(3.0s);
        double BufferMapIntervall     @unit(s) = default(1.0s);
        
        bool debugOutput = default(true);
}
