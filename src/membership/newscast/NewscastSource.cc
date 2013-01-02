/*
 * NewscastSource.cc
 *
 *      Author: Thorsten Jacobi
 */

#include "NewscastSource.h"

#include "regmacros.h"
Define_Module(NewscastSource);


#include "GossipUserData.h"
class SpecialObject : public GossipUserData {
public:
    int a;
    int b;
    int c;
    int d;



    SpecialObject();

    ~SpecialObject();

    GossipUserData* dup() const{
        return new SpecialObject();
    }

    long getSizeInBits(){
        return ((sizeof(a)+sizeof(b)+sizeof(c)+sizeof(d))*8);
    }
};

SpecialObject::SpecialObject() : GossipUserData(){

}
SpecialObject::~SpecialObject(){

}

NewscastSource::NewscastSource() {
    //ownValue = new SpecialObject(); // TODO: remove
}

NewscastSource::~NewscastSource() {
    // TODO Auto-generated destructor stub
}

