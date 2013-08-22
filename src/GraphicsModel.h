/* 
 * Graphics model class
 *  - inherits Subject class
 *  - implements vertices, edges, faces and 3d convex hull
 * File:   GraphicsModel.h
 * Author: munehiro
 *
 * Created on August 3, 2013, 7:20 PM
 */

#ifndef GRAPHICSMODEL_H
#define	GRAPHICSMODEL_H

#include <vector>
#include "Subject.h"
#include "Vector3d.h"
#include "Edge.h"
#include "Face.h"
#include "ConvexHull.h"

using namespace std;

class GraphicsModel : public Subject {
public:
    GraphicsModel();
    virtual ~GraphicsModel();
    void clear();
    void generate(int nv);
    void construct();
    const vector<Vector3d> vertices() const { return va; };
    const vector<Edge>& edges() const { return ea; };
    const vector<Face>& faces() const { return fa; };
private:
    void updateEdges();
    void updateFaces();
    vector<Vector3d> va;    // vertex array
    vector<Edge> ea;        // edge array
    vector<Face> fa;        // face array
    ConvexHull ch;          // 3d convex hull

};

#endif	/* GRAPHICSMODEL_H */

