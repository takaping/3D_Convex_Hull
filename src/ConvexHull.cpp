/* 
 * File:   ConvexHull.cpp
 * Author: munehiro
 * 
 * Created on August 4, 2013, 9:10 PM
 */

#include <algorithm>
#include "Memory.h"
#include "ConvexHull.h"

#define PREC    (1.0e-6)        // precision
#define SCALE   (1.0 / PREC)    // scale
#define NTV     (4)            // number of tetrahedron vertices
#define NDV     (3)            // number of dihedron vertices

/**
 * Constructor and Destructor
 */
ConvexHull::ConvexHull() {
}

ConvexHull::~ConvexHull() {
}

/**
 * Clears lists
 */
void ConvexHull::clear() {
    GeoGraph::clear();
    hva.clear();
    kch.clear();
    khnv.clear();
    khpv.clear();
    kcnxv.clear();
    kccnxv.clear();
}

/**
 * Constructs the 3d convex hull
 * @param vertex array
 */
void ConvexHull::construct(const vector<Vector3d>& va) {
    clear();
    // copies vertex array
    for_each(va.begin(), va.end(), [&](Vector3d v) {
        hva.push_back(v * SCALE);
    });
    // constructs initial convex hulls
    constInitHulls();
    // merges all convex hulls
    mergeAllHulls();
}

/**
 * Constructs initial convex hulls
 */
void ConvexHull::constInitHulls() {
    int nv = hva.size();
    int ntv = nv - (NDV - (nv - 1) % NTV) * NDV;
    // constructs tetrahedra
    for (int i = 0; i < ntv; i += NTV) {
        constTetrahedron(i);
    }
    // constructs dihedra
    for (int i = ntv; i < nv; i += NDV) {
        constDihedron(i);
    }
}

/**
 * Constructs the tetrahedron
 * @param vertex of the tetrahedron
 */
void ConvexHull::constTetrahedron(int iv) {
    int kv[] = { iv, iv+1, iv+2, iv+3 };
    Vector3d va[] = { hva[kv[0]], hva[kv[1]], hva[kv[2]], hva[kv[3]] };
    if (determ(kv, va)) {
        kv[2] = iv + 3;
        kv[3] = iv + 2;
    }
    
    // entries cyclic list of vertices
    for (int i = 0; i < NTV; i++) {
        int nxv = kv[(i+1)%NTV];
        khnv[kv[i]] = nxv;
        khpv[nxv]  = kv[i];
    }
    
    // gets index of new edges and faces
    int ke[] = { newEdge(), newEdge(), newEdge(), newEdge(), newEdge(), newEdge() };
    int kf[] = { newFace(), newFace(), newFace(), newFace() };
    
    // connects faces to edges
    kfe[kf[0]] = ke[0];
    kfe[kf[1]] = ke[1];
    kfe[kf[2]] = ke[2];
    kfe[kf[3]] = ke[3];
    
    // connects vertices to edges
    kve[kv[0]] = ke[0];
    kve[kv[1]] = ke[0];
    kve[kv[2]] = ke[4];
    kve[kv[3]] = ke[4];
    
    // connects edges to start and end vertices
    ksv[ke[0]] = kv[1];
    ksv[ke[1]] = kv[2];
    ksv[ke[2]] = kv[3];
    ksv[ke[3]] = kv[1];
    ksv[ke[4]] = kv[2];
    ksv[ke[5]] = kv[3];
    kev[ke[0]] = kv[0];
    kev[ke[1]] = kv[0];
    kev[ke[2]] = kv[0];
    kev[ke[3]] = kv[2];
    kev[ke[4]] = kv[3];
    kev[ke[5]] = kv[1];
    
    // connects edges to left and right faces
    klf[ke[0]] = kf[2];
    klf[ke[1]] = kf[0];
    klf[ke[2]] = kf[1];
    klf[ke[3]] = kf[0];
    klf[ke[4]] = kf[1];
    klf[ke[5]] = kf[2];
    krf[ke[0]] = kf[0];
    krf[ke[1]] = kf[1];
    krf[ke[2]] = kf[2];
    krf[ke[3]] = kf[3];
    krf[ke[4]] = kf[3];
    krf[ke[5]] = kf[3];

    // connects edges to contiguous edges
    ksce[ke[0]]  = ke[3];
    ksce[ke[1]]  = ke[4];
    ksce[ke[2]]  = ke[5];
    ksce[ke[3]]  = ke[5];
    ksce[ke[4]]  = ke[3];
    ksce[ke[5]]  = ke[4];
    kscce[ke[0]] = ke[5];
    kscce[ke[1]] = ke[3];
    kscce[ke[2]] = ke[4];
    kscce[ke[3]] = ke[0];
    kscce[ke[4]] = ke[1];
    kscce[ke[5]] = ke[2];
    kece[ke[0]]  = ke[2];
    kece[ke[1]]  = ke[0];
    kece[ke[2]]  = ke[1];
    kece[ke[3]]  = ke[1];
    kece[ke[4]]  = ke[2];
    kece[ke[5]]  = ke[0];
    kecce[ke[0]] = ke[1];
    kecce[ke[1]] = ke[2];
    kecce[ke[2]] = ke[0];
    kecce[ke[3]] = ke[4];
    kecce[ke[4]] = ke[5];
    kecce[ke[5]] = ke[3];
    
    // entries cyclic list of vertices on the silhouette of the convex hull
    // entries a left most vertex
    kch.push_back(searchSilhouette(NTV, kv));
}

