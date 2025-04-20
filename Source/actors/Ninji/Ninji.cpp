#include "Ninji.hpp"

void Ninji::activeCallback(ActiveCollider& self, ActiveCollider& other){

    Ninji* ninji = static_cast<Ninji*>(self.owner);
    if(other.owner->actorType == ActorType::Player){
        Player* player = static_cast<Player*>(other.owner);
    }
    
    
}

const LineSensorH Ninji::bottomSensor = {
    -9.5fx, // Start position from the origin (left)
    9.5fx, // End position from the origin (right)
    -12.0fx // Y Offset (negative = down)
};

const LineSensorV Ninji::sideSensor = {
    0.5fx, // Start position from the origin (top)
    4.5fx, // End position from the origin (bottom)
    5.5fx // X offset (negative = left)
};	

const ActiveColliderInfo Ninji::activeColliderInfo = {

    0.0fx, 0.0fx,
    9.0fx, 9.0fx,

    CollisionGroup::Entity,
    CollisionFlag::None,
    MAKE_GROUP_MASK(CollisionGroup::Player, CollisionGroup::PlayerSpecial,CollisionGroup::Entity,  CollisionGroup::Fireball),
    MAKE_FLAG_MASK_EX(CollisionFlag::None),
    0,

    Ninji::damagePlayerCallback

};

bool Ninji::loadResources(){
     FS::Cache::loadFile(2098 - 131, false);
     return 1;
}

void Ninji::damagePlayer(ActiveCollider& self, Player& player){
    auto stomped = updatePlayerStomp(self, 9.0fx/32.0fx, 1, false);
    if(stomped == PlayerStompType::MiniStomp)
        return;

    if(stomped == PlayerStompType::Stomp){
        setStompCollision(player);
        return;
    }
    player.dealDamage(*this, 0, 4.0fx, PlayerDamageType::Hit);

}

void Ninji::onStomped(){
    activeCollider.unlink();
    defeat(0, 0, -9.0fx/32.0fx, 0);
}

void Ninji::onFireballHit(){
    activeCollider.unlink();
    Actor::spawnActor(66, 0x4,  &this->position);
    defeat(0, 0, -9.0fx/32.0fx, 0);
}

void Ninji::defeat(fx32 velX, fx32 velY, fx32 accelY, u8 unk){
    Particle::Handler::createParticle(114, position);
    StageEntity::defeat(velX, velY, accelY, unk);
}

void Ninji::defeatMega(fx32 velX, fx32 velY, fx32 accelY){
    Particle::Handler::createParticle(114, position);
    StageEntity::defeatMega(velX, velY, accelY);
}

s32 Ninji::onCreate(){
    

	void* modelFile = FS::Cache::getFile(2098 - 131);
	model.create(modelFile, 0, 0);

    codeAnimTimer = 0;
    fogFlag = false;
	alpha = 31;
	renderOffset = {0, 0};
	rotationTranslation = {0, 0};
    this->scale = {0x2a00, 0x2a00, 0x2a00};
    switchState(&Ninji::codeAnim);
    Ninji::jumpCoolDown = 80;
    Ninji::JumpTimer = 0;

    NNSG3dResMatData* mat = model.getMaterialData(0);
    u32 texParams;
    u32 palParams;

    G3D::getTextureParams(model.texture, currentTex, texParams);
    G3D::getPaletteParams(model.texture, currentTex, palParams);
    mat->texImageParam = texParams;
    mat->texPlttBase = palParams;
    currentTex = 0;

    if(this->settings>>0 == 1)
        this->position.x += 0x8000;

	collisionMgr.init(this, &bottomSensor, nullptr, &sideSensor); 
    activeCollider.init(this, activeColliderInfo, 0);
    activeCollider.link();

    return 1;

}

void Ninji::switchState(StateFunction function) {
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

void Ninji::updateState(){
    (this->*updateFunction)();

}

void Ninji::codeAnim(){
    if (updateStep == Func::Init) {
        updateStep = Func::Step(0);
        // Initializing state
        return;
    }
    if (updateStep == Func::Exit) {
        // Exiting state
        codeAnimTimer = 0;
        return;

    }


    codeAnimTimer++;
    if(codeAnimTimer < 30){
        this->scale.x -= 32;
        this->position.x -= 16;
        this->scale.y -= 8;

    }
    if(codeAnimTimer > 30){
        this->scale.x += 32;
        this->position.x += 16;
        this->scale.y += 8;
    }
    if(codeAnimTimer == 60){
        switchState(&Ninji::updateJumpState);
    }
}

void Ninji::updateJumpState(){
    NNSG3dResMatData* mat = model.getMaterialData(0);
    u32 texParams;
    u32 palParams;

    G3D::getTextureParams(model.texture, currentTex, texParams);
    G3D::getPaletteParams(model.texture, currentTex, palParams);
    mat->texImageParam = texParams;
    mat->texPlttBase = palParams;
    
    if (updateStep == Func::Init) {
        currentTex = 0;
        updateStep = Func::Step(0);
        // Initializing state
        return;
    }
    if (updateStep == Func::Exit) {
        // Exiting state
        jumpCoolDown = 80;
        JumpTimer = 0;
        return;

    }

        if(JumpTimer < 140)
            JumpTimer++;
        if(JumpTimer == 5){

                SND::playSFX(370, 0);
            
        }
        if(JumpTimer == 10){
            currentTex = 1;
            this->velocity.y = 0x5000;
        }
        if(JumpTimer == 15){
            currentTex = 2;
        }
        if(JumpTimer == 20){
            currentTex = 3;
        }
        if(JumpTimer == 50){
            currentTex = 2;
        }
        if(Ninji::JumpTimer == 55){
            currentTex = 1;
        }
        if(Ninji::JumpTimer == 60){
            currentTex = 0;
            switchState(&Ninji::codeAnim);
        }
    
}

bool Ninji::updateMain(){
    Player *player;
    player = Game::getPlayer(0);
    if((this->position.x < player->position.x - 0x200000)||
        (this->position.x > player->position.x + 0x200000)||
        (this->position.y < player->position.y - 0x200000)||
        (this->position.y > player->position.y + 0x200000)){

    }
    else{
        Ninji::updateState();

    }
    updateVerticalVelocity(); // updates gravity
	applyMovement(); // applies velocity
	updateCollisionSensors(); // clamps position due to collision
    
    return true;      
}

bool Ninji::updateDefeated(){
    alpha = 0;
    updateVerticalVelocity(); // updates gravity
	applyMovement(); // applies velocity
    return true; 
}

bool Ninji::updateDefeatedMega(){
    alpha = 0;
    updateVerticalVelocity(); // updates gravity
	applyMovement(); // applies velocity
    return true; 
}

s32 Ninji::onDestroy(){
		return 1;
}







    