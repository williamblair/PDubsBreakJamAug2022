#ifndef END_GAME_SCREEN_H_INCLUDED
#define END_GAME_SCREEN_H_INCLUDED

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
    
    struct ActionEntry
    {
        Action action;
        irr::core::stringw name;
    };
    static const size_t mNumEntries = 2;
    ActionEntry mActionEntries[mNumEntries] = {
        { ACTION_TITLE, L"Title Screen" },
        { ACTION_QUIT, L"Quit" }
    };
    int mEntry;
    
    void HandleInput();
};

#endif // END_GAME_SCREEN_H_INCLUDED
