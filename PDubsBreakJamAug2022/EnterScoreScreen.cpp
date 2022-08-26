#include "EnterScoreScreen.h"
#include "InputManager.h"
#include "Renderer.h"
#include "GameStatManager.h"
#include "AudioManager.h"

#include <stdexcept>

using namespace irr;

// defined in main.cpp
extern std::vector<HighScoreData> gHighScoreData;

EnterScoreScreen::EnterScoreScreen() :
    mAction(EnterScoreScreen::ACTION_HIGHSCORES),
    mFont(nullptr),
    mRunning(true),
    mEnteredName(false),
    mEntry(0),
    mNewScore(0),
    mCurNameIdx(0)
{}
EnterScoreScreen::~EnterScoreScreen()
{}

void EnterScoreScreen::Init(uint64_t newScore)
{
    mFont = gRender.LoadFont("assets/bigfont.png");
    mSelectSound = gAudioMgr.LoadSound("assets/vgmenuhighlight.wav");
    mConfirmSound = gAudioMgr.LoadSound("assets/vgmenuselect.wav");
    mNewScore = newScore;
    mEnteredName = false;
    mScoreData.score = newScore;
    mScoreData.name[0] = 'A';
    mScoreData.name[1] = 'A';
    mScoreData.name[2] = 'A';
    mCurNameIdx = 0;
}

EnterScoreScreen::Action EnterScoreScreen::Run()
{
    mRunning = true;
    u32 then = gRender.GetDevice()->getTimer()->getTime();
    while (mRunning && gRender.GetDevice()->run())
    {
        HandleInput();

        u32 now = gRender.GetDevice()->getTimer()->getTime();
        const float dt = (f32)(now - then) / 1000.0f;
        then = now;
      
        // drawing
        gRender.Clear();
        video::SColor fntCol(255,255,255,255);
        mFont->draw(L"New High Score!", core::rect<s32>(250,10,450,10), fntCol);
        mFont->draw(L"Enter Your Name", core::rect<s32>(250,40,450,40), fntCol);
        s32 x = 100, y = 200, x2 = 300, y2 = y;
        for (size_t i=0; i<=mCurNameIdx && i<3; ++i) {
            core::stringw ltr = L"";
            ltr += (wchar_t)mScoreData.name[i];
            mFont->draw(ltr, core::rect<s32>(x,y,x2,y2),video::SColor(255,255,255,0));
            if (i == mCurNameIdx) {
                mFont->draw(L"+", core::rect<s32>(x,y-35,x2,y2-35),video::SColor(255,255,255,0));
                mFont->draw(L"-", core::rect<s32>(x,y+35,x2,y2+35),video::SColor(255,255,255,0));
            }
            x += 25;            
        }
        core::stringw scoreStr = L"";
        scoreStr += (const unsigned long)mNewScore;
        mFont->draw(scoreStr, core::rect<s32>(600,200,800,200),video::SColor(255,255,255,0));
        
        gRender.Update(false);
    }
    gHighScoreData.push_back(mScoreData);
    {
        FILE* fileDat = fopen("assets/highscores.dat", "wb");
        if (!fileDat) {
            throw std::runtime_error("Failed to open highscores.dat for writing\n");
        }
        size_t amnt = fwrite(gHighScoreData.data(), sizeof(HighScoreData), gHighScoreData.size(), fileDat);
        if (amnt != gHighScoreData.size()) {
            printf("Failed to write high score data to file\n");
        }
        fclose(fileDat);
    }
    return mAction;
}

void EnterScoreScreen::HandleInput()
{
    if (mEnteredName) {
        if (gInputMgr.UpPressed()) {
            --mEntry;
            if (mEntry < 0) {
                mEntry = mNumEntries-1;
            }
            mAction = mActionEntries[mEntry].action;
            gAudioMgr.PlaySound(mSelectSound);
        } else if (gInputMgr.DownPressed()) {
            ++mEntry;
            if ((size_t)mEntry >= mNumEntries) {
                mEntry = 0;
            }
            mAction = mActionEntries[mEntry].action;
            gAudioMgr.PlaySound(mSelectSound);
        } else if (gInputMgr.ConfirmPressed()) {
            mRunning = false;
            gAudioMgr.PlaySound(mConfirmSound);
        }
    } else {
        if (gInputMgr.UpPressed()) {
            ++mScoreData.name[mCurNameIdx];
            if (mScoreData.name[mCurNameIdx] > 'Z') {
                mScoreData.name[mCurNameIdx] = 'A';
            }
            gAudioMgr.PlaySound(mSelectSound);
        } else if (gInputMgr.DownPressed()) {
            --mScoreData.name[mCurNameIdx];
            if (mScoreData.name[mCurNameIdx] < 'A') {
                mScoreData.name[mCurNameIdx] = 'Z';
            }
            gAudioMgr.PlaySound(mSelectSound);
        } else if (gInputMgr.ConfirmPressed()) {
            ++mCurNameIdx;
            if (mCurNameIdx >= 3) {
                mEnteredName = true;
            }
            gAudioMgr.PlaySound(mConfirmSound);
        }
    }
}

