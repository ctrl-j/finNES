
#include <iostream>

#include "../../include/framebuffer.hh"
#include "../../gui.h"

using namespace std;

FrameBuffer::FrameBuffer(int u0, int v0, int _w, int _h) : 
	Fl_Gl_Window(u0, v0, _w, _h, 0) {

	w = _w;
	h = _h;
	pix = new unsigned int[w*h];
}

void FrameBuffer::draw() {
	glDrawPixels(w, h, GL_RGBA, GL_UNSIGNED_BYTE, pix);
	//glReadPixels(0, 0, w, h, GL_RGBA, GL_UNSIGNED_BYTE, pix);
}

int FrameBuffer::handle(int event) {

	switch (event)
	{
	case FL_KEYBOARD: {
		KeyboardHandle();
		return 0;
	}
	case FL_MOVE: {
		int u = Fl::event_x();
		int v = Fl::event_y();
		if (u < 0 || u > w - 1 || v < 0 || v > h - 1)
			return 0;

		cerr << u << " " << v << " " << "         \r";
		return 0;
	}
	default:
		return 0;
	}
	return 0;
}

void FrameBuffer::KeyboardHandle() {

	int key = Fl::event_key();
	switch (key) {
	case FL_Left: {
		cerr << "INFO: pressed left arrow key";
		break;
	}
	default:
		cerr << "INFO: do not understand keypress" << endl;
		return;
	}

}

void FrameBuffer::Set(unsigned int bgr) {

	for (int v = 0; v < h; v++) {
		for (int u = 0; u < w; u++) {
			pix[(h - 1 - v)*w + u] = bgr;
		}
	}

}

void FrameBuffer::SetGuarded(int u, int v, unsigned int col) {

	if (u < 0 || u > w - 1 || v < 0 || v > h - 1)
		return;

	Set(u, v, col);

}

void FrameBuffer::Set(int u, int v, unsigned int col) {

	pix[(h - 1 - v)*w + u] = col;

}

unsigned int FrameBuffer::Get(int u, int v) {

	return pix[(h - 1 - v)*w + u];

}
