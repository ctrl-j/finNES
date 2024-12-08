#pragma once

#include <FL/Fl.H>
#include <FL/Fl_Double_Window.H>

class Display;
#include "display.hh"

extern Display* DISPLAY;

class GUI {
    public:
        Fl_Double_Window *wins;

        GUI();
        ~GUI();

};