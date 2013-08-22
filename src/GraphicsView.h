/* 
 * Graphics view class
 *  - inherits Gtk::GL::DrawingArea and Observer class
 *  - draws vertices, edges and faces by OpenGL
 * File:   GraphicsView.h
 * Author: munehiro
 *
 * Created on August 3, 2013, 7:21 PM
 */

#ifndef GRAPHICSVIEW_H
#define	GRAPHICSVIEW_H

#include "Triangle.h"


#include <vector>
#include <gtkglextmm-1.2/gtkglmm.h>
#include "Observer.h"
#include "Vector3d.h"
#include "Edge.h"
#include "Face.h"

using namespace std;

class GraphicsView : public Gtk::GL::DrawingArea, public Observer {
public:
    GraphicsView();
    virtual ~GraphicsView();
    void update(const Subject* sbj);
protected:
    void on_realize();
    bool on_configure_event(GdkEventConfigure* evt);
    bool on_expose_event(GdkEventExpose* evt);
    bool on_button_press_event(GdkEventButton* evt);
    bool on_motion_notify_event(GdkEventMotion* evt);
private:
    void drawAxis(const float m[4][4]);
    void drawVertices();
    void drawEdges();
    void drawFaces();
    int mx, my;             // mouse point
    float currQ[4];         // current quaternion
    vector<Vector3d> va;    // vertex array
    vector<Edge> ea;        // edge array
    vector<Face> fa;        // face array

};

#endif	/* GRAPHICSVIEW_H */

