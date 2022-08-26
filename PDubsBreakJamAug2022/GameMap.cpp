#include "GameMap.h"
#include "Renderer.h"
#include "AIPlayer.h"

#include <fstream>
#include <sstream>

using namespace irr;

GameMap::GameMap()
{}

GameMap::~GameMap()
{
    for (MapFloorTile* tile : mFloorTiles) {
        delete tile;
    }
}

// 0 = grass
// 1 = path dirt
// 2 = tree
// 4 = waypoint
// 5 - tnt barrel

void GameMap::Init()
{
    std::ifstream mapFile("assets/map.txt");
    if (!mapFile.is_open()) {
        throw std::runtime_error("Failed to open assets/map.txt");
    }

    std::string curLine;
    while (std::getline(mapFile, curLine))
    {
        std::stringstream ss(curLine);
        char curChar;
        int curVal;
        std::vector<int> lineVals;
        while (ss >> curChar)
        {
            curVal = (int)(curChar - '0');
            lineVals.push_back(curVal);
        }
        mMap.push_back(lineVals);
    }
    mapFile.close();

    for (size_t i=0; i<mMap.size(); ++i) {
        for (size_t j=0; j<mMap[0].size(); ++j) {
            if (mMap[i][j] == 2) {
                mTrees.push_back(gRender.LoadAnimMesh("assets/pinetree.obj"));
                (*mTrees.rbegin())->setPosition(core::vector3df(i*100.0f,170.0f,j*100.0f));
                (*mTrees.rbegin())->setMaterialFlag(video::EMF_LIGHTING, false);
                (*mTrees.rbegin())->setScale(core::vector3df(100.0f,100.0f,100.0f));
            } else if (mMap[i][j] == 5) {
                mTntBarrels.push_back(gRender.LoadAnimMesh("assets/barrel.obj"));
                (*mTntBarrels.rbegin())->setPosition(core::vector3df(i*100.0f,00.0f,j*100.0f));
                (*mTntBarrels.rbegin())->setMaterialFlag(video::EMF_LIGHTING, false);
                (*mTntBarrels.rbegin())->setScale(core::vector3df(0.5f,0.5f,0.5f));
                (*mTntBarrels.rbegin())->setRotation(core::vector3df(0.0f,90.0f,0.0f));
                (*mTntBarrels.rbegin())->setMaterialTexture(0,gRender.LoadTexture("assets/barrel_diff.tga"));
                {
                    float x = i*100.0f;
                    float z = j*100.0f;
                    core::aabbox3df bndBox(
                        core::vector3df(x-15.0f,0.0f,z-15.0f), // min
                        core::vector3df(x+15.0f,15.0f,z+15.0f) // max
                    );
                    mExplosionBoxes.push_back(bndBox);
                }
            }
            mFloorTiles.emplace_back(
                new MapFloorTile(
                    gRender.GetSceneMgr()->getRootSceneNode(), gRender.GetSceneMgr(), mMap[i][j]
                )
            );
            (*mFloorTiles.rbegin())->setPosition(core::vector3df(i*100.0f,0.0f,j*100.0f));
        }
    }

    std::ifstream wayptFile("assets/waypoints.txt");
    if (!wayptFile.is_open()) {
        throw std::runtime_error("Failed to open waypoints file");
    }
    while (std::getline(wayptFile, curLine))
    {
        int i, j;
        std::stringstream ss(curLine);
        ss >> i;
        ss >> j;
        gAIPlayer.AddWaypoint(core::vector3df(i*100.0f,0.0f,j*100.0f));
    }
    wayptFile.close();
#if 0
    {
        core::vector3df waypoints[2] = {
            core::vector3df(5*100.0f,50.0f,5*100.0f),
            core::vector3df(7*100.0f,50.0f,5*100.0f)
        };
        scene::IAnimatedMeshSceneNode* node = gRender.LoadAnimMesh("assets/faerie.md2");
        node->setMaterialFlag(video::EMF_LIGHTING, false);
        node->setMaterialTexture(0,gRender.LoadTexture("assets/faerie2.bmp"));
        mAiEnemies.push_back(AIEnemy(node, waypoints, 2, -90.0f));
        mAiEnemies.rbegin()->SetState(AIEnemy::State::WALK_TOWARD_PLAYER);
    }
#endif
    std::ifstream enmyptFile("assets/enemywaypoints.txt");
    while (std::getline(enmyptFile, curLine))
    {
        printf("enemywaypoints file get line\n");
        int i, j;
        std::stringstream ss(curLine);
        std::vector<core::vector3df> points;
        while (ss >> i)
        {
            ss >> j;

            printf("  line i, j: %d,%d\n", i, j);

            // y 50.0f specific to faerie.md2
            points.push_back(core::vector3df(i*100.0f,50.0f,j*100.0f));
        }
        scene::IAnimatedMeshSceneNode* node = gRender.LoadAnimMesh("assets/faerie.md2");
        node->setMaterialFlag(video::EMF_LIGHTING, false);
        node->setMaterialTexture(0,gRender.LoadTexture("assets/faerie2.bmp"));
        node->setScale(core::vector3df(2.0f,2.0f,2.0f));
        printf("AIenemy before constructor\n");
        mAiEnemies.push_back(AIEnemy(node, points.data(), points.size(), -90.0f));
        printf("AIenemy after constructor\n");
        //mAiEnemies.rbegin()->SetState(AIEnemy::State::WALK_TOWARD_PLAYER);
    }
}

