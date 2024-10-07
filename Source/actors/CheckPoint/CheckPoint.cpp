#include "CheckPoint.hpp"

const LineSensorH CheckPoint::bottomSensor = {
    -4.5fx, // Start position from the origin (left)
    4.5fx, // End position from the origin (right)
    0.0fx // Y Offset (negative = down)
};

const LineSensorV CheckPoint::sideSensor = {
    0.5fx, // Start position from the origin (top)
    50.5fx, // End position from the origin (bottom)
    5.5fx // X offset (negative = left)
};	

void CheckPoint::activeCallback(ActiveCollider& self, ActiveCollider& other){

    CheckPoint* checkpoint = static_cast<CheckPoint*>(self.owner);
    if(other.owner->actorType == ActorType::Player){
        Player* player = static_cast<Player*>(other.owner);
    }
    checkpoint->SetCheckPoint = true;
}

const ActiveColliderInfo CheckPoint::activeColliderInfo = {

    0.0fx, 25.0fx,
    8.0fx, 25.fx,

    CollisionGroup::Entity,
    CollisionFlag::None,
    MAKE_GROUP_MASK(CollisionGroup::Player, CollisionGroup::PlayerSpecial),
    MAKE_FLAG_MASK_EX(CollisionFlag::None),
    0,

    CheckPoint::activeCallback

};


 bool CheckPoint::loadResources(){
     FS::Cache::loadFile(2133 - 131, false);
     FS::Cache::loadFile(2134 - 131, false);
     FS::Cache::loadFile(2135 - 131, false);
	 return 1;
 }

s32 CheckPoint::onCreate(){
    void* CheckPointFlag = FS::Cache::loadFile(2133 - 131, false);
    void* CheckPointFlagMario = FS::Cache::loadFile(2134 - 131, false);
    void* CheckPointFlagLuigi = FS::Cache::loadFile(2135 - 131, false);

    if(Game::checkpointCollected == false)
	    model.create(CheckPointFlag, 0, 0);
    else{
        if(Game::luigiMode == true)
            model.create(CheckPointFlagLuigi, 0, 0);
        else
            model.create(CheckPointFlagMario, 0, 0);
    }

    SetCheckPoint = false;

	fogFlag = false;
	alpha = 0xff;
	renderOffset = {0, 0};
	rotationTranslation = {0, 0};

	collisionMgr.init(this, &bottomSensor, nullptr, &sideSensor); 
    activeCollider.init(this, activeColliderInfo, 0);
    activeCollider.link();

	return 1;

}

void CheckPoint::modelUpdated(){
    void* CheckPointFlagMario = FS::Cache::loadFile(2134 - 131, false);
    void* CheckPointFlagLuigi = FS::Cache::loadFile(2135 - 131, false);
    if(Game::luigiMode == true)
        model.create(CheckPointFlagLuigi, 0, 0);
    else
        model.create(CheckPointFlagMario, 0, 0);
    if(Game::checkpointCollected == false){
        Player *player;
        player = Game::getPlayer(0);
        if(player->currentPowerup == PowerupState::Small)
            player->runtimePowerup = PowerupState::Super;

        Game::setStageCheckpoint();
        SND::playSFX(314);
        Game::setStageCheckpoint();
        AddSecondsToTimer(100);
        Game::checkpointCollected = true;
    }
}

bool CheckPoint::updateMain(){

    if(SetCheckPoint)
        CheckPoint::modelUpdated();

	updateVerticalVelocity(); // updates gravity
	applyMovement(); // applies velocity

	updateCollisionSensors(); // clamps position due to collision

	return true;
		
}

s32 CheckPoint::onDestroy(){
		
}
	
	