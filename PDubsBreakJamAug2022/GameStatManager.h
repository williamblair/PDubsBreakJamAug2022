#ifndef GAME_STAT_MANAGER_H_INCLUDED
#define GAME_STAT_MANAGER_H_INCLUDED

#include <cmath>
#include <cstdint>

class GameStatManager
{
public:
    GameStatManager();
    ~GameStatManager();

    void Reset();

    void UpdateGameTime(const float dt);
    float GetGameTime() const { return mGameTime; }
    int GetGameTimeMinutes() { return (int)(mGameTime / 60.0f); }
    int GetGameTimeSeconds() { return (int)fmodf(mGameTime, 60.0f); }


    uint64_t GetScore() const { return mScore; }
    uint64_t GetNumMkPlyrWtEvts() const { return mNumMkPlyrWtEvts; }
    uint64_t GetNumTauntPlyrEvts() const { return mNumTauntPlyrEvts; }
    uint64_t GetNumTrgrExplEvts() const { return mNumTrgrExplEvts; }
    uint64_t GetNumSeenByEnmyEvts() const { return mNumSeenByEnmyEvts; }
    enum ScoreEvent
    {
        EVT_MAKE_PLAYER_WAIT,
        EVT_TAUNT_PLAYER,
        EVT_TRIGGER_EXPLOSION,
        EVT_SEEN_BY_ENEMY,
        EVT_NONE
    };
    void AddScoreEvent(ScoreEvent evt);

private:
    uint64_t mScore; // total points earned
    float mGameTime; // how long the game has been running

    // how long until score is increased by making the game run longer
    float mGameTimeScoreDuration;
    float mGameTimeScoreCtr;
    uint64_t mGameTimeDurationScore; // how many points are added when duration is reached

    uint64_t mNumMkPlyrWtEvts; // number of 'make player wait' events
    uint64_t mNumTauntPlyrEvts; // number of 'taunt player' events
    uint64_t mNumTrgrExplEvts; // number of 'trigger explosion' events
    uint64_t mNumSeenByEnmyEvts; // number of 'scene by enemy' events
};

// Global game stat manager instance
extern GameStatManager gGameStatMgr;

#endif // GAME_STAT_MANAGER_H_INCLUDED