/**
 * Constructs the dihedron
 * @param vertex the dihedron
 */
void ConvexHull::constDihedron(int iv) {
    int kv[] = { iv, iv+1, iv+2 };
    
    // entries cyclic list of vertices
    for (int i = 0; i < NDV; i++) {
        int nxv = kv[(i+1)%NDV];
        khnv[kv[i]] = nxv;
        khpv[nxv]  = kv[i];
    }
    
    // gets index of new edges and faces
    int ke[] = { newEdge(), newEdge(), newEdge() };
    int kf[] = { newFace(), newFace() };
    
    // connects faces to edges
    kfe[kf[0]] = ke[0];
    kfe[kf[1]] = ke[0];
    
    // connects vertices to edges
    kve[kv[0]] = ke[0];
    kve[kv[1]] = ke[1];
    kve[kv[2]] = ke[2];
    
    // connects edges to start and end vertices
    ksv[ke[0]] = kv[0];
    ksv[ke[1]] = kv[1];
    ksv[ke[2]] = kv[2];
    kev[ke[0]] = kv[1];
    kev[ke[1]] = kv[2];
    kev[ke[2]] = kv[0];
    
    // connects edges to left and right faces
    klf[ke[0]] = kf[1];
    klf[ke[1]] = kf[1];
    klf[ke[2]] = kf[1];
    krf[ke[0]] = kf[0];
    krf[ke[1]] = kf[0];
    krf[ke[2]] = kf[0];

    // connects edges to contiguous edges
    ksce[ke[0]]  = ke[2];
    ksce[ke[1]]  = ke[0];
    ksce[ke[2]]  = ke[1];
    kscce[ke[0]] = ke[2];
    kscce[ke[1]] = ke[0];
    kscce[ke[2]] = ke[1];
    kece[ke[0]]  = ke[1];
    kece[ke[1]]  = ke[2];
    kece[ke[2]]  = ke[0];
    kecce[ke[0]] = ke[1];
    kecce[ke[1]] = ke[2];
    kecce[ke[2]] = ke[0];
    
    // entries cyclic list of vertices on the silhouette of the convex hull
    // entries a left most vertex
    kch.push_back(searchSilhouette(NDV, kv));
}

