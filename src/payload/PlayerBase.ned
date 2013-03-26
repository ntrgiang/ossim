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
// PlayerBase.ned
// -----------------------------------------------------------------------------
// (C) Copyright 2012-2013, by Giang Nguyen (P2P, TU Darmstadt) and Contributors
//
// Contributors: Giang;
// Code Reviewers: -;
// -----------------------------------------------------------------------------
//

package so.payload;

moduleinterface PlayerBase
{
   parameters:
      @class(PlayerBase);
      
      //double interval_reportStatistic @unit(s) = default(0.1s);

      //double interval_recheckVideoBuffer @unit(s) = default(0.1s);
      //double percent_buffer_low = default(0.0);
      //double percent_buffer_high = default(1.0);
      //int max_skipped_chunk = default(3);
      //double interval_probe_playerStart @unit(s) = default(1s);
        
		// --- Possible signals to collect
        //@signal[rebuffering_Local](type=long);
        //@signal[Signal_ChunkHit](type=long);
        //@signal[Signal_ChunkMiss](type=long);
        //@signal[Signal_ChunkSeek](type=long);
		
		// --- Statistics
      //@statistic[ChunkHit](source=Signal_ChunkHit; title="Number of chunk hit"; record=count,vector);
      //@statistic[ChunkMiss](source=Signal_ChunkMiss; title="Number of chunk Miss"; record=count,vector);
      //@statistic[ChunkSeek](source=Signal_ChunkSeek; title="Number of chunk Seek"; record=count,vector);
      //@statistic[CI](title="Continuity Index"; source="count(Signal_ChunkHit)/count(Signal_ChunkSeek)"; record=last);

      //@statistic[rebuffering](title="Number of Re-buffering events"; source=Signal_Rebuffering; record=sum,vector);
      //@statistic[stall](title="Number of stall events"; source=Signal_Stall; record=sum,vector);

}


