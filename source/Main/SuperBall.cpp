#include "SuperBall.hpp"


static const PointSensor BallbottomSensor = {0.0fx, 0.0fx, SensorFlags::ActivateCoins|SensorFlags::ActivateCoinOutlines|
SensorFlags::ActivateFallingBlocks|SensorFlags::ActivateQuestionBlocks|SensorFlags::ActivateBricksSmall};

static const PointSensor BallsideSensor = {5.0fx,3.5fx, SensorFlags::ActivateCoins|SensorFlags::ActivateCoinOutlines|
SensorFlags::ActivateFallingBlocks|SensorFlags::ActivateQuestionBlocks|SensorFlags::ActivateBricksSmall|
SensorFlags::ActivateBricksSuper};

static const PointSensor BalltopSensor = {0.0fx, 7.0fx,SensorFlags::ActivateCoins|SensorFlags::ActivateCoinOutlines|
SensorFlags::ActivateFallingBlocks|SensorFlags::ActivateQuestionBlocks|SensorFlags::ActivateBricksSmall};

void SuperBall::activeCallback(ActiveCollider& self, ActiveCollider& other){
	SuperBall* superBall = static_cast<SuperBall*>(self.owner);  
	StageEntity* actor = static_cast<StageEntity*>(other.owner);


	
	// boss collision
	if (actor->id == 113 || actor->id == 131 || actor->id == 132 || actor->id == 119 || 
		actor->id == 43 || actor->id == 128  || actor->id == 129 || actor->id == 135||actor->id == 0x1A4) { 
		actor->onFireballHit();
		superBall->activeCollider.unlink();
        superBall->Base::destroy();

        Fireballs::activeFireballs[0] -= 1;
		return;
	}

	if ((actor->id == 147) ||(actor->id == BanzaiBill::ObjectID)||(actor->id == BulletBill::ObjectID)|| (actor->id == 55)||(actor->id == 56)
        ||(actor->id == 37)||(actor->id == 38)||(actor->id == 387)) {
		StageEntity::damageEntityCallback(self, other);
        Fireballs::activeFireballs[0] -= 1;
        superBall->Base::destroy();
		return;
	}
    /*
    if (actor->id == 0x183){
        actor->defeat(actor->velocity.x*-1 / 3,0,0,0);
        Fireballs::activeFireballs[0] -= 1;
        hammer->Base::destroy();
		return;
    }
    */

	Fireball::playerActiveCallback(self,other);

	return;
}



const ActiveColliderInfo SuperBall::activeColliderInfo = {
    {0, 7.0fx, 4.0fx, 4.0fx},
    CollisionGroup::Fireball,
    CollisionFlag::Fireball,
    MAKE_GROUP_MASK(CollisionGroup::Player,CollisionGroup::Entity,
    				CollisionGroup::Hostile, CollisionGroup::Item, CollisionGroup::Bullet,
    				CollisionGroup::Debris),
    MAKE_FLAG_MASK_EX(CollisionFlag::None),
    0,
    SuperBall::activeCallback

};
bool SuperBall::loadResources(){
     FS::Cache::loadFile(2378 - 131, false);
     return 1;
}

 
s32 SuperBall::onCreate(){
    void* balls = FS::Cache::loadFile(2378 - 131, false);
    model.create(balls, 0, 0);

    fogFlag = false;
	alpha = 31;
	renderOffset = {0, 0};
	rotationTranslation = {0, 0};

    timer = 10;
    exsistenceTimer = 240;

    collisionMgr.init(this, &BallbottomSensor, &BalltopSensor, &BallsideSensor); 
    activeCollider.init(this, activeColliderInfo, 0);
    activeCollider.shape = ActiveCollider::Shape::Round;
    activeCollider.link();

    Player *player;
    player = Game::getPlayer(0);

    scale = Vec3(0x500, 0x500, 0x500);

    BallAccel.y = -0x2000;
    if(player->direction == 0){
        BallAccel.x = 0x2000;
        position.x += 0x8000;
    }
    else{
        BallAccel.x = -0x2000;
        position.x -= 0x8000;
    }
    Fireballs::activeFireballs[0] += 1;


    return 1;



}

bool SuperBall::updateMain(){
    position.x += BallAccel.x;
    position.y += BallAccel.y;

    if(timer != 0){
        timer--;
    }
    else{
    const CollisionMgrResult result = collisionMgr.collisionResult;

	// check wall collision
	if (bool(result & CollisionMgrResult::WallAny)){
        if(BallAccel.x == 0x2000) 
            BallAccel.x = -0x2000;
        else
            BallAccel.x = 0x2000;

        timer = 10;
	}
    if(bool(result & CollisionMgrResult::GroundPlatform)
        ||bool(result & CollisionMgrResult::GroundTile)
        ||bool(result & CollisionMgrResult::Ceiling)
        ||bool(result & CollisionMgrResult::GroundSlope)
        ||bool(result & CollisionMgrResult::CeilingSlope)){
        if(BallAccel.y == 0x2000) 
            BallAccel.y = -0x2000;
        else
            BallAccel.y = 0x2000;

        SND::playSFX(363, &this->position);
        timer = 10;
    }
    }

	applyMovement(); // applies velocity

	updateCollisionSensors(); // clamps position due to collision

    if(exsistenceTimer == 0){
        Particle::Handler::createParticle(114, this->position);
        Fireballs::activeFireballs[0] -= 1;
        this->Base::destroy();
    }
    else{
        exsistenceTimer--;
    }
    return true;      
}

s32 SuperBall::onDestroy(){
    Particle::Handler::createParticle(114, this->position);
	return 1;
}
