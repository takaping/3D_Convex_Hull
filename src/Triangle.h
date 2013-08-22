/* 
 * Triangle class
 *  - implements vertices and the normal vector of the triangle
 * File:   Triangle.h
 * Author: munehiro
 *
 * Created on August 5, 2013, 12:59 AM
 */

#ifndef TRIANGLE_H
#define	TRIANGLE_H

#include "Vector3d.h"

class Triangle {
public:
    Triangle();
    Triangle(const Triangle& orig);
    Triangle(const Vector3d& v1, const Vector3d& v2, const Vector3d& v3);
    virtual ~Triangle();
    // is valid the triangle
    bool isValid() const { return !(tv[0] == tv[1] && tv[1] == tv[2] && tv[2] == tv[0]); };
    // vertex of the triangle
    const Vector3d& vertex(int i) const { return tv[i]; };
    // normal vector of the triangle
    const Vector3d& normal() const { return nvec; };
    bool set(const Vector3d& v1, const Vector3d& v2, const Vector3d& v3);
private:
    Vector3d tv[3]; // vertices
    Vector3d nvec;  // normal vector

};

#endif	/* TRIANGLE_H */

