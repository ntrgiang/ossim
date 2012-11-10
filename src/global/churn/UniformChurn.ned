

package so.global.churn;

simple UniformChurn like IChurnModel
{
    parameters:
        // -- Lower & upper bound for Arrival Times
        double lowerBoundAT @unit(seconds);
        double upperBoundAT @unit(seconds);
        
        // -- Lower & upper bound for Departure Times
        double lowerBoundDT @unit(seconds);
        double upperBoundDT @unit(seconds);
}
