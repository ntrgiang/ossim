#include "AppCommon.h"

long int get2LastOctet(IPvXAddress address)
{
   int third_octet = address.get4().getDByte(2);
   int fourth_octet = address.get4().getDByte(3);

   return third_octet*256 + fourth_octet;
}

