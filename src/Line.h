/* 
 * Line class
 *  - implements vertices of the line
 * File:   Line.h
 * Author: munehiro
 *
 * Created on August 5, 2013, 12:27 AM
 */

#ifndef LINE_H
#define	LINE_H

#include "Vector3d.h"

class Line {
public:
    Line();
    Line(const Line& orig);
    Line(const Vector3d& v1, const Vector3d& v2);
    virtual ~Line();
    // is valid the line
    bool isValid() const { return (lv[0] != lv[1]); };
    // vertex of the line
    const Vector3d& vertex(int i) const { return lv[i]; };
    bool set(const Vector3d& v1, const Vector3d& v2);
private:
    Vector3d lv[2];

};

#endif	/* LINE_H */

