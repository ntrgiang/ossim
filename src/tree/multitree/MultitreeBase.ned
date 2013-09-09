package so.tree.multitree;

import so.common.CommBase;

simple MultitreeBase extends CommBase
{
    parameters:
        @class(MultitreeBase);
        @display("i=block/app;bgb=250,410");

		double bwCapacity = default(1);

		double delayOptimization @unit(s) = default(1s);
		bool optimize = default(true);
		bool sendMissingChunks = default(true);

		double weightT = default(0.2);
		double weightK1 = default(0.2);
		double weightK2 = default(0.2);
		double weightK3 = default(0.2);
		double weightK4 = default(0.2);
}
