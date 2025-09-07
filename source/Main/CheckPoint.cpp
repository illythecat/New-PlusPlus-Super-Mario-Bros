#include "CheckPoint.hpp"

static u32 respawnEntrance;

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
        if(bool(!checkpoint->SetCheckPoint)){
            checkpoint->switchState(&CheckPoint::collectedState);
            checkpoint->SetCheckPoint = true;
        }
        

    }
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
     FS::Cache::loadFile(2434 - 131, false);
     FS::Cache::loadFile(2435 - 131, false);
     FS::Cache::loadFile(2436 - 131, false);
	 return 1;
 }

s32 CheckPoint::onCreate(){
    void* CheckPointFlagAnm = FS::Cache::loadFile(2434 - 131, false);
    void* CheckPointFlagBowser = FS::Cache::loadFile(2435 - 131, false);
    void* CheckPointFlagMario = FS::Cache::loadFile(2436 - 131, false);

    if(!SetCheckPoint)
	    model.create(CheckPointFlagBowser, CheckPointFlagAnm, 0, 0, 0);
    else{
        model.create(CheckPointFlagMario, CheckPointFlagAnm, 0, 0, 0);
    }

    SetCheckPoint = false;
    timerCrap = 0;
	fogFlag = false;
	alpha = 0xff;
	renderOffset = {0, 0};
	rotationTranslation = {0, 0};
    scale = Vec3(100.0fx, 100.0fx, 100.0fx);
   // check point tagged model.init(1, FrameCtrl::Looping, 1.0fx, 0);
	collisionMgr.init(this, &bottomSensor, nullptr, &sideSensor); 
    activeCollider.init(this, activeColliderInfo, 0);
    activeCollider.link();
    rotation.y += 8.0fx;

    switchState(&CheckPoint::IdelState);


	return 1;

}




void CheckPoint::switchState(StateFunction function) {
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

void CheckPoint::updateState(){
    (this->*updateFunction)();

}

void CheckPoint::collectedState(){
    if (updateStep == Func::Init) {
        
        Player *player;
        player = Game::getPlayer(0);
        timerCrap = 0;

        if(player->currentPowerup == PowerupState::Small)
            player->runtimePowerup = PowerupState::Super;

        void* CheckPointFlagAnm = FS::Cache::getFile(2434 - 131);
        void* CheckPointFlagMario = FS::Cache::getFile(2436 - 131);
        Game::setStageCheckpoint();
        SND::playSFX(314);
        Game::setStageCheckpoint();
        AddSecondsToTimer(100);
        model.create(CheckPointFlagMario, CheckPointFlagAnm, 0, 0, 0);
        model.init(1, FrameCtrl::Standard , 1.0fx, 0);
        updateStep = Func::Step(0);

        // Initializing state
        return;
    }
    if (updateStep == Func::Exit) {
        // Exiting state
        return;

    }
    timerCrap++;
    if(timerCrap == 40){
        switchState(&CheckPoint::IdelState);
    }

}

void CheckPoint::IdelState(){
    if (updateStep == Func::Init) {
        //model.init(0, FrameCtrl::Standard , 1.0fx, 60);
        model.init(0, FrameCtrl::Looping , 1.0fx, 0);

        updateStep = Func::Step(0);
        // Initializing state
        return;
    }
    if (updateStep == Func::Exit) {
        // Exiting state
        return;

    }
}



bool CheckPoint::updateMain(){
    updateState();
	updateVerticalVelocity(); // updates gravity
	applyMovement(); // applies velocity
    updateAnimation();

	updateCollisionSensors(); // clamps position due to collision


	return true;
		
}

s32 CheckPoint::onDestroy(){
	return true;
}
	
