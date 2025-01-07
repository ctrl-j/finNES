#pragma once

#include <FL/Fl.H>
#include <FL/Fl_Gl_Window.H>
#include <GL/glut.h>

class FrameBuffer : public Fl_Gl_Window {
public:
    unsigned int *pix; // pixel array // 0xAABBGGRR; // red 0xFF0000FF
	int w, h;

    FrameBuffer(int u0, int v0, int _w, int _h);
	void draw();
    void KeyboardHandle();
	int handle(int guievent);
    void Set(unsigned int bgr);
	void Set(int u, int v, unsigned int col);
	unsigned int Get(int u, int v);
	void SetGuarded(int u, int v, unsigned int col);
};