#include "Kameck.hpp"
#include "main/SFX.hpp"
#include "main/undocumented.hpp"

const LineSensorH Kameck::bottomSensor = {
    -8.5fx, // Start position from the origin (left)
    8.5fx, // End position from the origin (right)
    0.0fx // Y Offset (negative = down)
};

void Kameck::activeCallback(ActiveCollider& self, ActiveCollider& other){
    Kameck* guy = static_cast<Kameck*>(self.owner);
    if(other.owner->actorType == ActorType::Player){
        Player* player = static_cast<Player*>(other.owner);
        auto stomped = guy->updatePlayerStomp(self, 9.0fx/32.0fx, 1, false);
        if(stomped == PlayerStompType::MiniStomp)
            return;

        if(stomped == PlayerStompType::Stomp){
            guy->health -= 60;
            guy->switchState(&Kameck::resetState); 
            SND::playExtSFX(SeqArc::KAMEK_SE::KAMEK_SE_5);

            return;
        }
        player->dealDamage(*guy, 0, 4.0fx, PlayerDamageType::Hit);
    }
    
    StageEntity* stageEntity = static_cast<StageEntity*>(other.owner);
    if(stageEntity->id == 54){
        guy->health -= 120;
        SND::playExtSFX(SeqArc::KAMEK_SE::KAMEK_SE_5);
        guy->switchState(&Kameck::resetState); 
        return;
    }
}

const ActiveColliderInfo Kameck::activeColliderInfo = {

    0.0fx, 10.0fx,   //x offset, y offset
    10.0fx, 10.0fx,   //x length, y lenght

    CollisionGroup::Entity,
    CollisionFlag::None,
    MAKE_GROUP_MASK(CollisionGroup::Player, CollisionGroup::PlayerSpecial, CollisionGroup::Entity, CollisionGroup::Fireball),
    MAKE_FLAG_MASK_EX(CollisionFlag::DamageEntity),
    0,

    Kameck::activeCallback


};

bool Kameck::loadResources(){
     FS::Cache::loadFile(2173 - 131, false);
     FS::Cache::loadFile(2174 - 131, false);
     FS::Cache::loadFile(2175 - 131, false);
     FS::Cache::loadFile(2176 - 131, false);
     FS::Cache::loadFile(2177 - 131, false);


     return 1;
}



void Kameck::onStomped(){
    return;

}

void Kameck::onFireballHit(){
    health -= 10;
    return;
}





void Kameck::defeat(fx32 velX, fx32 velY, fx32 accelY, u8 unk){
    StageEntity::defeat(velX, velY, -accelY, unk);
}

void Kameck::defeatMega(fx32 velX, fx32 velY, fx32 accelY){
    StageEntity::defeatMega(velX, velY, accelY);
}
 
s32 Kameck::onCreate(){
    void* kameckModel = FS::Cache::getFile(kamckID);
    void* kameckAnim  = FS::Cache::getFile(kameckAnimId);
    void* broomModel  = FS::Cache::getFile(broomID);
    void* broomAnim   = FS::Cache::getFile(broomAnimId);

    health = 360;
    scale = Vec3(70.0fx, 70.0fx, 70.0fx);
    rotation.y = -7.0fx;


    modelKameck.create(kameckModel, kameckAnim, 0, 0, 0);
    modelbroom.create(broomModel, broomAnim, 0, 0, 0);
    modelbroom.init(0, FrameCtrl::Standard, 1.0fx, 0);
    modelKameck.pushAnimation(KameckAnimation::CutsceneIntro, 0, FrameCtrl::Standard, 1.017fx, 0);

    *ViewStart = 0;
    *ViewEnd = *ViewStart + 0x10000*16;
    isScroling = false;

    spawnPos = position;
    broomScale = scale;

    fogFlag = false;
	renderOffset = {0, 0};
	rotationTranslation = {0, 0};

    
    NNS_G3dMdlSetMdlAlpha(modelKameck.data, 0, 31);

    switchState(&Kameck::idleState);

    collisionMgr.init(this, &bottomSensor, nullptr, nullptr); 
    activeCollider.init(this, activeColliderInfo, 0);
    activeCollider.link();

    return 1;

}

