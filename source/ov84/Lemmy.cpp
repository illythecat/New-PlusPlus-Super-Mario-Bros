#include "Lemmy.hpp"
#include "main/SFX.hpp"
#include "main/core.hpp"
#include "main/undocumented.hpp"


void Lemmy::activeCallback(ActiveCollider& self, ActiveCollider& other){

    Lemmy* balls = static_cast<Lemmy*>(self.owner);
    if(other.owner->actorType == ActorType::Player){
        Player* player = static_cast<Player*>(other.owner);
        if(player->position.x > balls->position.x){
            player->doBump(Vec2(0x3000, 0x4000));
        }
        else{
            player->doBump(Vec2(-0x3000, 0x4000));
        }
    }
    if(other.owner->actorType == ActorType::Entity){
        StageEntity* actor = static_cast<StageEntity*>(other.owner);
        if (actor->id == ObjectID::LemmysBalls){
            if(actor->settings == 0x8){
                actor->defeat(0, 0.0fx, -9.0fx/32.0fx, 0);
            }

            if(actor->position.x < balls->position.x)
                actor->velocity.x += 2.0fx;
            else
                actor->velocity.x -= 2.0fx;
        }
    }
}

const LineSensorH Lemmy::bottomSensor = {
    -8.5fx, // Start position from the origin (left)
    8.5fx, // End position from the origin (right)
    0.0fx // Y Offset (negative = down)
};

const LineSensorV Lemmy::sideSensor = {
    16.0fx, // Start position from the origin (top)
    2.0fx, // End position from the origin (bottom)
    12.5fx // X offset (negative = left)
};	

const ActiveColliderInfo Lemmy::activeColliderInfoMain = {

    0.0fx, 36.0fx,
    12.0fx, 12.0fx,

    CollisionGroup::Entity,
    CollisionFlag::None,
    MAKE_GROUP_MASK(CollisionGroup::Player, CollisionGroup::PlayerSpecial, CollisionGroup::Fireball),
    MAKE_FLAG_MASK_EX(CollisionFlag::None),
    0,

    Lemmy::damagePlayerCallback

};
const ActiveColliderInfo Lemmy::activeColliderInfo2 = {
    0.0fx, 20.0fx,
    20.0fx, 20.0fx,
    CollisionGroup::Entity,
    CollisionFlag::None,
    MAKE_GROUP_MASK(CollisionGroup::Player, CollisionGroup::PlayerSpecial, CollisionGroup::Entity),
    MAKE_FLAG_MASK_EX(CollisionFlag::None),
    0,
    Lemmy::activeCallback

};

bool Lemmy::loadResources(){
     FS::Cache::loadFile(2373 - 131, false);
     FS::Cache::loadFile(2374 - 131, false);
     FS::Cache::loadFile(2375 - 131, false);
     FS::Cache::loadFile(2379 - 131, false);
     FS::Cache::loadFile(2410 - 131, false);
     FS::Cache::loadFile(2412 - 131, false);
     FS::Cache::loadFile(2413 - 131, false);

     return 1;
}

void Lemmy::damagePlayer(ActiveCollider& self, Player& player){
        auto stomped = updatePlayerStomp(self, 9.0fx/32.0fx, 1, false);
        if(cutscene)
            return;
        if(inShell){
            player.dealDamage(*this, 0, 4.0fx, PlayerDamageType::Hit);
            return;
        }

        if(stomped == PlayerStompType::MiniStomp)
            return;

        if(stomped == PlayerStompType::Stomp){
            setStompCollision(player);
            return;
        }
        player.dealDamage(*this, 0, 4.0fx, PlayerDamageType::Hit);
}

void Lemmy::onStomped(){
    Player *player;
    player = Game::getPlayer(0);
    if(!cutscene){
    if(!inShell){
    BossHealth -= 120;
    }
    if(!phase2){
        if(BossHealth > 0){
            SND::playExtSFXUnique(SeqArc::LEMMY_SE::LEMMY_SE_5);
            Vec3 posRedThing = Vec3(position.x, position.y + 0x30000, position.z + 0x30000);
            Particle::Handler::createParticle(83, posRedThing);
            switchState(&Lemmy::ShellState);
        }
        else{
            switchState(&Lemmy::fakeDeath);
            cutscene = true;
        }
    }
    else{
        if(BossHealth2 > 0){
        BossHealth2 -= 120;
        switchState(&Lemmy::switchSide);
        }  
        else{
            exitTimer = 0;
            defeat(0, 0, -9.0fx/32.0fx, 0); 
        }   
    }
    }
    //defeat(0, 0, 9.0fx/32.0fx, 0);
}

