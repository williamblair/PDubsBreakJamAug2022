#include "HighScoreScreen.h"
#include "Renderer.h"
#include "InputManager.h"
#include "AudioManager.h"

using namespace irr;

extern std::vector<HighScoreData> gHighScoreData;

HighScoreScreen::HighScoreScreen() :
    mAction(HighScoreScreen::Action::TITLE),
    mFont(nullptr),
    mRunning(true)
{}

HighScoreScreen::~HighScoreScreen()
{}

void HighScoreScreen::Init()
{
    mFont = gRender.LoadFont("assets/bigfont.png");
    mSelectSound = gAudioMgr.LoadSound("assets/vgmenuhighlight.wav");
    mConfirmSound = gAudioMgr.LoadSound("assets/vgmenuselect.wav");
}

HighScoreScreen::Action HighScoreScreen::Run()
{
    u32 then = gRender.GetDevice()->getTimer()->getTime();
    while (mRunning && gRender.GetDevice()->run())
    {
        HandleInput();

        u32 now = gRender.GetDevice()->getTimer()->getTime();
        const float dt = (f32)(now - then) / 1000.0f;
        then = now;
        
        gRender.Clear();
        DrawHighScores();
        s32 x = 200, y = 500, x2 = 550, y2 = 600;
        video::SColor fntColHilighted(255,255,0,0);
        video::SColor fntCol(255,255,255,255);
        for (int i=0; i<(int)mNumEntries; ++i) {
            ActionEntry& entry = mActionEntries[i];
            mFont->draw(
                entry.name,
                core::rect<s32>(x,y,x2,y2),
                (i == (size_t)mEntry) ? fntColHilighted : fntCol
            );
            x += 300;
            x2 += 300;
        }
        mFont->draw(L"High Scores", core::rect<s32>(275,10,500,10), video::SColor(255,255,255,255));
        gRender.Update(false);
    }
    return mAction;
}

void HighScoreScreen::HandleInput()
{
    if (gInputMgr.DownPressed()) {
        ++mEntry;
        if (mEntry >= mNumEntries) {
            mEntry = 0;
        }
        mAction = mActionEntries[mEntry].action;
        gAudioMgr.PlaySound(mSelectSound);
    } else if (gInputMgr.UpPressed()) {
        --mEntry;
        if (mEntry < 0) {
            mEntry = mNumEntries-1;
        }
        mAction = mActionEntries[mEntry].action;
        gAudioMgr.PlaySound(mSelectSound);
    }

    if (gInputMgr.ConfirmPressed()) {
        mRunning = false;
        gAudioMgr.PlaySound(mConfirmSound);
    }
}

void HighScoreScreen::DrawHighScores()
{
    video::SColor fntCol(255,255,255,0);
    s32 x = 100, y = 200, x2 = 400, y2 = y;
    int i = (int)gHighScoreData.size()-1;
    if (i > 10) { i = 10; }
    for (; i>=0; --i)
    {
        HighScoreData& data = gHighScoreData[i];
        core::stringw name = L"";
        for (size_t j=0; j<3 && data.name[j] != 0; ++j) {
            name += (wchar_t)data.name[j];
        }
        mFont->draw(name, core::rect<s32>(x,y,x2,y2),fntCol);
        y += 30;
        y2 = y;
    }

    x = 600, y = 200, x2 = 800, y2 = y;
    i = (int)gHighScoreData.size()-1;
    if (i > 10) { i = 10; }
    for (; i>=0; --i)
    {
        HighScoreData& data = gHighScoreData[i];
        core::stringw score = L"";
        score += (const unsigned long)data.score;
        mFont->draw(score, core::rect<s32>(x,y,x2,y2),fntCol);
        y += 30;
        y2 = y;
    }
}
