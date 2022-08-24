#ifndef MAP_FLOOR_TILE_H_INCLUDED
#define MAP_FLOOR_TILE_H_INCLUDED

#include <irrlicht/irrlicht.h>

class MapFloorTile : public irr::scene::ISceneNode
{
public:
    MapFloorTile(
        irr::scene::ISceneNode* parent,
        irr::scene::ISceneManager* mgr,
        irr::s32 id
    );
    ~MapFloorTile();

    virtual void OnRegisterSceneNode();

    virtual void render();

    virtual const irr::core::aabbox3d<irr::f32>& getBoundingBox() const;
    virtual irr::u32 getMaterialCount() const;
    virtual irr::video::SMaterial& getMaterial(irr::u32 i);

private:
    irr::core::aabbox3d<irr::f32> mBox;
    irr::video::S3DVertex mVertices[6];
    irr::video::SMaterial mMaterial;
};

#endif
