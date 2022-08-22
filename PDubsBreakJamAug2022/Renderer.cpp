#include "Renderer.h"
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
    mGuienv(nullptr)
{}

Renderer::~Renderer()
{}

void Renderer::Init()
{
    mDevice = createDevice(video::EDT_OPENGL, dimension2d<u32>(640,480), 32, false, false, false, 0);
    if (!mDevice) {
        throw std::runtime_error("Failed to create irrlicht device");
    }
    mDevice->setWindowCaption(L"Hello World - Irrlicht Engine Demo");
    mDriver = mDevice->getVideoDriver();
    mSmgr = mDevice->getSceneManager();
    mGuienv = mDevice->getGUIEnvironment();

    mSmgr->addCameraSceneNode(0, vector3df(0,30,-40), vector3df(0,5,0));
}

void Renderer::Clear()
{
    mDriver->beginScene(true, true, SColor(255,100,101,140));
}

void Renderer::Update()
{
    mSmgr->drawAll();
    mGuienv->drawAll();
    mDriver->endScene();
}

IAnimatedMeshSceneNode* Renderer::LoadAnimMesh(const std::string fileName)
{
    std::string fileExt = "";
    fileExt += fileName[fileName.size()-3];
    fileExt += fileName[fileName.size()-2];
    fileExt += fileName[fileName.size()-1];
    // TODO - handle other mesh types
    if (fileExt != "md2" && fileExt != "md2") {
        throw std::runtime_error("Invalid anim mesh file extension\n");
    }
    IAnimatedMesh* mesh = mSmgr->getMesh(fileName.c_str());
    if (!mesh) {
        throw std::runtime_error("Failed to load mesh: " + fileName);
    }
    IAnimatedMeshSceneNode* node = mSmgr->addAnimatedMeshSceneNode(mesh);
    if (!node) {
        throw std::runtime_error("Failed to create IAnimatedMeshSceneNode for file: " + fileName);
    }
    return node;
}
