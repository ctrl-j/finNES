#pragma once

class FrameBuffer;
#include "framebuffer.hh"
class GUI;
#include "../gui.h"

class Scene {
    public:
        GUI *gui;
	    FrameBuffer *fb;

        Scene();
        void DBG();
        void NewButton();
        void Render();
};

extern Scene *scene;