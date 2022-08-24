#include "MapFloorTile.h"
#include "Renderer.h"
#include <cstdio>

using namespace irr;

MapFloorTile::MapFloorTile(
    irr::scene::ISceneNode* parent,
    irr::scene::ISceneManager* mgr,
    s32 id) :
    scene::ISceneNode(parent, mgr, id)
{
    mMaterial.Wireframe = false;
    mMaterial.Lighting = false;

    mVertices[0] = video::S3DVertex(-50,0,-50, 0,1,0, video::SColor(255,255,255,255), 0,1);
    mVertices[1] = video::S3DVertex(50,0,-50, 0,1,0, video::SColor(255,255,255,255), 1,1);
    mVertices[2] = video::S3DVertex(50,0,50, 0,1,0, video::SColor(255,255,255,255), 1,0);

    mVertices[3] = video::S3DVertex(-50,0,-50, 0,1,0, video::SColor(255,255,255,255), 0,1);
    mVertices[4] = video::S3DVertex(50,0,50, 0,1,0, video::SColor(255,255,255,255), 1,0);
    mVertices[5] = video::S3DVertex(-50,0,50, 0,1,0, video::SColor(255,255,255,255), 0,0);

    mBox.reset(mVertices[0].Pos);
    for (s32 i=0; i<6; ++i) {
        mBox.addInternalPoint(mVertices[i].Pos);
    }

    switch (id)
    {
    case 0:
    case 2:
        mMaterial.setTexture(0, gRender.GetDriver()->getTexture("assets/grass.jpg"));
        break;
    case 1:
    case 4:
        mMaterial.setTexture(0, gRender.GetDriver()->getTexture("assets/dirt.jpg"));
        break;
        break;
    default:
        break;
    }
}

MapFloorTile::~MapFloorTile()
{
}

void MapFloorTile::OnRegisterSceneNode()
{
    if (IsVisible) {
        gRender.GetSceneMgr()->registerNodeForRendering(this);
    }
    scene::ISceneNode::OnRegisterSceneNode();
}

void MapFloorTile::render()
{
    //u16 indices[] = { 0,2,3, 2,1,3, 1,0,3, 2,0,1 };
    u16 indices[] = { 2,1,0, 5,4,3 };
    // possibly TODO - indices?
    auto* driver = gRender.GetDriver();
    driver->setMaterial(mMaterial);
    driver->setTransform(video::ETS_WORLD, AbsoluteTransformation);
    driver->drawVertexPrimitiveList(&mVertices[0], 6, &indices[0], 2, video::EVT_STANDARD, scene::EPT_TRIANGLES, video::EIT_16BIT);
    //driver->drawVertexPrimitiveList(&mVertices[0], 4, &indices[0], 4, video::EVT_STANDARD, scene::EPT_TRIANGLES, video::EIT_16BIT);
}

const irr::core::aabbox3d<f32>& MapFloorTile::getBoundingBox() const
{
    return mBox;
}

u32 MapFloorTile::getMaterialCount() const
{
    return 1;
}

video::SMaterial& MapFloorTile::getMaterial(u32 i)
{
    return mMaterial;
}
