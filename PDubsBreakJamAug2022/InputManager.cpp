#include "InputManager.h"

InputManager::InputManager() :
    mUpPressed(false),
    mDownPressed(false),
    mConfirmPressed(false)
{}

InputManager::~InputManager()
{}

bool InputManager::OnEvent(const irr::SEvent& event)
{
    switch (event.EventType)
    {
    case irr::EET_KEY_INPUT_EVENT:
        switch (event.KeyInput.Key)
        {
        case irr::KEY_KEY_W:
        case irr::KEY_UP:
            mUpPressed = event.KeyInput.PressedDown;
            break;
        case irr::KEY_KEY_S:
        case irr::KEY_DOWN:
            mDownPressed = event.KeyInput.PressedDown;
            break;
        case irr::KEY_RETURN:
            mConfirmPressed = event.KeyInput.PressedDown;
            break;
        default:
            break;
        }
    default:
        break;
    }
  
    return false;
}

// global input manager instance
InputManager gInputMgr;
