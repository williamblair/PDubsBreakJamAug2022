#ifndef RENDERER_H_INCLUDED
#define RENDERER_H_INCLUDED

#include <string>

#include <irrlicht/irrlicht.h>

class Renderer
{
public:
    Renderer();
    ~Renderer();

    void Init();

    irr::scene::IAnimatedMeshSceneNode*
        LoadAnimMesh(const std::string fileName);

    void Clear();
    void Update();

    irr::IrrlichtDevice* GetDevice() const { return mDevice; }
    irr::video::IVideoDriver* GetDriver() const { return mDriver; }
    irr::scene::ISceneManager* GetSceneMgr() const { return mSmgr; }
    irr::gui::IGUIEnvironment* GetGuiEnv() const { return mGuienv; }
    irr::scene::ICameraSceneNode* GetCam() const { return mCamera; }

private:
    irr::IrrlichtDevice* mDevice;
    irr::video::IVideoDriver* mDriver;
    irr::scene::ISceneManager* mSmgr;
    irr::gui::IGUIEnvironment* mGuienv;
    irr::scene::ICameraSceneNode* mCamera;
};

// global Renderer instance
extern Renderer gRender;

#endif // RENDERER_H_INCLUDED
