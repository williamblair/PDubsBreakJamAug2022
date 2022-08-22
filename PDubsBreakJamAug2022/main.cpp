#include <cstdio>

#include "Renderer.h"


using namespace irr;

using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

static Renderer gRender;

int main()
{
    gRender.Init();

    auto* node = gRender.LoadAnimMesh("assets/sydney.md2");

    if (node) {
        node->setMaterialFlag(EMF_LIGHTING, false);
        node->setMD2Animation(scene::EMAT_STAND);
        node->setMaterialTexture(0, gRender.GetDriver()->getTexture("assets/sydney.bmp"));
    }

    while (gRender.GetDevice()->run())
    {
        gRender.Clear();
        gRender.Update();
    }

    gRender.GetDevice()->drop();
    return 0;
}
