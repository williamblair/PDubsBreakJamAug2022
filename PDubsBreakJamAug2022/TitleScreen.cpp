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
    float timeCtr = 0;
    u32 then = gRender.GetDevice()->getTimer()->getTime();
    while (mRunning && gRender.GetDevice()->run())
    {
        // logic updates
        u32 now = gRender.GetDevice()->getTimer()->getTime();
        const float frameDeltaTime = (f32)(now - then) / 1000.0f;
        then = now;
        timeCtr += std::max(frameDeltaTime,1.0f/60.0f);
        if (timeCtr >= 10.0f) {
            mRunning = false;
        }
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
    printf("Exit titleScreen\n");
    return mAction;
}

void TitleScreen::HandleInput()
{
    if (gInputMgr.UpPressed()) {
        --mEntry;
        if (mEntry < 0) {
            mEntry = mNumEntries-1;
            mAction = mActionEntries[mEntry].action;
        }
    } else if (gInputMgr.DownPressed()) {
        ++mEntry;
        if ((size_t)mEntry >= mNumEntries) {
            mEntry = 0;
            mAction = mActionEntries[mEntry].action;
        }
    } else if (gInputMgr.ConfirmPressed()) {
        mRunning = false;
    }
}
