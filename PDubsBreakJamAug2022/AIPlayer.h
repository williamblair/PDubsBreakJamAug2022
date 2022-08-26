#ifndef AI_PLAYER_H_INCLUDED
#define AI_PLAYER_H_INCLUDED

#include <list>

#include "Renderer.h"
#include "AIEnemy.h"

class AIPlayer
{
public:

    enum class State
    {
        WALK_TOWARDS_WAYPOINT,
        ROTATE_TOWARDS_PLAYER,
        WAIT_FOR_PLAYER,
        IDLE,
        WALK_TOWARD_ENEMY,
        ATTACK_ENEMY
    };

    AIPlayer();
    ~AIPlayer();

    void Init();
    void SetNpcPos(irr::core::vector3df pos) { mNpcPos = pos; }
    void SetState(State state) { mState = state; }
    void AddWaypoint(irr::core::vector3df pos) { mWaypoints.push_back(pos); }
    void Update(const float dt);

private:
    irr::scene::IAnimatedMeshSceneNode* mNode;
    State mState;
    AIEnemy* mEnemy;
    irr::core::vector3df mNpcPos;
    std::list<irr::core::vector3df> mWaypoints;

    float mIdleTime;
    float mIdleCtr;

    float mAttackEnemyTime;
    float mAttackEnemyCtr;

    void WalkTowardsWaypoint(const float dt);
    void RunIdle(const float dt);
    void WaitForPlayer(const float dt);
    void WalkTowardsEnemy(const float dt);
    void AttackEnemy(const float dt);

    void TriggerMakePlayerWait();
};

extern AIPlayer gAIPlayer;

#endif // AI_PLAYER_H_INCLUDED 