void Lemmy::onGroundPound(){
    Player *player;
    player = Game::getPlayer(0);
    if(!inShell){
    if(BossHealth > 0){
        switchState(&Lemmy::ShellState);
    }
    BossHealth -= 120;
    }

    //defeat(0, 0, 9.0fx/32.0fx, 0);
}

void Lemmy::onFireballHit(){
    Player *player;
    player = Game::getPlayer(0);
    SND::playExtSFXUnique(SeqArc::LEMMY_SE::LEMMY_SE_5);
    recoveryTimer = 20;
    BossHealth -= 1;
    //defeat(0, 0, 9.0fx/32.0fx, 0);
}

void Lemmy::defeat(fx32 velX, fx32 velY, fx32 accelY, u8 unk){
    StageEntity::defeat(velX, velY, accelY, unk);

}

void Lemmy::defeatMega(fx32 velX, fx32 velY, fx32 accelY){
    StageEntity::defeatMega(velX, velY, accelY);
}



void Lemmy::switchState(StateFunction function) {
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

void Lemmy::updateState(){
    (this->*updateFunction)();
}

s32 Lemmy::onCreate(){

    void* BossModel = FS::Cache::getFile(LemmyID);
    void* BossAnim = FS::Cache::getFile(LemmyAnimationID);
    void* BallAnim = FS::Cache::getFile(LemmyAnimationID);
    void* BallModel = FS::Cache::getFile(Ball1ModelID);
    void* ShellModel = FS::Cache::getFile(LemmyShellId);
    void* carModel = FS::Cache::getFile(carId);
    void* carAnm = FS::Cache::getFile(carAnmId);
    maxFireChain = 0;
    switchState(&Lemmy::IntroCutScene);    
    Vec3 ballSpawnPos = Vec3(position.x, position.y + 0x10000, position.z);
    ball = Actor::spawnActor(416, 0x8, &ballSpawnPos);

    LemmyScale = Vec3(1.0fx, 1.0fx, 1.0fx);
    spawnPos = Vec3(position.x, position.y, position.z);

    modelLemmy.create(BossModel, BossAnim, 0, 0, 0);
    modelCar.create(carModel, carAnm, 0, 0, 0);
    modelCar.init(0, FrameCtrl::Looping, 1.0fx, 0);

    LemmyShell.create(ShellModel, 0, 0);
    modelLemmy.init(Demo_Battle_A_Wait, FrameCtrl::Looping, 1.0fx, 0);
    carPos = Vec3(position.x + 0xa0000, position.y + 0xa0000, position.x);
    fogFlag = false;
	alpha = 0xff;
	renderOffset = {0, 0};
	rotationTranslation = {0, 0};

    this->rotation.y = 2.0fx;
    cutscene = false;
    cutSceneTimer = 0;
    FightState = 0;
    BossHealth2 = 360;
    collisionMgr.init(this, &bottomSensor, nullptr, &sideSensor); 
    activeColliderMain.init(this, activeColliderInfoMain, 0);
    activeColliderMain.link();
        ball->scale = Vec3(0.8fx, 1.15fx, 0.8fx);
    LemmyPosition = Vec3(position.x, position.y + 0x21000, position.z);

    BossHealth = 350;
    exitTimer = 0;

    return 1;

}

s32 Lemmy::onRender() {
    if(inShell){
        MTX::identity(LemmyShell.matrix);
        MTX::translate(LemmyShell.matrix, Vec3(position.x, position.y + 0x21000, position.z));
        MTX::rotate(LemmyShell.matrix, VecFx16(rotation.x, rotation.y + 8.0fx, rotation.z));
        Game::modelMatrix = LemmyShell.matrix;
        Vec3 ShellScale = Vec3(scale.x *2.2, scale.y*2.2, scale.z*2.2);
        ball->scale = Vec3(0.8fx, 1.15fx, 1.15fx);
        LemmyShell.render(&ShellScale);  
    }
    else{
        MTX::identity(modelLemmy.matrix);
        MTX::translate(modelLemmy.matrix, LemmyPosition);
        MTX::rotate(modelLemmy.matrix, VecFx16(rotation.x, rotation.y + 8.0fx, rotation.z));
        Game::modelMatrix = modelLemmy.matrix;
        modelLemmy.render(&LemmyScale);  
        ball->rotation = VecFx16(0, 0, 0);

    }  
    if(updateModelPos){
        if(!phase2)
            ball->position = Vec3(position.x, position.y + 0x10000, position.z);
        LemmyPosition = Vec3(position.x, position.y + 0x21000, position.z);
    }
    if(phase2){
        MTX::identity(modelCar.matrix);
        MTX::translate(modelCar.matrix, carPos);
        MTX::rotate(modelCar.matrix, VecFx16(rotation.x, rotation.y - 4.0fx, rotation.z));
        Game::modelMatrix = modelCar.matrix;
        Vec3 carScale = Vec3(1.7fx, 1.7fx, 1.7fx);
        modelCar.render(&carScale);  

        updateModelPos = true;
    }
    else{

    }

    return 1;
}


void Lemmy::IntroCutScene() {
    Player *player = Game::getPlayer(0); 
    if (updateStep == Func::Init) {
        player->beginCutscene(0);
        updateStep = Func::Step(0);
        updateModelPos = true;
        // Initializing state
        return;
    }
    if (updateStep == Func::Exit) {
        // Exiting state
    	SND::initialize;
	    SND::playBGM(7, 0);
        player->endCutscene();
        
        return;
    }
    timeCrap++;
    if(timeCrap == 10){
        modelLemmy.init(Demo_Battle_A_Start, FrameCtrl::Standard, 1.0fx, 0);
        SND::playExtSFXUnique(SeqArc::LEMMY_SE::LEMMY_SE_0);

    }

    if(timeCrap == 60){
        modelLemmy.init(Demo_Battle_A, FrameCtrl::Standard, 1.0fx, 0);
        Vec3 firePos3 = Vec3(position.x, position.y +0x10000, position.z + 0x10000);
        SND::playExtSFXUnique(SeqArc::LEMMY_SE::LEMMY_SE_1);
        Particle::Handler::createParticle(46, firePos3);
        Particle::Handler::createParticle(277, firePos3);
    }
    if(timeCrap == 120){
        modelLemmy.init(Demo_Battle_A, FrameCtrl::Standard, 1.0fx, 0);
        Vec3 firePos3 = Vec3(position.x, position.y +0x10000, position.z + 0x10000);
        SND::playExtSFXUnique(SeqArc::LEMMY_SE::LEMMY_SE_1);
        Particle::Handler::createParticle(46, firePos3);
        Particle::Handler::createParticle(277, firePos3);      
    }

    if(timeCrap == 160){
        switchState(&Lemmy::jumpState);
    }
}


void Lemmy::VictoryAnimation(){
    Player *player;
    player = Game::getPlayer(0);
	player->actionFlag.bowserJrBeaten	 = true;
}

void Lemmy::jumpState(){
    Player *player = Game::getPlayer(0); 
    if (updateStep == Func::Init) {
        timeCrap = -40;
        updateStep = Func::Step(0);
        // Initializing state
        return;
    }

    if (updateStep == Func::Exit) {
        // Exiting state
        modelLemmy.init(Jump_End,  FrameCtrl::Standard, 1.0fx, 0);
        return;
    }
    timeCrap++;
    if(timeCrap == 0){
        modelLemmy.init(Jump_Start,  FrameCtrl::Standard, 1.0fx, 0);
    }
    if(timeCrap == 12){
        velocity.y = 0x7000;
        SND::playExtSFX(SeqArc::LEMMY_SE::LEMMY_SE_2);

        SND::playSFX(370, 0);
        if(player->position.x < position.x){
            velocity.x = -0x1000;
        }
        else{
            velocity.x = 0x1000;
        }
        
        modelLemmy.init(Jump_A,  FrameCtrl::Standard, 0.8fx, 0);
    }
    if(timeCrap < -11){
        updateModelPos = false;
        if(timeCrap == -35){
        modelLemmy.init(Jump_Start,  FrameCtrl::Standard, 1.0fx, 0);
        }
        if (timeCrap == -30){
            modelLemmy.init(Jump_A,  FrameCtrl::Standard, 3.0fx, 0);
        }
        if((timeCrap > -30) && (timeCrap < -20)){
            LemmyPosition.y +=(0.1fx * (30 +timeCrap)^10)/2;
        }
        if(timeCrap == -15){
            modelLemmy.init(Jump_End,  FrameCtrl::Standard, 0.4fx, 0);
        }

        if((timeCrap > -20) && (timeCrap < -10)){
            LemmyPosition.y -=(0.1fx * (timeCrap+10)^10)/2;
        }
    }
    else if (timeCrap <11){
        if(timeCrap < 0){
        ball->scale.y +=(0.015fx * timeCrap^10)/2;
        ball->position.y -= 0x800;
        LemmyPosition.y = ball->position.y + 0x10000;
        }
        else{
        ball->scale.y -=(0.015fx * (-11+timeCrap)^10)/2;
        ball->position.y += 0x800;
        LemmyPosition.y = ball->position.y + 0x10000;
        }
        updateModelPos = false;
    }
    else{
        updateModelPos = true;
    }
    if (timeCrap % 10 == 0 && timeCrap >= 10){
        Particle::Handler::createParticle(115 , Vec3(position.x, position.y, position.z - 10.0fx));
    }
    const CollisionMgrResult result = collisionMgr.collisionResult;

    if( bool(result & CollisionMgrResult::GroundTile) && (timeCrap > 60)){
        velocity = 0;
        Particle::Handler::createParticle(57, Vec3(position.x, position.y, position.z));
        Particle::Handler::createParticle(10 , position);

        u8 nextState = Stage::getRandom(0, 2);
        if(nextState == 1)
            switchState(&Lemmy::fireState);
        else if(nextState == 2)
            switchState(&Lemmy::fireState2);
        else
            switchState(&Lemmy::jumpState);
    }

    if(position.x < *ViewStart + 0x30000){
        velocity.x *=-0.1;
        position.x += 1.0fx;
    }
    if(position.x > *ViewEnd - 0x30000){
        velocity.x *=-0.1;
        position.x -= 1.0fx;
    }


}

void Lemmy::spawnFire(s8 speed, bool fire){
    Vec3 firePos = Vec3(position.x - 0x20000*speed, position.y + 0x38000, position.z + 0x10000);
    if(speed == 1){
        if(fire)
            Actor::spawnActor(416, 0x0, &firePos);  
        
        else{
            Actor::spawnActor(408, 0xa5, &firePos);

            /*
            Actor::spawnActor(408, 0x05, &firePos);
            Actor::spawnActor(408, 0x45, &firePos);
            Actor::spawnActor(408, 0x65, &firePos);
            */
        }

    }
    else{
        if(fire){
            Actor::spawnActor(416, 0x1, &firePos);  
        }
        else{
            Actor::spawnActor(408, 0xa5, &firePos);
            /*
            Actor::spawnActor(408, 0x15, &firePos);
            Actor::spawnActor(408, 0x35, &firePos);
            Actor::spawnActor(408, 0x55, &firePos);  
            */
        }
    }

    SND::playExtSFX(SeqArc::LEMMY_SE::LEMMY_SE_1);
    Particle::Handler::createParticle(87, firePos);

}

void Lemmy::fireState2(){
    Player *player = Game::getPlayer(0); 
    s8 speed;
    if(position.x > player->position.x){
        speed = 1;
    }
    else{
        speed = -1;
    }
    Vec3 firePos2 = Vec3(position.x - 0x10000, position.y + 0x20000, position.z);


    if (updateStep == Func::Init) {
        timeCrap = 0;
        if(phase2)
            timeCrap = -30;
        updateStep = Func::Step(0);
        
        // Initializing state
        return;
    }

    if (updateStep == Func::Exit) {
        // Exiting state
        return;

    }
    timeCrap++;

    if(timeCrap == 1){
        modelLemmy.init(Through_Start,  FrameCtrl::Standard, 1.5fx, 0);
    }

    if(timeCrap == 10){
        modelLemmy.init(Through_Wait,  FrameCtrl::Looping, 1.5fx, 0);
        Particle::Handler::createParticle(149, firePos2);
    }
    if((timeCrap == 15)||(timeCrap == 80)){
            Particle::Handler::createParticle(149, firePos2);
    }


    if(timeCrap == 26){
        modelLemmy.init(Through_End,  FrameCtrl::Standard, 1.5fx, 0);
        Particle::Handler::createParticle(149, firePos2);
    }

    if(timeCrap == 31){
        modelLemmy.init(Through_End_Wait,  FrameCtrl::Looping, 1.5fx, 0);
        spawnFire(speed, false);
    }
    if(timeCrap == 50){
        u8 nextState = Stage::getRandom(0, 2);
        maxFireChain += 1;
        if(maxFireChain == 4){
            maxFireChain = 0;
            switchState(&Lemmy::switchSide);
        }
        if(nextState == 1){
            maxFireChain = 0;
            switchState(&Lemmy::fireState);
        }
        else if(nextState == 2){
            switchState(&Lemmy::fireState2);
        }
            
        else{
            if(!phase2){
            switchState(&Lemmy::jumpState);    
            }
            else{
            maxFireChain = 0;
            switchState(&Lemmy::fireState);
            }
        }  
    }
}

void Lemmy::fireState(){
    Player *player = Game::getPlayer(0); 
    s8 speed;
    if(position.x > player->position.x){
        speed = 1;
    }
    else{
        speed = -1;
    }
    Vec3 firePos2 = Vec3(position.x - 0x10000, position.y + 0x20000, position.z);


    if (updateStep == Func::Init) {
        timeCrap = 0;
           if(phase2)
            timeCrap = -30;
        updateStep = Func::Step(0);
        
        // Initializing state
        return;
    }

    if (updateStep == Func::Exit) {
        // Exiting state
        return;

    }
    timeCrap++;
    if(timeCrap == 1){
        modelLemmy.init(Through_Start,  FrameCtrl::Standard, 1.5fx, 0);
    }

    if(timeCrap == 10){
        modelLemmy.init(Through_Wait,  FrameCtrl::Looping, 1.5fx, 0);
        Particle::Handler::createParticle(149, firePos2);
    }
    if((timeCrap == 15)||(timeCrap == 80)){
            Particle::Handler::createParticle(149, firePos2);
    }


    if(timeCrap == 26){
        modelLemmy.init(Through_End,  FrameCtrl::Standard, 1.5fx, 0);
        Particle::Handler::createParticle(149, firePos2);
    }

    if(timeCrap == 31){
        modelLemmy.init(Through_End_Wait,  FrameCtrl::Looping, 1.5fx, 0);


        spawnFire(speed, true);
    }
    if(timeCrap == 50){
        u8 nextState = Stage::getRandom(0, 2);
        if(nextState == 1)
            switchState(&Lemmy::fireState);
        else if(nextState == 2)
            switchState(&Lemmy::fireState2);
        else{
            switchState(&Lemmy::switchSide);
        }
    }
}

void Lemmy::fakeDeath(){
    Player *player;
    player = Game::getPlayer(0);
    if (updateStep == Func::Init) {
        timeCrap = 0;
        modelLemmy.init(Chase,  FrameCtrl::Looping, 1.0fx, 0);
        SND::playExtSFX(SeqArc::LEMMY_SE::LEMMY_SE_0);
        phase2 = true;
        updateModelPos = true;
        velocity.x = 0;

        updateStep = Func::Step(0);
        return;
    }
    if (updateStep == Func::Exit) {
        ball->destroy();
        cutscene = false;

        return;
    }
    
    const CollisionMgrResult result = collisionMgr.collisionResult;
    if(!deathAnim){
        modelLemmy.init(Demo_Escape_1,  FrameCtrl::Standard, 1.0fx, 0);
        deathAnim = true;
    }

    if(!(player->subActionFlag & 0x40)){
        exitTimer++;
         if(exitTimer == 1)
            SND::playBGM(22, 1);
        else if(exitTimer < 140){
            player->rotation.y = 0;
            player->beginCutscene(0);
            player->setAnimation(0x63, true,Player::FrameMode::Continue,0xb00,0);
        }
    }
    else{
        updateVerticalVelocity(); // updates gravity
    }
    if(exitTimer == 120){
        modelLemmy.init(Demo_Escape_3,  FrameCtrl::Standard, 1.0fx, 0);
    }
    if(exitTimer == 180){
        modelLemmy.init(Jump_Start,  FrameCtrl::Standard, 1.0fx, 0);
    }
    if(exitTimer == 200){
        modelLemmy.init(Jump_A,  FrameCtrl::Standard, 1.0fx, 0);
        Particle::Handler::createParticle(0, position);
        Particle::Handler::createParticle(64, position);
    }

    if(exitTimer == 210){
        modelLemmy.init(Jump_End,  FrameCtrl::Standard, 1.0fx, 0);
	    NWAV::play(2269);

    }
    if(exitTimer == 220){
        modelLemmy.init(Demo_Battle_A_Wait,  FrameCtrl::Standard, 1.0fx, 0);
    }
    if (exitTimer > 120){
        if (position.y + 0x16000 < carPos.y){
           carPos.y -= 0x3000;
        }

        if(carPos.x > position.x){
            carPos.x -= 0x3000;
        }

        //carPos = Vec3(position.x - 0x400, position.y - 0x4001, position.x);
    }

    
    if(exitTimer == 220){
        player->endCutscene();
        switchState(&Lemmy::phase2mainState);

    }
    modelLemmy.update();
    modelCar.update();
    updateCollisionSensors();
}

void Lemmy::MainState(){    
}

void Lemmy::switchSide(){    
    Player *player = Game::getPlayer(0); 
    static bool switchDone;
    if (updateStep == Func::Init) {
        timeCrap = 0;
        modelLemmy.init(Chase,  FrameCtrl::Looping, 1.0fx, 0);
        switchDone = false;
        if(!phase2){
            switchState(&Lemmy::jumpState);
        }
        updateStep = Func::Step(0);
        
        return;
    }
    if (updateStep == Func::Exit) {
        return;
    }
    if(!switchDone){
    if(onRight){
        if(position.x > *ViewStart + 0x30000){
            position.x -= 0x4000;
        }
        else{
            switchDone = true;
            onRight = false;
        }
    }   
    else {
        if(position.x < *ViewEnd - 0x30000){
        position.x += 0x4000;
        } 
        else{
            switchDone = true;
            onRight = true;
        }      
    }
    }
    timeCrap++;
    if(timeCrap == 90){
            u8 nextState = Stage::getRandom(0,2);
            if(nextState == 1)
                switchState(&Lemmy::fireState);
            else if(nextState == 2)
                switchState(&Lemmy::switchSide);
            else 
                switchState(&Lemmy::fireState2);
    }
    carPos = Vec3(position.x, position.y + 0x18000, position.x);
}


void Lemmy::phase2mainState(){
    Player *player = Game::getPlayer(0); 
    if (updateStep == Func::Init) {
        timeCrap = 0;
        modelLemmy.init(Chase,  FrameCtrl::Looping, 1.0fx, 0);
        posLocked = false;
        activeCollider.link();
        activeCollider2.init(this, activeColliderInfo2, 0);
        activeCollider2.shape = ActiveCollider::Shape::Round;
        activeCollider2.link();
        onRight = true;
        updateStep = Func::Step(0);
        return;
    }
    if (updateStep == Func::Exit) {
        return;
    }

    if(position.x < *ViewEnd - 0x30000){
        position.x += 0x3000;
    }
    if(position.y < spawnPos.y + 0x30000){
        position.y += 0x2000;
    }
    timeCrap++;
    if(timeCrap == 60){
            u8 nextState = Stage::getRandom(0,2);
            if(nextState == 1)
                switchState(&Lemmy::fireState);
            else if(nextState == 2)
                switchState(&Lemmy::switchSide);
            else 
                switchState(&Lemmy::fireState2);
    }
    carPos = Vec3(position.x, position.y + 0x18000, position.x);

}

void Lemmy::ShellState(){
    Player* player = Game::getPlayer(0);
    if (updateStep == Func::Init) {
        timeCrap = -10;
        modelLemmy.init(Damaged,  FrameCtrl::Looping, 1.0fx, 0);
        Particle::Handler::createParticle(0, position);
        Particle::Handler::createParticle(64, position);
        updateModelPos = true;
        startShellSlide = false;
        inShell = true;



        velocity.x = 0;
        velocity.y = 0;
        ball->scale = Vec3(0.8fx, 1.15fx, 0.8fx);

        updateStep = Func::Step(0);


        // Initializing state
        return;
    }

    if (updateStep == Func::Exit) {
        inShell = false;

        if(position.x > player->position.x){
                rotation.y = -6.0fx;
        }
        if(position.x < player->position.x){
                rotation.y =  -2.0fx;
        } 

        // Exiting state
        return;

    }

    const CollisionMgrResult result = collisionMgr.collisionResult;

    //rotation.y += velocity.x/2;

    if(timeCrap < 20){
        velocity.x = 0.1fx *((settings & 0x2) + 1) * timeCrap*direction;
    }
    if(bool(result & CollisionMgrResult::GroundTile) && !startShellSlide){
        Particle::Handler::createParticle(57, Vec3(position.x, position.y, position.z));
        Particle::Handler::createParticle(10 , position);
        modelLemmy.init(Shell_In,  FrameCtrl::Standard, 1.0fx, 0);
        startShellSlide = true;
    }
    if(startShellSlide){
        timeCrap++;
        rotation.y += velocity.x/10;
        ball->rotation.y = rotation.y;
        
    }
    if(timeCrap == 10){
        direction = 1;
    }
    if (bool(position.x < *ViewStart + 0x30000)|(position.x > *ViewEnd - 0x30000)){
        direction *= -1;
        velocity.x *= direction;
        Particle::Handler::createParticle(4, position);
        SND::playSFX(363, &this->position);
    }
    //rotation.y += velocity.x/2;

    if(timeCrap < 40){
        velocity.x = 0.08fx *((settings & 0x2) + 1) *  (timeCrap)*direction;
    }
    if(timeCrap == 380){
        inShell = false;
        modelLemmy.init(Shell_Out,  FrameCtrl::Standard, 1.0fx, 0);
        velocity.x = 0;
        //rotation.y = 0;
    }
    if(timeCrap == 420){
        switchState(&Lemmy::jumpState);    
    }


}

void Lemmy::UpdateRotation(){
    Player* player = Game::getPlayer(0);
        if(position.x > player->position.x){
            if(rotation.y > 2.0fx)
                rotation.y -=  0.1fx;

        }
        if(position.x < player->position.x){
            if(rotation.y < 6.0fx)
                rotation.y +=  0.1fx;
        } 
}

bool Lemmy::updateMain(){

    
    Player *player;
    player = Game::getPlayer(0);
    const CollisionMgrResult result = collisionMgr.collisionResult;
    updateState();
    modelLemmy.update();
    modelCar.update();


   // 
	applyMovement(); // applies velocity
	updateCollisionSensors(); // clamps position due to collision

    UpdateRotation();
    if(BossHealth < 1){
        //position.y += 0x300;
        //defeat(0, 0, 9.0fx/32.0fx, 0);

    }   
    else{
        updateVerticalVelocity(); // updates gravity
        phase2 = false;
    }


    return true;      
}




bool Lemmy::updateDefeated(){
    Player *player;
    player = Game::getPlayer(0);
    activeCollider.unlink();
    phase2 = false;
    const CollisionMgrResult result = collisionMgr.collisionResult;
    if(!deathAnim){
        modelLemmy.init(Demo_Escape_1,  FrameCtrl::Standard, 1.0fx, 0);
        deathAnim = true;
    }

    if(!(player->subActionFlag & 0x40)){
        exitTimer++;
         if(exitTimer == 1)
            SND::playBGM(22, 1);
        else if(exitTimer < 140){
            player->rotation.y = 0;
            player->beginCutscene(0);
            player->setAnimation(0x63, true,Player::FrameMode::Continue,0xb00,0);
        }
    }
    if(exitTimer == 140){
        //player->endCutscene();
        Stage::exitLevel(1);
    }
    applyMovement(); // applies velocity
    updateVerticalVelocity(); // updates gravity
    modelLemmy.update();

    return true; 

}

bool Lemmy::updateDefeatedMega(){
    return true; 
}

s32 Lemmy::onDestroy(){
		return 1;
}







    














    