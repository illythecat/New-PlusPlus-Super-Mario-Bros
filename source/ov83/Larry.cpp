#include "Larry.hpp"
#include "main/SFX.hpp"

ncp_repl(0x020bec0c, 0, "BX    LR")


void Larry::activeCallback(ActiveCollider& self, ActiveCollider& other){

    Larry* cb = static_cast<Larry*>(self.owner);
    if(other.owner->actorType == ActorType::Player){
        Player* player = static_cast<Player*>(other.owner);
    }
}

const LineSensorH Larry::bottomSensor = {
    -8.5fx, // Start position from the origin (left)
    8.5fx, // End position from the origin (right)
    0.0fx // Y Offset (negative = down)
};

const LineSensorV Larry::sideSensor = {
    16.0fx, // Start position from the origin (top)f
    2.0fx, // End position from the origin (bottom)
    12.5fx // X offset (negative = left)
};	

const ActiveColliderInfo Larry::activeColliderInfo = {

    0.0fx, 12.0fx,
    12.0fx, 12.0fx,

    CollisionGroup::Entity,
    CollisionFlag::None,
    MAKE_GROUP_MASK(CollisionGroup::Player, CollisionGroup::PlayerSpecial, CollisionGroup::Fireball),
    MAKE_FLAG_MASK_EX(CollisionFlag::None),
    0,

    Larry::damagePlayerCallback

};

bool Larry::loadResources(){
     FS::Cache::loadFile(2183 - 131, false);
     FS::Cache::loadFile(2182 - 131, false);
     FS::Cache::loadFile(2184 - 131, false);


     return 1;
}

void Larry::damagePlayer(ActiveCollider& self, Player& player){
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

void Larry::onStomped(){
    Player *player;
    player = Game::getPlayer(0);
    if(BossHealth > 0){
        switchState(&Larry::ShellState);
    }
    BossHealth -= 120;
    


    //defeat(0, 0, 9.0fx/32.0fx, 0);
}

void Larry::onGroundPound(){
    Player *player;
    player = Game::getPlayer(0);
    if(BossHealth > 0){
        switchState(&Larry::ShellState);
    }
    BossHealth -= 120;

    //defeat(0, 0, 9.0fx/32.0fx, 0);
}

void Larry::onFireballHit(){
    Player *player;
    player = Game::getPlayer(0);
    recoveryTimer = 20;
    BossHealth -= 1;
    SND::playExtSFXUnique(SeqArc::SAR_LARRY_BASE::SE_LARRY_2);
    //defeat(0, 0, 9.0fx/32.0fx, 0);
}

void Larry::defeat(fx32 velX, fx32 velY, fx32 accelY, u8 unk){
    StageEntity::defeat(velX, velY, accelY, unk);

}

void Larry::defeatMega(fx32 velX, fx32 velY, fx32 accelY){
    StageEntity::defeatMega(velX, velY, accelY);
}

s32 Larry::onCreate(){
    void* BossAnim = FS::Cache::getFile(2182 - 131);
    void* BossModel = FS::Cache::getFile(2183 - 131);
    void* BossShell = FS::Cache::getFile(2184 - 131);

    modelLarry.create(BossModel, BossAnim, 0, 0, 0);
    modelLarryShell.create(BossShell, 0, 0, 0, 0);


    modelLarry.pushAnimation(LarryAnimation::_wait, 8, FrameCtrl::Looping, 1.0fx, 0);

    switchState(&Larry::IntroCutScene);



    rotation.y -= 6.0fx,
    cutSceneTimer = 0;
    FightState = 0;
    scale = Vec3(scale.x *120, scale.y *120, scale.z *120);

    collisionMgr.init(this, &bottomSensor, nullptr, &sideSensor); 
    activeCollider.init(this, activeColliderInfo, 0);
    activeCollider.link();

    BossHealth = 350;
    recoveryTimer = 0;
    exitTimer = 0;

    return 1;

}

s32 Larry::onRender() {

    applyFireballWiggle();
    if(inShell){
        MTX::identity(modelLarryShell.matrix);
        MTX::translate(modelLarryShell.matrix, position);
        MTX::rotate(modelLarryShell.matrix, rotation);
        Game::modelMatrix = modelLarryShell.matrix;
        Vec3 newScale = Vec3(scale.x / 170, scale.y / 170, scale.z / 170);
        modelLarryShell.render(&newScale);
    }
    else{
        MTX::identity(modelLarry.matrix);
        MTX::translate(modelLarry.matrix, position);
        MTX::rotate(modelLarry.matrix, rotation);
        Game::modelMatrix = modelLarry.matrix;
        if (recoveryTimer == 0) {
            modelLarry.render(&scale);
        	NNS_G3dMdlSetMdlFogEnableFlagAll(modelLarry.data,FALSE);

	    } else {
		    NNS_G3dMdlSetMdlFogEnableFlagAll(modelLarry.data,TRUE);
            recoveryTimer--;
            modelLarry.render(&wiggleScale);
	    }
    }



    /*
    NNSG3dResMatData* mat = modelLarry.getMaterialData(0);
    u32 texParams;
    u32 palParams;

    G3D::getTextureParams(modelLarry.texture, currentTex, texParams);
    G3D::getPaletteParams(modelLarry.texture, currentTex, palParams);
    mat->texImageParam = texParams;
    mat->texPlttBase = palParams;
    */

    return true;
    
}

void Larry::switchState(StateFunction function) {
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

void Larry::updateState(){
    (this->*updateFunction)();

}

void Larry::IntroCutScene() {
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
        SND::playExtSFX(SeqArc::SAR_LARRY_BASE::SE_LARRY_1);
        modelLarry.pushAnimation(LarryAnimation::_Shocked, 8, FrameCtrl::Standard, 1.0fx, 0);

    }
    if(timeCrap == 60){
        modelLarry.pushAnimation(LarryAnimation::_Suprised, 8, FrameCtrl::Standard, 1.0fx, 0);
        Vec3 firePos3 = Vec3(position.x, position.y +0x10000, position.z + 0x10000);
        Particle::Handler::createParticle(46, firePos3);
        Particle::Handler::createParticle(277, firePos3);


    }

    if(timeCrap == 160){
        switchState(&Larry::jumpState);
    }
}


void Larry::VictoryAnimation(){
    Player *player;
    player = Game::getPlayer(0);
	player->actionFlag.bowserJrBeaten	 = true;
}

void Larry::jumpState(){
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
        modelLarry.pushAnimation(LarryAnimation::_BeginJump, 6, FrameCtrl::Standard, 1.0fx, 0);
    }
    if(timeCrap == 12){
        velocity.y = 0x7000;
        SND::playSFX(370, 0);
        if(player->position.x < position.x){
            velocity.x = -0x1000;
        }
        else{
            velocity.x = 0x1000;
        }
        modelLarry.pushAnimation(LarryAnimation::_Jump1, 6, FrameCtrl::Standard, 0.8fx, 0);
    }
    if (timeCrap % 10 == 0 && timeCrap >= 10){
        Particle::Handler::createParticle(115 , Vec3(position.x, position.y, position.z - 10.0fx));
    }
    const CollisionMgrResult result = collisionMgr.collisionResult;

    if( bool(result & CollisionMgrResult::GroundTile) && (timeCrap > 60)){
        velocity = 0;
        Particle::Handler::createParticle(57, Vec3(position.x, position.y, position.z));
        Particle::Handler::createParticle(10 , position);

        modelLarry.pushAnimation(LarryAnimation::_JumpLand, 6, FrameCtrl::Standard, 1.0fx, 0);
        u8 nextState = Stage::getRandom(0, 1);
        if(nextState == 1)
            switchState(&Larry::fireState);
        else
            switchState(&Larry::jumpState);

    }



}

