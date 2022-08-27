#include <cstdio>
#include <vector>
#include <stdexcept>

#include "Renderer.h"
#include "AIPlayer.h"
#include "GameStatManager.h"
#include "TitleScreen.h"
#include "EndGameScreen.h"
#include "InputManager.h"
#include "AudioManager.h"
#include "GameMap.h"
#include "GameHud.h"
#include "HighScoreData.h"
#include "HighScoreScreen.h"
#include "EnterScoreScreen.h"

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
bool gMainGameRunning = true;
std::vector<HighScoreData> gHighScoreData;

static bool gRunning = true;


static TitleScreen::Action RunTitleScreen(bool startMusic)
{
    TitleScreen ts;
    ts.Init();
    TitleScreen::Action act = ts.Run(startMusic);
    return act;
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
static void LoadHighScores()
{
    FILE* datFile = fopen("assets/highscores.dat", "rb");
    if (datFile) {
        // get file size
        fseek(datFile, 0, SEEK_END);
        size_t fileSizeBytes = ftell(datFile);
        fseek(datFile, 0, SEEK_SET);

        // calculate number of data entries
        size_t numHsDatas = fileSizeBytes / sizeof(HighScoreData);
        gHighScoreData.resize(numHsDatas);

        // read the file data
        size_t amntRead = fread(gHighScoreData.data(), sizeof(HighScoreData), numHsDatas, datFile);
        if (amntRead != numHsDatas) {
            throw std::runtime_error("Failed to read high score data\n");
        }
        fclose(datFile);
    }
}
static HighScoreScreen::Action RunHighScoreScreen()
{
    HighScoreScreen hss;
    LoadHighScores();
    hss.Init();
    return hss.Run();
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
    gMap.Init();
    gAIPlayer.Init();
    gHud.Init();
    
    LoadHighScores();

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

    bool titleStartMusic = true;
    while (gRunning)
    {
        switch (RunTitleScreen(titleStartMusic))
        {
        case TitleScreen::Action::ACTION_HIGHSCORES:
            if (RunHighScoreScreen() == HighScoreScreen::Action::QUIT) {
                gRunning = false;
                gMainGameRunning = false;
            }
            titleStartMusic = false;
            continue;
        case TitleScreen::Action::ACTION_QUIT:
            gRunning = false;
            gMainGameRunning = false;
            break;
        default:
            break;
        }

        bool mainGamePaused = false;
        u32 then = gRender.GetDevice()->getTimer()->getTime();
        gAudioMgr.PlayMusic(bgMusic);
        while (gMainGameRunning)
        {
            // logic updates
            if (!gRender.GetDevice()->run()) {
                gMainGameRunning = false;
                gRunning = false;
            }
            if (gInputMgr.QuitMainGamePressed()) {
                gMainGameRunning = false;
            }
            if (gInputMgr.ConfirmPressed()) {
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
            bool shouldQuit = RunEndGameScreen();
            if (!gRunning) { break; }
            if (gHighScoreData.empty() || (gGameStatMgr.GetScore() > gHighScoreData.rbegin()->score)) {
                EnterScoreScreen ess;
                ess.Init(gGameStatMgr.GetScore());
                EnterScoreScreen::Action act = ess.Run();
                switch (act)
                {
                case EnterScoreScreen::ACTION_HIGHSCORES:
                    shouldQuit = false;
                    break;
                case EnterScoreScreen::ACTION_QUIT:
                    shouldQuit = true;
                    break;
                default:
                    break;
                }
            }
            if (shouldQuit || !gRunning) { break; }
            if (RunHighScoreScreen() == HighScoreScreen::Action::QUIT) {
                gRunning = false;
                gMainGameRunning = false;
                break;
            }
        }
        titleStartMusic = true;
    }

    gRender.GetDevice()->drop();
    return 0;
}
