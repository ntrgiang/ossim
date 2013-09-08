package so.global.topology;

simple OverlayTopology
{
    parameters:
        @class(OverlayTopology);

        bool debug = default(false);
        int streamStripes = default(1);
        int observeTime @unit(s) = default(1s);
        int observeTimeEnd @unit(s) = default(2s);
        int observeTimeStep @unit(s) = default(1s);

        string sourceAddress = default("192.168.0.1");
        // -- For result recording to an external file
        string outputFilename = default("");
        string snapshotFolder = default("");

        // -- To save the overlay topology
        string overlayTopologyFile = default("");
        int numPeers = default(1);
}
