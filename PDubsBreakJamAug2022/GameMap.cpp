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
            }
            if (mMap[i][j] == 4) {
                gAIPlayer.AddWaypoint(core::vector3df(i*100.0f,0.0f,j*100.0f));
            }
            mFloorTiles.emplace_back(
                new MapFloorTile(
                    gRender.GetSceneMgr()->getRootSceneNode(), gRender.GetSceneMgr(), mMap[i][j]
                )
            );
            (*mFloorTiles.rbegin())->setPosition(core::vector3df(i*100.0f,0.0f,j*100.0f));
        }
    }
}