/**
 * Searches vertices on the silhouette of the convex hull
 * @param number of vertices
 * @param vertices
 * @return left most vertex on the silhouette of the convex hull
 */
int ConvexHull::searchSilhouette(int nv, int* kv0) {
    int iv0 = 0;
    for (int i = 0; i < nv; i++) {
        iv0 = kv0[i];
        int iv = iv0;
        bool found = false;
        do {
            int e0 = kve[iv];
            int e = e0;
            bool or2 = isFront(rightFace(iv, e));
            found = false;
            do {
                bool or1 = isFront(leftFace(iv, e));
                // if the silhouette edge is found, entries vertex to cyclic list
                if (!or1 && or2) {
                    int nxv = otherVertex(iv, e);
                    kcnxv[iv] = nxv;
                    kccnxv[nxv] = iv;
                    iv = nxv;
                    found = true;
                    break;
                }
                e = nextCCWEdge(iv, e);
                or2 = or1;
            } while (e != e0);
            if (!found) {
                break;
            }
        } while (iv != iv0);
        if (found) {
            break;
        }
        // if silhouette edge is not found, the vertex is not on the silhouette edge
        // then, searches next vertex
    }
    return iv0;
}

/**
 * Merges all convex hulls
 */
void ConvexHull::mergeAllHulls() {
    int nch = kch.size();
    // computes until the 1 convex hull
    while (nch > 1) {
        // initializes property of edges and faces
        initProperty();
        // merges 2 adjacent convex hulls
        for (int i = 0; i < (nch - 1); i += 2) {
            merge2Hulls(kch[i], kch[i+1]);
        }
        // deletes index of merged convex hulls
        int ir = (nch % 2 == 0 ? nch-1 : nch-2);
        vector<int>::iterator head = kch.begin();
        for (int i = ir; i >= 0; i -= 2) {
            kch.erase(head+i);
        }
        nch = kch.size();
    }
}

/**
 * Initializes property of edges and faces
 */
void ConvexHull::initProperty() {
    kep.clear();
    for_each(ksv.begin(), ksv.end(), [this](unordered_map<int, int>::value_type k) {
        kep[k.first] = PrimProperty::NOTDEFINED;
    });

    kfp.clear();
    for_each(kfe.begin(), kfe.end(), [this](unordered_map<int, int>::value_type k) {
        kfp[k.first] = PrimProperty::NOTDEFINED;
    });
}

/**
 * Merges 2 adjacent convex hulls
 * @param vertex of the left convex hull
 * @param vertex of the right convex hull
 */
void ConvexHull::merge2Hulls(int liv0, int riv0) {
    int liv = liv0;
    int livr = liv0;
    // searches the right most vertex of the left convex hull
    do {
        liv = kcnxv[liv];
        if (hva[livr].x() < hva[liv].x()) {
            livr = liv;
        }
    } while (liv != liv0);
    int liv1 = livr;  int liv2 = livr;
    int riv1 = riv0;  int riv2 = riv0;
    // searches the common tangent edge
    searchCTEdge(liv1, kccnxv, riv1, kcnxv , false);
    searchCTEdge(liv2, kcnxv , riv2, kccnxv, true);
    kcnxv[liv1]  = riv1;
    kccnxv[riv1] = liv1;
    kcnxv[riv2]  = liv2;
    kccnxv[liv2] = riv2;
    int cte = newEdge();
    ksv[cte] = liv1;
    kev[cte] = riv1;
    kep[cte] = PrimProperty::NEW;
    // wraps 2 convex hulls in cylindrical
    wrapInCylindrical(cte);
    // deletes non convex hull primitives
    deleteNonHullPrims(liv1, cte, ScanDir::CW);
    deleteNonHullPrims(riv1, cte, ScanDir::CCW);
    // updates convex hull primitives
    updatePrimitives(cte);
}

