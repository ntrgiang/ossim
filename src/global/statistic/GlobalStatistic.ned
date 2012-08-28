//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

package simpleoverlay.global.statistic;

simple GlobalStatistic
{
    parameters:
        // --- Signals
        @signal[Signal_ChunkHit](type="long");
        @signal[Signal_ChunkMiss](type="long");
        @signal[Signal_ChunkNeed](type="long");
        
        // --- Statistics
        @statistic[ChunkHit](source=Signal_ChunkHit; record=count);
        @statistic[ChunkMiss](source=Signal_ChunkMiss; record=count);
        @statistic[ChunkSeek](source=Signal_ChunkSeek; record=count);
        @statistic[Rebuffering](source=Signal_Rebuffering; record=count);
        
        //@statistic[CI](source="count(Signal_ChunkHit)/(count(Signal_ChunkHit)+count(Signal_ChunkMiss))"; record=last);
        @statistic[CI](source="count(Signal_ChunkHit)/count(Signal_ChunkNeed)"; record=last);
        
        // -- Join time
        @statistic[MeshJoin](source=Signal_MeshJoin; record=vector);
        
        @statistic[NumberOfPartner](title="Number Of Partner"; source=Signal_NumberOfPartner; record=vector);

}
