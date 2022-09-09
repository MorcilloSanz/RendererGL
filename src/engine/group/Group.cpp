#include "Group.h"

Group::Group(unsigned int _primitive, bool _showWire) 
    : primitive(_primitive), showWire(_showWire), modelMatrix(1.f), visible(true), pointSize(1), lineWidth(1) {
}

Group::Group() 
    : primitive(GL_TRIANGLES), showWire(false), modelMatrix(1.f), visible(true), pointSize(1), lineWidth(1){
}