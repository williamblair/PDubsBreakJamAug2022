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
    irr::gui::IGUIFont*
        LoadFont(const std::string fileName);
    irr::video::ITexture*
        LoadTexture(const std::string fileName);

    void Clear();
    void Update(bool drawAll = true);

    irr::IrrlichtDevice* GetDevice() const { return mDevice; }
    irr::video::IVideoDriver* GetDriver() const { return mDriver; }
    irr::scene::ISceneManager* GetSceneMgr() const { return mSmgr; }
    irr::gui::IGUIEnvironment* GetGuiEnv() const { return mGuienv; }
    irr::scene::ICameraSceneNode* GetCam() const { return mCamera; }

    irr::core::aabbox3df GetCameraCollisionBox() {
        irr::core::vector3df pos = mCamera->getAbsolutePosition();
        irr::core::vector3df plyrMin(pos.X-50.0f, 0.0f, pos.Z-50.0f);
        irr::core::vector3df plyrMax(pos.X+50.0f, 0.0f, pos.Z+50.0f);
        return irr::core::aabbox3df(plyrMin, plyrMax);
    }

    void SetFPSInputEnabled(bool isEnabled) {
        mCamera->setInputReceiverEnabled(isEnabled);
    }

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
