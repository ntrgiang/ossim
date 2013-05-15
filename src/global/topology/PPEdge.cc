#include "PPEdge.h"

PPEdge::PPEdge(IPvXAddress begin, IPvXAddress end, double beginJoinTime, double endJoinTime) {
    _begin         = begin;
    _end           = end;
    _beginJoinTime = beginJoinTime;
    _endJoinTime   = endJoinTime;
}
