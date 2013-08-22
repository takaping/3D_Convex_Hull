/* 
 * File:   MainWindow.cpp
 * Author: munehiro
 * 
 * Created on July 26, 2013, 7:42 PM
 */

#include "MainWindow.h"

/**
 * Constructor and Destructor
 */
MainWindow::MainWindow(BaseObjectType* obj, const Glib::RefPtr<Gtk::Builder> bld)
: Gtk::Window(obj) {
    view = shared_ptr<GraphicsView>(new GraphicsView);
    model.addObserver(view);
    
    // defines UI parts
    Gtk::Viewport* viewport;
    Gtk::Button*   genButton;
    Gtk::Button*   conButton;
    
    bld->get_widget("viewport",   viewport);
    bld->get_widget("spin_button", spinButton);
    bld->get_widget("gen_button", genButton);
    bld->get_widget("con_button", conButton);
    
    spinButton->set_range(7.0, 30000.0);
    spinButton->set_increments(1.0, 10.0);
    spinButton->set_value(7.0);
    
    viewport->add(*view);
    
    genButton->signal_clicked().connect(sigc::mem_fun(*this , &MainWindow::generate));
    conButton->signal_clicked().connect(sigc::mem_fun(*this , &MainWindow::construct));
    
    show_all_children();
}

MainWindow::~MainWindow() {
    model.deleteObserver(view);
}

/**
 * Generates vertices
 */
void MainWindow::generate() {
    model.generate(spinButton->get_value_as_int());
}

/**
 * Constructs the 3d convex hull
 */
void MainWindow::construct() {
    model.construct();
}

