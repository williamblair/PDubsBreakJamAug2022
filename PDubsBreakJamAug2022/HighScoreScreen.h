#ifndef HIGH_SCORE_SCREEN_H_INCLUDED
#define HIGH_SCORE_SCREEN_H_INCLUDED

#include <vector>
#include <irrlicht/irrlicht.h>
#include "HighScoreData.h"

class HighScoreScreen
{
public:

    enum class Action
    {
        TITLE,
        QUIT
    };

    HighScoreScreen();
    ~HighScoreScreen();

    void Init();

    Action Run();

private:
    Action mAction;
    irr::gui::IGUIFont* mFont;
    bool mRunning;
    size_t mSelectSound;
    size_t mConfirmSound;

    struct ActionEntry
    {
        Action action;
        irr::core::stringw name;
    };
    static const size_t mNumEntries = 2;
    ActionEntry mActionEntries[mNumEntries] = {
        { Action::TITLE, L"Title" },
        { Action::QUIT, L"Quit" }
    };
    int mEntry;
    void HandleInput();

    void DrawHighScores();
};

#endif // HIGH_SCORE_SCREEN_H_INCLUDED
