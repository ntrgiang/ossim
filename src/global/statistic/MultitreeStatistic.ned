package so.global.statistic;

simple MultitreeStatistic like IStreamingStatistic
{
	parameters:
		string gstatLog = default("..//results//gstatlog.txt");

		double interval_reportGlobal @unit(s) = default(1s);

        @statistic[minTreeHeight](source="Signal_Min_Tree_Height"; record=vector, last);
        @statistic[maxTreeHeight](source="Signal_Max_Tree_Height"; record=vector, last);
        @statistic[meanTreeHeight](source="Signal_Mean_Tree_Height"; record=vector, last);
        @statistic[medianTreeHeight](source="Signal_Median_Tree_Height"; record=vector, last);

        @statistic[bwUtil](source="Signal_BW_Utilization"; record=vector, last);
        @statistic[packetloss](source="Signal_Packet_Loss"; record=vector, last);
        @statistic[conntime](source="Signal_Connection_Time"; record=vector, last);

        @statistic[maxRetrys](source="Signal_Max_Retrys"; record=vector, last);
        @statistic[totalRetrys](source="Signal_Total_Retrys"; record=vector, last);
        @statistic[meanRetrysPerNode](source="Signal_Mean_Retrys_Per_Node"; record=vector, last);
        @statistic[meanRetrysPerStripe](source="Signal_Mean_Retrys_Per_Stripe"; record=vector, last);

        @statistic[meanOutDegree](source="Signal_Mean_Out_Degree"; record=vector, last);
		@statistic[hopCount](source="Signal_Chunk_Arrival"; record=vector;);
		@statistic[meanHopcount](source="Signal_Mean_Hopcount"; record=vector, last;);
        @statistic[meanNumTrees](source="Signal_Mean_Num_Trees"; record=vector, last);

		@statistic[forwardingInNone](source="Signal_Forwarding_In_None"; record=vector, last;);
		@statistic[forwardingInOne](source="Signal_Forwarding_In_One"; record=vector, last;);
        @statistic[forwardingInMoreThanOne](source="Signal_Forwarding_In_More_Than_One"; record=vector, last);

        @statistic[messageCount](source="Signal_Message_Count"; record=vector, last);
        @statistic[messageCountCR](source="Signal_Message_Count_CR"; record=vector, last);
        @statistic[messageCountDR](source="Signal_Message_Count_DR"; record=vector, last);
        @statistic[messageCountCC](source="Signal_Message_Count_CC"; record=vector, last);
        @statistic[messageCountPNR](source="Signal_Message_Count_PNR"; record=vector, last);
        @statistic[messageCountSI](source="Signal_Message_Count_SI"; record=vector, last);
}
