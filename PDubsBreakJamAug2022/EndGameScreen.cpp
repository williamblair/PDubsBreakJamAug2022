#include "EndGameScreen.h"
#include "InputManager.h"
#include "Renderer.h"
#include "GameStatManager.h"
#include "AudioManager.h"

using namespace irr;

EndGameScreen::EndGameScreen() :
    mAction(EndGameScreen::ACTION_QUIT),
    mFont(nullptr),
    mRunning(true),
    mTimeCtr(0.0f),
    mScoreItemCtr(0),
    mItemSound(0),
    mEntry(0)
{}
EndGameScreen::~EndGameScreen()
{}

void EndGameScreen::Init()
{
    mFont = gRender.LoadFont("assets/bigfont.png");
    mItemSound = gAudioMgr.LoadSound("assets/vgmenuhighlight.wav");
    mScoreItemCtr = 0;
    mTimeCtr = 0.0f;
}

EndGameScreen::Action EndGameScreen::Run()
{
    mRunning = true;
    u32 then = gRender.GetDevice()->getTimer()->getTime();
    while (mRunning && gRender.GetDevice()->run())
    {
        if (mScoreItemCtr >= 13) {
            HandleInput();
        }

        u32 now = gRender.GetDevice()->getTimer()->getTime();
        const float dt = (f32)(now - then) / 1000.0f;
        then = now;

        if (mScoreItemCtr <= 12) { mTimeCtr += dt; }
        if (mTimeCtr >= 0.75f && mScoreItemCtr <= 12) {
            ++mScoreItemCtr;
            while (mTimeCtr >= 0.75f) { mTimeCtr -= 0.75f; }
            gAudioMgr.PlaySound(mItemSound);
        }
      
        // drawing
        gRender.Clear();
        video::SColor fntCol(255,255,255,255);
        mFont->draw(L"End Level!", core::rect<s32>(290,10,450,10), fntCol);
        if (mScoreItemCtr >= 13) {
            s32 x = 350, y = 500, x2 = 550, y2 = 600;
            video::SColor fntColHilighted(255,255,0,0);
            for (size_t i=0; i<mNumEntries; ++i) {
                ActionEntry& entry = mActionEntries[i];
                mFont->draw(
                    entry.name,
                    core::rect<s32>(x,y,x2,y2),
                    (i == (size_t)mEntry) ? fntColHilighted : fntCol
                );
                x += 100;
                x2 += 100;
            }
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
    uint64_t tauntPlyr = gGameStatMgr.GetNumTauntPlyrEvts();
    uint64_t numExpl = gGameStatMgr.GetNumTrgrExplEvts();
    uint64_t numGetSeen = gGameStatMgr.GetNumSeenByEnmyEvts();
    uint64_t score = gGameStatMgr.GetScore();
    
    core::stringw scoreStr = L"Score... ";
    core::stringw scoreNumStr = L"";
    scoreNumStr += (const unsigned long)score;
    core::stringw timeStr = L"Time... ";
    core::stringw timeNumStr = L"";
    timeNumStr += min;
    timeNumStr += L":";
    if (sec < 10) { timeNumStr += L"0"; }
    timeNumStr += (const unsigned long)sec;
    core::stringw plyrWtStr = L"Get Away From Player Events... ";
    core::stringw plyrWtNumStr = L"";
    plyrWtNumStr += (const unsigned long)mkPlyrWt;
    core::stringw tauntStr = L"Taunt Player Events... ";
    core::stringw tauntNumStr = L"";
    tauntNumStr += (const unsigned long)tauntPlyr;
    core::stringw numExplStr = L"Trigger Explosion Events... ";
    core::stringw numExplNumStr = L"";
    numExplNumStr += (const unsigned long)numExpl;
    core::stringw numSeenStr = L"Seen By Enemy Events... ";
    core::stringw numSeenNumStr = L"";
    numSeenNumStr += (const unsigned long)numGetSeen;
    
    s32 x = 130, y = 80, x2 = 400, y2 = y;
    video::SColor fntCol(255,255,255,0);
    if (mScoreItemCtr >= 11) {
        mFont->draw(scoreStr, core::rect<s32>(x,y,x2,y2), fntCol);
    }
    x += 450;
    if (mScoreItemCtr >= 12) {
        mFont->draw(scoreNumStr, core::rect<s32>(x,y,x2,y2), fntCol);
    }
    y += 30; y2 = y;
    x = 10; x2 = 10+400;
    if (mScoreItemCtr >= 1) {
        mFont->draw(timeStr, core::rect<s32>(x,y,x2,y2), fntCol);
    }
    y += 30; y2 = y;
    if (mScoreItemCtr >= 3) {
        mFont->draw(plyrWtStr, core::rect<s32>(x,y,x2,y2), fntCol);
    }
    y += 30; y2 = y;
    if (mScoreItemCtr >= 5) {
        mFont->draw(tauntStr, core::rect<s32>(x,y,x2,y2), fntCol);
    }
    y += 30; y2 = y;
    if (mScoreItemCtr >= 7) {
        mFont->draw(numExplStr, core::rect<s32>(x,y,x2,y2), fntCol);
    }
    y += 30; y2 = y;
    if (mScoreItemCtr >= 9) {
        mFont->draw(numSeenStr, core::rect<s32>(x,y,x2,y2), fntCol);
    }

    x = 700; x2 = 800;
    y = 110; y2 = 110+30;
    if (mScoreItemCtr >= 2) {
        mFont->draw(timeNumStr, core::rect<s32>(x,y,x2,y2), fntCol);
    }
    y += 30; y2 += 30;
    if (mScoreItemCtr >= 4) {
        mFont->draw(plyrWtNumStr, core::rect<s32>(x,y,x2,y2), fntCol);
    }
    y += 30; y2 += 30;
    if (mScoreItemCtr >= 6) {
        mFont->draw(tauntNumStr, core::rect<s32>(x,y,x2,y2), fntCol);
    }
    y += 30; y2 += 30;
    if (mScoreItemCtr >= 8) {
        mFont->draw(numExplNumStr, core::rect<s32>(x,y,x2,y2), fntCol);
    }
    y += 30; y2 += 30;
    if (mScoreItemCtr >= 10) {
        mFont->draw(numSeenNumStr, core::rect<s32>(x,y,x2,y2), fntCol);
    }
}

