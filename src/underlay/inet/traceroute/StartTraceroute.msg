cplusplus {{
#include <IPvXAddress.h>
#include <ICMPMessage_m.h>
}}

class noncobject IPvXAddress;
class noncobject ICMPMessage;

message StartTraceroute extends ICMPMessage
{
    IPvXAddress Dest;
    int port;
}
