

package so.global;

simple AppSetting
{
    parameters:
        @class(AppSetting);
        
        int localPort = default(9000);
        int destPort  = default(9000);
}