/**
 * Searches the common tangent edge
 * @param vertex of left convex hull
 * @param cyclic list of vertices on the silhouette of the left convex hull
 * @param vertex of right convex hull
 * @param cyclic list of vertices on the silhouette of the right convex hull
 * @param turnning direction
 */
void ConvexHull::searchCTEdge(int& liv, unordered_map<int, int>& klnxv, int& riv, unordered_map<int, int>& krnxv, bool dir) {
    bool changed = false;
    do {
        changed = false;
        // searches the vertex of the common tangent edge on the right convex hull
        while (1) {
            int nxv = krnxv[riv];
            int kv[] = { liv, riv, nxv, (int)hva.size() };
            Vector3d va[] = { Vector3d(hva[kv[0]].x(), hva[kv[0]].y(), 0.0),
                              Vector3d(hva[kv[1]].x(), hva[kv[1]].y(), 0.0),
                              Vector3d(hva[kv[2]].x(), hva[kv[2]].y(), 0.0),
                              Vector3d(hva[kv[2]].x(), hva[kv[2]].y(), SCALE) };
            if (determ(kv, va) == dir) {
                break;
            }
            krnxv.erase(riv);
            riv = nxv;
            changed = true;
        }
        // searches the vertex of the common tangent edge on the left convex hull
        while (1) {
            int nxv = klnxv[liv];
            int kv[] = { riv, liv, nxv, (int)hva.size() };
            Vector3d va[] = { Vector3d(hva[kv[0]].x(), hva[kv[0]].y(), 0.0),
                              Vector3d(hva[kv[1]].x(), hva[kv[1]].y(), 0.0),
                              Vector3d(hva[kv[2]].x(), hva[kv[2]].y(), 0.0),
                              Vector3d(hva[kv[2]].x(), hva[kv[2]].y(), SCALE) };
            if (determ(kv, va) != dir) {
                break;
            }
            klnxv.erase(liv);
            liv = nxv;
            changed = true;
        }
    } while (changed);
}

/**
 * Wraps 2 convex hulls in cylindrical
 * @param common tangent edge
 */
void ConvexHull::wrapInCylindrical(int cte0) {
    int cte1 = cte0;
    int cte2 = cte0;
    int liv0 = ksv[cte0];
    int riv0 = kev[cte0];
    int liv1 = liv0;
    int riv1 = riv0;
    do {
        cte1 = cte2;
        
        // searches the edge of the common tangent face
        int le = searchEdgeOfCTFace(liv1, riv1, ScanDir::CW);
        int re = searchEdgeOfCTFace(riv1, liv1, ScanDir::CCW);
        int liv2 = (liv1 == ksv[le] ? kev[le] : ksv[le]);
        int riv2 = (riv1 == ksv[re] ? kev[re] : ksv[re]);
        
        // searches the exterior edge of the common tangent face
        int kv[] = { riv1, liv1, riv2, liv2 };
        Vector3d va[] = { hva[kv[0]], hva[kv[1]], hva[kv[2]], hva[kv[3]] };
        bool lext = determ(kv, va);
        if (lext) {
            kep[le] = PrimProperty::BOUNDARY;
            liv1 = liv2;
        } else {
            kep[re] = PrimProperty::BOUNDARY;
            riv1 = riv2;
        }
        // entries new common tangent face
        int f = newFace();
        kfp[f] = PrimProperty::NEW;
        if (liv1 == liv0 && riv1 == riv0) {
            cte2 = cte0;
        } else {
            cte2 = newEdge();
            kep[cte2] = PrimProperty::NEW;
        }
        klf[cte2] = f;
        krf[cte1] = f;
        kfe[f]    = cte2;
        ksv[cte2] = liv1;
        kev[cte2] = riv1;
        if (lext) {
            kscce[cte2] = le;
            ksce[cte1]  = le;
            kece[cte2]  = cte1;
            kecce[cte1] = cte2;
        } else {
            kece[cte2]  = re;
            kecce[cte1] = re;
            kscce[cte2] = cte1;
            ksce[cte1]  = cte2;
        }
    } while (cte2 != cte0);
}

