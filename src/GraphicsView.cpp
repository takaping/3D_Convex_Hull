/* 
 * File:   GraphicsView.cpp
 * Author: munehiro
 * 
 * Created on August 3, 2013, 7:21 PM
 */

#include <algorithm>
#include <GL/freeglut.h>
#include "GraphicsView.h"
#include "GraphicsModel.h"
extern "C" {
#include "trackball.h"
}

#define AXIS        ( 0.8)  // axis position
#define AXISLEN     ( 0.2)  // axis length
#define NEARCLIP    ( 5.0)  // near clip position
#define FARCLIP     (40.0)  // far clip position
#define EYEPOINT    (-7.0)  // eye point

/**
 * Constructor and Destructor
 */
GraphicsView::GraphicsView() : mx(0), my(0) {
    currQ[0] = 0.0f; currQ[1] = 0.0f; currQ[2] = 0.0f; currQ[3] = 1.0f;
    Glib::RefPtr<Gdk::GL::Config> cfg(Gdk::GL::Config::create(Gdk::GL::MODE_RGB   |
                                                              Gdk::GL::MODE_DEPTH |
                                                              Gdk::GL::MODE_DOUBLE));
    if (!cfg) {
        cfg = Gdk::GL::Config::create(Gdk::GL::MODE_RGB |
                                      Gdk::GL::MODE_DEPTH);
        if (!cfg) {
            exit(-1);
        }
    }
    set_gl_capability(cfg);
    add_events(Gdk::BUTTON_PRESS_MASK |
               Gdk::BUTTON1_MOTION_MASK);
}

GraphicsView::~GraphicsView() {
}

/**
 * Updates view
 * @param subject
 */
void GraphicsView::update(const Subject* sbj) {
    GraphicsModel* model = (GraphicsModel*)sbj;
    if (model) {
        va = model->vertices(); // gets vertices
        ea = model->edges();    // gets vertices of the edges
        fa = model->faces();    // gets vertices and normal vector of the faces
        get_window()->invalidate_rect(get_allocation(), false);
    }
}
/**
 * On realize
 */
void GraphicsView::on_realize() {
    Gtk::DrawingArea::on_realize();
    Glib::RefPtr<Gdk::GL::Drawable> drw(get_gl_drawable());
    if (!drw->gl_begin(get_gl_context())) {
        return;
    }
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClearDepth(1.0);
    {
        // light properties
        GLfloat pos[] = { -10.0f, 10.0f, 10.0f, 0.0f };
        GLfloat spc[] = {   1.0f,  1.0f,  1.0f, 1.0f };
        GLfloat dif[] = {   1.0f,  1.0f,  1.0f, 1.0f };
        GLfloat amb[] = {   0.8f,  0.8f,  0.8f, 1.0f };
        glEnable(GL_LIGHT0);
        glLightfv(GL_LIGHT0, GL_POSITION, pos);
        glLightfv(GL_LIGHT0, GL_SPECULAR, spc);
        glLightfv(GL_LIGHT0, GL_DIFFUSE,  dif);
        glLightfv(GL_LIGHT0, GL_AMBIENT,  amb);
    }
    {
        // material properties of the face
        GLfloat spc[] = { 0.0f,  0.4f,  1.0f, 0.7f };
        GLfloat dif[] = { 0.0f,  0.2f,  0.6f, 0.7f };
        GLfloat amb[] = { 0.0f,  0.1f,  0.3f, 0.7f };
        glMaterialf(GL_FRONT, GL_SHININESS, 10.0f);
        glMaterialfv(GL_FRONT, GL_SPECULAR, spc);
        glMaterialfv(GL_FRONT, GL_DIFFUSE,  dif);
        glMaterialfv(GL_FRONT, GL_AMBIENT,  amb);
    }
    glEnable(GL_LIGHTING);
    glEnable(GL_DEPTH_TEST);
    glShadeModel(GL_SMOOTH);
    drw->gl_end();
}

/**
 * On configure event
 * @param event
 * @return 
 */
bool GraphicsView::on_configure_event(GdkEventConfigure* evt) {
    Glib::RefPtr<Gdk::GL::Drawable> drw(get_gl_drawable());
    if (!drw->gl_begin(get_gl_context())) {
        return false;
    }
    GLsizei w = get_width();
    GLsizei h = get_height();
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    GLdouble aspect = (GLdouble)w / (GLdouble)h;
    if (h < w) {
/**
        glFrustum(-aspect, aspect, -1.0, 1.0, NEARCLIP, FARCLIP);
**/
        glOrtho(-aspect, aspect, -1.0, 1.0, NEARCLIP, FARCLIP);
    } else {
        aspect = 1.0 / aspect;
/**
        glFrustum(-1.0, 1.0, -aspect, aspect, NEARCLIP, FARCLIP);
**/
        glOrtho(-1.0, 1.0, -aspect, aspect, NEARCLIP, FARCLIP);
    }
    glMatrixMode(GL_MODELVIEW);
    drw->gl_end();
    return true;
}

/**
 * On expose event
 * @param event
 * @return 
 */
