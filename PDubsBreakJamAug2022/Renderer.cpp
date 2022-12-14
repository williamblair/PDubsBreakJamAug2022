#include "Renderer.h"
#include "InputManager.h"
#include <stdexcept>

using namespace irr;

using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

Renderer::Renderer() :
    mDevice(nullptr),
    mDriver(nullptr),
    mSmgr(nullptr),
    mGuienv(nullptr),
    mCamera(nullptr)
{}

Renderer::~Renderer()
{}

void Renderer::Init()
{
    mDevice = createDevice(video::EDT_OPENGL, dimension2d<u32>(800,600), 32, false, false, false, &gInputMgr);
    if (!mDevice) {
        throw std::runtime_error("Failed to create irrlicht device");
    }
    mDevice->setWindowCaption(L"NPC Escort Simulator");
    mDriver = mDevice->getVideoDriver();
    mSmgr = mDevice->getSceneManager();
    mGuienv = mDevice->getGUIEnvironment();

    SKeyMap keyMap[4];
    keyMap[0].Action = EKA_MOVE_FORWARD;
    keyMap[0].KeyCode = KEY_KEY_W;
    keyMap[1].Action = EKA_MOVE_BACKWARD;
    keyMap[1].KeyCode = KEY_KEY_S;
    keyMap[2].Action = EKA_STRAFE_LEFT;
    keyMap[2].KeyCode = KEY_KEY_A;
    keyMap[3].Action = EKA_STRAFE_RIGHT;
    keyMap[3].KeyCode = KEY_KEY_D;

    //mSmgr->addCameraSceneNode(0, vector3df(0,30,-40), vector3df(0,5,0));
    mDevice->getCursorControl()->setVisible(false);
    mCamera = 
        mSmgr->addCameraSceneNodeFPS(0, 100, 0.3f, 0, keyMap, 4, true, 3.0f);
    mCamera->setPosition(core::vector3df(50,50,-60));
    mCamera->setTarget(core::vector3df(-70,30,-60));
}

void Renderer::Clear()
{
    mDriver->beginScene(true, true, SColor(255,100,101,140));
}

void Renderer::Update(bool drawAll)
{
    if (drawAll) {
        mSmgr->drawAll();
        mGuienv->drawAll();
    }
    mDriver->endScene();
}

IAnimatedMeshSceneNode* Renderer::LoadAnimMesh(const std::string fileName)
{
    IAnimatedMesh* mesh = mSmgr->getMesh(fileName.c_str());
    if (!mesh) {
        throw std::runtime_error("Failed to load mesh: " + fileName);
    }
    // TODO - add desired IDs for collision checking
    IAnimatedMeshSceneNode* node = mSmgr->addAnimatedMeshSceneNode(mesh);
    if (!node) {
        throw std::runtime_error("Failed to create IAnimatedMeshSceneNode for file: " + fileName);
    }
    return node;
}

IGUIFont* Renderer::LoadFont(const std::string fileName)
{
    IGUIFont* font = mGuienv->getFont(fileName.c_str());
    if (!font) {
        throw std::runtime_error("Failed to load font: " + fileName);
    }
    return font;
}

ITexture* Renderer::LoadTexture(const std::string fileName)
{
    ITexture* tex = mDriver->getTexture(fileName.c_str());
    if (!tex) {
        throw std::runtime_error("Failed to load texture: " + fileName);
    }
    return tex;
}

// global Renderer instance
Renderer gRender;
