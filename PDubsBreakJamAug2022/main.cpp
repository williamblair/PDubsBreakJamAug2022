#include <cstdio>

#include "Renderer.h"
#include "AIPlayer.h"
#include "GameStatManager.h"
#include "TitleScreen.h"
#include "EndGameScreen.h"
#include "InputManager.h"
#include "AudioManager.h"
#include "GameMap.h"
#include "GameHud.h"

#ifdef _WIN32
#undef main
#endif

using namespace irr;

using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

GameMap gMap;
bool gGamePaused = false;

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
    size_t trainSound;

    gRender.Init();
    gAudioMgr.Init();
    gAIPlayer.Init();
    gMap.Init();
    gHud.Init();
    

    trainSound = gAudioMgr.LoadSound("assets/train.mp3");

    while (gRunning)
    {
        if (!RunTitleScreen()) { break; }
        // TODO - high scores, then back to title screen

        bool mainGameRunning = true;
        bool mainGamePaused = false;
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
            if (gInputMgr.ConfirmPressed()) {
                printf("playing train sound\n");
                gAudioMgr.PlaySound(trainSound, 0);
                gGameStatMgr.AddScoreEvent(GameStatManager::EVT_TAUNT_PLAYER);
            }
            if (gMap.BoxCollidesWithExplosion(gRender.GetCameraCollisionBox())) {
                gHud.AddTriggerExplosion();
                gRender.SetFPSInputEnabled(false);
                gGamePaused = true;
            }
          
            
            u32 now = gRender.GetDevice()->getTimer()->getTime();
            const float frameDeltaTime = (f32)(now - then) / 1000.0f;
            then = now;

            if (!gGamePaused) {
                gAIPlayer.SetNpcPos(gRender.GetCam()->getPosition());
                gAIPlayer.Update(frameDeltaTime);

                gGameStatMgr.UpdateGameTime(frameDeltaTime);
            }

            gHud.Update(frameDeltaTime);

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
