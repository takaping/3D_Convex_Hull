/* 
 * File:   Edge.cpp
 * Author: munehiro
 * 
 * Created on August 11, 2013, 5:56 PM
 */

#include "Edge.h"

/**
 * Constructors and Destructor
 */
Edge::Edge() : eid(0) {
}

Edge::Edge(const Edge& orig) : eid(orig.eid), ev(orig.ev) {
}

Edge::Edge(int id, const Vector3d& v1, const Vector3d& v2) : eid(id), ev(v1, v2) {
}

Edge::~Edge() {
}

