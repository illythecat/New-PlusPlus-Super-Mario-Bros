#include "TalkingFlower.hpp"
#include "core.hpp"

void TalkingFlower::activeCallback(ActiveCollider& self, ActiveCollider& other){

    TalkingFlower* talkingFlower = static_cast<TalkingFlower*>(self.owner);
    if(other.owner->actorType == ActorType::Player){
        Player* player = static_cast<Player*>(other.owner);
        if(!talkingFlower->spoken){
            talkingFlower->model.init(1, FrameCtrl::Looping, 1.0fx, 0);
            switch(talkingFlower->settings){
                case 0:{
                    NWAV::play(2415);
                    talkingFlower->speachTimer = 120;
                    break;
                }
                case 1:{
                    NWAV::play(2419);
                    talkingFlower->speachTimer = 90;
                    break;
                }
                case 2:{
                    NWAV::play(2420);
                    talkingFlower->speachTimer = 90;
                    break;
                }
                case 3:{
                    NWAV::play(2421);
                    talkingFlower->speachTimer = 90;
                    break;
                }
                case 4:{
                    NWAV::play(2422);
                    talkingFlower->speachTimer = 180;
                    break;
                }
                case 5:{
                    NWAV::play(2423);
                    talkingFlower->speachTimer = 180;
                    break;
                }
            }


        }
        talkingFlower->spoken = true;
    }
    
    
}

const LineSensorH TalkingFlower::bottomSensor = {
    -9.5fx, // Start position from the origin (left)
    9.5fx, // End position from the origin (right)
    -10.0fx // Y Offset (negative = down)
};

const LineSensorH TalkingFlower::topSensor = {
    -9.5fx, // Start position from the origin (left)
    9.5fx, // End position from the origin (right)
    12.0fx // Y Offset (negative = down)
};


const LineSensorV TalkingFlower::sideSensor = {
    0.5fx, // Start position from the origin (top)
    4.5fx, // End position from the origin (bottom)
    5.5fx // X offset (negative = left)
};	

const ActiveColliderInfo TalkingFlower::activeColliderInfo = {

    0.0fx, 0.0fx,
    9.0fx, 9.0fx,

    CollisionGroup::Entity,
    CollisionFlag::None,
    MAKE_GROUP_MASK(CollisionGroup::Player, CollisionGroup::PlayerSpecial,CollisionGroup::Entity,  CollisionGroup::Fireball),
    MAKE_FLAG_MASK_EX(CollisionFlag::None),
    0,

    TalkingFlower::activeCallback

};

bool TalkingFlower::loadResources(){
     FS::Cache::loadFile(2417 - 131, false);
     FS::Cache::loadFile(2418 - 131, false);

     return 1;
}

s32 TalkingFlower::onCreate(){
    

	void* modelFile = FS::Cache::getFile(2418 - 131);
	void* anmFile = FS::Cache::getFile(2417 - 131);

	model.create(modelFile, anmFile, 0, 0, 0);
    model.init(0, FrameCtrl::Looping, 1.0fx, 0);

    fogFlag = false;
	alpha = 31;
	renderOffset = {0, 0};
	rotationTranslation = {0, 0};
    this->scale = {0x1a00, 0x1a00, 0x1a00};
    rotation.y = -4.0fx;




	collisionMgr.init(this, &bottomSensor, &topSensor, &sideSensor); 
    activeCollider.init(this, activeColliderInfo, 0);
    activeCollider.link();
    spoken = false;
    return 1;

}

void TalkingFlower::switchState(StateFunction function) {
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

void TalkingFlower::updateState(){
    (this->*updateFunction)();

}

bool TalkingFlower::updateMain(){
    Player *player;
    player = Game::getPlayer(0);
    updateAnimation();
    updateVerticalVelocity(); // updates gravity
	applyMovement(); // applies velocity
	updateCollisionSensors(); // clamps position due to collision
    if(speachTimer != 0){
        speachTimer--;
    }
    if(speachTimer == 1){
        model.init(0, FrameCtrl::Looping, 1.0fx, 0);    
        NWAV::stop(2);
    }

    
    return true;      
}

bool TalkingFlower::updateDefeated(){
    alpha = 0;
    updateVerticalVelocity(); // updates gravity
	applyMovement(); // applies velocity
    return true; 
}

bool TalkingFlower::updateDefeatedMega(){
    alpha = 0;
    updateVerticalVelocity(); // updates gravity
	applyMovement(); // applies velocity
    return true; 
}

s32 TalkingFlower::onDestroy(){
		return 1;
}







    