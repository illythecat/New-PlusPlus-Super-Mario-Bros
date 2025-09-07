#include "Goonie.hpp"

void Goonie::activeCallback(ActiveCollider& self, ActiveCollider& other){

    Goonie* goonie = static_cast<Goonie*>(self.owner);
    if(other.owner->actorType == ActorType::Player){
        Player* player = static_cast<Player*>(other.owner);
    }
    
    
}

const LineSensorH Goonie::bottomSensor = {
    -9.5fx, // Start position from the origin (left)
    9.5fx, // End position from the origin (right)
    -12.0fx // Y Offset (negative = down)
};

const LineSensorH Goonie::topSensor = {
    -9.5fx, // Start position from the origin (left)
    9.5fx, // End position from the origin (right)
    12.0fx // Y Offset (negative = down)
};


const LineSensorV Goonie::sideSensor = {
    0.5fx, // Start position from the origin (top)
    4.5fx, // End position from the origin (bottom)
    5.5fx // X offset (negative = left)
};	

const ActiveColliderInfo Goonie::activeColliderInfo = {

    0.0fx, 0.0fx,
    9.0fx, 9.0fx,

    CollisionGroup::Entity,
    CollisionFlag::None,
    MAKE_GROUP_MASK(CollisionGroup::Player, CollisionGroup::PlayerSpecial,CollisionGroup::Entity,  CollisionGroup::Fireball),
    MAKE_FLAG_MASK_EX(CollisionFlag::None),
    0,

    Goonie::damagePlayerCallback

};

bool Goonie::loadResources(){
     FS::Cache::loadFile(2394 - 131, false);
     FS::Cache::loadFile(2395 - 131, false);
     FS::Cache::loadFile(2396 - 131, false);
     FS::Cache::loadFile(2397 - 131, false);
     FS::Cache::loadFile(2398 - 131, false);



     return 1;
}

void Goonie::damagePlayer(ActiveCollider& self, Player& player){
    auto stomped = updatePlayerStomp(self, 9.0fx/32.0fx, 1, false);
    if(stomped == PlayerStompType::MiniStomp)
        return;

    if(stomped == PlayerStompType::Stomp){
        setStompCollision(player);
        return;
    }
    player.dealDamage(*this, 0, 4.0fx, PlayerDamageType::Hit);

}

void Goonie::onStomped(){
    activeCollider.unlink();
    defeat(0, 0, -9.0fx/32.0fx, 0);
}

void Goonie::onFireballHit(){
    activeCollider.unlink();
    Actor::spawnActor(66, 0x4,  &this->position);
    defeat(0, 0, -9.0fx/32.0fx, 0);
}

void Goonie::defeat(fx32 velX, fx32 velY, fx32 accelY, u8 unk){
    Particle::Handler::createParticle(114, position);
    StageEntity::defeat(velX, velY, accelY, unk);
}

void Goonie::defeatMega(fx32 velX, fx32 velY, fx32 accelY){
    Particle::Handler::createParticle(114, position);
    StageEntity::defeatMega(velX, velY, accelY);
}

s32 Goonie::onCreate(){
    
    if(settings & 0x1){
        void* Shy_modelFile = FS::Cache::getFile(ShyGuyID);
        void* Shy_modelFileAnm = FS::Cache::getFile(ShyGuyAnimId);
    	void* modelFile = FS::Cache::getFile(GoonieHoldId);
    	void* modelFileAnm = FS::Cache::getFile(GoonieAnimId);

	    modelShyGuy.create(Shy_modelFile, Shy_modelFileAnm, 0, 0, 0);
        modelShyGuy.init(0, FrameCtrl::Looping, 0.4fx, 0);
    	modelGoonie.create(modelFile, modelFileAnm, 0, 0, 0);
        modelGoonie.init(0, FrameCtrl::Looping, 1.5fx, 0);
    }
    else{
        void* modelFileAnm = FS::Cache::getFile(GoonieAnimId);
        void* modelFile = FS::Cache::getFile(GoonieId);
    	modelGoonie.create(modelFile, modelFileAnm, 0, 0, 0);
        modelGoonie.init(0, FrameCtrl::Looping, 1.5fx, 0);

    }

    switchState(&Goonie::swoopState);

    rotation.y = -5.0fx;

    scale = Vec3(13.3fx, 13.3fx, 13.3fx);
    shyGuyDroped = false;



	collisionMgr.init(this, &bottomSensor, &topSensor, &sideSensor); 
    activeCollider.init(this, activeColliderInfo, 0);
    activeCollider.link();

    return 1;

}

