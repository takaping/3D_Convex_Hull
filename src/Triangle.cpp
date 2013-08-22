/* 
 * File:   Triangle.cpp
 * Author: munehiro
 * 
 * Created on August 5, 2013, 1:00 AM
 */

#include <cmath>
#include "Triangle.h"

/**
 * Constructors and destructor
 */
Triangle::Triangle() : nvec(0.0, 0.0, 0.0) {
    tv[0] = Vector3d(0.0, 0.0, 0.0);
    tv[1] = Vector3d(0.0, 0.0, 0.0);
    tv[2] = Vector3d(0.0, 0.0, 0.0);
}

Triangle::Triangle(const Triangle& orig) : nvec(orig.nvec) {
    tv[0] = orig.tv[0]; tv[1] = orig.tv[1]; tv[2] = orig.tv[2];
}

Triangle::Triangle(const Vector3d& v1, const Vector3d& v2, const Vector3d& v3) {
    set(v1, v2, v3);
}

Triangle::~Triangle() {
}

/**
 * Sets vertices and the normal vector of the triangle
 * @param vertex
 * @param vertex
 * @param vertex
 * @return valid or not
 */
bool Triangle::set(const Vector3d& v1, const Vector3d& v2, const Vector3d& v3) {
    tv[0] = v1; tv[1] = v2; tv[2] = v3;
    bool valid = isValid();
    if (valid) {
        Vector3d vec1 = tv[1] - tv[0];
        Vector3d vec2 = tv[2] - tv[1];
        Vector3d cvec = vec1.cross(vec2);
        double l = sqrt(cvec.dot(cvec));
        nvec = cvec / l;
    }
    return valid;
}

