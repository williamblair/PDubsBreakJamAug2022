#include <cstdio>

#include "Renderer.h"
#include "AIPlayer.h"

using namespace irr;

using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

AIPlayer gAiPlyr;

int main()
{
    gRender.Init();

    auto* node = gRender.LoadAnimMesh("assets/sydney.md2");
    if (node) {
        node->setMaterialFlag(EMF_LIGHTING, false);
        node->setMD2Animation(scene::EMAT_STAND);
        node->setMaterialTexture(0, gRender.GetDriver()->getTexture("assets/sydney.bmp"));
    }

    gAiPlyr.Init();

    u32 then = gRender.GetDevice()->getTimer()->getTime();

    while (gRender.GetDevice()->run())
    {
        u32 now = gRender.GetDevice()->getTimer()->getTime();
        const float frameDeltaTime = (f32)(now - then) / 1000.0f;
        then = now;

        gAiPlyr.SetNpcPos(gRender.GetCam()->getPosition());
        gAiPlyr.Update(frameDeltaTime);

        gRender.Clear();
        gRender.Update();
    }

    gRender.GetDevice()->drop();
    return 0;
}
