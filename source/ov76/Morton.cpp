#include "Morton.hpp"
#include "main/SFX.hpp"
#include "main/undocumented.hpp"

void Morton::activeCallback(ActiveCollider& self, ActiveCollider& other){

    Morton* morton = static_cast<Morton*>(self.owner);
    if(other.owner->actorType == ActorType::Player){
        Player* player = static_cast<Player*>(other.owner);
    }
    
    
}

const LineSensorH Morton::bottomSensor = {
    -10.5fx, // Start position from the origin (left)
    10.5fx, // End position from the origin (right)
    -0.0fx, // Y Offset (negative = down)
};

const LineSensorH Morton::topSensor = {
    -14.5fx, // Start position from the origin (left)
    14.5fx, // End position from the origin (right)
    1.0fx // Y Offset (negative = down)
};


const LineSensorV Morton::sideSensor = {
    0.5fx, // Start position from the origin (top)
    0.0fx, // End position from the origin (bottom)
    16.5fx // X offset (negative = left)
};	

const ActiveColliderInfo Morton::activeColliderInfo = {

    0.0fx, 15.0fx,
    15.0fx, 15.0fx,

    CollisionGroup::Entity,
    CollisionFlag::None,
    MAKE_GROUP_MASK(CollisionGroup::Player, CollisionGroup::PlayerSpecial,CollisionGroup::Entity,  CollisionGroup::Fireball),
    MAKE_FLAG_MASK_EX(CollisionFlag::None),
    0,

    Morton::damagePlayerCallback

};
const ActiveColliderInfo Morton::activeColliderInfo2 = {

    0.0fx, -15.0fx,
    15.0fx, 15.0fx,

    CollisionGroup::Entity,
    CollisionFlag::None,
    MAKE_GROUP_MASK(CollisionGroup::Player, CollisionGroup::PlayerSpecial,CollisionGroup::Entity,  CollisionGroup::Fireball),
    MAKE_FLAG_MASK_EX(CollisionFlag::None),
    0,

    Morton::damagePlayerCallback

};

bool Morton::loadResources(){
     FS::Cache::loadFile(2178 - 131, false);
     FS::Cache::loadFile(2203 - 131, false);
     FS::Cache::loadFile(2170 - 131, false);

     return 1;
}

void Morton::damagePlayer(ActiveCollider& self, Player& player){
    if(!phase2){
        auto stomped = updatePlayerStomp(self, 9.0fx/32.0fx, 1, false);

        if(stomped == PlayerStompType::MiniStomp)
            return;

        if(stomped == PlayerStompType::Stomp){
            setStompCollision(player);
            return;
        }
    }
    player.dealDamage(*this, 0, 4.0fx, PlayerDamageType::Hit);

}

void Morton::onStomped(){
    if(!phase2){
        switchState(&Morton::recoveryState);
        health -= 130;

        recoverAttacksCompleated = 0;
        velocity.x = 0;
        velocity.y = 0;
        currentTex = 2;
        colourchange = 4;
        activeCollider.unlink();


    }

    //activeCollider.unlink();
    //defeat(0, 0, -9.0fx/32.0fx, 0);
}

void Morton::onFireballHit(){
    currentTex = 2;
    colourchange = 10;
    health -= 10;
    SND::playSFX(122, &position);

    //Actor::spawnActor(66, 0x4,  &this->position);
    //defeat(0, 0, -9.0fx/32.0fx, 0);
}

void Morton::defeat(fx32 velX, fx32 velY, fx32 accelY, u8 unk){
    Particle::Handler::createParticle(114, position);
    StageEntity::defeat(velX, velY, accelY, unk);
}

void Morton::defeatMega(fx32 velX, fx32 velY, fx32 accelY){
    Particle::Handler::createParticle(114, position);
    StageEntity::defeatMega(velX, velY, accelY);
}





