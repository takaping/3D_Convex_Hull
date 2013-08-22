/* 
 * Main window class
 *  - instantiates the model and the view object
 * File:   MainWindow.h
 * Author: munehiro
 *
 * Created on July 26, 2013, 7:42 PM
 */

#ifndef MAINWINDOW_H
#define	MAINWINDOW_H

#include <memory>
#include <gtkmm-2.4/gtkmm.h>
#include "GraphicsModel.h"
#include "GraphicsView.h"

using namespace std;

class MainWindow : public Gtk::Window {
public:
    MainWindow(BaseObjectType* obj, const Glib::RefPtr<Gtk::Builder> bld);
    virtual ~MainWindow();
private:
    void generate();
    void construct();
    Gtk::SpinButton* spinButton;    // spin button
    GraphicsModel model;            // model of this application
    shared_ptr<GraphicsView> view;  // view of this application

};

#endif	/* MAINWINDOW_H */

