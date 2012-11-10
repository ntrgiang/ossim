

//import simpleoverlay.global.churn.IChurnModel;

package so.global.churn;


// This model implements the Lifetime model with
// -- Poisson arrival process --> inter-arrival times follow an Exponential distribution
// -- Session times follow an Exponential distribution

simple ExponentialChurnModel like IChurnModel
{
    parameters:
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


