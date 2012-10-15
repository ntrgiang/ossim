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

package simpleoverlay.applications.common.streaming;

simple VideoBuffer
{
    parameters:
        @class(VideoBuffer);
        @display("i=block/table");
        
        @signal[Signal_RecevedChunk](type=long);
        
        @statistic[receivedChunk](title="Sequence Number of received chunks"; source=Signal_RecevedChunk; record=count,vector);

         @signal[Signal_LateChunk](type=long);
         @statistic[lateChunk](title="Sequence Number of late chunks"; source=Signal_LateChunk; record=count,vector);

         @signal[Signal_NonsenseChunk](type=long);
         @statistic[nonsenseChunk](title="Sequence Number of non-sense chunks"; source=Signal_NonsenseChunk; record=count,vector);
        
         @signal[Signal_InrangeChunk](type=long);
         @statistic[inrangeChunk](title="Sequence Number of in-range chunks"; source=Signal_InrangeChunk; record=count,vector);
}
