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
    mDevice = createDevice(video::EDT_OPENGL, dimension2d<u32>(640,480), 32, false, false, false, &gInputMgr);
    if (!mDevice) {
        throw std::runtime_error("Failed to create irrlicht device");
    }
    mDevice->setWindowCaption(L"Hello World - Irrlicht Engine Demo");
    mDriver = mDevice->getVideoDriver();
    mSmgr = mDevice->getSceneManager();
    mGuienv = mDevice->getGUIEnvironment();

    //mSmgr->addCameraSceneNode(0, vector3df(0,30,-40), vector3df(0,5,0));
    mDevice->getCursorControl()->setVisible(false);
    mCamera = 
        mSmgr->addCameraSceneNodeFPS(0, 100, 0.3f, 0, 0, 0, true, 3.0f);
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

// global Renderer instance
Renderer gRender;
