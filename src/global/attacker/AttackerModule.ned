// This is the global attacker module.
// It has a global view of the topology
// and is therefore capable of choosing
// and or the best node to drop.
// Its topology information is managed
// by the nodes themselves.


package so.global.attacker;

simple AttackerModule
{
    parameters:
        volatile double start;
        volatile double stop;
        volatile double interval;
        volatile double percentage;
		volatile int numAttack;
        double rejoin;
}
