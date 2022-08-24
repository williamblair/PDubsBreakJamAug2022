#include "GameStatManager.h"
#include "GameHud.h"
#include <cstdio>

using namespace irr;

GameStatManager::GameStatManager() :
    mScore(0),
    mGameTime(0.0f),
    mGameTimeScoreDuration(5.0f),
    mGameTimeScoreCtr(0.0f),
    mGameTimeDurationScore(10)
{}

GameStatManager::~GameStatManager()
{}

void GameStatManager::Reset()
{
    mScore = 0;
    mGameTime = 0.0f;
    mGameTimeScoreCtr = 0.0f;
}

void GameStatManager::UpdateGameTime(const float dt)
{
    mGameTime += dt;
    mGameTimeScoreCtr += dt;
    if (mGameTimeScoreCtr >= mGameTimeScoreDuration) {
        while (mGameTimeScoreCtr >= mGameTimeScoreDuration) {
            mGameTimeScoreCtr -= mGameTimeScoreDuration;
        }
        mScore += mGameTimeDurationScore;
        printf("mScore: %llu\n", mScore);
    }
}

void GameStatManager::AddScoreEvent(GameStatManager::ScoreEvent evt)
{
    core::stringw message = "";
    switch (evt)
    {
    case EVT_MAKE_PLAYER_WAIT:
        ++mNumMkPlyrWtEvts;
        mScore += 50;
        message = L"Make Player Wait!";
        break;
    default:
        break;
    }
    gHud.AddEventNotification(message);
}

// Global game stat manager instance
GameStatManager gGameStatMgr;