s32 Kameck::onRender() {
    MTX::identity(modelKameck.matrix);
    MTX::translate(modelKameck.matrix, position);
    MTX::rotate(modelKameck.matrix, rotation);
    Game::modelMatrix = modelKameck.matrix;
    modelKameck.render(&scale);

    
    MTX::identity(modelbroom.matrix);
    MTX::translate(modelbroom.matrix, position);
    MTX::rotate(modelbroom.matrix, rotation);
    Game::modelMatrix = modelbroom.matrix;
    modelbroom.render(&broomScale);
    
    return true;
    
}

void Kameck::switchState(StateFunction function) {

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

void Kameck::updateState() {

    (this->*updateFunction)();

}

void Kameck::deathState(){
    Player *player = Game::getPlayer(0); 
    if (updateStep == Func::Init) {
        NNS_G3dMdlSetMdlAlpha(modelKameck.data, 0, 31);
        timeCrap = 0;
        activeCollider.unlink();
        modelKameck.pushAnimation(KameckAnimation::defeat, 20, FrameCtrl::Standard, 1.0fx, 0);
        //Particle::Handler::createParticle(153, position);
        broomScale = Vec3(0, 0, 0);
        isScroling = false;
        SND::playBGM(22, 1);
        SND::playExtSFX(SeqArc::KAMEK_SE::KAMEK_SE_5);

        updateStep = Func::Step(0);
        return;

    }

    if (updateStep == Func::Exit) {
        return;
    }

    if(timeCrap == 200){
        switchState(&Kameck::idleState);
        Stage::exitLevel(0);
    }
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

    updateVerticalVelocity(); // updates gravity
	applyMovement(); // applies velocity
	updateCollisionSensors(); // clamps position due to collision

}
void Kameck::resetState(){
    Player *player = Game::getPlayer(0); 
    if (updateStep == Func::Init) {
        NNS_G3dMdlSetMdlAlpha(modelKameck.data, 0, 31);
        timeCrap = 31;
        activeCollider.unlink();
        modelKameck.pushAnimation(KameckAnimation::disapear, 5, FrameCtrl::Looping, 1.0fx, 0);
        updateStep = Func::Step(0);
        // Initializing state
        return;
    }

    if (updateStep == Func::Exit) {
        Particle::Handler::createParticle(114, position);
        Particle::Handler::createParticle(35, position);

        // Exiting state
        timeCrap = 0;
        NNS_G3dMdlSetMdlAlpha(modelKameck.data, 0, 0);
        return;
    }
    NNS_G3dMdlSetMdlAlpha(modelKameck.data, 0, timeCrap);
    if(timeCrap != 1){
        timeCrap--;
    }
    else{
        switchState(&Kameck::spawnEnemyAttack);
        activeCollider.link();
    }
}

void Kameck::dashLeftAttack() {
    Player *player = Game::getPlayer(0); 
    if (updateStep == Func::Init) {
        position.y = player->position.y + 0x10000;
        position.x = *ViewEnd + 0x80000;
        NNS_G3dMdlSetMdlAlpha(modelKameck.data, 0, 31);
        modelKameck.pushAnimation(KameckAnimation::flying, 5, FrameCtrl::Looping, 1.0fx, 0);

        //isScroling = false;

        updateStep = Func::Step(0);
        // Initializing state

        return;

    }

    if (updateStep == Func::Exit) {
        // Exiting state
        timeCrap = 0;
        NNS_G3dMdlSetMdlAlpha(modelKameck.data, 0, 0);
        //isScroling = true;

        return;

    }
    //timeCrap++;
    if(position.x < *ViewStart - 0x40000){
        switchState(&Kameck::fireAttack);
    }
    else{
        position.x -= 0x2000;
    }
    

}

void Kameck::dashRightAttack() {
    Player *player = Game::getPlayer(0); 
    if (updateStep == Func::Init) {
        position.y = player->position.y + 0x10000;
        position.x = *ViewStart - 0x80000;
        rotation.y += 7.0fx;
        NNS_G3dMdlSetMdlAlpha(modelKameck.data, 0, 31);
        
        modelKameck.pushAnimation(KameckAnimation::flying, 5, FrameCtrl::Looping, 1.0fx, 0);

        //isScroling = false;

        updateStep = Func::Step(0);
        // Initializing state

        return;

    }

    if (updateStep == Func::Exit) {
        // Exiting state
        rotation.y -= 7.0fx;        
        timeCrap = 0;
        NNS_G3dMdlSetMdlAlpha(modelKameck.data, 0, 0);
        //isScroling = true;

        return;

    }
    //timeCrap++;
    if(position.x > *ViewEnd + 0x60000){
        switchState(&Kameck::fireAttack);
    }
    else{
        position.x += 0x3000;
    }
    

}

void Kameck::randomFire() {
    Player *player = Game::getPlayer(0); 
    if (updateStep == Func::Init) {
        position.y = spawnPos.y + 0x10000*8;
        position.x = *ViewEnd ;

        updateStep = Func::Step(0);
        // Initializing state

        return;

    }

    if (updateStep == Func::Exit) {
        // Exiting state
        timeCrap = 0;
        NNS_G3dMdlSetMdlAlpha(modelKameck.data, 0, 0);

        return;

    }

    *ViewStart -= 0xe00;
    *ViewEnd   -= 0xe00;

    timeCrap++;
    Vec3 firePos = Vec3(position.x - 0x20000, position.y, position.z);
    if(timeCrap == 20){
        firePos.x = *ViewEnd - Stage::getRandom(0, 16) *0x10000;
        Actor::spawnActor(408, 0x73, &firePos);
    }
    if(timeCrap == 40){
        firePos.x = *ViewEnd - Stage::getRandom(0, 16) *0x10000;
        Actor::spawnActor(408, 0x83, &firePos);
        
    }
    if(timeCrap == 60){
        firePos.x = *ViewEnd - Stage::getRandom(0, 16) *0x10000;
        Actor::spawnActor(408, 0x93, &firePos);      
    }
    if(timeCrap == 80){
        firePos.x = *ViewEnd - Stage::getRandom(0, 16) *0x10000;
        Actor::spawnActor(408, 0x73, &firePos);    
    }
    if(timeCrap == 100){
        firePos.x = *ViewEnd - Stage::getRandom(0, 16) *0x10000;
        Actor::spawnActor(408, 0x83, &firePos);
      
    }
    if(timeCrap == 120){
        firePos.x = *ViewEnd - Stage::getRandom(0, 16) *0x10000;
        Actor::spawnActor(408, 0x93, &firePos);
    }
    if(timeCrap == 140){
        firePos.x = *ViewEnd - Stage::getRandom(0, 16) *0x10000;
        Actor::spawnActor(408, 0x73, &firePos);
      
    }
    if(timeCrap == 160){
        firePos.x = *ViewEnd - Stage::getRandom(0, 16) *0x10000;
        Actor::spawnActor(408, 0x83, &firePos);
       
    }
    if(timeCrap == 180){
        firePos.x = *ViewEnd - Stage::getRandom(0, 16) *0x10000;
        Actor::spawnActor(408, 0x93, &firePos);
       
    }
    if(timeCrap == 200){
        firePos.x = *ViewEnd - Stage::getRandom(0, 16) *0x10000;
        Actor::spawnActor(408, 0x73, &firePos);
       
    }
    if(timeCrap == 220){
        firePos.x = *ViewEnd - Stage::getRandom(0, 16) *0x10000;
        Actor::spawnActor(408, 0x83, &firePos);
       
    }
    if(timeCrap == 240){
        firePos.x = *ViewEnd - Stage::getRandom(0, 16) *0x10000;
        Actor::spawnActor(408, 0x93, &firePos);
       
    }
    if(timeCrap == 260){
        firePos.x = *ViewEnd - Stage::getRandom(0, 16) *0x10000;
        Actor::spawnActor(408, 0x73, &firePos);
       
    }
    if(timeCrap == 600){
        switchState(&Kameck::spawnEnemyAttack);

    }
}

void Kameck::fireAttack() {
    Player *player = Game::getPlayer(0); 
    if (updateStep == Func::Init) {
        position.y = spawnPos.y + 0x10000*8;
        position.x = *ViewEnd ;
        isScroling = false;
        SND::playExtSFX(SeqArc::KAMEK_SE::KAMEK_SE_4);
        updateStep = Func::Step(0);
        // Initializing state

        return;

    }

    if (updateStep == Func::Exit) {
        // Exiting state
        timeCrap = 0;
        NNS_G3dMdlSetMdlAlpha(modelKameck.data, 0, 0);
        isScroling = true;

        return;

    }
    timeCrap++;
    Vec3 firePos = Vec3(position.x - 0x10000, position.y, position.z);
    if(timeCrap == 20){
        Actor::spawnActor(408, 0x93, &firePos);
    }
    if(timeCrap == 40){
        firePos.x -= 0x10000*2;
        Actor::spawnActor(408, 0x93, &firePos);
        
    }
    if(timeCrap == 60){
        firePos.x -= 0x10000*4;
        Actor::spawnActor(408, 0x93, &firePos);      
    }
    if(timeCrap == 80){
        firePos.x -= 0x10000*6;
        Actor::spawnActor(408, 0x93, &firePos);    
    }
    if(timeCrap == 100){
         firePos.x -= 0x10000*8;
        Actor::spawnActor(408, 0x93, &firePos);
      
    }
    if(timeCrap == 120){
         firePos.x -= 0x10000*10;
        Actor::spawnActor(408, 0x93, &firePos);
    }
    if(timeCrap == 140){
        firePos.x -= 0x10000*12;
        Actor::spawnActor(408, 0x93, &firePos);
      
    }
    if(timeCrap == 160){
         firePos.x -= 0x10000*14;
        Actor::spawnActor(408, 0x93, &firePos);
       
    }
    if(timeCrap == 500){
        switchState(&Kameck::spawnEnemyAttack);

    }
}

void Kameck::idleState() {
    Player *player = Game::getPlayer(0); 
    if (updateStep == Func::Init) {

        updateStep = Func::Step(0);
        player->beginCutscene(0);


        // Initializing state

        return;

    }

    if (updateStep == Func::Exit) {
        // Exiting state
        SND::playBGM(99, 0);
        player->endCutscene();
        isScroling = true;
        NNS_G3dMdlSetMdlAlpha(modelKameck.data, 0, 0);
        return;

    }

    // State is running
    timeCrap++;
    if(timeCrap == 100){
        SND::playExtSFX(SeqArc::KAMEK_SE::KAMEK_SE_0);
    }
    if(timeCrap == 300){
        switchState(&Kameck::fireAttack);
        timeCrap = 0;
    }

}

void Kameck::spawnEnemyAttack() {
    Player *player = Game::getPlayer(0); 
    if (updateStep == Func::Init) {
        position.y = spawnPos.y;
        position.x = *ViewEnd + 0x50000;


        updateStep = Func::Step(0);
        // Initializing state

        return;

    }

    if (updateStep == Func::Exit) {
        // Exiting state
        timeCrap = 0;
        NNS_G3dMdlSetMdlAlpha(modelKameck.data, 0, 0);

        return;

    }
    timeCrap++;

    if (timeCrap < 32) {
       NNS_G3dMdlSetMdlAlpha(modelKameck.data, 0, timeCrap);
    }

    switch (timeCrap) {
        case 100:{
            modelbroom.init(1, FrameCtrl::Standard, 100.0fx, 0);
            modelKameck.pushAnimation(0, 0, FrameCtrl::Standard, 1.0fx, 0);
            break;
        }

        case 120:{
            modelKameck.pushAnimation(KameckAnimation::startAttack, 5, FrameCtrl::Standard, 1.0fx, 0);
           break;
        }

        case 140:{
            modelKameck.pushAnimation(KameckAnimation::attackChargeHold, 5, FrameCtrl::Looping, 1.0fx, 0);
            break;
        }

        case 200:{
            modelKameck.pushAnimation(KameckAnimation::finishAttack, 5, FrameCtrl::Standard, 1.0fx, 0);

            break;
        }
        case 220:{
           modelKameck.pushAnimation(KameckAnimation::attackEndHold, 5, FrameCtrl::Looping, 1.0fx, 0);
            Vec3 firePos = Vec3(position.x - 0x20000, position.y + 0x10000, position.z - 0x10000);
            Particle::Handler::createParticle(87, firePos);
            u8 actorToSpawn = Stage::getRandom(1, 16);
            switch (actorToSpawn) {
                case 1:{
                    Actor::spawnActor(35, 0x20000100, &firePos);
                    break;
                }
                case 2:{
                     Actor::spawnActor(54, 01, &firePos);
                    break;
                }
                case 3:{
                    Actor::spawnActor(37, 0x0, &firePos);
                    break;
                }   
                case 4:{
                    Actor::spawnActor(38, 0x0, &firePos);
                    break;
                }         
                case 5:{
                    Actor::spawnActor(55, 0x0, &firePos);
                    break;
                } 
                case 6:{
                    Actor::spawnActor(56, 0x0, &firePos);
                    break;
                } 
                case 7:{
                    Actor::spawnActor(248, 0x1, &firePos);
                    break;
                } 
                case 8:{
                    Actor::spawnActor(248, 0x21, &firePos);
                    break;
                } 
                case 9:{
                    Actor::spawnActor(262, 0x020102, &firePos);
                    break;
                } 
                case 10:{
                    Actor::spawnActor(418, 0x1001022, &firePos);
                    break;
                } 
                case 11:{
                    Actor::spawnActor(237, 0x1, &firePos);
                    break;
                } 
                case 12:{
                    Actor::spawnActor(54, 01, &firePos);
                    break;
                } 
                
                case 13:{   
                    Actor::spawnActor(408, 0x73, &firePos);
                    Actor::spawnActor(408, 0x83, &firePos);
                    Actor::spawnActor(408, 0x93, &firePos);                    
                    break;
                } 
                case 14:{   
                    Actor::spawnActor(408, 0x74, &firePos);
                    Actor::spawnActor(408, 0x84, &firePos);
                    Actor::spawnActor(408, 0x94, &firePos);                    
                    break;
                } 
                case 15:{   
                    Actor::spawnActor(408, 0x74, &firePos);
                    Actor::spawnActor(408, 0x84, &firePos);
                    Actor::spawnActor(408, 0x94, &firePos);                    
                    break;
                } 
                case 16:{   
                    Actor::spawnActor(408, 0x75, &firePos);
                    Actor::spawnActor(408, 0x85, &firePos);
                    Actor::spawnActor(408, 0x95, &firePos);                    
                    break;
                } 
            }
            SND::playExtSFX(SeqArc::KAMEK_SE::KAMEK_SE_1);

            
           break;
        }

        case 280:{
           modelKameck.pushAnimation(KameckAnimation::flying, 5, FrameCtrl::Looping, 1.0fx, 0);
           break;
        }
        case 310:{
            u8 nextState = Stage::getRandom(1,8);
            if(nextState == 3){
                 switchState(&Kameck::fireAttack);
            }
            else if(nextState == 4){
                 switchState(&Kameck::dashLeftAttack);
            }
            else if(nextState == 5){
                 switchState(&Kameck::dashRightAttack);
            }
            else if(nextState == 6){
                 switchState(&Kameck::randomFire);
            }
            else{
                 switchState(&Kameck::spawnEnemyAttack);
            }
           break;
        }
    }

    if (timeCrap > 280) {
        position.x -= 0x2500;
        position.y += (timeCrap - 280)*2*0x200;
    }
    // State is running

}

bool Kameck::updateMain(){
    Player *player;
    player = Game::getPlayer(0);

    Kameck::updateState();

    if(health < 0){
       switchState(&Kameck::deathState);
       health = 999;
    }
    // We always want Mario/Luigi to look at the boss
    Vec3 lookAtPos = Vec3(position.x, position.y + 32fx, 0);
    Game::setPlayerLookAtPosition(lookAtPos);
    Game::setPlayerLookingAtTarget(true);
    modelKameck.update();
    modelbroom.update();


    if(isScroling){
        *ViewStart += 0x800;
        *ViewEnd   += 0x800;
    }


    return true;      
}

bool Kameck::updateDefeated(){
    this->Base::destroy();

    return true; 
}

bool Kameck::updateDefeatedMega(){

    return true; 
}

s32 Kameck::onDestroy(){
		return 1;
}







    