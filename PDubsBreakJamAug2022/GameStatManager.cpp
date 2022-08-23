#include "GameStatManager.h"
#include <cstdio>

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
    // possibly TODO - GUI popup of event?
    // or maybe game event manager for listeners, emitters?
    switch (evt)
    {
    case EVT_MAKE_PLAYER_WAIT: ++mNumMkPlyrWtEvts; mScore += 50; break;
    default:
        break;
    }
}

// Global game stat manager instance
GameStatManager gGameStatMgr;
