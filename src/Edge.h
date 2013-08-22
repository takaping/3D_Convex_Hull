/* 
 * Edge class
 *  - implements id and vertices of the edge
 * Vertices of the edge
 * File:   Edge.h
 * Author: munehiro
 *
 * Created on August 11, 2013, 5:56 PM
 */

#ifndef EDGE_H
#define	EDGE_H

#include "Line.h"

class Edge {
public:
    Edge();
    Edge(const Edge& orig);
    Edge(int id, const Vector3d& v1, const Vector3d& v2);
    virtual ~Edge();
    // is valid the edge
    bool isValid() const { return ev.isValid(); };
    // id of the edge
    int id() const { return eid; };
    // vertex of the edge
    const Vector3d& vertex(int i) const { return ev.vertex(i); };
private:
    int eid;    // id of the edge
    Line ev;    // vertices of the edge

};

#endif	/* EDGE_H */

