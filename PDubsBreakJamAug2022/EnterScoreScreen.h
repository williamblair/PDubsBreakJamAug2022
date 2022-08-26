#ifndef ENTER_SCORE_SCREEN_H_INCLUDED
#define ENTER_SCORE_SCREEN_H_INCLUDED

#include <vector>
#include <cstdint>

#include <irrlicht/irrlicht.h>

#include "HighScoreData.h"

class EnterScoreScreen
{
public:

    enum Action
    {
        ACTION_HIGHSCORES,
        ACTION_QUIT
    };

    EnterScoreScreen();
    ~EnterScoreScreen();

    void Init(uint64_t newScore);
    Action Run();

private:
    Action mAction;
    irr::gui::IGUIFont* mFont;
    bool mRunning;
    bool mEnteredName;
    uint64_t mNewScore;
    size_t mCurNameIdx;
    HighScoreData mScoreData;
    size_t mSelectSound;
    size_t mConfirmSound;
    
    struct ActionEntry
    {
        Action action;
        irr::core::stringw name;
    };
    static const size_t mNumEntries = 2;
    ActionEntry mActionEntries[mNumEntries] = {
        { ACTION_HIGHSCORES, L"High Scores" },
        { ACTION_QUIT, L"Quit" }
    };
    int mEntry;
    
    void HandleInput();
};

#endif // ENTER_SCORE_SCREEN_H_INCLUDED
