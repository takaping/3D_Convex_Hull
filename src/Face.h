/* 
 * Face class
 *  - implements id, vertices and the normal vector of the face
 * File:   Face.h
 * Author: munehiro
 *
 * Created on August 11, 2013, 5:56 PM
 */

#ifndef FACE_H
#define	FACE_H

#include "Triangle.h"

class Face {
public:
    Face();
    Face(const Face& orig);
    Face(int id, const Vector3d& v1, const Vector3d& v2, const Vector3d& v3);
    virtual ~Face();
    // is valid the face
    bool isValid() const { return fv.isValid(); };
    // id of the face
    int id() const { return fid; };
    // vertex of the face
    const Vector3d& vertex(int i) const { return fv.vertex(i); };
    // normal vector of the face
    const Vector3d& normal() const { return fv.normal(); };
private:
    int fid;        // id of the face
    Triangle fv;    // vertices and the normal vector of the triangle

};

#endif	/* FACE_H */

