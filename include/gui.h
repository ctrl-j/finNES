#pragma once

#include <FL/Fl.H>
#include <Fl/Fl_Box.H>
#include <FL/Fl_Double_Window.H>

class Display;
#include "display.hh"

extern Display* DISPLAY;

class GUI {
    public:
        Fl_Double_Window *wins;
        Fl_Box *msg_box;

        GUI();
        ~GUI();

};