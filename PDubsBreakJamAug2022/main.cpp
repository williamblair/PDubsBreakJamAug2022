#include <cstdio>

#include "Renderer.h"
#include "AIPlayer.h"
#include "GameStatManager.h"
#include "TitleScreen.h"
#include "EndGameScreen.h"
#include "InputManager.h"
#include "GameMap.h"
#include "GameHud.h"

using namespace irr;

using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

GameMap gMap;
GameHud gHud;

static bool gRunning = true;

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
static bool RunEndGameScreen()
{
    EndGameScreen egs;
    egs.Init();
    EndGameScreen::Action act = egs.Run();
    switch (act)
    {
    case EndGameScreen::ACTION_QUIT:
        return false;
    default:
        break;
    }
    return true;
}

int main()
{
    gRender.Init();

    //auto* node = gRender.LoadAnimMesh("assets/sydney.md2");
    //if (node) {
    //    node->setMaterialFlag(EMF_LIGHTING, false);
    //    node->setMD2Animation(scene::EMAT_STAND);
    //    node->setMaterialTexture(0, gRender.GetDriver()->getTexture("assets/sydney.bmp"));
    //}

    gAIPlayer.Init();
    gMap.Init();
    gHud.Init();

    while (gRunning)
    {
        if (!RunTitleScreen()) { break; }
        // TODO - high scores, then back to title screen

        bool mainGameRunning = true;
        u32 then = gRender.GetDevice()->getTimer()->getTime();
        while (mainGameRunning)
        {
            // logic updates
            if (!gRender.GetDevice()->run()) {
                mainGameRunning = false;
                gRunning = false;
            }
            if (gInputMgr.QuitMainGamePressed()) {
                mainGameRunning = false;
            }
          
            
            u32 now = gRender.GetDevice()->getTimer()->getTime();
            const float frameDeltaTime = (f32)(now - then) / 1000.0f;
            then = now;

            gAIPlayer.SetNpcPos(gRender.GetCam()->getPosition());
            gAIPlayer.Update(frameDeltaTime);

            gGameStatMgr.UpdateGameTime(frameDeltaTime);

            gHud.Update();

            // drawing
            gRender.Clear();
            gRender.Update();
        }
        
        if (gRunning) {
            if (!RunEndGameScreen()) { break; }
        }
    }

    gRender.GetDevice()->drop();
    return 0;
}