/**
 * Searches the edge of the common tangent face
 * @param target vertex
 * @param eye point
 * @param scan direction
 * @return edge of the common tangent face
 */
int ConvexHull::searchEdgeOfCTFace(int iv0, int eye, ScanDir dir) {
    int e = kve[iv0];
    bool or2 = false;
    if (dir == ScanDir::CW) {
        or2 = isFront(leftFace(iv0, e), eye);
    } else {
        or2 = isFront(rightFace(iv0, e), eye);
    }
    while (1) {
        bool or1 = false;
        if (dir == ScanDir::CW) {
            or1 = isFront(rightFace(iv0, e), eye);
        } else {
            or1 = isFront(leftFace(iv0, e), eye);
        }
        // if the edge between 2 adjacent faces is the ridge line for the eye point,
        // that is the edge of the common tangent face
        if (!or1 && or2) {
            break;
        }
        if (dir == ScanDir::CW) {
            e = nextCWEdge(iv0, e);
        } else {
            e = nextCCWEdge(iv0, e);
        }
        or2 = or1;
    }
    return e;
}

/**
 * Searches and deletes non convex hull primitives
 * @param vertex of the common tangent edge
 * @param common tangent edge
 * @param scan direction
 */
void ConvexHull::deleteNonHullPrims(int iv0, int cte0, ScanDir dir) {
    int e = cte0;
    do {
        e = (dir == ScanDir::CW ? nextCWEdge(iv0, e) : nextCCWEdge(iv0, e));
        
        // deletes all primitives
        if (e == cte0) {
            deleteAllPrimitives(iv0);
            break;
        }
    } while (kep[e] != PrimProperty::BOUNDARY);
    
    // deletes interior primitives
    if (kep[e] == PrimProperty::BOUNDARY) {
        deleteIntPrimitives(dir == ScanDir::CW ? leftFace(iv0, e) : rightFace(iv0, e));
    }
}

/**
 * Deletes all primitives on the convex hull
 * @param vertex of the common tangent edge
 */
void ConvexHull::deleteAllPrimitives(int iv0) {
    // searches edges and faces to delete
    vector<int> ketd;
    vector<int> kftd;
    int f0 = krf[kve[iv0]];
    kfp[f0] = PrimProperty::DELETE;
    kftd.push_back(f0);
    for (unsigned int i = 0; i < kftd.size(); i++) {
        int f1 = kftd[i];
        int ke[3];
        getEdgesOfTriangle(f1, ke);
        for (int j = 0; j < 3; j++) {
            int e = ke[j];
            if (kep[e] == PrimProperty::NOTDEFINED) {
                ketd.push_back(e);
                kep[e] = PrimProperty::DELETE;
                int f2 = (f1 == krf[e] ? klf[e] : krf[e]);
                if (kfp[f2] != PrimProperty::DELETE) {
                    kfp[f2] = PrimProperty::DELETE;
                    kftd.push_back(f2);
                }
            }
        }
    }
    
    // searches vertices to delete
    vector<int> kvtd;
    int iv = khnv[iv0];
    do {
      kvtd.push_back(iv);
      iv = khnv[iv];
    } while (iv != iv0);
    
    // executes delete of primitives
    deletePrimitives(kvtd, ketd, kftd);
}

/**
 * Deletes interior primitives on the convex hull
 * @param interior face
 */
