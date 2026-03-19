#include "Iggy.hpp"
#include "main/SFX.hpp"



void Iggy::activeCallback(ActiveCollider& self, ActiveCollider& other){

    Iggy* cb = static_cast<Iggy*>(self.owner);
    if(other.owner->actorType == ActorType::Player){
        Player* player = static_cast<Player*>(other.owner);
    }
}

const LineSensorH Iggy::bottomSensor = {
    -8.5fx, // Start position from the origin (left)
    8.5fx, // End position from the origin (right)
    0.0fx // Y Offset (negative = down)
};

const LineSensorV Iggy::sideSensor = {
    16.0fx, // Start position from the origin (top)f
    2.0fx, // End position from the origin (bottom)
    12.5fx // X offset (negative = left)
};	

const ActiveColliderInfo Iggy::activeColliderInfo = {

    0.0fx, 12.0fx,
    12.0fx, 12.0fx,

    CollisionGroup::Entity,
    CollisionFlag::None,
    MAKE_GROUP_MASK(CollisionGroup::Player, CollisionGroup::PlayerSpecial, CollisionGroup::Fireball),
    MAKE_FLAG_MASK_EX(CollisionFlag::None),
    0,

    Iggy::damagePlayerCallback

};

bool Iggy::loadResources(){
     FS::Cache::loadFile(2537 - 131, false);
     FS::Cache::loadFile(2536 - 131, false);
     FS::Cache::loadFile(2552 - 131, false);


     return 1;
}

void Iggy::damagePlayer(ActiveCollider& self, Player& player){
        if(!inShell){
            auto stomped = updatePlayerStomp(self, 9.0fx/32.0fx, 1, false);
            if(stomped == PlayerStompType::MiniStomp)
                return;

            if(stomped == PlayerStompType::Stomp){
                setStompCollision(player);
                return;
            }
            player.dealDamage(*this, 0, 4.0fx, PlayerDamageType::Hit);
        }
        else{
            player.dealDamage(*this, 0, 4.0fx, PlayerDamageType::Hit);
            return;
        }
}

void Iggy::onStomped(){
    Player *player;
    player = Game::getPlayer(0);
    if(BossHealth > 0){
        switchState(&Iggy::ShellState);
    }
    BossHealth -= 120;
    


    //defeat(0, 0, 9.0fx/32.0fx, 0);
}

void Iggy::onGroundPound(){
    Player *player;
    player = Game::getPlayer(0);
    if(BossHealth > 0){
        switchState(&Iggy::ShellState);
    }
    BossHealth -= 120/(settings & 0x2);

    //defeat(0, 0, 9.0fx/32.0fx, 0);
}

void Iggy::onFireballHit(){
    Player *player;
    player = Game::getPlayer(0);
    recoveryTimer = 20;
    BossHealth -= 1;
    //SND::playExtSFXUnique(SeqArc::SAR_Iggy_BASE::SE_Iggy_2);
    //defeat(0, 0, 9.0fx/32.0fx, 0);
}

void Iggy::defeat(fx32 velX, fx32 velY, fx32 accelY, u8 unk){
    StageEntity::defeat(velX, velY, accelY, unk);

}

void Iggy::defeatMega(fx32 velX, fx32 velY, fx32 accelY){
    StageEntity::defeatMega(velX, velY, accelY);
}

s32 Iggy::onCreate(){
    void* BossAnim = FS::Cache::getFile(2537 - 131);
    void* BossModel = FS::Cache::getFile(2536 - 131);
    void* BossShell = FS::Cache::getFile(2552 - 131);

    modelIggy.create(BossModel, BossAnim, 0, 0, 0);
    modelIggyShell.create(BossShell, 0, 0);


    modelIggy.pushAnimation(IggyAnimation::Wait, 8, FrameCtrl::Looping, 1.0fx, 0);

    switchState(&Iggy::IntroCutScene);



    rotation.y -= 6.0fx,
    cutSceneTimer = 0;
    FightState = 0;
    scale = Vec3(scale.x *120, scale.y *120, scale.z *120);

    collisionMgr.init(this, &bottomSensor, nullptr, &sideSensor); 
    activeCollider.init(this, activeColliderInfo, 0);
    activeCollider.link();

    BossHealth = 360;
    recoveryTimer = 0;
    exitTimer = 0;

    return 1;

}

