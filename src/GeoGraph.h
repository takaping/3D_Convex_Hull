/* 
 * Geometric graph class
 *  - implements edges and faces relation
 * File:   GeoGraph.h
 * Author: munehiro
 *
 * Created on August 4, 2013, 9:10 PM
 */

#ifndef GEOGRAPH_H
#define	GEOGRAPH_H

#include <unordered_map>

using namespace std;

class GeoGraph {
public:
    GeoGraph();
    virtual ~GeoGraph();
    virtual void clear();
    vector<int> edges();
    vector<int> faces();
    void getVerticesOfEdge(int e, int* kv);
    void getVerticesOfTriangle(int f, int* kv);
protected:
    // @return new edge
    int newEdge() { return ep++; };
    // @return new face
    int newFace() { return fp++; };
    // @param vertex
    // @param edge
    // @return other vertex of edge
    int otherVertex(int iv, int e) { return (iv == ksv[e] ? kev[e] : ksv[e]); };
    // @param vertex
    // @param edge
    // @return next clockwise edge
    int nextCWEdge(int iv, int e) { return (iv == ksv[e] ? ksce[e] : kece[e]); };
    // @param vertex
    // @param edge
    // @return next counter-clockwise edge
    int nextCCWEdge(int iv, int e) { return (iv == ksv[e] ? kscce[e] : kecce[e]); };
    void getEdgesOfTriangle(int f, int* ke);
    // @param vertex
    // @param edge
    // @return left face of the edge
    int leftFace(int iv, int e) { return (iv == ksv[e] ? klf[e] : krf[e]); };
    // @param vertex
    // @param edge
    // @return right face of the edge
    int rightFace(int iv, int e) { return (iv == ksv[e] ? krf[e] : klf[e]); };
    unordered_map<int, int> kfe;    // list of the face to the edge
    unordered_map<int, int> kve;    // list of the vertex to the edge
    unordered_map<int, int> ksv;    // list of the edge to the start vertex
    unordered_map<int, int> kev;    // list of the edge to the end vertex
    unordered_map<int, int> klf;    // list of the edge to the left face
    unordered_map<int, int> krf;    // list of the edge to the right face
    unordered_map<int, int> ksce;   // list of the edge to the next clockwise edge at the start vertex
    unordered_map<int, int> kscce;  // list of the edge to the next counter-clockwise edge at the start vertex
    unordered_map<int, int> kece;   // list of the edge to the next clockwise edge at the end vertex
    unordered_map<int, int> kecce;  // list of the edge to the next counter-clockwise edge at the end vertex
private:
    int ep; // pointer to the id of the edge
    int fp; // pointer to the id of the face

};

#endif	/* GEOGRAPH_H */