void ConvexHull::deleteIntPrimitives(int f0) {
    // searches edges and faces to delete
    vector<int> ketd;
    vector<int> kftd;
    kfp[f0] = PrimProperty::DELETE;
    kftd.push_back(f0);
    for (unsigned int i = 0; i < kftd.size(); i++) {
        int f1 = kftd[i];
        int ke[3];
        getEdgesOfTriangle(f1, ke);
        for (int j = 0; j < 3; j++) {
            int e = ke[j];
            if (kep[e] == PrimProperty::NOTDEFINED) {
                ketd.push_back(e);
                kep[e] = PrimProperty::DELETE;
                int f2 = (f1 == krf[e] ? klf[e] : krf[e]);
                if (kfp[f2] != PrimProperty::DELETE) {
                    kfp[f2] = PrimProperty::DELETE;
                    kftd.push_back(f2);
                }
            }
        }
    }
    
    // searches vertices to delete
    vector<int> kvtd;
    int iv0 = ksv[kfe[f0]];
    int iv = iv0;
    do {
        int e0 = kve[iv];
        int e = e0;
        bool deleted = true;
        do {
            if (kep[e] == PrimProperty::DELETE) {
                e = nextCCWEdge(iv, e);
            } else {
                deleted = false;
            }
        } while (e != e0 && deleted == true);
        if (deleted == true) {
            kvtd.push_back(iv);
        }
        iv = khnv[iv];
    } while (iv != iv0);
    
    // executes delete of primitives
    deletePrimitives(kvtd, ketd, kftd);
}

/**
 * Executes delete of primitives
 * @param vertices to delete
 * @param edges to delete
 * @param faces to delete
 */
void ConvexHull::deletePrimitives(vector<int>& kv, vector<int>& ke, vector<int>& kf) {
    // deletes faces
    for_each(kf.begin(), kf.end(), [this](int f) {
        kfe.erase(f);
    });
    // deletes edges
    for_each(ke.begin(), ke.end(), [this](int e) {
        ksv.erase(e);
        kev.erase(e);
        krf.erase(e);
        klf.erase(e);
        ksce.erase(e);
        kscce.erase(e);
        kece.erase(e);
        kecce.erase(e);
    });
    // deletes vertices
    for_each(kv.begin(), kv.end(), [this](int iv) {
        kve.erase(iv);
        int nv = khnv[iv];
        int pv = khpv[iv];
        khnv[pv] = nv;
        khpv[nv] = pv;
        khnv.erase(iv);
        khpv.erase(iv);
    });
}

/**
 * Updates convex hull primitives
 * @param cte0
 */
void ConvexHull::updatePrimitives(int cte0) {
    // updates edges and faces
    int cte1 = cte0;
    int cte2 = cte0;
    do {
        cte1 = cte2;
        int sv = ksv[cte1];
        int ev = kev[cte1];
        kve[sv] = cte1;
        kve[ev] = cte1;
        if (kep[ksce[cte1]] == PrimProperty::NEW) {
            cte2 = ksce[cte1];
            int e = kecce[cte1];
            if (ksv[e] == ev) {
                ksce[e]  = cte1;
                kecce[e] = cte2;
                krf[e]   = krf[cte1];
            } else {
                kece[e]  = cte1;
                kscce[e] = cte2;
                klf[e]   = krf[cte1];
            }
        } else {
            cte2 = kecce[cte1];
            int e = ksce[cte1];
            if (ksv[e] == sv) {
                kscce[e] = cte1;
                kece[e]  = cte2;
                klf[e]   = krf[cte1];
            } else {
                ksce[e]  = cte2;
                kecce[e] = cte1;
                krf[e]   = krf[cte1];
            }
        }
    } while (cte2 != cte0);

    // updates vertices
    int sv = ksv[cte0];
    int ev = kev[cte0];
    int nsv = khnv[sv];
    int nev = khnv[ev];
    khnv[sv] = nev;
    khnv[ev] = nsv;
    khpv[nsv] = ev;
    khpv[nev] = sv;
}

/**
 * Is the face front
 * @param face
 * @return 
 */
