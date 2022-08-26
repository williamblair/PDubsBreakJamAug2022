#include "GameStatManager.h"
#include "GameHud.h"
#include <cstdio>

using namespace irr;

GameStatManager::GameStatManager() :
    mScore(0),
    mGameTime(0.0f),
    mGameTimeScoreDuration(5.0f),
    mGameTimeScoreCtr(0.0f),
    mGameTimeDurationScore(10),
    mNumMkPlyrWtEvts(0),
    mNumTauntPlyrEvts(0),
    mNumTrgrExplEvts(0),
    mNumSeenByEnmyEvts(0)
{}

GameStatManager::~GameStatManager()
{}

void GameStatManager::Reset()
{
    mScore = 0;
    mGameTime = 0.0f;
    mGameTimeScoreCtr = 0.0f;
    mNumMkPlyrWtEvts = 0;
    mNumTauntPlyrEvts = 0;
    mNumTrgrExplEvts = 0;
    mNumSeenByEnmyEvts = 0;
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
        message = L"Get Away From Player! +50";
        break;
    case EVT_TAUNT_PLAYER:
        ++mNumTauntPlyrEvts;
        mScore += 15;
        message = L"Taunt Player! +15";
        break;
    case EVT_TRIGGER_EXPLOSION:
        ++mNumTrgrExplEvts;
        mScore += 100;
        message = L"Trigger Explosion! +100";
        break;
    case EVT_SEEN_BY_ENEMY:
        ++mNumSeenByEnmyEvts;
        mScore += 500;
        message = L"Spotted By Enemy! +500";
        break;
    default:
        break;
    }
    gHud.AddEventNotification(message);
}

// Global game stat manager instance
GameStatManager gGameStatMgr;
