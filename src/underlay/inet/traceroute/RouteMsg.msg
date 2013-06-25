cplusplus {{
#include <TracerouteTable.h>
}}

class noncobject TracerouteTable::Route;
class noncobject IPvXAddress;

message RouteMsg extends cPacket
{
    TracerouteTable::Route route;
    IPvXAddress Dest;
}
