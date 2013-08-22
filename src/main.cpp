/* 
 * File:   main.cpp
 * Author: munehiro
 *
 * Created on July 26, 2013, 7:31 PM
 */

#include <gtkmm-2.4/gtkmm.h>
#include <gtkglextmm-1.2/gtkglmm.h>
#include <GL/freeglut.h>
#include "MainWindow.h"

/*
 * 
 */
int main(int argc, char** argv) {
    Gtk::Main kit(argc, argv);
    Gtk::GL::init(argc, argv);
    glutInit(&argc, argv);
    
    Glib::RefPtr<Gtk::Builder> bldr;
    MainWindow* mainWindow = nullptr;
    try {
        bldr = Gtk::Builder::create_from_file("MainWindow.glade");
        bldr->get_widget_derived("main_window", mainWindow);
        if (mainWindow) {
            kit.run(*mainWindow);
        }
    } catch (const Glib::FileError& ex) {
        return -1;
    }
    return 0;
}