s32 Morton::onCreate(){
    

	void* modelFile = FS::Cache::getFile(MortonID);
    void* modelAnm = FS::Cache::getFile(MortonAnimID);
    void* shellmetFile = FS::Cache::getFile(spinyModel);


	modelMorton.create(modelFile, modelAnm, 0, 0, 0);
	shellmet.create(shellmetFile, 0, 0);
    
    rotation.y -= 5.0fx,

    fogFlag = false;
	alpha = 31;
	renderOffset = {0, 0};
	rotationTranslation = {0, 0};
    switchState(&Morton::introCutScene);
    


    NNSG3dResMatData* mat = modelMorton.getMaterialData(0);
    u32 texParams;
    u32 palParams;

    health = 360;


    scale = Vec3(121.0fx, 121.0fx, 121.0fx);

    modelMorton.pushAnimation(MortonAnim::PreFightPos, 0, FrameCtrl::Looping, 1.0fx, 0);


	collisionMgr.init(this, &bottomSensor, nullptr, &sideSensor); 
    activeCollider.init(this, activeColliderInfo, 0);
    activeCollider.link();


    activeCollider2.init(this, activeColliderInfo2, 0);
    activeCollider2.unlink();


    currentTex = 0;

    return 1;

}



s32 Morton::onRender() {
    MTX::identity(modelMorton.matrix);
    MTX::translate(modelMorton.matrix, position);
    MTX::rotate(modelMorton.matrix, rotation);
    Game::modelMatrix = modelMorton.matrix;
    modelMorton.render(&scale);

    if(phase2){
    modelMorton.getNodeMatrix(10, &shellmet.matrix);
    Game::modelMatrix = shellmet.matrix;
    Vec3 shellScale = Vec3(0.01fx, 0.035fx, 0.01fx);
    shellmet.render(&shellScale);
    }

    NNSG3dResMatData* mat = modelMorton.getMaterialData(0);
    u32 texParams;
    u32 palParams;

    G3D::getTextureParams(modelMorton.texture, currentTex, texParams);
    G3D::getPaletteParams(modelMorton.texture, currentTex, palParams);
    mat->texImageParam = texParams;
    mat->texPlttBase = palParams;
    return true;
    
}