s32 Iggy::onRender() {

    applyFireballWiggle();
    if(inShell){
        MTX::identity(modelIggyShell.matrix);
        MTX::translate(modelIggyShell.matrix, position);
        MTX::rotate(modelIggyShell.matrix, rotation);
        Game::modelMatrix = modelIggyShell.matrix;
        //Vec3 newScale = Vec3(scale.x / 170, scale.y / 170, scale.z / 170);
        modelIggyShell.render(&scale);
    }
    else{
        MTX::identity(modelIggy.matrix);
        MTX::translate(modelIggy.matrix, position);
        MTX::rotate(modelIggy.matrix, rotation);
        Game::modelMatrix = modelIggy.matrix;
        if (recoveryTimer == 0) {
            modelIggy.render(&scale);
        	NNS_G3dMdlSetMdlFogEnableFlagAll(modelIggy.data,FALSE);

	    } else {
		    NNS_G3dMdlSetMdlFogEnableFlagAll(modelIggy.data,TRUE);
            recoveryTimer--;
            modelIggy.render(&wiggleScale);
	    }
    }



    /*
    NNSG3dResMatData* mat = modelIggy.getMaterialData(0);
    u32 texParams;
    u32 palParams;

    G3D::getTextureParams(modelIggy.texture, currentTex, texParams);
    G3D::getPaletteParams(modelIggy.texture, currentTex, palParams);
    mat->texImageParam = texParams;
    mat->texPlttBase = palParams;
    */

    return true;
    
}