void Larry::spawnFire(s8 speed){
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

    SND::playExtSFXUnique(SeqArc::SAR_LARRY_BASE::SE_LARRY_4);
}

void Larry::fireState(){
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
        modelLarry.pushAnimation(LarryAnimation::_throw_st, 6, FrameCtrl::Standard, 1.0fx, 0);
    }

    if(timeCrap == 40){
        modelLarry.pushAnimation(LarryAnimation::_throw_st_wait, 6, FrameCtrl::Looping, 1.0fx, 0);
        Particle::Handler::createParticle(149, firePos2);
    }
    if((timeCrap == 60)||(timeCrap == 80)){
            Particle::Handler::createParticle(149, firePos2);
    }


    if(timeCrap == 105){
        modelLarry.pushAnimation(LarryAnimation::_throw_end, 6, FrameCtrl::Standard, 1.0fx, 0);
        Particle::Handler::createParticle(149, firePos2);
    }

    if(timeCrap == 125){
        modelLarry.pushAnimation(LarryAnimation::_throw_end_wait, 6, FrameCtrl::Looping, 1.0fx, 0);


        spawnFire(speed);
    }
    if(timeCrap == 200){
        u8 nextState = Stage::getRandom(0, 1);
        if(nextState == 1)
            switchState(&Larry::fireState);
        else
            switchState(&Larry::jumpState);    
    }
}

void Larry::shellCharge(){
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

void Larry::MainState(){    
}

void Larry::ShellState(){
    Player* player = Game::getPlayer(0);
    if (updateStep == Func::Init) {
        timeCrap = -10;
        modelLarry.pushAnimation(LarryAnimation::_Damaged, 6, FrameCtrl::Looping, 1.0fx, 0);
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
        modelLarry.pushAnimation(LarryAnimation::_enterShell, 6, FrameCtrl::Standard, 1.0fx, 0);
        startShellSlide = true;
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
        modelLarry.pushAnimation(LarryAnimation::_exitShell, 6, FrameCtrl::Standard, 1.0fx, 0);
        velocity.x = 0;
        //rotation.y = 0;
    }
    if(timeCrap == 420){
        switchState(&Larry::jumpState);    
    }


}

void Larry::UpdateRotation(){
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

bool Larry::updateMain(){
    Player *player;
    player = Game::getPlayer(0);
    const CollisionMgrResult result = collisionMgr.collisionResult;
    updateState();
    modelLarry.update();


    updateVerticalVelocity(); // updates gravity
   // 
	applyMovement(); // applies velocity
	updateCollisionSensors(); // clamps position due to collision

    UpdateRotation();
    if(BossHealth < 1){
        defeat(0, 0, 9.0fx/32.0fx, 0);
    }


    /*
    u8 LarryState = FightState;
    if(LarryState == 0){
            IntroCutScene();
                return true;      
    }
    if(LarryState == 1){
        MainState();
        return true;      

    }
    if(LarryState == 2){
        if(bool(result & CollisionMgrResult::GroundTile)){
            ShellState();
            return true;      
        }
    }
    */
             

    
    return true;      
}




bool Larry::updateDefeated(){
    Player *player;
    player = Game::getPlayer(0);
    activeCollider.unlink();
    
    const CollisionMgrResult result = collisionMgr.collisionResult;
    if(!deathAnim){
        modelLarry.pushAnimation(LarryAnimation::_Defeated, 6, FrameCtrl::Standard, 1.0fx, 0);
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
    modelLarry.update();
    updateCollisionSensors();
    return true; 

}

bool Larry::updateDefeatedMega(){
    return true; 
}

s32 Larry::onDestroy(){
		return 1;
}







    