/* 
 * File:   Face.cpp
 * Author: munehiro
 * 
 * Created on August 11, 2013, 5:56 PM
 */

#include "Face.h"

/**
 * Constructors and Destructor
 */
Face::Face() : fid(0) {
}

Face::Face(const Face& orig) : fid(orig.fid), fv(orig.fv) {
}

Face::Face(int id, const Vector3d& v1, const Vector3d& v2, const Vector3d& v3)
: fid(id), fv(v1, v2, v3) {
}

Face::~Face() {
}

