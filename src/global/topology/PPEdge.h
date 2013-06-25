#ifndef PPEDGE_H
#define PPEDGE_H

#include "IPvXAddress.h"

class PPEdge {
    IPvXAddress _begin;
    IPvXAddress _end;
    double      _beginJoinTime;
    double      _endJoinTime;
public:
    PPEdge(IPvXAddress begin, IPvXAddress end, double beginJoinTime, double endJoinTime);

    inline double      getAgeDistance()   const { return _endJoinTime - _beginJoinTime; }
    inline IPvXAddress begin()            const { return _begin; }
    inline IPvXAddress end()              const { return _end; }
    inline double      getBeginJoinTime() const { return _beginJoinTime; }
    inline double      getEndJoinTime()   const { return _endJoinTime; }
};

#endif // PPEDGE_H
