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

    void AddTriggerExplosion();
private:
    irr::gui::IGUIFont* mFont;
    irr::gui::IGUIStaticText* mTimeText;
    irr::gui::IGUIStaticText* mScoreText;
    irr::gui::IGUIStaticText* mEventText;
    irr::video::ITexture* mRightBackTex;
    irr::video::ITexture* mExplosionTex;
    irr::gui::IGUIImage* mRightBackImg;
    irr::gui::IGUIImage* mExplosionImg;

    // how long event notification popup should last
    float mEventTimeCtr;
    float mEventTime;

    // how long explosion popup should last
    float mExplosionTime;
    // we'll be right back sound effect
    size_t mRightBackSound;
    bool mPlayingExplosion;
};

// global GameHud instance
extern GameHud gHud;

#endif // GAME_HUD_H_INCLUDED