bool ConvexHull::isFront(int f) {
    int kv[4];
    getVerticesOfTriangle(f, kv);
    kv[3] = hva.size();
    Vector3d va[] = { Vector3d(hva[kv[0]].x(), hva[kv[0]].y(), 0.0),
                      Vector3d(hva[kv[1]].x(), hva[kv[1]].y(), 0.0),
                      Vector3d(hva[kv[2]].x(), hva[kv[2]].y(), 0.0),
                      Vector3d(hva[kv[2]].x(), hva[kv[2]].y(), SCALE) };
    return determ(kv, va);
}

/**
 * Is the face front for the eye point
 * @param face
 * @return 
 */
bool ConvexHull::isFront(int f, int eye) {
    int kv[4];
    getVerticesOfTriangle(f, kv);
    kv[3] = eye;
    Vector3d va[] = { hva[kv[0]], hva[kv[1]], hva[kv[2]], hva[kv[3]] };
    return determ(kv, va);
}

/**
 * Is left the direction of 4 vertices
 * @param id of vertices
 * @param vertices
 * @return left or not
 */
bool ConvexHull::determ(const int* kv0, const Vector3d* va0) {
    int* kv = new int[4];
    Vector3d* va = new Vector3d[4];
    copy(kv0, kv0 + 4, kv);
    copy(va0, va0 + 4, va);
    // sorts vertices by id
    bool even = sortById(kv, va);
    // calculates determinant
    bool posi = determ(va);
    Memory::releaseArray(&kv);
    Memory::releaseArray(&va);
    return !(even ^ posi);
}

/**
 * Sorts vertices by id
 * @param id of vertices
 * @param vertices
 * @return even or not of swap times
 */
bool ConvexHull::sortById(int* kv, Vector3d* va) {
    bool even = true;
    for (int i = (4 - 2); i >= 0; i--) {
        for (int j = 0; j <= i; j++) {
            if (kv[j] > kv[j+1]) {
                swap(kv[j], kv[j+1]);
                swap(va[j], va[j+1]);
                even = !even;
            }
        }
    }
    return even;
}

/**
 * Calculates determinant
 * @param vertices
 * @return positive or not of determinant
 */
bool ConvexHull::determ(const Vector3d* va) {
    Vector3d dij = va[1] - va[0];
    Vector3d dik = va[2] - va[0];
    Vector3d dil = va[3] - va[0];
    Vector3d djk = va[2] - va[1];
    Vector3d djl = va[3] - va[1];
    Vector3d dkl = va[3] - va[2];

    // Calculates in floating point number
    int is = 0;
    double df[][3] = {{dij.x(), dij.y(), dij.z()},
                      {dik.x(), dik.y(), dik.z()},
                      {dil.x(), dil.y(), dil.z()}};
    if ((is = determ(df)) != 0)    { return (is > 0 ? true : false); }

    // Calculates in integer and symbol perturbation
    INT128 d0[][3] = {{ (INT128)dij.x(), (INT128)dij.y(), (INT128)dij.z() },
                      { (INT128)dik.x(), (INT128)dik.y(), (INT128)dik.z() },
                      { (INT128)dil.x(), (INT128)dil.y(), (INT128)dil.z() }};
    if ((is = determ(d0)) != 0)    { return (is > 0 ? true : false); }
    
    INT128 d1[][2] = {{ (INT128)djk.y(), (INT128)djk.z() },
                      { (INT128)djl.y(), (INT128)djl.z() }};
    if ((is = -determ(d1)) != 0)   { return (is > 0 ? true : false); }
    
    INT128 d2[][2] = {{ (INT128)dik.y(), (INT128)dik.z() },
                      { (INT128)dil.y(), (INT128)dil.z() }};
    if ((is = determ(d2)) != 0)    { return (is > 0 ? true : false); }
    
    INT128 d3[][2] = {{ (INT128)dij.y(), (INT128)dij.z() },
                      { (INT128)dil.y(), (INT128)dil.z() }};
    if ((is = -determ(d3)) != 0)   { return (is > 0 ? true : false); }
    
    INT128 d4[][2] = {{ (INT128)djk.x(), (INT128)djk.z() },
                      { (INT128)djl.x(), (INT128)djl.z() }};
    if ((is = determ(d4)) != 0)    { return (is > 0 ? true : false); }

    if ((is = -(int)dkl.z()) != 0) { return (is > 0 ? true : false); }

    if ((is = (int)djl.z()) != 0)  { return (is > 0 ? true : false); }
    
    INT128 d7[][2] = {{ (INT128)dik.x(), (INT128)dik.z() },
                      { (INT128)dil.x(), (INT128)dil.z() }};
    if ((is = -determ(d7)) != 0)   { return (is > 0 ? true : false); }

    if ((is = -(int)dil.z()) != 0) { return (is > 0 ? true : false); }
    
    INT128 d9[][2] = {{ (INT128)dij.x(), (INT128)dij.z() },
                      { (INT128)dil.x(), (INT128)dil.z() }};
    if ((is = determ(d9)) != 0)    { return (is > 0 ? true : false); }
    
    INT128 d10[][2] = {{ (INT128)djk.x(), (INT128)djk.y() },
                       { (INT128)djl.x(), (INT128)djl.y() }};
    if ((is = -determ(d10)) != 0)  { return (is > 0 ? true : false); }

    if ((is = (int)dkl.y()) != 0)  { return (is > 0 ? true : false); }

    if ((is = -(int)djl.y()) != 0) { return (is > 0 ? true : false); }

    if ((is = -(int)dkl.x()) != 0) { return (is > 0 ? true : false); }
    
    return true;
}