void GameMap::Update(const float dt)
{
    for (AIEnemy& enemy : mAiEnemies) {
        enemy.Update(dt);
    }
}

bool GameMap::BoxCollidesWithExplosion(core::aabbox3df box)
{
    if (mExplosionBoxes.empty()) {
        return false;
    }
    auto exBox = mExplosionBoxes.begin();
    auto brlObj = mTntBarrels.begin();
    while (exBox != mExplosionBoxes.end() && !mExplosionBoxes.empty())
    {
        if (exBox->intersectsWithBox(box)) {
            mExplosionBoxes.erase(exBox);
            (*brlObj)->setVisible(false);
            mTntBarrels.erase(brlObj);
            return true;
        }
        ++exBox;
        ++brlObj;
    }
    return false;
}

bool GameMap::BoxCollidesWithEnemyPatrol(core::aabbox3df box) {
    if (mAiEnemies.empty()) {
        return false;
    }
    /*printf("Input box to enemy control: %f,%f,%f,  %f,%f,%f\n",
        box.MinEdge.X,box.MinEdge.Y,box.MinEdge.Z,
        box.MaxEdge.X,box.MaxEdge.Y,box.MaxEdge.Z);*/
    auto en = mAiEnemies.begin();
    while (en != mAiEnemies.end() && !mAiEnemies.empty()) {
        /*if (en->GetBoundingBox()) {
            auto* ebox = en->GetBoundingBox();
            printf("enemy control box: %f,%f,%f,  %f,%f,%f\n",
                ebox->MinEdge.X,ebox->MinEdge.Y,ebox->MinEdge.Z,
                ebox->MaxEdge.X,ebox->MaxEdge.Y,ebox->MaxEdge.Z);
        }*/
        if (en->GetBoundingBox() && box.intersectsWithBox(*en->GetBoundingBox())) {
            //printf("GameMap enemy patrol box Intersects\n");
            en->DestroyBoundingBox();
            return true;
        }
        ++en;
    }
    return false;
}

AIEnemy* GameMap::GetNextEnemy()
{
    if (mAiEnemies.empty()) {
        return nullptr;
    }
    return &(*mAiEnemies.begin());
}

void GameMap::DestroyEnemy(AIEnemy* enemy)
{
    if (mAiEnemies.empty()) {
        return;
    }

    auto iter = mAiEnemies.begin();
    while (iter != mAiEnemies.end()) {
        if (&(*iter) == enemy) {
            enemy->GetNode()->setVisible(false);
            mAiEnemies.erase(iter);
            return;
        }
    }
}

// global map instance
GameMap gMap;
