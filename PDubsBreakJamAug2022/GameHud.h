#ifndef GAME_HUD_H_INCLUDED
#define GAME_HUD_H_INCLUDED

#include <irrlicht/irrlicht.h>

class GameHud
{
public:
    GameHud();
    ~GameHud();

    void Init();
    void Update(const float dt);

    void AddEventNotification(irr::core::stringw message);
private:
    irr::gui::IGUIFont* mFont;
    irr::gui::IGUIStaticText* mTimeText;
    irr::gui::IGUIStaticText* mScoreText;
    irr::gui::IGUIStaticText* mEventText;

    // how long event notification popup should last
    float mEventTimeCtr;
    float mEventTime;
};

// global GameHud instance
extern GameHud gHud;

#endif // GAME_HUD_H_INCLUDED


