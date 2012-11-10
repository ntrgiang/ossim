
//import simpleoverlay.global.churn.IChurnModel;

package so.global.churn;


// This model implements the churn model following Veloso's paper

simple VelosoChurnModel like IChurnModel
{
    parameters:
        int rng = default(1);
        
        // -- For the Pareto distribution
        double a;
        double b;
        
        // -- For the Lognornal distribution
        double mu;
        double lambda;
        
        // -- Possible signals
        @signal[arrivalTime](type="double");
        @signal[sessionDuration](type="double");
        
        // -- Statistic collection
        @statistic[arrivalTime](title="Discrete arrival times"; source=arrivalTime; record=count,vector);
        @statistic[sessionDuration](title="Separate session durations";source=sessionDuration; record=count,vector);        
}


