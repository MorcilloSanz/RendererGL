#include "Group.h"

unsigned long Group::groupCount = 0;

Group::Group(unsigned int _primitive, bool _showWire) 
    : primitive(_primitive), showWire(_showWire), modelMatrix(1.f), visible(true), 
    pointSize(POINT_SIZE), lineWidth(LINE_WIDTH), outliningWidth(OUTLINING_WIDTH),
    id(groupCount) {
    groupCount ++;
}

Group::Group() 
    : primitive(GL_TRIANGLES), showWire(false), modelMatrix(1.f), visible(true), 
    pointSize(POINT_SIZE), lineWidth(LINE_WIDTH), outliningWidth(OUTLINING_WIDTH),
    id(groupCount) {
    groupCount ++;
}

void Group::removePolytope(Polytope::Ptr& polytope) {
    unsigned int index = 0;
    for(auto& p : polytopes) {
        if(p.get() == polytope.get()) {
            removePolytope(index);
            break;
        }
        index ++;
    }
}

bool Group::isSelected() {
    bool selected = true;
    for(auto& p : polytopes) {
        if(!p->isSelected()) selected = false;
    }
    return selected;
}

void Group::setSelected(bool selected) {
    for(auto& p : polytopes) p->setSelected(selected);
}