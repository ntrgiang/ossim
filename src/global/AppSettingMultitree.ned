package so.global;

simple AppSettingMultitree extends AppSetting
{
    parameters:
        @class(AppSettingMultitree);

        int numStripes							= default(4);
        int videoStreamBitRate		@unit(bps)  = default(100kbps);
        int chunkSize           	@unit(byte) = default(1250B);
        int bufferMapSize       	@unit(s)    = default(2s);
}
