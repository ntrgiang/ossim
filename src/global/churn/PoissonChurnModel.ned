

//import simpleoverlay.global.churn.IChurnModel;

package so.global.churn;


// This model implements the Lifetime model with
// -- Poisson arrival process --> inter-arrival times follow an Exponential distribution
// -- Session times follow an Exponential distribution

simple PoissonChurnModel like IChurnModel
{
    parameters:
        int rng = default(1234);
        
        double arrivalRate = default(1.0);      // in [arrivals per second]
        double meanSessionTime = default(3.0);  // in [seconds]
}


