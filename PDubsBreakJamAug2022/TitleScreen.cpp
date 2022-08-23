#include "TitleScreen.h"
#include "Renderer.h"
#include "InputManager.h"
#include <algorithm>

using namespace irr;

TitleScreen::TitleScreen() :
    mAction(TitleScreen::ACTION_PLAY),
    mFont(nullptr),
    mRunning(true),
    mEntry(0)
{
}

TitleScreen::~TitleScreen()
{
}

void TitleScreen::Init()
{
    mFont = gRender.LoadFont("assets/fontcourier.bmp");
}

TitleScreen::Action TitleScreen::Run()
{
    mRunning = true;
    while (mRunning && gRender.GetDevice()->run())
    {
        // logic updates
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
        gRender.Update(false);
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
