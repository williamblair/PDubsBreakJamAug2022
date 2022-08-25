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
            //printf("exBox: %f,%f,%f,  %f,%f,%f\n",
            //    exBox->MinEdge.X, exBox->MinEdge.Y, exBox->MinEdge.Z,
            //    exBox->MaxEdge.X, exBox->MaxEdge.Y, exBox->MaxEdge.Z);
            //printf("box: %f,%f,%f,  %f,%f,%f\n",
            //    box.MinEdge.X, box.MinEdge.Y, box.MinEdge.Z,
            //    box.MaxEdge.X, box.MaxEdge.Y, box.MaxEdge.Z);
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
