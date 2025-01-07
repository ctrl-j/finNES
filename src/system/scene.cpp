#include "../../include/scene.hh"
#include "../../gui.h"
#include "../../include/framebuffer.hh"

#include <iostream>

Scene *scene;

using namespace std;

Scene::Scene() {


	int u0 = 20;
	int v0 = 40;
	int h = 400;
	int w = 600;
	
	fb = new FrameBuffer(u0, v0, w, h);
	fb->position(u0, v0);
	fb->label("SW Framebuffer");
	fb->show();
	fb->redraw();

	gui = new GUI();
	gui->show();
	gui->uiw->position(u0, v0 + fb->h + v0);
}

void Scene::DBG() {

}

void Scene::NewButton() {

}

void Scene::Render() {
    if (!fb)
		return;

	fb->Set(0xFFFFFFFF);

	fb->redraw();
}