#ifndef END_GAME_SCREEN_H_INCLUDED
#define END_GAME_SCREEN_H_INCLUDED

#include <vector>

#include <irrlicht/irrlicht.h>


class EndGameScreen
{
public:

    enum Action
    {
        ACTION_TITLE,
        ACTION_QUIT
    };

    EndGameScreen();
    ~EndGameScreen();

    void Init();
    Action Run();

private:
    Action mAction;
    irr::gui::IGUIFont* mFont;
    bool mRunning;
    float mTimeCtr;
    int mScoreItemCtr;
    size_t mItemSound;
    
    struct ActionEntry
    {
        Action action;
        irr::core::stringw name;
    };
    static const size_t mNumEntries = 1;
    ActionEntry mActionEntries[mNumEntries] = {
        { ACTION_QUIT, L"Continue" }
    };
    int mEntry;
    
    void HandleInput();
    void DrawStats();
};

#endif // END_GAME_SCREEN_H_INCLUDED
