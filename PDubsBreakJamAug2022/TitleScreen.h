#ifndef TITLE_SCREEN_H_INCLUDED
#define TITLE_SCREEN_H_INCLUDED

#include <irrlicht/irrlicht.h>

class TitleScreen
{
public:

    enum Action
    {
        ACTION_PLAY,
        ACTION_QUIT
    };

    TitleScreen();
    ~TitleScreen();
    
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
        { ACTION_PLAY, L"Play" },
        { ACTION_QUIT, L"Quit" }
    };
    int mEntry;
    void HandleInput();
};

#endif // TITLE_SCREEN_H_INCLUDED
