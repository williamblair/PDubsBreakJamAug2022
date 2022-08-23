#include <cstdio>

#include "Renderer.h"
#include "AIPlayer.h"
#include "GameStatManager.h"
#include "TitleScreen.h"

using namespace irr;

using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

AIPlayer gAiPlyr;

static bool RunTitleScreen()
{
    TitleScreen ts;
    ts.Init();
    TitleScreen::Action act = ts.Run();
    switch (act)
    {
    case TitleScreen::ACTION_QUIT:
        return false;
    default:
        break;
    }
    return true;
}

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
    
    if (!RunTitleScreen()) { return 0; }

    u32 then = gRender.GetDevice()->getTimer()->getTime();
    while (gRender.GetDevice()->run())
    {
        // logic updates
        u32 now = gRender.GetDevice()->getTimer()->getTime();
        const float frameDeltaTime = (f32)(now - then) / 1000.0f;
        then = now;

        gAiPlyr.SetNpcPos(gRender.GetCam()->getPosition());
        gAiPlyr.Update(frameDeltaTime);

        gGameStatMgr.UpdateGameTime(frameDeltaTime);


        // drawing
        gRender.Clear();
        gRender.Update();
    }

    gRender.GetDevice()->drop();
    return 0;
}
