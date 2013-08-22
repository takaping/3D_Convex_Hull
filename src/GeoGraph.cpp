/* 
 * File:   GeoGraph.cpp
 * Author: munehiro
 * 
 * Created on August 4, 2013, 9:10 PM
 */

#include <algorithm>
#include <vector>
#include "GeoGraph.h"

/**
 * Constructor and Destructor
 */
GeoGraph::GeoGraph() : ep(0), fp(0) {
}

GeoGraph::~GeoGraph() {
}

/**
 * Clears lists
 */
void GeoGraph::clear() {
    ep = 0;
    fp = 0;
    kfe.clear();
    kve.clear();
    ksv.clear();
    kev.clear();
    klf.clear();
    krf.clear();
    ksce.clear();
    kscce.clear();
    kece.clear();
    kecce.clear();
}

/**
 * @return edges
 */
vector<int> GeoGraph::edges() {
    vector<int> ke;
    for_each(ksv.begin(), ksv.end(), [&](unordered_map<int, int>::value_type k) {
        ke.push_back(k.first);
    });
    return ke;
}

/**
 * @return faces
 */
vector<int> GeoGraph::faces() {
    vector<int> kf;
    for_each(kfe.begin(), kfe.end(), [&](unordered_map<int, int>::value_type k) {
        kf.push_back(k.first);
    });
    return kf;
}

/**
 * Gets vertices of the edge
 * @param e
 * @param kv
 */
void GeoGraph::getVerticesOfEdge(int e, int* kv) {
    kv[0] = ksv[e];
    kv[1] = kev[e];
}

/**
 * Gets vertices of the face as the triangle
 * @param face
 * @param vertices
 */
void GeoGraph::getVerticesOfTriangle(int f, int* kv) {
    int e = kfe[f];
    for (int i = 0; i < 3; i++) {
        if (f == krf[e]) {
            kv[i] = ksv[e];
            e = ksce[e];
        } else {
            kv[i] = kev[e];
            e = kece[e];
        }
    }
}

/**
 * Gets edges of the face as the triangle
 * @param face
 * @param edges
 */
void GeoGraph::getEdgesOfTriangle(int f, int* ke) {
    int e = kfe[f];
    for (int i = 0; i < 3; i++) {
        ke[i] = e;
        if (f == krf[e]) {
            e = ksce[e];
        } else {
            e = kece[e];
        }
    }
}

