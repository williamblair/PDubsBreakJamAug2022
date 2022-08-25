#include "GameHud.h"
#include "Renderer.h"
#include "GameStatManager.h"
#include "AudioManager.h"

using namespace irr;

extern bool gGamePaused; // declared in main.cpp

GameHud::GameHud() :
    mFont(nullptr),
    mTimeText(nullptr),
    mScoreText(nullptr),
    mEventText(nullptr),
    mEventTimeCtr(0.0f),
    mEventTime(5.0f)
{}

GameHud::~GameHud()
{}

void GameHud::Init()
{
    mFont = gRender.LoadFont("assets/fontcourier.bmp");
    mRightBackTex = gRender.LoadTexture("assets/rightback.png");
    mRightBackSound = gAudioMgr.LoadSound("assets/rightback2.mp3");
    mExplosionTex = gRender.LoadTexture("assets/explosion.png");

    s32 x = 10, y = 10, x2 = 400, y2 = 30;
    video::SColor fntCol(255,255,255,0);
    mTimeText = gRender.GetGuiEnv()->addStaticText(L"Time: 0:00", core::rect<s32>(x,y,x2,y2));
    mTimeText->setOverrideColor(fntCol);
    mTimeText->setOverrideFont(mFont);
    y = 30;
    y2 = y+20;
    mScoreText = gRender.GetGuiEnv()->addStaticText(L"Score: 0", core::rect<s32>(x,y,x2,y2));
    mScoreText->setOverrideColor(fntCol);
    mScoreText->setOverrideFont(mFont);

    x = 300; y = 10;
    x2 = 400; y2 = 40;
    mEventText = gRender.GetGuiEnv()->addStaticText(
        L"Some Event!",
        core::rect<s32>(x,y,x2,y2), // pos
        true, // 3d border?
        true, // word wrap?
        nullptr, // parent
        -1, // element ID
        true // fill background?
    );
    mEventText->setOverrideColor(fntCol);
    mEventText->setOverrideFont(mFont);
    mEventText->setTextAlignment(gui::EGUIA_CENTER, gui::EGUIA_CENTER);
    mEventText->setOverrideColor(video::SColor(0,255,255,0)); // transparent
    mEventText->setBackgroundColor(video::SColor(0,0,0,0));
    mEventText->setDrawBorder(false);

    mRightBackImg = gRender.GetGuiEnv()->addImage(
        core::rect<s32>(20,100,224,400),
        nullptr, // parent
        -1, // id
        nullptr, // text
        true // use alpha channel?
    );
    mRightBackImg->setImage(mRightBackTex);
    mRightBackImg->setScaleImage(true);
    mRightBackImg->setColor(video::SColor(0,255,255,255)); // fully transparent
    mPlayingExplosion = false;
    mExplosionTime = 3.2f;

    mExplosionImg = gRender.GetGuiEnv()->addImage(
        core::rect<s32>(200,75,600,500),
        nullptr,
        -1,
        nullptr,
        true
    );
    mExplosionImg->setImage(mExplosionTex);
    mExplosionImg->setScaleImage(true);
    mExplosionImg->setColor(video::SColor(0,255,255,255)); // fully transparent
}

void GameHud::Update(const float dt)
{
    int min = gGameStatMgr.GetGameTimeMinutes();
    int sec = gGameStatMgr.GetGameTimeSeconds();
    uint64_t score = gGameStatMgr.GetScore();

    video::SColor fntCol(255,255,255,0);
    core::stringw timeStr = L"Time: ";
    timeStr += min;
    timeStr += ":";
    if (sec < 10) { timeStr += L"0"; }
    timeStr += sec;
    mTimeText->setText(timeStr.c_str());

    core::stringw scoreStr = L"Score: ";
    scoreStr += (unsigned long)score;
    mScoreText->setText(scoreStr.c_str());

    mEventTimeCtr += dt;
    if (mPlayingExplosion) {
        if (mEventTimeCtr >= 0.2f) {
            mRightBackImg->setColor(video::SColor(255,255,255,255));
        }
        if (mEventTimeCtr >= mExplosionTime) {
            mRightBackImg->setColor(video::SColor(0,255,255,255));
            mExplosionImg->setColor(video::SColor(0,255,255,255));
            mEventTimeCtr = 0.0f;
            mPlayingExplosion = false;
            AddEventNotification(L"Trigger Explosion!");
            gGamePaused = false;
            gRender.SetFPSInputEnabled(true);
        }
    }
    else {
        if (mEventTimeCtr >= mEventTime) {
            mEventTimeCtr = 0.0f;
            mEventText->setOverrideColor(video::SColor(0,255,255,0)); // transparent
            mEventText->setBackgroundColor(video::SColor(0,0,0,0));
            mEventText->setDrawBorder(false);
        }
    }
}

void GameHud::AddEventNotification(core::stringw message)
{
    mEventTimeCtr = 0.0f;
    mEventText->setOverrideColor(video::SColor(255,255,255,0));
    mEventText->setBackgroundColor(video::SColor(50,255,255,255));
    mEventText->setText(message.c_str());
    mEventText->setDrawBorder(true);
}

void GameHud::AddTriggerExplosion()
{
    gAudioMgr.PlaySound(mRightBackSound);
    mEventTimeCtr = 0.0f;
    mPlayingExplosion = true;
    //mRightBackImg->setColor(video::SColor(255,255,255,255));
    mExplosionImg->setColor(video::SColor(255,255,255,255));
}

// global instance
GameHud gHud;