s32 Goonie::onRender() {
    MTX::identity(modelGoonie.matrix);
    MTX::translate(modelGoonie.matrix, position);
    MTX::rotate(modelGoonie.matrix, rotation);
    Game::modelMatrix = modelGoonie.matrix;
    modelGoonie.render(&scale);

    if((settings & 0x1) && (!shyGuyDroped)){
    MTX::identity(modelShyGuy.matrix);
    MTX::translate(modelShyGuy.matrix, Vec3(position.x + 3.0fx, position.y - 20.0fx, position.z - 20.0fx));
    MTX::rotate(modelShyGuy.matrix, VecFx16(rotation.x + 1.0fx, rotation.y + 3.0fx, 0));
    Game::modelMatrix = modelShyGuy.matrix;
    Vec3 newScale = Vec3(1.0fx, 1.0fx, 1.0fx);
    modelShyGuy.render(&newScale);
    }
    return true;

}


void Goonie::switchState(StateFunction function) {
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

void Goonie::updateState(){
    (this->*updateFunction)();

}



void Goonie::swoopState(){
     Player *player = Game::getPlayer(0); 
    if (updateStep == Func::Init) {
        updateStep = Func::Step(0);
        return;
    }

    if (updateStep == Func::Exit) {
        return;
    }
    position.x -= 1.4fx;
    if((player->position.x > position.x -180.0fx)&&(player->position.x < position.x -140.0fx)){
        if(position.y > player->position.y){
            position.y -= 1.0fx ;
        }
        if(position.y < player->position.y){
            position.y += 1.0fx ;
        }
    }

    if((player->position.x > position.x -140.0fx)&&(player->position.x < position.x -100.0fx)){
        if(position.y > player->position.y){
            position.y -= 0.75fx ;
        }
        if(position.y < player->position.y){
            position.y += 0.75fx ;
        }
    }
    if((player->position.x > position.x -100.0fx)&&(player->position.x < position.x -60.0fx)){
        if(position.y > player->position.y){
            position.y -= 0.5fx ;
        }
        if(position.y < player->position.y){
            position.y += 0.5fx ;
        }
    }
}

bool Goonie::updateMain(){
    Player *player;
    player = Game::getPlayer(0);
    Goonie::updateState();
    modelGoonie.update();
    if(settings & 0x1)
        modelShyGuy.update();

    if((player->position.x > position.x - 10.0fx) && (!shyGuyDroped) && (settings & 0x1)) {
        Vec3 firePos =  Vec3(position.x + 3.0fx, position.y - 20.0fx, position.z - 20.0fx);
        void* modelFileAnm = FS::Cache::getFile(GoonieAnimId);
        void* modelFile = FS::Cache::getFile(GoonieId);
    	modelGoonie.create(modelFile, modelFileAnm, 0, 0, 0);
        modelGoonie.init(0, FrameCtrl::Looping, 1.5fx, 0);
        Actor::spawnActor(127, 0x30000, &firePos);
        shyGuyDroped = true;
    }

    
    //updateVerticalVelocity(); // updates gravity
	applyMovement(); // applies velocity
    
    return true;      
}

bool Goonie::updateDefeated(){
    alpha = 0;
    updateVerticalVelocity(); // updates gravity
	applyMovement(); // applies velocity
    return true; 
}

bool Goonie::updateDefeatedMega(){
    updateVerticalVelocity(); // updates gravity
	applyMovement(); // applies velocity
    return true; 
}

s32 Goonie::onDestroy(){
		return 1;
}







    