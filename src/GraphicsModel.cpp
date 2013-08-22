/* 
 * File:   GraphicsModel.cpp
 * Author: munehiro
 * 
 * Created on August 3, 2013, 7:20 PM
 */

#include <ctime>
#include <random>
#include <stdlib.h>
#include <algorithm>
#include "GraphicsModel.h"

/**
 * Constructor and Destructor
 */
GraphicsModel::GraphicsModel() {
}

GraphicsModel::~GraphicsModel() {
}

/**
 * Clears lists
 */
void GraphicsModel::clear() {
    va.clear();
    ea.clear();
    fa.clear();
    ch.clear();
}

/**
 * Generates vertices
 * @param number of vertices
 */
void GraphicsModel::generate(int nv) {
    clear();
    srand(time(nullptr));
    // generates the vertex array in random order
    // (or loads the vertex array from the file)
    for (int i = 0; i < nv; i++) {
        double x, y, z;
        do {
            x = (double)rand() / RAND_MAX * 2.0 - 1.0;
            y = (double)rand() / RAND_MAX * 2.0 - 1.0;
            z = (double)rand() / RAND_MAX * 2.0 - 1.0;
        } while ((x * x + y * y + z * z) > 1.0);
        va.push_back(Vector3d(x, y, z));
    }
    /********************************
     *  must sort vertices in x order
     ********************************/
    sort(va.begin(), va.end(), Vector3d::lessX);
    // notifies observers
    notifyObservers();
}

/**
 * Constructs the 3d convex hull
 */
void GraphicsModel::construct() {
    // constructs the 3d convex hull
    ch.construct(va);
    // updates the edge and the face array
//    updateEdges();
    updateFaces();
    // notifies observers
    notifyObservers();
}

/**
 * Updates the edge array
 */
void GraphicsModel::updateEdges() {
    vector<int> ke = ch.edges();
    ea.clear();
    for_each(ke.begin(), ke.end(), [&](int k) {
        int kv[2];
        ch.getVerticesOfEdge(k, kv);
        Edge e(k, va[kv[0]], va[kv[1]]);
        if (e.isValid()) {
            ea.push_back(e);
        }
    });
}

/**
 * Updates the face array
 */
void GraphicsModel::updateFaces() {
    vector<int> kf = ch.faces();
    fa.clear();
    for_each(kf.begin(), kf.end(), [&](int k) {
        int kv[3];
        ch.getVerticesOfTriangle(k, kv);
        Face f(k, va[kv[0]], va[kv[1]], va[kv[2]]);
        if (f.isValid()) {
            fa.push_back(f);
        }
    });
}