void Morton::switchState(StateFunction function) {
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

void Morton::updateState(){
    (this->*updateFunction)();

}

void Morton::introCutScene(){
    Player *player = Game::getPlayer(0); 
    if (updateStep == Func::Init) {
        timeCrap = 0;
        player->beginCutscene(0);

        updateStep = Func::Step(0);

        // Initializing state
        return;
    }

    if (updateStep == Func::Exit) {
        // Exiting state
        SND::playBGM(7, 0);
        player->endCutscene();
        return;

    }

    // State is running
    timeCrap++;
    if(timeCrap == 10){
        modelMorton.pushAnimation(MortonAnim::FightCutsceneNotice, 6, FrameCtrl::Standard, 1.0fx, 0);
        SND::playExtSFX(SeqArc::MORTON_SE::MORTON_SE_3);

    }
    if(timeCrap == 90){
        modelMorton.pushAnimation(MortonAnim::FightCutsceneEngage, 6, FrameCtrl::Standard, 1.0fx, 0);
        SND::playExtSFX(SeqArc::MORTON_SE::MORTON_SE_1);


    }
    if(timeCrap == 200){
        switchState(&Morton::updateJumpState);
        timeCrap = 0;
    }

    updateVerticalVelocity(); // updates gravity
	updateCollisionSensors(); // clamps position due to collision



}

void Morton::invertedJump(){
    Player *player = Game::getPlayer(0); 
    if (updateStep == Func::Init) {
        timeCrap = 0;
        landAnimation = false;
        modelMorton.pushAnimation(MortonAnim::StartJump, 6, FrameCtrl::Standard, 1.0fx, 0);
        updateStep = Func::Step(0);
        // Initializing state
        return;
    }

    if (updateStep == Func::Exit) {
        // Exiting state
        return;

    }
    timeCrap++;
    if(timeCrap == 12){
        velocity.y = -0x7000;
        position.y -= 3.0fx;
        if(player->position.x < position.x){
            velocity.x = -0x1000;
        }
        else{
            velocity.x = 0x1000;
        }
        SND::playExtSFX(SeqArc::MORTON_SE::MORTON_SE_2);
        modelMorton.pushAnimation(MortonAnim::Jumping, 6, FrameCtrl::Standard, 0.4fx, 0);
    }
    if((position.y < *ViewTop - 0x20000)){
       
       if(velocity.y < 0x3000){
        velocity.y += 0x400;
       }
    }
    else if (timeCrap > 20){
        velocity = 0;
        Particle::Handler::createParticle(46, Vec3(position.x, position.y - 0x10000, position.z));
        modelMorton.pushAnimation(MortonAnim::StartJump, 6, FrameCtrl::Standard, -1.0fx, 0);
        switchState(&Morton::recoverAttck);
    }

}


void Morton::recoverAttck(){
    Player *player = Game::getPlayer(0); 
    
    if (updateStep == Func::Init) {
        timeCrap = 0;
        activeCollider.unlink();

        activeCollider2.link();
        modelMorton.pushAnimation(MortonAnim::Idle, 6, FrameCtrl::Looping, 1.0fx, 0);

        updateStep = Func::Step(0);
        // Initializing state
        return;
    }

    if (updateStep == Func::Exit) {
        modelMorton.pushAnimation(MortonAnim::Idle, 6, FrameCtrl::Looping, 1.0fx, 0);
        // Exiting state
        return;

    }
    timeCrap++;

    if(timeCrap == 10){
        modelMorton.pushAnimation(MortonAnim::startingAttack, 6, FrameCtrl::Standard, 1.0fx, 0);
    }
    if(timeCrap == 20){
        modelMorton.pushAnimation(MortonAnim::ChargeAttack, 6, FrameCtrl::Looping, 1.0fx, 0);
    }
    if(timeCrap == 40){
        modelMorton.pushAnimation(MortonAnim::EndAttack, 6, FrameCtrl::Standard, 1.0fx, 0);

    }
    Vec3 firePos = Vec3(position.x, position.y - 0x20000, position.z);
    if(timeCrap == 50){
        SND::playExtSFX(SeqArc::MORTON_SE::MORTON_SE_1);
        if(player->position.x < position.x){
            firePos.x -= 0x20000;
            Actor::spawnActor(408, 0x75, &firePos);
            Actor::spawnActor(408, 0x85, &firePos);
            Actor::spawnActor(408, 0x95, &firePos);        
        }
        else{
            firePos.x += 0x30000;
            Actor::spawnActor(408, 0x75, &firePos);
            Actor::spawnActor(408, 0x85, &firePos);
            Actor::spawnActor(408, 0x95, &firePos);  
        }
    }
    if(timeCrap == 60){
        modelMorton.pushAnimation(MortonAnim::EndAttackHold, 6, FrameCtrl::Standard, 1.0fx, 0);


    }
    if((recoverAttacksCompleated == 5)&& (timeCrap > 60)){
            position.y -= 0.5fx;
    }
    if(timeCrap == 100){
        recoverAttacksCompleated += 1;
        if(recoverAttacksCompleated == 3){
            switchState(&Morton::resetState);
            activeCollider2.unlink();

        }
        else{
            switchState(&Morton::invertedJump);
        }
    }


}
void Morton::recoveryState(){
    Player *player = Game::getPlayer(0); 
    if (updateStep == Func::Init) {
        timeCrap = 0;
        fliped = true;
         activeCollider.unlink();

        updateStep = Func::Step(0);

        // Initializing state
        return;
    }

    if (updateStep == Func::Exit) {
        // Exiting state
        return;

    }

    timeCrap++;

    if(timeCrap < 16){
        rotation.z -= 0.5fx;
    }
    else{
        rotation.z = 8.0fx;
    }
    if(position.y < *ViewTop - 0x20000){
        position.y += 3.0fx;
    }
    if(timeCrap == 90){
        switchState(&Morton::recoverAttck);
    }

}

void Morton::resetState(){
    Player *player = Game::getPlayer(0); 
    if (updateStep == Func::Init) {
        recoverAttacksCompleated = 0;
        timeCrap = 0;
        fliped = false;

        updateStep = Func::Step(0);
         activeCollider.link();

        // Initializing state
        return;
    }

    if (updateStep == Func::Exit) {
        // Exiting state
        return;

    }

    timeCrap++;
    const CollisionMgrResult result = collisionMgr.collisionResult;

    if(timeCrap < 16){
        rotation.z -= 0.5fx;
    }
    else{
        rotation.z = 0.0fx;
    }
    if(timeCrap == 90){
        rotation.y < -1.0fx;
        switchState(&Morton::updateJumpState);
    }
    updateVerticalVelocity(); // updates gravity
	updateCollisionSensors(); // clamps position due to collision
}


void Morton::fakeDeath(){
    Player *player = Game::getPlayer(0); 
    if (updateStep == Func::Init) {
        modelMorton.pushAnimation(MortonAnim::DeathBounce, 6, FrameCtrl::Standard, 1.0fx, 0);
        SND::stopBGM(0);
        SND::Internal::loadSeq(133);
        timeCrap = 0;
        fliped = false;

        updateStep = Func::Step(0);
        activeCollider.link();
        // Initializing state
        return;
    }

    if (updateStep == Func::Exit) {
        player->endCutscene();
        SND::playBGM(133, 0);
        // Exitng state
        return;

    }

    if(timeCrap == 200){
        player->endCutscene();
        switchState(&Morton::groundPound);
        
    }
    if(timeCrap == 1){
        player->beginCutscene(0);
    }
    if(!(player->subActionFlag & 0x40)){
        timeCrap++;
        if(timeCrap < 140){
            player->rotation.y = 0;
            player->setAnimation(0x63, true,Player::FrameMode::Continue,0xb00,0);
        }
    }

    updateVerticalVelocity(); // updates gravity
	updateCollisionSensors(); // clamps position due to collision
}



void Morton::updateJumpState(){
    Player *player = Game::getPlayer(0); 
    if (updateStep == Func::Init) {
        timeCrap = -10;
        landAnimation = false;
        modelMorton.pushAnimation(MortonAnim::StartJump, 6, FrameCtrl::Standard, 1.0fx, 0);
        updateStep = Func::Step(0);
        // Initializing state
        return;
    }

    if (updateStep == Func::Exit) {
        // Exiting state
        return;

    }
    timeCrap++;
    if(timeCrap == 12){
        velocity.y = 0x6000;
        if(player->position.x < position.x){
            velocity.x = -0x1000;
        }
        else{
            velocity.x = 0x1000;
        }
        SND::playExtSFX(SeqArc::MORTON_SE::MORTON_SE_2);
        modelMorton.pushAnimation(MortonAnim::Jumping, 6, FrameCtrl::Standard, 0.4fx, 0);
    }
    updateVerticalVelocity(); // updates gravity
	updateCollisionSensors(); // clamps position due to collision



    const CollisionMgrResult result = collisionMgr.collisionResult;


    if( bool(result & CollisionMgrResult::GroundTile) && (timeCrap > 60)){
        velocity = 0;
        Particle::Handler::createParticle(46, Vec3(position.x, position.y - 0x10000, position.z));
        modelMorton.pushAnimation(MortonAnim::StartJump, 6, FrameCtrl::Standard, -1.0fx, 0);
        u8 nextState;
        if(phase2)
           nextState = Stage::getRandom(0, 3);
        else
            nextState = Stage::getRandom(0, 1);
        if(nextState == 1)
            switchState(&Morton::updateFireState);
        else if(nextState == 0)
            switchState(&Morton::updateJumpState);
        else if(nextState == 2)
            switchState(&Morton::groundPound);
        else if(nextState == 3)
            switchState(&Morton::groundPound);


    } 
}


void Morton::groundPound(){
    Player *player = Game::getPlayer(0); 
    if (updateStep == Func::Init) {
        timeCrap = 0;
        JumpTimer = 0;
        landAnimation = false;
        modelMorton.pushAnimation(MortonAnim::Idle, 6, FrameCtrl::Standard, 1.0fx, 0);


        targetLocked = false;
        target = player->position.x;
        updateStep = Func::Step(0);
        // Initializing state
        return;
    }

    if (updateStep == Func::Exit) {
        // Exiting state
        return;

    }
    timeCrap++;
    if(timeCrap == 12){
        velocity.y = 0x7000;
        SND::playExtSFX(SeqArc::MORTON_SE::MORTON_SE_2);
        modelMorton.pushAnimation(MortonAnim::startEarthqaukeJump, 6, FrameCtrl::Standard, 0.4fx, 0);
    }
    else if((timeCrap > 12)&&(!targetLocked)){
        if(target < position.x){
            position.x -= 2.0fx;
            if(target > position.x)
                targetLocked = true;
        }
        else if (target > position.x){
            position.x += 2.0fx;
            if(target < position.x)
                targetLocked = true;
        }
    }
    if((timeCrap <140)&&(timeCrap > 40)){
        velocity.y = 0;

    }
    if(timeCrap == 30){
        modelMorton.pushAnimation(MortonAnim::earthqaukeJumping, 6, FrameCtrl::Standard, 0.4fx, 0);
    }
    if(timeCrap == 80){
        modelMorton.pushAnimation(MortonAnim::earthqaukefalling, 6, FrameCtrl::Standard, 0.4fx, 0);
    }


    updateVerticalVelocity(); // updates gravity
	updateCollisionSensors(); // clamps position due to collision

    const CollisionMgrResult result = collisionMgr.collisionResult;

    if( bool(result & CollisionMgrResult::GroundTile) && (timeCrap > 60)){
        
        u32 xPos = (position.x >> 12) - 12;
        u32 yPos = (-position.y >> 12) + 12;

        if(player->actionFlag.noGround == false){
            player->paralyze();
            shockCooldown = 180;
        }

        Stage::stageLayout->changeTile(xPos, yPos, 0);
        Stage::stageLayout->changeTile(xPos + 16, yPos, 0);
        Particle::Handler::createParticle(46, Vec3(position.x, position.y - 0x10000, position.z));
        Particle::Handler::createParticle(2, Vec3(position.x, position.y - 0x10000, position.z));
        Particle::Handler::createParticle(1, Vec3(position.x, position.y - 0x10000, position.z));
        Particle::Handler::createParticle(5, Vec3(position.x, position.y - 0x10000, position.z));
        Particle::Handler::createParticle(6, Vec3(position.x, position.y - 0x10000, position.z));
        Particle::Handler::createParticle(7, Vec3(position.x, position.y - 0x10000, position.z));
        SND::playSFX(376, &position);



        modelMorton.pushAnimation(MortonAnim::Landearthqaukeing, 6, FrameCtrl::Standard, 1.0fx, 0);
        u8 nextState = Stage::getRandom(0, 1);
        if(nextState == 1)
            switchState(&Morton::updateFireState);
        else
            switchState(&Morton::updateJumpState);

    } 
    else if( timeCrap > 140){
        rotation.y= 0;
        position.y -= 3.0fx;
    }
}

void Morton::updateFireState(){
    Player *player = Game::getPlayer(0); 
    if (updateStep == Func::Init) {
        timeCrap = 0;
        modelMorton.pushAnimation(MortonAnim::Idle, 6, FrameCtrl::Looping, 1.0fx, 0);
        updateStep = Func::Step(0);
        // Initializing state
        return;
    }

    if (updateStep == Func::Exit) {
        modelMorton.pushAnimation(MortonAnim::Idle, 6, FrameCtrl::Looping, 1.0fx, 0);
        // Exiting state
        return;

    }
    timeCrap++;

    if(timeCrap == 10){
        modelMorton.pushAnimation(MortonAnim::startingAttack, 6, FrameCtrl::Standard, 1.0fx, 0);
    }
    if(timeCrap == 40){
        modelMorton.pushAnimation(MortonAnim::ChargeAttack, 6, FrameCtrl::Looping, 1.0fx, 0);
    }
    if(timeCrap == 100){
        modelMorton.pushAnimation(MortonAnim::EndAttack, 6, FrameCtrl::Standard, 1.0fx, 0);

    }
    Vec3 firePos = Vec3(position.x, position.y + 0x10000, position.z);
    if(timeCrap == 120){
        SND::playExtSFX(SeqArc::MORTON_SE::MORTON_SE_1);
        if(player->position.x < position.x){
            firePos.x -= 0x20000;
            Actor::spawnActor(408, 0x44, &firePos);
            Actor::spawnActor(408, 0x04, &firePos);        
        }
        else{
            firePos.x += 0x30000;
            Actor::spawnActor(408, 0x34, &firePos);
            Actor::spawnActor(408, 0x14, &firePos);
        }
    }
    if(timeCrap == 130){
        modelMorton.pushAnimation(MortonAnim::EndAttackHold, 6, FrameCtrl::Standard, 1.0fx, 0);
    }
    if(timeCrap == 180){
        u8 nextState = Stage::getRandom(0, 1);
        if(nextState == 1)
            switchState(&Morton::updateFireState);
        else
            switchState(&Morton::updateJumpState);

    }

    updateVerticalVelocity(); // updates gravity
    updateCollisionSensors(); // clamps position due to collision


}

bool Morton::updateMain(){
    Player *player;
    player = Game::getPlayer(0);

    updateState();

    modelMorton.update();
	applyMovement(); // applies velocity

    if(position.y < *ViewBottom + 0x18000){
        
        defeat(0, 0, -9.0fx/32.0fx, 0);
        modelMorton.pushAnimation(MortonAnim::hit2, 0, FrameCtrl::Looping, 1.0fx, 0);
        timeCrap = 0;
    }

    if(fliped){
        if(position.x > player->position.x){
            if(rotation.y > 2.0fx)
                rotation.y -=  0.1fx;
        }
        if(position.x < player->position.x){
            if(rotation.y < 6.0fx)
                rotation.y +=  0.1fx;
        }      
    }
    else{
        if(position.x > player->position.x){
            if(rotation.y > -6.0fx)
                rotation.y -=  0.1fx;
        }
        if(position.x < player->position.x){
           if(rotation.y < -1.0fx)
                rotation.y +=  0.1fx;
        }      
    }
   
    if((health < 0) && (!phase2)){
        phase2 = true;
        switchState(&Morton::fakeDeath);

    }

    if(shockCooldown != 0){
        shockCooldown--;
    }
    else{
        player->physicsFlag.paralyzed = false;
    }

    if(currentTex == 2){
        colourchange--;
        if(colourchange == 0){
            currentTex = 0;
            colourchange = 10;

        }
    }
    
    return true;      
}

bool Morton::updateDefeated(){
    Player *player;
    player = Game::getPlayer(0);
    activeCollider.unlink();
    
    const CollisionMgrResult result = collisionMgr.collisionResult;

    if(!(player->subActionFlag & 0x40)){
        timeCrap++;
         if(timeCrap == 1)
            SND::playBGM(22, 1);
        else if(timeCrap < 140){
            player->rotation.y = 0;
            player->beginCutscene(0);
            player->setAnimation(0x63, true,Player::FrameMode::Continue,0xb00,0);
        }
    }
    else{
        timeCrap = 0;
    }
    if(timeCrap == 140){
        Stage::exitLevel(1);
        Game::setStageCompleted;
    }

    updateVerticalVelocity(); // updates gravity
	applyMovement(); // applies velocity
    
    return true; 
}

bool Morton::updateDefeatedMega(){
    alpha = 0;
    updateVerticalVelocity(); // updates gravity
	applyMovement(); // applies velocity
    return true; 
}

s32 Morton::onDestroy(){
		return 1;
}







    