/*
 * NewscastSource.cc
 *
 *      Author: Thorsten Jacobi
 */

#include "NewscastSource.h"

#include "regmacros.h"
Define_Module(NewscastSource);


class SpecialObject : public cObject {
public:
    int a;
    int b;
    int c;
    int d;
    SpecialObject() : cObject (){
    };
    ~SpecialObject(){
    }

    cObject* dup() const{
        return new SpecialObject();
    }
};

NewscastSource::NewscastSource() {
    ownValue = new SpecialObject();
}

NewscastSource::~NewscastSource() {
    // TODO Auto-generated destructor stub
}

