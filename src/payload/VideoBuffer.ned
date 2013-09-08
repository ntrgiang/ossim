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
// VideoBuffer.ned
// -----------------------------------------------------------------------------
// (C) Copyright 2012-2013, by Giang Nguyen (P2P, TU Darmstadt) and Contributors
//
// Contributors: Giang;
// Code Reviewers: -;
// -----------------------------------------------------------------------------
//


package so.payload;

simple VideoBuffer
{
    parameters:
        @class(VideoBuffer);
        @display("i=block/table");

        bool debug = default(false);
        
        @statistic[receivedChunk](title="Sequence Number of received chunks"; source=Signal_ReceivedChunk; record=count,last,vector);

         //@statistic[lateChunk](title="Sequence Number of late chunks"; source=Signal_LateChunk; record=count,vector);
         //@statistic[nonsenseChunk](title="Sequence Number of non-sense chunks"; source=Signal_NonsenseChunk; record=count,vector);
         //@statistic[inrangeChunk](title="Sequence Number of in-range chunks"; source=Signal_InrangeChunk; record=count,vector);
         //@statistic[duplicatedChunk](title="Sequence Number of duplicated chunks"; source=Signal_DuplicatedChunk; record=count,vector);
         
     //gates:
        // From Forwarder
     //   input forwarderIn   @label(VideoChunk/up);
     //   output forwarderOut @label(VideoChunk/down);
        
        // To Player
     //   output playerOut @label(VideoChunk/up);
        
}
