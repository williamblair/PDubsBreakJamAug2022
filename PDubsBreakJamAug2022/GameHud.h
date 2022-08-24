#ifndef GAME_HUD_H_INCLUDED
#define GAME_HUD_H_INCLUDED

#include <irrlicht/irrlicht.h>

class GameHud
{
public:
    GameHud();
    ~GameHud();

    void Init();
    void Update();
private:
    irr::gui::IGUIFont* mFont;
    irr::gui::IGUIStaticText* mTimeText;
    irr::gui::IGUIStaticText* mScoreText;
};

#endif // GAME_HUD_H_INCLUDED


