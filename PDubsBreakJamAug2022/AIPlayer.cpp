#include "AIPlayer.h"
#include "GameStatManager.h"
#include "GameMap.h"
#include "AudioManager.h"

using namespace irr;

// defined in main.cpp
extern bool gMainGameRunning;

AIPlayer::AIPlayer() :
    mNode(nullptr),
    mState(State::WALK_TOWARDS_WAYPOINT),
    mEnemy(nullptr),
    mIdleTime(3.0f),
    mIdleCtr(0.0f),
    mAttackEnemyTime(3.0f),
    mAttackEnemyCtr(0.0f)
{}

AIPlayer::~AIPlayer()
{}

void AIPlayer::Init()
{
    mNode = gRender.LoadAnimMesh("assets/ninja.b3d");
    mNode->setScale(core::vector3df(10));
    mNode->setRotation(core::vector3df(0,90,0));
    mNode->setAnimationSpeed(8.0f);
    mNode->setFrameLoop(0,13); // walk animation
    //mNode->getMaterial(0).NormalizeNormals = true;
    //mNode->getMaterial(0).Lighting = true;
    mNode->setMaterialFlag(video::EMF_LIGHTING, false);
    if (mWaypoints.size() > 0) {
        mNode->setPosition(*mWaypoints.begin());
    }

    mNpcPos = core::vector3df(0.0f,0.0f,0.0f);

    mHitSound = gAudioMgr.LoadSound("assets/punchhitkick.mp3");
    mGruntSound = gAudioMgr.LoadSound("assets/uhoh.mp3");
}

void AIPlayer::Update(const float dt)
{
    switch (mState)
    {
    case State::WALK_TOWARDS_WAYPOINT: WalkTowardsWaypoint(dt); break;
    case State::IDLE: RunIdle(dt); break;
    case State::WAIT_FOR_PLAYER: WaitForPlayer(dt); break;
    case State::WALK_TOWARD_ENEMY: WalkTowardsEnemy(dt); break;
    case State::ATTACK_ENEMY: AttackEnemy(dt); break;
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
    float walkSpeed = 70.0f;
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
            gAudioMgr.PlaySound(mGruntSound);
        }
        else {
            // otherwise, we can move towards the next waypoint
            mState = State::WALK_TOWARDS_WAYPOINT;
            if (mWaypoints.empty()) {
                gMainGameRunning = false;
            }
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
            gAudioMgr.PlaySound(mGruntSound);
            mIdleCtr = 0.0f;
        }
    }
}

void AIPlayer::WalkTowardsEnemy(const float dt)
{
    if (mEnemy == nullptr) {
        mEnemy = gMap.GetNextEnemy();
        if (mEnemy == nullptr) {
            mState = State::WAIT_FOR_PLAYER;
            return;
        }
        mAttackEnemyCtr = 0.0f;
    }

    // Get vector from our position to waypoint
    core::vector3df enPos = mEnemy->GetNode()->getAbsolutePosition();
    enPos.Y -= 50.0f;
    core::vector3df aiToPoint = enPos - mNode->getPosition();

    // Test if we are close enough to the waypoint to stop
    if (aiToPoint.getLengthSQ() < 800.0f) {
        //mWaypoints.erase(mWaypoints.begin());
        mState = State::ATTACK_ENEMY;
        mNode->setFrameLoop(31,43); // specific to ninja model
        gAudioMgr.PlaySound(mHitSound,1,true);
        return;
    }
    
    float rot = atan2f(aiToPoint.X, aiToPoint.Z);
    mNode->setRotation(core::vector3df(0,core::radToDeg(rot),0));

    // walk forward
    float walkSpeed = 70.0f;
    aiToPoint.normalize();
    mNode->setPosition(mNode->getPosition() + aiToPoint*dt*walkSpeed);
}

void AIPlayer::AttackEnemy(const float dt)
{
    if (mEnemy == nullptr) {
        mEnemy = gMap.GetNextEnemy();
        if (mEnemy == nullptr) {
            mState = State::WAIT_FOR_PLAYER;
            gAudioMgr.StopSound(1);
            return;
        }
         mNode->setFrameLoop(31,43); // specific to ninja model
    }

    // Get vector from our position to waypoint
    core::vector3df enPos = mEnemy->GetNode()->getAbsolutePosition();
    enPos.Y -= 50.0f;
    core::vector3df aiToPoint = enPos - mNode->getPosition();

    float rot = atan2f(aiToPoint.X, aiToPoint.Z);
    mNode->setRotation(core::vector3df(0,core::radToDeg(rot),0));

    // Test if we are close enough to the waypoint to stop
    if (aiToPoint.getLengthSQ() >= 6000.0f) {
        mState = State::WALK_TOWARD_ENEMY;
        mNode->setFrameLoop(0,13); // specific to ninja model
        gAudioMgr.StopSound(1);
        return;
    }

    mAttackEnemyCtr += dt;
    if (mAttackEnemyCtr >= mAttackEnemyTime) {
        gMap.DestroyEnemy(mEnemy);
        mEnemy = nullptr;
        mState = State::ATTACK_ENEMY;
        mNode->setFrameLoop(0,13);
        gAudioMgr.PlaySound(mHitSound,1,true);
    }
}

void AIPlayer::TriggerMakePlayerWait()
{
    gGameStatMgr.AddScoreEvent(GameStatManager::EVT_MAKE_PLAYER_WAIT);
}

// global AI instance
AIPlayer gAIPlayer;
