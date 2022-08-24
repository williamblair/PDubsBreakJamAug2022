#include "AIPlayer.h"
#include "GameStatManager.h"

using namespace irr;

AIPlayer::AIPlayer() :
    mNode(nullptr),
    mState(State::WALK_TOWARDS_WAYPOINT),
    mIdleTime(5.0f),
    mIdleCtr(0.0f)
{}

AIPlayer::~AIPlayer()
{}

void AIPlayer::Init()
{
    mNode = gRender.LoadAnimMesh("assets/ninja.b3d");
    mNode->setScale(core::vector3df(10));
    mNode->setPosition(core::vector3df(-75,-66,-80));
    mNode->setRotation(core::vector3df(0,90,0));
    mNode->setAnimationSpeed(8.0f);
    mNode->setFrameLoop(0,13); // walk animation
    //mNode->getMaterial(0).NormalizeNormals = true;
    //mNode->getMaterial(0).Lighting = true;
    mNode->setMaterialFlag(video::EMF_LIGHTING, false);

    mNpcPos = core::vector3df(0.0f,0.0f,0.0f);

    /*AddWaypoint(core::vector3df(-100,-66,-80));
    AddWaypoint(core::vector3df(100,-66,-80));
    AddWaypoint(core::vector3df(100,-66,80));
    AddWaypoint(core::vector3df(-100,-66,80));*/
}

void AIPlayer::Update(const float dt)
{
    switch (mState)
    {
    case State::WALK_TOWARDS_WAYPOINT: WalkTowardsWaypoint(dt); break;
    case State::IDLE: RunIdle(dt); break;
    case State::WAIT_FOR_PLAYER: WaitForPlayer(dt); break;
    default:
        break;
    }
}

void AIPlayer::WalkTowardsWaypoint(const float dt)
{
    if (mWaypoints.empty()) {
        mState = State::IDLE;
        mIdleCtr = 0.0f;
        return;
    }

    // Get vector from our position to waypoint
    core::vector3df aiToPoint = *mWaypoints.begin() - mNode->getPosition();

    // Test if we are close enough to the waypoint to stop
    if (aiToPoint.getLengthSQ() < 100.0f) {
        mWaypoints.erase(mWaypoints.begin());
        mState = State::IDLE;
        mIdleCtr = 0.0f;
        return;
    }
    
    float rot = atan2f(aiToPoint.X, aiToPoint.Z);
    mNode->setRotation(core::vector3df(0,core::radToDeg(rot),0));

    // walk forward
    float walkSpeed = 50.0f;
    aiToPoint.normalize();
    mNode->setPosition(mNode->getPosition() + aiToPoint*dt*walkSpeed);
}

void AIPlayer::RunIdle(const float dt)
{
    mIdleCtr += dt;
    if (mIdleCtr >= mIdleTime) {

        mIdleCtr = 0.0f;

        // get vector from AI to actual player
        core::vector3df aiToPlayer = mNpcPos - mNode->getPosition();

        // if not close enough, we need to wait for them to get close enough
        if (aiToPlayer.getLengthSQ() > 20000.0f) {
            mState = State::WAIT_FOR_PLAYER;
            TriggerMakePlayerWait();
        }
        else {
            // otherwise, we can move towards the next waypoint
            mState = State::WALK_TOWARDS_WAYPOINT;
        }
    }
}

void AIPlayer::WaitForPlayer(const float dt)
{
    // get vector from AI to actual player
    core::vector3df aiToPlayer = mNpcPos - mNode->getPosition();

    // rotate to face player
    float rot = atan2f(aiToPlayer.X, aiToPlayer.Z);
    mNode->setRotation(core::vector3df(0,core::radToDeg(rot),0));

    // wait for a little bit regardless
    if (mIdleCtr < mIdleTime) { mIdleCtr += dt; }
    if (mIdleCtr >= mIdleTime) {
        // if the player is close enough, we can continue towards the next waypoint
        if (aiToPlayer.getLengthSQ() < 20000.0f) {
            mState = State::WALK_TOWARDS_WAYPOINT;
        } else {
            // reset idle waiting time
            TriggerMakePlayerWait();
            mIdleCtr = 0.0f;
        }
    }
}

void AIPlayer::TriggerMakePlayerWait()
{
    // TODO - sound effects, player angry animation, GUI popup
    gGameStatMgr.AddScoreEvent(GameStatManager::EVT_MAKE_PLAYER_WAIT);
}

// global AI instance
AIPlayer gAIPlayer;
