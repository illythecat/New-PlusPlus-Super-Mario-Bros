#include "Hammer.hpp"


static const PointSensor BallbottomSensor = {5.0fx, -5.0fx, SensorFlags::ActivateCoins|SensorFlags::ActivateCoinOutlines|
SensorFlags::ActivateFallingBlocks|SensorFlags::ActivateQuestionBlocks|SensorFlags::ActivateBricksSmall};

static const PointSensor BallsideSensor = {5.0fx,3.5fx, SensorFlags::ActivateCoins|SensorFlags::ActivateCoinOutlines|
SensorFlags::ActivateFallingBlocks|SensorFlags::ActivateQuestionBlocks|SensorFlags::ActivateBricksSmall|
SensorFlags::ActivateBricksSuper};

static const PointSensor BalltopSensor = {5.0fx, 5.0fx, SensorFlags::ActivateCoins|SensorFlags::ActivateCoinOutlines|
SensorFlags::ActivateFallingBlocks|SensorFlags::ActivateQuestionBlocks|SensorFlags::ActivateBricksSmall};

void Hammer::activeCallback(ActiveCollider& self, ActiveCollider& other){
	Hammer* hammer = static_cast<Hammer*>(self.owner);  
	StageEntity* actor = static_cast<StageEntity*>(other.owner);

	
	// boss collision
	if (actor->id == 113 || actor->id == 131 || actor->id == 132 || actor->id == 119 || 
		actor->id == 43 || actor->id == 128  || actor->id == 129 || actor->id == 135||actor->id == 37||actor->id == 38||actor->id == 0x1A4) { 
		actor->onFireballHit();
		hammer->activeCollider.unlink();
        hammer->Base::destroy();

        Fireballs::activeFireballs[0] -= 1;
		return;
	}

	if ((actor->id == 147) ||(actor->id == BanzaiBill::ObjectID)||(actor->id == BulletBill::ObjectID)|| (actor->id == 55)||(actor->id == 56)
        ||(actor->id == 37)||(actor->id == 38)||(actor->id == 387)) {
		StageEntity::damageEntityCallback(self, other);
        Fireballs::activeFireballs[0] -= 1;
        hammer->Base::destroy();
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

const ActiveColliderInfo Hammer::activeColliderInfo = {
    {0, 0.0fx, 8.0fx, 8.0fx},
    CollisionGroup::Fireball ,
    CollisionFlag::Fireball,
    MAKE_GROUP_MASK(CollisionGroup::Entity,
    				CollisionGroup::Hostile, CollisionGroup::Item, CollisionGroup::Bullet,
    				CollisionGroup::Debris),
    MAKE_FLAG_MASK_EX(CollisionFlag::None),
    0,
    Hammer::activeCallback

};
bool Hammer::loadResources(){
     FS::Cache::loadFile(1450 - 131, false);
     return 1;
}

 
s32 Hammer::onCreate(){
    void* balls = FS::Cache::loadFile(1450 - 131, false);
    model.create(balls, 0, 0);

    fogFlag = false;
	alpha = 31;
	renderOffset = {0, 0};
	rotationTranslation = {0, 0};

    timer = 10;
    exsistenceTimer = 480;

    collisionMgr.init(this, &BallbottomSensor, &BalltopSensor, &BallsideSensor); 
    activeCollider.init(this, activeColliderInfo, 0);
    activeCollider.link();

    Player *player;
    player = Game::getPlayer(0);
    exsistenceTimer2 = 0;
    //scale = Vec3(0x500, 0x500, 0x500);

    BallAccel.y = player->velocity.y;
    if(player->direction == 0){
        BallAccel.x = 0xc00 + player->velocity.x;
        position.x += 0x4000;
    }
    else{
        BallAccel.x = -0xc00 + player->velocity.x;
        position.x -= 0x4000;
    }

    Fireballs::activeFireballs[0] += 1;
    
    return 1;

}

bool Hammer::updateMain(){
    Player *player;
    player = Game::getPlayer(0);

    position.x += BallAccel.x;

    position.y -= 6* Math::sin((exsistenceTimer-240)*180);
    position.y -= exsistenceTimer2*0.1fx;
    position.y += BallAccel.y*0.1 + 0x100;
    

    if(player->direction == 0){
        rotation.z -= exsistenceTimer2*0.02fx;
    }
    else{
        rotation.z += exsistenceTimer2*0.02fx;
    }

	updateCollisionSensors(); // clamps position due to collision
    updateLiquids(-0.1875fx);


    const CollisionMgrResult result = collisionMgr.collisionResult;


    if(
        bool(result & CollisionMgrResult::GroundTile)
        ||bool(result & CollisionMgrResult::Ceiling)
        ||bool(result & CollisionMgrResult::GroundSlope)
        ||bool(result & CollisionMgrResult::CeilingSlope)){
            exsistenceTimer = 0;
        }

    if(position.y < player->position.y - 0x10000*20){
        exsistenceTimer = 0;  
    }


    if(exsistenceTimer == 0){
        Particle::Handler::createParticle(114, this->position);
        Fireballs::activeFireballs[0] -= 1;
        defeat(velocity.x*-1 / 3,0,0,0);
        //this->Base::destroy();
    }
    else{
        exsistenceTimer--;
        exsistenceTimer2++;
    }
    return true;      
}

bool Hammer::updateDefeated(){
    updateVerticalVelocity();
	applyMovement();
	updateLiquids(-0.1875fx);
	destroyInactive(0);
    this->Base::destroy();
    return 1;
}


s32 Hammer::onDestroy(){
		return 1;
}
