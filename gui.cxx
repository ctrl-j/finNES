// generated by Fast Light User Interface Designer (fluid) version 1.0304
#include <FL/Fl.H>
#include "gui.h"

void GUI::cb_DBG_i(Fl_Button*, void*) {
  DBG_cb();
}
void GUI::cb_DBG(Fl_Button* o, void* v) {
  ((GUI*)(o->parent()->user_data()))->cb_DBG_i(o,v);
}

void GUI::cb_NewButton_i(Fl_Return_Button*, void*) {
  NewButton_cb();
}
void GUI::cb_NewButton(Fl_Return_Button* o, void* v) {
  ((GUI*)(o->parent()->user_data()))->cb_NewButton_i(o,v);
}
#include "include/scene.hh"

GUI::GUI() {
  { uiw = new Fl_Double_Window(199, 197, "GUI");
    uiw->user_data((void*)(this));
    { Fl_Button* o = new Fl_Button(15, 15, 95, 40, "DBG");
      o->selection_color(FL_DARK_RED);
      o->callback((Fl_Callback*)cb_DBG);
    } // Fl_Button* o
    { Fl_Return_Button* o = new Fl_Return_Button(30, 80, 110, 100, "NewButton");
      o->callback((Fl_Callback*)cb_NewButton);
    } // Fl_Return_Button* o
    uiw->end();
  } // Fl_Double_Window* uiw
}

void GUI::show() {
  uiw->show();
}

void GUI::DBG_cb() {
  scene->DBG();
}

void GUI::NewButton_cb() {
  scene->NewButton();
}
