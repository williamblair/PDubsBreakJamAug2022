#ifndef AI_PLAYER_H_INCLUDED
#define AI_PLAYER_H_INCLUDED

#include <list>

#include "Renderer.h"

class AIPlayer
{
public:

    enum class State
    {
        WALK_TOWARDS_WAYPOINT,
        ROTATE_TOWARDS_PLAYER,
        WAIT_FOR_PLAYER,
        IDLE
    };

    AIPlayer();
    ~AIPlayer();

    void Init();
    void SetNpcPos(irr::core::vector3df pos) { mNpcPos = pos; }
    void AddWaypoint(irr::core::vector3df pos) { mWaypoints.push_back(pos); }
    void Update(const float dt);

private:
    irr::scene::IAnimatedMeshSceneNode* mNode;
    State mState;
    irr::core::vector3df mNpcPos;
    std::list<irr::core::vector3df> mWaypoints;

    float mIdleTime;
    float mIdleCtr;

    void WalkTowardsWaypoint(const float dt);
    void RunIdle(const float dt);
    void WaitForPlayer(const float dt);

    void TriggerMakePlayerWait();
};

#endif // AI_PLAYER_H_INCLUDED 
