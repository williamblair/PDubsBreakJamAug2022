#ifndef AI_ENEMY_H_INCLUDED
#define AI_ENEMY_H_INCLUDED

#include <vector>

#include <irrlicht/irrlicht.h>

class AIEnemy
{
public:

    enum class State
    {
        PATROL_AREA,
        WALK_TOWARD_PLAYER,
        ATTACK_PLAYER,
        DYING
    };

    AIEnemy(
        irr::scene::IAnimatedMeshSceneNode* node,
        irr::core::vector3df* waypoints,
        size_t numWaypoints,
        float rotOffset
    );
    ~AIEnemy();

    void Update(const float dt);
    void SetState(State state) { mState = state; }

    irr::scene::IAnimatedMeshSceneNode* GetNode() const { return mNode; }
    irr::core::aabbox3df* GetBoundingBox() { if (!mPtrlBoxDest) { return &mPatrolBox; } return nullptr; }
    void DestroyBoundingBox() { mPtrlBoxDest = true; }

private:
    State mState;
    irr::scene::IAnimatedMeshSceneNode* mNode;
    float mRotOffset;
    std::vector<irr::core::vector3df> mWaypoints;
    size_t mCurWaypoint;
    irr::core::aabbox3df mPatrolBox;
    bool mPtrlBoxDest;

    void WalkTowardsWaypoint(const float dt);
    void WalkTowardsPlayer(const float dt);
    void AttackPlayer(const float dt);
};

#endif // AI_ENEMY_H_INCLUDED

