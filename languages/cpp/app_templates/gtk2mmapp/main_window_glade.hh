%{HH_TEMPLATE}

// DO NOT EDIT THIS FILE ! It was created using glade--
// for gtk 2.8.3 and gtkmm 2.8.0
//
// Please modify the corresponding derived classes in ./src/main_window.hh and./src/main_window.cc

#ifndef _MAIN_WINDOW_GLADE_HH
#  define _MAIN_WINDOW_GLADE_HH


#if !defined(GLADEMM_DATA)
#define GLADEMM_DATA 
#include <gtkmm/accelgroup.h>

class GlademmData
{  
        
        Glib::RefPtr<Gtk::AccelGroup> accgrp;
public:
        
        GlademmData(Glib::RefPtr<Gtk::AccelGroup> ag) : accgrp(ag)
        {  
        }
        
        Glib::RefPtr<Gtk::AccelGroup>  getAccelGroup()
        {  return accgrp;
        }
};
#endif //GLADEMM_DATA

#include <gtkmm/window.h>

class main_window_glade : public Gtk::Window
{  
        
        GlademmData *gmm_data;
public:
        class Gtk::Window * main_window;
protected:
        
        main_window_glade();
        
        ~main_window_glade();
private:
        virtual bool quit(GdkEventAny *ev) = 0;
};
#endif