bool GraphicsView::on_expose_event(GdkEventExpose* evt) {
    Glib::RefPtr<Gdk::GL::Drawable> drw(get_gl_drawable());
    if (!drw->gl_begin(get_gl_context())) {
        return false;
    }
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    // converts the current quaternion to the transformation matrix
    float m[4][4];
    build_rotmatrix(m, currQ);
    drawAxis(m);        // draws x, y and z axis
    glPushMatrix();
    glTranslated(0.0, 0.0, EYEPOINT);
    glMultMatrixf(&m[0][0]);
    drawVertices();     // draws vertices
    drawFaces();        // draws edges
    drawEdges();        // draws faces
    glPopMatrix();
    if (drw->is_double_buffered()) {
        drw->swap_buffers();
    } else {
        glFlush();
    }
    drw->gl_end();
    return true;
}

/**
 * On button press event
 * @param event
 * @return 
 */
bool GraphicsView::on_button_press_event(GdkEventButton* evt) {
    mx = (float)evt->x;
    my = (float)evt->y;
    return false;
}

/**
 * On motion notify event
 * @param event
 * @return 
 */
bool GraphicsView::on_motion_notify_event(GdkEventMotion* evt) {
    float cx = (float)evt->x;
    float cy = (float)evt->y;
    if (evt->state & GDK_BUTTON1_MASK) {
        float w = (float)this->get_width();
        float h = (float)this->get_height();
        // updates the current quaternion
        float lastQ[4];
        trackball(lastQ, (2.f * mx - w) / w,
                         (h - 2.f * my) / h,
                         (2.f * cx - w) / w,
                         (h - 2.f * cy) / h);
        add_quats(lastQ, currQ, currQ);
        get_window()->invalidate_rect(get_allocation(), false);
    }
    mx = cx;
    my = cy;
    return false;
}

/**
 * Draws x, y, z axis
 * @param transformation matrix
 */
void GraphicsView::drawAxis(const float m[4][4]) {
    glPushMatrix();
    glTranslated(AXIS, -AXIS, EYEPOINT);
    glMultMatrixf(&m[0][0]);
    glDisable(GL_LIGHTING);
    glLineWidth(2.0f);
    glBegin(GL_LINES);
    // draws x axis
    glColor3d( 1.0, 0.0, 0.0);
    glVertex3d(0.0, 0.0, 0.0);
    glVertex3d(AXISLEN, 0.0, 0.0);
    // draws y axis
    glColor3d( 0.0, 1.0, 0.0);
    glVertex3d(0.0, 0.0, 0.0);
    glVertex3d(0.0, AXISLEN, 0.0);
    // draws z axis
    glColor3d( 0.0, 0.0, 1.0);
    glVertex3d(0.0, 0.0, 0.0);
    glVertex3d(0.0, 0.0, AXISLEN);
    glEnd();
    string a[] = { "x", "y", "z" };
    glColor3d(1.0, 1.0, 1.0);
    glRasterPos3d(AXISLEN, 0.0, 0.0);
    glutBitmapString(GLUT_BITMAP_HELVETICA_12, (unsigned char*)a[0].c_str());
    glRasterPos3d(0.0, AXISLEN, 0.0);
    glutBitmapString(GLUT_BITMAP_HELVETICA_12, (unsigned char*)a[1].c_str());
    glRasterPos3d(0.0, 0.0, AXISLEN);
    glutBitmapString(GLUT_BITMAP_HELVETICA_12, (unsigned char*)a[2].c_str());
    glEnable(GL_LIGHTING);
    glPopMatrix();
}

/**
 * Draws vertices
 */
void GraphicsView::drawVertices() {
    glPushMatrix();
    glDisable(GL_LIGHTING);
    glColor3d(1.0, 0.4, 0.0);
    glPointSize(3.0f);
    int i = 0;
    for_each(va.begin(), va.end(), [&i](Vector3d v) {
        glBegin(GL_POINTS);
        glVertex3dv(v.get());
        glEnd();
/**
        ostringstream str;
        str << i;
        glRasterPos3dv((v + 0.01).get());
        glutBitmapString(GLUT_BITMAP_HELVETICA_12, (unsigned char*)str.str().c_str());
        i++;
**/
    });
    glEnable(GL_LIGHTING);
    glPopMatrix();
}

/**
 * Draws edges
 */
void GraphicsView::drawEdges() {
    glPushMatrix();
    glDisable(GL_LIGHTING);
    glColor3d(0.9, 1.0, 0.0);
    glLineWidth(1.0f);
    for_each(ea.begin(), ea.end(), [](Edge e) {
        glBegin(GL_LINES);
        glVertex3dv(e.vertex(0).get());
        glVertex3dv(e.vertex(1).get());
        glEnd();
/**
        ostringstream str;
        str << e.id();
        Vector3d v1(e.vertex(0));
        Vector3d v2(e.vertex(1));
        Vector3d cv = (v1 + v2) / 2.0;
        glRasterPos3dv(cv.get());
        glutBitmapString(GLUT_BITMAP_HELVETICA_12, (unsigned char*)str.str().c_str());
**/
    });
    glEnable(GL_LIGHTING);
    glPopMatrix();
}

/**
 * Draws faces
 */
void GraphicsView::drawFaces() {
    glPushMatrix();

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDepthMask(GL_FALSE);

    for_each(fa.begin(), fa.end(), [](Face f) {
        glBegin(GL_TRIANGLES);
        glNormal3dv(f.normal().get());
        glVertex3dv(f.vertex(0).get());
        glVertex3dv(f.vertex(1).get());
        glVertex3dv(f.vertex(2).get());
        glEnd();
    });

    glDepthMask(GL_TRUE);
    glDisable(GL_BLEND);

    glPopMatrix();
}

