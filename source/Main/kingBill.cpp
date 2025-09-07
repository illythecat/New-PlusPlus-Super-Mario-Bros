#include "kingBill.hpp"

void kingBill::activeCallback(ActiveCollider& self, ActiveCollider& other){

    kingBill* kb = static_cast<kingBill*>(self.owner);
	StageEntity* actor = static_cast<StageEntity*>(other.owner);
    
    if ((actor->id == 147) ||(actor->id == BanzaiBill::ObjectID)||(actor->id == BulletBill::ObjectID)|| (actor->id == 55)||(actor->id == 56)
        ||(actor->id == 37)||(actor->id == 38)||(actor->id == 387)) {
		StageEntity::damageEntityCallback(self, other);
		return;
	}
    Fireball::playerActiveCallback(self,other);

    if(other.owner->actorType == ActorType::Player){
        Player* player = static_cast<Player*>(other.owner);
        player->dealDamage(*kb, 0, 4.0fx, PlayerDamageType::Lava);

    }


}



const LineSensorV kingBill::sideSensor = {
    80fx, // Start position from the origin (top)
    -80fx, // End position from the origin (bottom)
    80fx, // X offset (negative = left)
    SensorFlags::ActivateBricksSuper|SensorFlags::IgnoreColliders|SensorFlags::DestroyPipesBottom|SensorFlags::DestroyPipesTop|SensorFlags::ActivateQuestionBlocks
};	
const LineSensorV kingBill::sideSensor3 = {
    60fx, // Start position from the origin (top)
    -60fx, // End position from the origin (bottom)
    100fx, // X offset (negative = left)
    SensorFlags::ActivateBricksSuper|SensorFlags::IgnoreColliders|SensorFlags::DestroyPipesBottom|SensorFlags::DestroyPipesTop|SensorFlags::ActivateQuestionBlocks
};
const LineSensorV kingBill::sideSensor2 = {
    110fx, // Start position from the origin (top)
    -110fx, // End position from the origin (bottom)
    60fx, // X offset (negative = left)
    SensorFlags::ActivateBricksSuper|SensorFlags::IgnoreColliders|SensorFlags::DestroyPipesBottom|SensorFlags::DestroyPipesTop|SensorFlags::ActivateQuestionBlocks
};	

const ActiveColliderInfo kingBill::activeColliderInfo = {

    0.0fx, 0.0fx,
    100.0fx, 100.0fx,

    CollisionGroup::Entity,
    CollisionFlag::None,
        MAKE_GROUP_MASK(CollisionGroup::Entity,
    				CollisionGroup::Hostile, CollisionGroup::Item, CollisionGroup::Bullet,
    				CollisionGroup::Debris, CollisionGroup::Player, CollisionGroup::PlayerSpecial),
    MAKE_FLAG_MASK_EX(CollisionFlag::None),
    0,

    kingBill::activeCallback

};

bool kingBill::loadResources(){
     FS::Cache::loadFile(2443 - 131, false);
     FS::Cache::loadFile(2444 - 131, false);

     return 1;
}

void kingBill::damagePlayer(ActiveCollider& self, Player& player){
    player.dealDamage(*this, 0, 4.0fx, PlayerDamageType::Hit);

}


void kingBill::onFireballHit(){

}

void kingBill::defeat(fx32 velX, fx32 velY, fx32 accelY, u8 unk){
    Particle::Handler::createParticle(114, position);
    StageEntity::defeat(velX, velY, accelY, unk);
}

void kingBill::defeatMega(fx32 velX, fx32 velY, fx32 accelY){
    Particle::Handler::createParticle(114, position);
    StageEntity::defeatMega(velX, velY, accelY);
}

s32 kingBill::onCreate(){
    

	void* modelFile = FS::Cache::getFile(2443 - 131);
	void* animFile = FS::Cache::getFile(2444 - 131);

	model.create(modelFile, animFile, 0, 0, 0);
    model.init(0, FrameCtrl::Looping, 0.5fx, 0);
    timeCrap = 0;
    codeAnimTimer = 0;
    fogFlag = false;
	alpha = 31;
	renderOffset = {0, 0};
	rotationTranslation = {0, 0};
    scale = {4.0fx, 4.0fx, 4.0fx};
    codeAnimTimer = 0;
    if(settings & 0x1){
        rotation.y = -4.0fx;
        position.x += 0x30000;
    }
    else{
        rotation.y = 4.0fx;
    }
    activeCollider.init(this, activeColliderInfo, 0);
    activeCollider.shape = ActiveCollider::Shape::Round;
    activeCollider.link();
    SND::playSFX(104, nullptr);
    estimatedPos = position.x;

    return 1;

}

void kingBill::switchState(StateFunction function) {
    if (updateFunction == function && !updateFunction) {
    }

    if (updateFunction) {
        updateStep = Func::Exit;
        (this->*updateFunction)();
    }

    updateFunction = function;
    updateStep = Func::Init;
    (this->*updateFunction)();

}

void kingBill::updateState(){
    (this->*updateFunction)();

}



bool kingBill::updateMain(){
    Player *player;
    player = Game::getPlayer(0);
    codeAnimTimer++;
    if(codeAnimTimer == 2){
        collisionMgr.init(this, nullptr, nullptr, &sideSensor2); 
        codeAnimTimer = 0;
    }
    else if(codeAnimTimer == 1){
        collisionMgr.init(this, nullptr, nullptr, &sideSensor3); 

    }
    else{
         collisionMgr.init(this, nullptr, nullptr, &sideSensor); 
    }


    
    updateCollisionSensors(); // clamps position due to collision
    

    updateState();
    //updateVerticalVelocity(); // updates gravity
	applyMovement(); // applies velocity
    updateAnimation();

    if(settings &0x1){
        estimatedPos -= 3.0fx;
    }
    else{
        estimatedPos += 3.0fx;
    }
    position.x = estimatedPos;
    
    timeCrap    += 180;
    rotation.z += 0.05*Math::cos(timeCrap);
    return true;     

}

bool kingBill::updateDefeated(){
    alpha = 0;
    updateVerticalVelocity(); // updates gravity
	applyMovement(); // applies velocity
    return true; 
}

bool kingBill::updateDefeatedMega(){
    alpha = 0;
    updateVerticalVelocity(); // updates gravity
	applyMovement(); // applies velocity
    return true; 
}

s32 kingBill::onDestroy(){
		return 1;
}







    