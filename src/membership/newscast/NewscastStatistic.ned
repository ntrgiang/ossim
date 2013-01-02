

package so.membership.newscast;

simple NewscastStatistic
{
   parameters:
		@class(NewscastStatistic);
		
	string newscastGetRandomIPsLOG = default("newscastGetRandomIPs.txt");
	string newscastConnectionsLOG = default("newscastConnections.txt");
}
