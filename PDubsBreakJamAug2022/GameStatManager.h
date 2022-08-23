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
    enum ScoreEvent
    {
        EVT_MAKE_PLAYER_WAIT,
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
};

// Global game stat manager instance
extern GameStatManager gGameStatMgr;

#endif // GAME_STAT_MANAGER_H_INCLUDED