/**
 * Calculates determinant in floating point number
 * @param 3x3 matrix
 * @return positive or not of determinant
 */
int ConvexHull::determ(const double (*m)[3]) {
    double h[] = { m[0][0] * m[1][1] * m[2][2],
                   m[1][0] * m[2][1] * m[0][2],
                   m[2][0] * m[0][1] * m[1][2],
                   m[2][0] * m[1][1] * m[0][2],
                   m[1][0] * m[0][1] * m[2][2],
                   m[0][0] * m[2][1] * m[1][2] };
    double hmax = fabs(h[0]);
    for (int i = 1; i < 6; i++) {
        double ah = fabs(h[i]);
        if (hmax < ah) {
            hmax = ah;
        }
    }
    hmax = (hmax > 1.0 ? hmax : 1.0);
    double vthr = PREC * hmax;
    double vdet = h[0] + h[1] + h[2] - h[3] - h[4] - h[5];
    int is = 0;
    if (fabs(vdet) < vthr) {
        is = 0;
    } else if (vdet > 0.0) {
        is = 1;
    } else {
        is = -1;
    }
    return is;
}

/**
 * Calculates determinant in integer
 * @param 3x3 matrix
 * @return positive or not of determinant
 */
int ConvexHull::determ(const INT128 (*m)[3]) {
    INT128 vdet = m[0][0] * m[1][1] * m[2][2] +
                  m[1][0] * m[2][1] * m[0][2] +
                  m[2][0] * m[0][1] * m[1][2] -
                  m[2][0] * m[1][1] * m[0][2] -
                  m[1][0] * m[0][1] * m[2][2] -
                  m[0][0] * m[2][1] * m[1][2];
    int is = 0;
    if (vdet == 0) {
        is = 0;
    } else if (vdet > 0) {
        is = 1;
    } else {
        is = -1;
    }
    return is;
}

/**
 * Calculates determinant in integer
 * @param 2x2 matrix
 * @return positive or not of determinant
 */
int ConvexHull::determ(const INT128 (*m)[2]) {
    INT128 vdet = m[0][0] * m[1][1] -
                  m[1][0] * m[0][1];
    int is = 0;
    if (vdet == 0) {
        is = 0;
    } else if (vdet > 0) {
        is = 1;
    } else {
        is = -1;
    }
    return is;
}

