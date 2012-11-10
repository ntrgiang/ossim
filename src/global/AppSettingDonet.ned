

package so.global;

simple AppSettingDonet extends AppSetting
{
    parameters:
        @class(AppSettingDonet);

        int videoStreamBitRate  @unit(bps)  = default(100kbps);
        int chunkSize           @unit(byte) = default(1250B);
        int bufferMapSize       @unit(s)    = default(2s);
        
        double interval_bufferMap @unit(s) = default(0s);
}
