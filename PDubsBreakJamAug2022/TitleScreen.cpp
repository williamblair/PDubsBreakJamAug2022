#include "TitleScreen.h"
#include "Renderer.h"
#include "InputManager.h"
#include "AudioManager.h"
#include <algorithm>

using namespace irr;

TitleScreen::TitleScreen() :
    mAction(TitleScreen::ACTION_PLAY),
    mFont(nullptr),
    mNamePos(650.0f),
    mRunning(true),
    mEntry(0)
{
}

TitleScreen::~TitleScreen()
{
}

void TitleScreen::Init()
{
    //mFont = gRender.LoadFont("assets/fontcourier.bmp");
    mFont = gRender.LoadFont("assets/bigfont.png");
    mBgMusic = gAudioMgr.LoadMusic("assets/title.mp3");
    mHilightSound = gAudioMgr.LoadSound("assets/vgmenuhighlight.wav");
    mSelectSound = gAudioMgr.LoadSound("assets/vgmenuselect.wav");
}

TitleScreen::Action TitleScreen::Run(bool startMusic)
{
    mRunning = true;
    if (startMusic) { gAudioMgr.PlayMusic(mBgMusic); }
    u32 then = gRender.GetDevice()->getTimer()->getTime();
    while (mRunning && gRender.GetDevice()->run())
    {
        // logic updates
        HandleInput();

        u32 now = gRender.GetDevice()->getTimer()->getTime();
        const float dt = (f32)(now - then) / 1000.0f;
        then = now;

        mNamePos -= dt * 50.0f;
        if (mNamePos < -600.0f) {
            mNamePos = 810.0f;
        }
      
        // drawing
        gRender.Clear();
        video::SColor fntCol(255,255,255,255);
        video::SColor fntColHilighted(255,255,0,0);
        s32 x = 200, y = 10, x2 = 600, y2 = y;
        mFont->draw(L"NPC Escort Simulator", core::rect<s32>(x,y,x2,y2),fntCol);
        x = 300, y = 200, x2 = 600, y2 = y;
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
        x = (s32)mNamePos; x2 = x + 300;
        y = 500; y2 = 500;
        mFont->draw(L"BJ Blair - PDubs Break Jam Aug 2022",core::rect<s32>(x,y,x2,y2),fntCol);
        mFont->draw(L"W/S - Select", core::rect<s32>(50,550,300,550),fntCol);
        mFont->draw(L"Enter - Confirm", core::rect<s32>(450,550,800,550),fntCol);
        gRender.Update(false);
    }
    if (mAction != Action::ACTION_HIGHSCORES) {
        gAudioMgr.StopMusic();
    }
    return mAction;
}

void TitleScreen::HandleInput()
{
    if (gInputMgr.UpPressed()) {
        --mEntry;
        if (mEntry < 0) {
            mEntry = mNumEntries-1;
        }
        mAction = mActionEntries[mEntry].action;
        gAudioMgr.PlaySound(mHilightSound);
    } else if (gInputMgr.DownPressed()) {
        ++mEntry;
        if ((size_t)mEntry >= mNumEntries) {
            mEntry = 0;
        }
        mAction = mActionEntries[mEntry].action;
        gAudioMgr.PlaySound(mHilightSound);
    } else if (gInputMgr.ConfirmPressed()) {
        mRunning = false;
        gAudioMgr.PlaySound(mSelectSound);
    }
}
