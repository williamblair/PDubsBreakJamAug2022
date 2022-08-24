#include "GameHud.h"
#include "Renderer.h"
#include "GameStatManager.h"

using namespace irr;

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
    if (mEventTimeCtr >= mEventTime) {
        mEventTimeCtr = 0.0f;
        mEventText->setOverrideColor(video::SColor(0,255,255,0)); // transparent
        mEventText->setBackgroundColor(video::SColor(0,0,0,0));
        mEventText->setDrawBorder(false);
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

// global instance
GameHud gHud;

