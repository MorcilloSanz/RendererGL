#include "Group.h"

Group::Group(unsigned int _primitive, bool _showWire) 
    : primitive(_primitive), showWire(_showWire), modelMatrix(1.f), visible(true), pointSize(1), lineWidth(1) {
}

Group::Group() 
    : primitive(GL_TRIANGLES), showWire(false), modelMatrix(1.f), visible(true), pointSize(1), lineWidth(1){
}

void Group::removePolytope(std::shared_ptr<Polytope>& polytope) {
    unsigned int index = 0;
    for(auto& p : polytopes) {
        if(p.get() == polytope.get()) {
            removePolytope(index);
            break;
        }
        index ++;
    }
}