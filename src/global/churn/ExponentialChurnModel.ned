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
// ExponentialChurnModel.ned
// -----------------------------------------------------------------------------
// (C) Copyright 2012-2013, by Giang Nguyen (P2P, TU Darmstadt) and Contributors
//
// Contributors: Giang;
// Code Reviewers: -;
// -----------------------------------------------------------------------------
//



//import simpleoverlay.global.churn.IChurnModel;

package so.global.churn;


// This model implements the Lifetime model with
// -- Poisson arrival process --> inter-arrival times follow an Exponential distribution
// -- Session times follow an Exponential distribution

simple ExponentialChurnModel like IChurnModel
{
    parameters:
        bool leave = default(false);

        int rng = default(1234);
        
        // -- For the Exponential distribution of inter-arrival times
        double arrivalRate;
        
        // -- For the Exponential distribution of session durations 
        double meanDuration @unit(seconds);
        
        // -- Possible signals
        @signal[arrivalTime](type="double");
        @signal[sessionDuration](type="double");
        
        // -- Statistic collection
        @statistic[arrivalTime](title="Discrete arrival times"; source=arrivalTime; record=count,vector);
        @statistic[sessionDuration](title="Separate session durations"; source=sessionDuration; record=count,vector);
}


