#include "AIEnemy.h"
#include "Renderer.h"

using namespace irr;

AIEnemy::AIEnemy(
    irr::scene::IAnimatedMeshSceneNode* node,
    irr::core::vector3df* waypoints,
    size_t numWaypoints,
    float rotOffset
) :
    mState(State::PATROL_AREA),
    mNode(node),
    mRotOffset(rotOffset),
    mPtrlBoxDest(false)
{
    mWaypoints.resize(numWaypoints);
    for (size_t i=0; i<numWaypoints; ++i) {
        mWaypoints[i] = waypoints[i];
    }
    node->setPosition(mWaypoints[0]);
    mNode->setMD2Animation(scene::EMAT_RUN);
    mCurWaypoint = 1;

    const float scale = 25.0f;
    core::vector3df ptrlMin = mWaypoints[0];
    ptrlMin.X -= scale;
    //ptrlMin.Y -= scale;
    ptrlMin.Z -= scale;
    core::vector3df ptrlMax = mWaypoints[0];
    ptrlMax.X += scale;
    ptrlMax.Y += scale;
    ptrlMax.Z += scale;
    for (size_t i=1; i<mWaypoints.size(); ++i) {
        if ((mWaypoints[i].X+scale) > ptrlMax.X) { ptrlMax.X = mWaypoints[i].X+scale; }
        if ((mWaypoints[i].Y+scale) > ptrlMax.Y) { ptrlMax.Y = mWaypoints[i].Y+scale; }
        if ((mWaypoints[i].Z+scale) > ptrlMax.Z) { ptrlMax.Z = mWaypoints[i].Z+scale; }
        if ((mWaypoints[i].X-scale) < ptrlMin.X) { ptrlMin.X = mWaypoints[i].X-scale; }
        //if ((mWaypoints[i].Y-scale) < ptrlMin.Y) { ptrlMin.Y = mWaypoints[i].Y-scale; }
        if ((mWaypoints[i].Z-scale) < ptrlMin.Z) { ptrlMin.Z = mWaypoints[i].Z-scale; }
    }
    mPatrolBox = core::aabbox3df(ptrlMin, ptrlMax);
}

AIEnemy::~AIEnemy()
{
}

void AIEnemy::Update(const float dt)
{
    switch (mState)
    {
    case State::PATROL_AREA: WalkTowardsWaypoint(dt); break;
    case State::WALK_TOWARD_PLAYER: WalkTowardsPlayer(dt); break;
    case State::ATTACK_PLAYER: AttackPlayer(dt); break;
    default:
        break;
    }
}

void AIEnemy::WalkTowardsWaypoint(const float dt)
{
    // Get vector from our position to waypoint
    core::vector3df aiToPoint = mWaypoints[mCurWaypoint] - mNode->getPosition();
    
    float rot = atan2f(aiToPoint.X, aiToPoint.Z);
    mNode->setRotation(core::vector3df(0,core::radToDeg(rot)+mRotOffset,0));

    // Test if we are close enough to the waypoint to stop
    if (aiToPoint.getLengthSQ() < 50.0f) {
        ++mCurWaypoint;
        if (mCurWaypoint >= mWaypoints.size()) {
            mCurWaypoint = 0;
        }
    } else {
        // walk forward
        float walkSpeed = 50.0f;
        aiToPoint.normalize();
        mNode->setPosition(mNode->getPosition() + aiToPoint*dt*walkSpeed);
    }
}

void AIEnemy::WalkTowardsPlayer(const float dt)
{
    core::vector3df playerPos = gRender.GetCam()->getAbsolutePosition();

    // Get vector from our position to player
    core::vector3df aiToPoint = playerPos - mNode->getPosition();
    
    // Test if we are close enough to player to want to attach
    if (aiToPoint.getLengthSQ() < 1500.0f) {
        mState = State::ATTACK_PLAYER;
        mNode->setMD2Animation(scene::EMAT_ATTACK);
    }

    float rot = atan2f(aiToPoint.X, aiToPoint.Z);
    mNode->setRotation(core::vector3df(0,core::radToDeg(rot)+mRotOffset,0));

    // walk forward
    float walkSpeed = 50.0f;
    aiToPoint.normalize();
    mNode->setPosition(mNode->getPosition() + aiToPoint*dt*walkSpeed);
}

void AIEnemy::AttackPlayer(const float dt)
{
    core::vector3df playerPos = gRender.GetCam()->getAbsolutePosition();
    // Get vector from our position to player
    core::vector3df aiToPoint = playerPos - mNode->getPosition();
    // if we are far enough away from player, we want to move towards them
    if (aiToPoint.getLengthSQ() >= 3000.0f) {
        mState = State::WALK_TOWARD_PLAYER;
        mNode->setMD2Animation(scene::EMAT_RUN);
    }
}
