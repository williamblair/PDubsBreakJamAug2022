#ifndef GAME_MAP_H_INCLUDED
#define GAME_MAP_H_INCLUDED

#include "MapFloorTile.h"
#include "AIEnemy.h"

#include <irrlicht/irrlicht.h>

#include <vector>
#include <list>

class GameMap
{
public:
    GameMap();
    ~GameMap();

    void Init();

    // TODO - is this needed?
    //void Draw();

    void Update(const float dt);

    bool BoxCollidesWithExplosion(irr::core::aabbox3df box);
    bool BoxCollidesWithEnemyPatrol(irr::core::aabbox3df box);

    AIEnemy* GetNextEnemy();
    void DestroyEnemy(AIEnemy* enemy);

private:
    std::vector<std::vector<int>> mMap;
    std::vector<irr::scene::IAnimatedMesh*> mMeshes;
    std::vector<MapFloorTile*> mFloorTiles;
    std::vector<irr::scene::IAnimatedMeshSceneNode*> mTrees;
    std::list<irr::core::aabbox3df> mExplosionBoxes; // aabb's which trigger an explosion if player collides
    std::list<irr::scene::IAnimatedMeshSceneNode*> mTntBarrels;
    std::list<AIEnemy> mAiEnemies;
};

// global map instance
extern GameMap gMap;

#endif // GAME_MAP_H_INCLUDED
