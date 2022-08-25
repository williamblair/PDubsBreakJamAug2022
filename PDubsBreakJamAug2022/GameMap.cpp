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

    {
        int i = 1;
        int j = 5;
        float x = i*100.0f;
        float z = j*100.0f;
        mExplosionBoxes.push_back(irr::core::aabbox3df(
            core::vector3df(x-50.0f,0.0f,z-50.0f), // min
            core::vector3df(x+50.0f,0.0f,z+50.0f) // max
        ));
    }

}

bool GameMap::BoxCollidesWithExplosion(core::aabbox3df box)
{
    if (mExplosionBoxes.empty()) {
        return false;
    }
    auto exBox = mExplosionBoxes.begin();
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
            return true;
        }
        ++exBox;
    }
    return false;
}
