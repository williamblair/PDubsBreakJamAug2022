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

// global instance
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
    size_t alertSound;
    size_t bgMusic;
    IBillboardSceneNode* alertBill;
    float alertTime = 1.5f;
    float alertTimeCtr = 0.0f;
    bool playingAlert = false;

    gRender.Init();
    gAudioMgr.Init();
    gAIPlayer.Init();
    gMap.Init();
    gHud.Init();
    

    trainSound = gAudioMgr.LoadSound("assets/train.mp3");
    alertSound = gAudioMgr.LoadSound("assets/alert.mp3");
    bgMusic = gAudioMgr.LoadMusic("assets/overworld.mp3");

    alertBill = gRender.GetSceneMgr()->addBillboardSceneNode();
    alertBill->setMaterialType(video::EMT_TRANSPARENT_ALPHA_CHANNEL);
    alertBill->setMaterialTexture(0, gRender.LoadTexture("assets/alert.png"));
    alertBill->setMaterialFlag(video::EMF_LIGHTING, false);
    alertBill->setMaterialFlag(video::EMF_ZBUFFER, false);
    alertBill->setSize(core::dimension2d<f32>(20.0f,20.0f));
    alertBill->setVisible(false);

    while (gRunning)
    {
        if (!RunTitleScreen()) { break; }
        // TODO - high scores, then back to title screen

        bool mainGameRunning = true;
        bool mainGamePaused = false;
        u32 then = gRender.GetDevice()->getTimer()->getTime();
        gAudioMgr.PlayMusic(bgMusic);
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
                gGameStatMgr.AddScoreEvent(GameStatManager::EVT_TRIGGER_EXPLOSION);
            }
            if (gMap.BoxCollidesWithEnemyPatrol(gRender.GetCameraCollisionBox())) {
                gAIPlayer.SetState(AIPlayer::State::WALK_TOWARD_ENEMY);
                gMap.GetNextEnemy()->SetState(AIEnemy::State::WALK_TOWARD_PLAYER);
                gGameStatMgr.AddScoreEvent(GameStatManager::EVT_SEEN_BY_ENEMY);
                gAudioMgr.PlaySound(alertSound);
                playingAlert = true;
                alertTimeCtr = 0.0f;
                alertBill->setVisible(true);
            }
            if (gMap.GetNextEnemy() && playingAlert) {
                core::vector3df pos = gMap.GetNextEnemy()->GetNode()->getAbsolutePosition();
                pos.Y += 60.0f;
                alertBill->setPosition(pos);
            }
          
            
            u32 now = gRender.GetDevice()->getTimer()->getTime();
            const float frameDeltaTime = (f32)(now - then) / 1000.0f;
            then = now;

            if (playingAlert) {
                alertTimeCtr += frameDeltaTime;
                if (alertTimeCtr >= alertTime) {
                    alertTimeCtr = 0.0f;
                    playingAlert = false;
                    alertBill->setVisible(false);
                }
            }

            if (!gGamePaused) {
                gAIPlayer.SetNpcPos(gRender.GetCam()->getPosition());
                gAIPlayer.Update(frameDeltaTime);

                gGameStatMgr.UpdateGameTime(frameDeltaTime);
                gMap.Update(frameDeltaTime);
            }

            gHud.Update(frameDeltaTime);

            // drawing
            gRender.Clear();
            gRender.Update();
        }
        // keep playing for end game screen
        //gAudioMgr.StopMusic();
        
        if (gRunning) {
            if (!RunEndGameScreen()) { break; }
        }
    }

    gRender.GetDevice()->drop();
    return 0;
}
