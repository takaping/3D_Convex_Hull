/* 
 * File:   Line.cpp
 * Author: munehiro
 * 
 * Created on August 5, 2013, 12:27 AM
 */

#include "Line.h"

/**
 * Constructors and Destructor
 */
Line::Line() {
    lv[0] = Vector3d(0.0, 0.0, 0.0);
    lv[1] = Vector3d(0.0, 0.0, 0.0);
}

Line::Line(const Line& orig) {
    lv[0] = orig.lv[0]; lv[1] = orig.lv[1];
}

Line::Line(const Vector3d& v1, const Vector3d& v2) {
    set(v1, v2);
}

Line::~Line() {
}

/**
 * Sets vertices of the line
 * @param start vertex
 * @param end vertex
 * @return valid or not
 */
bool Line::set(const Vector3d& v1, const Vector3d& v2) {
    lv[0] = v1; lv[1] = v2;
    return isValid();
}

