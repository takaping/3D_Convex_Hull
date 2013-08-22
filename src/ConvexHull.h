/* 
 * 3d convex hull class
 *  - inherits GeoGraph class
 *  - constructs the 3d convex hull
 * File:   ConvexHull.h
 * Author: munehiro
 *
 * Created on August 4, 2013, 9:10 PM
 */

#ifndef CONVEXHULL_H
#define	CONVEXHULL_H

#include <vector>
#include <unordered_map>
#include <boost/multiprecision/cpp_int.hpp>
#include "GeoGraph.h"
#include "Vector3d.h"

using namespace std;

typedef boost::multiprecision::int128_t INT128;

class ConvexHull : public GeoGraph {
public:
    ConvexHull();
    virtual ~ConvexHull();
    void clear();
    void construct(const vector<Vector3d>& va);
private:
    // primitive property
    enum class PrimProperty : int {
        NOTDEFINED,
        DELETE,
        BOUNDARY,
        NEW
    };
    // scan direction
    enum class ScanDir : int {
        CW,
        CCW
    };
    void constInitHulls();
    void constTetrahedron(int iv);
    void constDihedron(int iv);
    int searchSilhouette(int nv, int* kv0);
    void mergeAllHulls();
    void initProperty();
    void merge2Hulls(int liv0, int riv0);
    void searchCTEdge(int& liv, unordered_map<int, int>& klnxv, int& riv, unordered_map<int, int>& krnxv, bool dir);
    void wrapInCylindrical(int cte0);
    int searchEdgeOfCTFace(int iv0, int eye, ScanDir dir);
    void deleteNonHullPrims(int iv0, int cte0, ScanDir dir);
    void deleteAllPrimitives(int iv0);
    void deleteIntPrimitives(int f0);
    void deletePrimitives(vector<int>& kv, vector<int>& ke, vector<int>& kf);
    void updatePrimitives(int cte0);
    bool isFront(int f);
    bool isFront(int f, int eye);
    bool determ(const int* kv0, const Vector3d* va0);
    bool sortById(int* kv, Vector3d* va);
    bool determ(const Vector3d* va);
    int determ(const double (*m)[3]);
    int determ(const INT128 (*m)[3]);
    int determ(const INT128 (*m)[2]);
    vector<Vector3d> hva;                   // vertex array
    vector<int> kch;                        // index of convex hulls
    unordered_map<int, int> khnv;           // cyclic list of vertices on the convex hull
    unordered_map<int, int> khpv;           // cyclic list of vertices on the convex hull
    unordered_map<int, int> kcnxv;          // cyclic list of vertices on the silhouette of the convex hull
    unordered_map<int, int> kccnxv;         // cyclic list of vertices on the silhouette of the convex hull
    unordered_map<int, PrimProperty> kep;   // property of edges
    unordered_map<int, PrimProperty> kfp;   // property of faces

};

#endif	/* CONVEXHULL_H */

