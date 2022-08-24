#include "GameHud.h"
#include "Renderer.h"
#include "GameStatManager.h"

using namespace irr;

GameHud::GameHud() :
    mFont(nullptr)
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
}

void GameHud::Update()
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
}
