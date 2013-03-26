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
// StaticChurn.ned
// -----------------------------------------------------------------------------
// (C) Copyright 2012-2013, by Giang Nguyen (P2P, TU Darmstadt) and Contributors
//
// Contributors: Giang;
// Code Reviewers: -;
// -----------------------------------------------------------------------------
//



package so.global.churn;

simple StaticChurn like IChurnModel
{
    parameters:
//        double arrivalTime      @unit(seconds) = default(1.0);
//        double sessionDuration  @unit(seconds) = default(11.0);
        
        // -- Lower & upper bound for Arrival Times
        double lowerBoundAT @unit(seconds);
        double upperBoundAT @unit(seconds);
        
//        double arrivalTime @units(milliseconds) = default(1.0);
//        double departTime @units(milliseconds) = default(11.0);
        
        // -- Possible signals
        @signal[arrivalTime](type="double");
        @signal[sessionDuration](type="double");
        
        // -- Statistics
        @statistic[arrivalTime](title="Global Arrival Times"; source=arrivalTime; record=count, vector);
        @statistic[sessionDuration](title="Global Session Durations"; source=sessionDuration; record=count, vector);
}
