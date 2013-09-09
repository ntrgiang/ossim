package so.tree.multitree;

simple MultitreePeer extends MultitreeBase
{
    parameters:
        @class(MultitreePeer);
        @display("i=block/app;bgb=250,410");

        // Timer
        volatile double startTime @unit(s);

		double retryLeave @unit(s) = default(1s);
		double intervalReportStats @unit(s) = default(1s);

		double delaySuccessorInfo @unit(s) = default(10ms);
		double delayRetryConnect @unit(s) = default(10ms);
		
        @statistic[numTrees](source="Signal_Num_Trees"; record=vector);
        @statistic[outDegree](source="Signal_Out_Degree"; record=vector);
}
