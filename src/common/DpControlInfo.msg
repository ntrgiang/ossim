//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
//


cplusplus {{
#include "IPvXAddress.h"
}}


class noncobject IPvXAddress;

class DpControlInfo
{
    IPvXAddress srcAddr;  // source \IP or \IPv6 address, or local address with BIND
    IPvXAddress destAddr; // destination \IP or \IPv6 address
    int srcPort;   // \UDP source port in packet, or local port with BIND
    int destPort;  // \UDP destination port in packet
}
