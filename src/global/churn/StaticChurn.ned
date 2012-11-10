

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
