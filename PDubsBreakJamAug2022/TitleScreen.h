#ifndef TITLE_SCREEN_H_INCLUDED
#define TITLE_SCREEN_H_INCLUDED

#include <irrlicht/irrlicht.h>

class TitleScreen
{
public:

    enum Action
    {
        ACTION_PLAY,
        ACTION_HIGHSCORES,
        ACTION_QUIT
    };

    TitleScreen();
    ~TitleScreen();
    
    void Init();

    Action Run(bool startMusic);
    
private:
    Action mAction;
    irr::gui::IGUIFont* mFont;
    size_t mBgMusic;
    size_t mHilightSound;
    size_t mSelectSound;
    float mNamePos;
    
    bool mRunning;

    struct ActionEntry
    {
        Action action;
        irr::core::stringw name;
    };
    static const size_t mNumEntries = 3;
    ActionEntry mActionEntries[mNumEntries] = {
        { ACTION_PLAY, L"Play" },
        { ACTION_HIGHSCORES, L"High Scores" },
        { ACTION_QUIT, L"Quit" }
    };
    int mEntry;
    void HandleInput();
};

#endif // TITLE_SCREEN_H_INCLUDED
