#ifndef INPUT_MANAGER_H_INCLUDED
#define INPUT_MANAGER_H_INCLUDED

#include <irrlicht/irrlicht.h>

class InputManager : public irr::IEventReceiver
{
public:
    InputManager();
    ~InputManager();
    
    virtual bool OnEvent(const irr::SEvent& event);
    
    bool UpPressed() { bool tmp = mUpPressed; if (tmp) mUpPressed = false; return tmp; }
    bool DownPressed() { bool tmp = mDownPressed; if (tmp) mDownPressed = false; return tmp; }
    bool ConfirmPressed() { bool tmp = mConfirmPressed; if (tmp) mConfirmPressed = false; return tmp; }

private:
    bool mUpPressed;
    bool mDownPressed;
    bool mConfirmPressed;
};

// global input manager instance
extern InputManager gInputMgr;

#endif // INPUT_MANAGER_H_INCLUDED