void Iggy::switchState(StateFunction function) {
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

void Iggy::updateState(){
    (this->*updateFunction)();

}

void Iggy::IntroCutScene() {
    Player *player = Game::getPlayer(0); 
    if (updateStep == Func::Init) {
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

    timeCrap++;

    if(timeCrap == 10){
        //SND::playExtSFX(SeqArc::SAR_Iggy_BASE::SE_Iggy_1);
        modelIggy.pushAnimation(IggyAnimation::IntroStart, 8, FrameCtrl::Standard, 1.0fx, 0);

    }
    if(timeCrap == 64){
        modelIggy.pushAnimation(IggyAnimation::Intro, 8, FrameCtrl::Standard, 1.0fx, 0);
        Vec3 firePos3 = Vec3(position.x, position.y +0x10000, position.z + 0x10000);
        Particle::Handler::createParticle(46, firePos3);
        Particle::Handler::createParticle(277, firePos3);


    }

    if(timeCrap == 164){
        switchState(&Iggy::jumpState);
    }
}


void Iggy::VictoryAnimation(){
    Player *player;
    player = Game::getPlayer(0);
	player->actionFlag.bowserJrBeaten	 = true;
}

void Iggy::jumpState(){
    Player *player = Game::getPlayer(0); 
    if (updateStep == Func::Init) {
        timeCrap = -10;
        updateStep = Func::Step(0);
        // Initializing state
        return;
    }

    if (updateStep == Func::Exit) {
        // Exiting state
        return;

    }
    timeCrap++;
    if(timeCrap == 0){
        modelIggy.pushAnimation(IggyAnimation::JumpStart, 6, FrameCtrl::Standard, 1.0fx, 0);
    }
    if(timeCrap == 20){
        velocity.y = 0x7000;
        SND::playSFX(370, 0);
        if(player->position.x < position.x){
            velocity.x = -0x1000;
        }
        else{
            velocity.x = 0x1000;
        }
        modelIggy.pushAnimation(IggyAnimation::Jump, 6, FrameCtrl::Standard, 0.8fx, 0);
    }
    if (timeCrap % 10 == 0 && timeCrap >= 10){
        Particle::Handler::createParticle(115 , Vec3(position.x, position.y, position.z - 10.0fx));
    }
    const CollisionMgrResult result = collisionMgr.collisionResult;

    if( bool(result & CollisionMgrResult::GroundTile) && (timeCrap > 60)){
        velocity = 0;
        Particle::Handler::createParticle(57, Vec3(position.x, position.y, position.z));
        Particle::Handler::createParticle(10 , position);

        modelIggy.pushAnimation(IggyAnimation::JumpEnd, 6, FrameCtrl::Standard, 1.0fx, 0);
        u8 nextState = Stage::getRandom(0, 1);
        if(nextState == 1)
            switchState(&Iggy::fireState);
        else
            switchState(&Iggy::jumpState);

    }



}

void Iggy::spawnFire(s8 speed){
    Vec3 firePos = Vec3(position.x - 0x20000*speed, position.y + 0x10000, position.z + 0x10000);
    if(speed == 1){
        Actor::spawnActor(408, 0x04, &firePos);
            Actor::spawnActor(408, 0x44, &firePos);
            Actor::spawnActor(408, 0x64, &firePos);
        
    }
    else{
        Actor::spawnActor(408, 0x14, &firePos);
            Actor::spawnActor(408, 0x34, &firePos);
            Actor::spawnActor(408, 0x54, &firePos);  
             
        
    }

    Particle::Handler::createParticle(87, firePos);

    //SND::playExtSFXUnique(SeqArc::SAR_Iggy_BASE::SE_Iggy_4);
}




void Iggy::fireState(){
    Player *player = Game::getPlayer(0); 
    s8 speed;
    if(position.x > player->position.x){
        speed = 1;
    }
    else{
        speed = -1;
    }
    Vec3 firePos2 = Vec3(position.x - 0x20000 *speed, position.y + 0x20000, position.z);


    if (updateStep == Func::Init) {
        timeCrap = 0;
        updateStep = Func::Step(0);
        
        // Initializing state
        return;
    }

    if (updateStep == Func::Exit) {
        // Exiting state
        return;

    }
    timeCrap++;
    if(FireBallChainNum == maxFireChain){
        mainStateState = 1;
        Timer = 0;
    }


    if(timeCrap == 10){
            modelIggy.pushAnimation(IggyAnimation::ShootHold, 12, FrameCtrl::Looping, 1.0fx, 0);

    }
    if(timeCrap == 34){
        Particle::Handler::createParticle(149, firePos2);
    }
    if((timeCrap == 40)||(timeCrap == 60)){
            Particle::Handler::createParticle(149, firePos2);
    }


    if(timeCrap == 80){
        Particle::Handler::createParticle(149, firePos2);
        spawnFire(speed);

    }

    if(timeCrap == 60){
        modelIggy.pushAnimation(IggyAnimation::ShootEnd, 2, FrameCtrl::Standard, 1.0fx, 0);


    }
    if(timeCrap == 100){
        u8 nextState = Stage::getRandom(0, 1);
        if(nextState == 1)
            switchState(&Iggy::fireState);
        else
            switchState(&Iggy::jumpState);    
    }
}

void Iggy::shellCharge(){
    Player* player = Game::getPlayer(0);
    const CollisionMgrResult result = collisionMgr.collisionResult;
    if(Timer == 1){
        velocity.x = 0.0fx;
        direction = 1;
    }
    if(Timer == 3){
        inShell = true;
    }
    if (bool(result & CollisionMgrResult::WallAny)){
        direction *= -1;
        velocity.x *= direction;
        SND::playSFX(363, &this->position);
    }
    //rotation.y += velocity.x/2;

    if(Timer < 20){
        velocity.x = 0.1fx *((settings & 0x2) + 1) * Timer*direction;
    }

    if(Timer == 118){
        scale = Vec3(1.0fx, 1.0fx, 1.0fx);
        velocity.x = 0;
        velocity.x = 0;
        //rotation.y = 0;
        inShell = false;
    }
}

void Iggy::MainState(){    
}

void Iggy::ShellState(){
    Player* player = Game::getPlayer(0);
    if (updateStep == Func::Init) {
        timeCrap = -10;
        modelIggy.pushAnimation(0, 6, FrameCtrl::Looping, 1.0fx, 0);
        Particle::Handler::createParticle(0, position);
        Particle::Handler::createParticle(64, position);

        startShellSlide = false;
        velocity.x = 0;
        velocity.y = 0;

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

    if(bool(result & CollisionMgrResult::GroundTile) && !startShellSlide){
        Particle::Handler::createParticle(57, Vec3(position.x, position.y, position.z));
        Particle::Handler::createParticle(10 , position);
        modelIggy.pushAnimation(IggyAnimation::EnterShell, 6, FrameCtrl::Standard, 1.0fx, 0);
        startShellSlide = true;
    }
    if(position.x < *ViewStart + 0x30000){
        direction *= -1;
        velocity.x *= direction;
        position.x += 1.0fx;
    }
    if(position.x > *ViewEnd - 0x30000){
        direction *= -1;
        velocity.x *= direction;
        position.x -= 1.0fx;
    }
    if(startShellSlide){
        timeCrap++;
        rotation.y += velocity.x;
    }
    if(timeCrap == 10){
        inShell = true;
        direction = 1;
    }
    if (bool(result & CollisionMgrResult::WallAny)){
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
        modelIggy.pushAnimation(IggyAnimation::EnterShellOut, 6, FrameCtrl::Standard, 1.0fx, 0);
        velocity.x = 0;
        //rotation.y = 0;
    }
    if(timeCrap == 410){
        switchState(&Iggy::jumpState);    
    }


}

void Iggy::UpdateRotation(){
    Player* player = Game::getPlayer(0);
        if(position.x > player->position.x){
            if(rotation.y > -6.0fx)
                rotation.y -=  0.1fx;
        }
        if(position.x < player->position.x){
           if(rotation.y < -2.0fx)
                rotation.y +=  0.1fx;
        } 
}

bool Iggy::updateMain(){
    Player *player;
    player = Game::getPlayer(0);
    const CollisionMgrResult result = collisionMgr.collisionResult;
    updateState();
    modelIggy.update();


    updateVerticalVelocity(); // updates gravity
   // 
	applyMovement(); // applies velocity
	updateCollisionSensors(); // clamps position due to collision

    UpdateRotation();
    if(BossHealth < 1){
        defeat(0, 0, 9.0fx/32.0fx, 0);
    }


    /*
    u8 IggyState = FightState;
    if(IggyState == 0){
            IntroCutScene();
                return true;      
    }
    if(IggyState == 1){
        MainState();
        return true;      

    }
    if(IggyState == 2){
        if(bool(result & CollisionMgrResult::GroundTile)){
            ShellState();
            return true;      
        }
    }
    */
             

    
    return true;      
}




bool Iggy::updateDefeated(){
    Player *player;
    player = Game::getPlayer(0);
    activeCollider.unlink();
    
    const CollisionMgrResult result = collisionMgr.collisionResult;
    if(!deathAnim){
        modelIggy.pushAnimation(0, 6, FrameCtrl::Standard, 1.0fx, 0);
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
        player->endCutscene();
        activateEvent(63);
    }
    
    
    updateVerticalVelocity(); // updates gravity
    modelIggy.update();
    updateCollisionSensors();
    return true; 

}

bool Iggy::updateDefeatedMega(){
    return true; 
}

s32 Iggy::onDestroy(){
		return 1;
}







    