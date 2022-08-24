#ifndef GAME_MAP_H_INCLUDED
#define GAME_MAP_H_INCLUDED

#include "MapFloorTile.h"

#include <irrlicht/irrlicht.h>

#include <vector>

class GameMap
{
public:
    GameMap();
    ~GameMap();

    void Init();

    // TODO - is this needed?
    //void Draw();

private:
    std::vector<std::vector<int>> mMap;
    std::vector<irr::scene::IAnimatedMesh*> mMeshes;
    std::vector<MapFloorTile*> mFloorTiles;
    std::vector<irr::scene::IAnimatedMeshSceneNode*> mTrees;
};

#endif // GAME_MAP_H_INCLUDED
