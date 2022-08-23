#include "EndGameScreen.h"
#include "InputManager.h"
#include "Renderer.h"
#include "GameStatManager.h"

using namespace irr;

EndGameScreen::EndGameScreen() :
    mAction(EndGameScreen::ACTION_TITLE),
    mFont(nullptr),
    mRunning(true),
    mEntry(0)
{}
EndGameScreen::~EndGameScreen()
{}

void EndGameScreen::Init()
{
    mFont = gRender.LoadFont("assets/fontcourier.bmp");
}

EndGameScreen::Action EndGameScreen::Run()
{
    mRunning = true;
    while (mRunning && gRender.GetDevice()->run())
    {
        HandleInput();
      
        // drawing
        gRender.Clear();
        s32 x = 130, y = 10, x2 = 400, y2 = y;
        video::SColor fntCol(255,255,255,255);
        video::SColor fntColHilighted(255,255,0,0);
        for (size_t i=0; i<mNumEntries; ++i) {
            ActionEntry& entry = mActionEntries[i];
            mFont->draw(
                entry.name,
                core::rect<s32>(x,y,x2,y2),
                (i == (size_t)mEntry) ? fntColHilighted : fntCol
            );
            y += 50;
            y2 = y;
        }
        DrawStats();
        gRender.Update(false);
    }
    return mAction;
}

void EndGameScreen::HandleInput()
{
    if (gInputMgr.UpPressed()) {
        --mEntry;
        if (mEntry < 0) {
            mEntry = mNumEntries-1;
        }
        mAction = mActionEntries[mEntry].action;
    } else if (gInputMgr.DownPressed()) {
        ++mEntry;
        if ((size_t)mEntry >= mNumEntries) {
            mEntry = 0;
        }
        mAction = mActionEntries[mEntry].action;
    } else if (gInputMgr.ConfirmPressed()) {
        mRunning = false;
    }
}

void EndGameScreen::DrawStats()
{
    int min = gGameStatMgr.GetGameTimeMinutes();
    int sec = gGameStatMgr.GetGameTimeSeconds();
    uint64_t mkPlyrWt = gGameStatMgr.GetNumMkPlyrWtEvts();
    uint64_t score = gGameStatMgr.GetScore();
    
    core::stringw scoreStr = L"Score... ";
    scoreStr += score;
    core::stringw timeStr = L"Time... ";
    timeStr += min;
    timeStr += L":";
    timeStr += sec;
    core::stringw plyrWtStr = L"Make Player Wait Events... ";
    plyrWtStr += mkPlyrWt;
    
    s32 x = 130, y = 80, x2 = 400, y2 = y;
    video::SColor fntCol(255,255,255,0);
    mFont->draw(scoreStr, core::rect<s32>(x,y,x2,y2), fntCol);
    y +=20; y2 = y;
    x += 50; x2 += 50;
    mFont->draw(timeStr, core::rect<s32>(x,y,x2,y2), fntCol);
    y +=20; y2 = y;
    mFont->draw(plyrWtStr, core::rect<s32>(x,y,x2,y2), fntCol);
}

