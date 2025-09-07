#include "Spike.hpp"

void Spike::activeCallback(ActiveCollider& self, ActiveCollider& other){

    Spike* spike = static_cast<Spike*>(self.owner);
    if(other.owner->actorType == ActorType::Player){
        Player* player = static_cast<Player*>(other.owner);
        player->dealDamage(*spike, 0, 4.0fx, PlayerDamageType::Hit);

    }
    
    
}

const LineSensorH Spike::bottomSensor = {
    -1.5fx, // Start position from the origin (left)
    1.5fx, // End position from the origin (right)
    0.0fx // Y Offset (negative = down)
};

const LineSensorH Spike::topSensor = {
    -9.5fx, // Start position from the origin (left)
    9.5fx, // End position from the origin (right)
    12.0fx // Y Offset (negative = down)
};


const LineSensorV Spike::sideSensor = {
    0.5fx, // Start position from the origin (top)
    4.5fx, // End position from the origin (bottom)
    5.5fx // X offset (negative = left)
};	

const ActiveColliderInfo Spike::activeColliderInfo = {

    0.0fx, 9.0fx,
    9.0fx, 9.0fx,

    CollisionGroup::Entity,
    CollisionFlag::None,
    MAKE_GROUP_MASK(CollisionGroup::Player, CollisionGroup::PlayerSpecial,CollisionGroup::Entity,  CollisionGroup::Fireball),
    MAKE_FLAG_MASK_EX(CollisionFlag::None),
    0,

    Spike::damagePlayerCallback

};

bool Spike::loadResources(){
     FS::Cache::loadFile(2430 - 131, false);
     FS::Cache::loadFile(2431 - 131, false);
     FS::Cache::loadFile(2432 - 131, false);
     FS::Cache::loadFile(2433 - 131, false);
     FS::Cache::loadFile(1504 - 131, false);

     return 1;
}

void Spike::damagePlayer(ActiveCollider& self, Player& player){
    auto stomped = updatePlayerStomp(self, 9.0fx/32.0fx, 1, false);
    if(stomped == PlayerStompType::MiniStomp)
        return;

    if(stomped == PlayerStompType::Stomp){
        setStompCollision(player);
        return;
    }
    player.dealDamage(*this, 0, 4.0fx, PlayerDamageType::Hit);

}

void Spike::onStomped(){
    activeCollider.unlink();
    defeat(0, 0, -9.0fx/32.0fx, 0);
}

void Spike::onFireballHit(){
    activeCollider.unlink();
    Actor::spawnActor(66, 0x4,  &this->position);
    defeat(0, 0, -9.0fx/32.0fx, 0);
}

void Spike::defeat(fx32 velX, fx32 velY, fx32 accelY, u8 unk){
    Particle::Handler::createParticle(114, position);
    StageEntity::defeat(velX, velY, accelY, unk);
}

void Spike::defeatMega(fx32 velX, fx32 velY, fx32 accelY){
    Particle::Handler::createParticle(114, position);
    StageEntity::defeatMega(velX, velY, accelY);
}

s32 Spike::onCreate(){
	void* modelFile = FS::Cache::getFile(SpikeID);
	void* modelFileHead = FS::Cache::getFile(SpikeEyesID);
	void* modelFileHair = FS::Cache::getFile(HairID);
    void* anmFile = FS::Cache::getFile(SpikeAnimID);
	void* modelFileBall = FS::Cache::getFile(BallID);


	modelSpike.create(modelFile, anmFile, 0, 0, 0);
	modelEyes.create(modelFileHead, 0, 0);
	modelHair.create(modelFileHair, 0, 0);
    modelBall.create(modelFileBall, 0, 0);
    switchState(&Spike::IdleState);



    //rotation.y -= 5.0fx,

    fogFlag = false;
	alpha = 31;
	renderOffset = {0, 0};
	rotationTranslation = {0, 0};
    rotation.y =-2.0fx;
    renderBall = false;


	collisionMgr.init(this, &bottomSensor, nullptr, &sideSensor); 
    activeCollider.init(this, activeColliderInfo, 0);
    activeCollider.link();
    return 1;

}

s32 Spike::onRender() {
    MTX::identity(modelSpike.matrix);
    MTX::translate(modelSpike.matrix, position);
    MTX::rotate(modelSpike.matrix, rotation);
    Game::modelMatrix = modelSpike.matrix;
    modelSpike.render(&scale);
    modelSpike.update();

    if(settings & 0x1){
        rotation.y = 2.0fx;
        modelSpike.getNodeMatrix(8, &modelEyes.matrix);
        MTX::rotateZ(modelEyes.matrix, 4.0fx);
        MTX::rotateX(modelEyes.matrix, -4.0fx);

        MTX::rotateY(modelEyes.matrix, -1.8fx);
        MTX::translate(modelEyes.matrix, Vec3(0x2000, -0x11000, 0));
        Vec3 headScale = Vec3(1.0fx, 1.0fx, 1.0fx);
        modelEyes.render(&headScale);

        modelSpike.getNodeMatrix(8, &modelHair.matrix);
        MTX::rotateZ(modelHair.matrix, 6.0fx);
        MTX::rotateX(modelHair.matrix, -4.0fx);
        MTX::rotateY(modelHair.matrix, 5.0fx);
        MTX::translate(modelHair.matrix, Vec3(0x8000, -0x14000, 0));
        Vec3 hairScale = Vec3(1.2fx, 1.2fx, 1.2fx);
        modelHair.render(&hairScale);
        if(renderBall){
            modelSpike.getNodeMatrix(4, &modelBall.matrix);
            modelSpike.getNodePosition(4, BallPos);
            MTX::translate(modelBall.matrix, Vec3(-0x6000, -0x12000, 0));
            modelBall.render(&BallScale);      
        }
    }
    else{
        modelSpike.getNodeMatrix(8, &modelEyes.matrix);
        MTX::rotateZ(modelEyes.matrix, 4.0fx);
        MTX::rotateX(modelEyes.matrix, -4.0fx);

        MTX::rotateY(modelEyes.matrix, 1.0fx);
        MTX::translate(modelEyes.matrix, Vec3(0x3000, -0x11000, 0));
        Vec3 headScale = Vec3(1.0fx, 1.0fx, 1.0fx);
        modelEyes.render(&headScale);

        modelSpike.getNodeMatrix(8, &modelHair.matrix);
        MTX::rotateZ(modelHair.matrix, 6.0fx);
        MTX::rotateX(modelHair.matrix, -4.0fx);
        MTX::rotateY(modelHair.matrix, -1.0fx);
        MTX::translate(modelHair.matrix, Vec3(-0x8000, -0x14000, 0));
        Vec3 hairScale = Vec3(1.2fx, 1.2fx, 1.2fx);
        modelHair.render(&hairScale);
        if(renderBall){
            modelSpike.getNodeMatrix(4, &modelBall.matrix);
            modelSpike.getNodePosition(4, BallPos);
            MTX::rotateZ(modelBall.matrix, 6.0fx);
            MTX::rotateX(modelBall.matrix, -4.0fx);
            MTX::rotateY(modelBall.matrix, -1.0fx);
            MTX::translate(modelBall.matrix, Vec3(0x12000, 0, 0));
            modelBall.render(&BallScale);      
        }
    }
    return true;
    
}



void Spike::switchState(StateFunction function) {
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

void Spike::updateState(){
    (this->*updateFunction)();

}

void Spike::liftState(){
    Player *player = Game::getPlayer(0); 
    if (updateStep == Func::Init) {
        modelSpike.pushAnimation(1, 8, FrameCtrl::Standard, 0.2fx, 0);
        timer = 0;
        BallScale = Vec3(0.5fx, 0.5fx, 0.5fx);
        SND::playSFX(SeqArc::SAR_ENEMY_GABON::SE_EMY_YUKIDAMA_APP);
        
        updateStep = Func::Step(0);


        return;
    }

    if (updateStep == Func::Exit) {
        spikeBallHurtBox = true;
        return;
    }
    timer++;
    if(timer > 19){
        renderBall = true;

        BallScale += Vec3(0.021fx, 0.021fx, 0.021fx);
    }
    if(timer == 40){
        switchState(&Spike::holdState);
    }
}

void Spike::holdState(){
    Player *player = Game::getPlayer(0); 
    if (updateStep == Func::Init) {
        modelSpike.pushAnimation(3, 8, FrameCtrl::Looping, 0.3fx, 0);
        scale = Vec3(0.8fx, 0.8fx, 0.8fx);
        timer = 0;
        updateStep = Func::Step(0);

        return;
    }

    if (updateStep == Func::Exit) {
        return;
    }
    timer++;
    if(timer == 60){
        switchState(&Spike::throughState);
    }
}

void Spike::throughState(){
    Player *player = Game::getPlayer(0); 
    if (updateStep == Func::Init) {
        modelSpike.pushAnimation(2, 8, FrameCtrl::Standard, 0.3fx, 0);
        timer = 0;
        SND::playSFX(SeqArc::SAR_ENEMY_GABON::SE_EMY_GABON_THROW);
        updateStep = Func::Step(0);

        return;
    }

    if (updateStep == Func::Exit) {
        scale = Vec3(0.9fx, 0.9fx, 0.9fx);

        return;
    }
    timer++;
    if(timer == 10){
        renderBall = false;
        spikeBallHurtBox = false;
        ball = Actor::spawnActor(103, 0x0700001, &BallPos);
        if(settings & 0x1)
            ball->velocity.x = 2.0fx;
        else
            ball->velocity.x = -2.0fx;
        ball->velocity.y = 1.5fx;


    }
    if(timer == 60){
        switchState(&Spike::IdleState);
    }
}

void Spike::IdleState(){
    Player *player = Game::getPlayer(0); 
    if (updateStep == Func::Init) {
        modelSpike.pushAnimation(0, 4, FrameCtrl::Looping, 0.3fx, 0);
        timer = 0;
        spikeBallHurtBox = false;
        updateStep = Func::Step(0);
        return;
    }

    if (updateStep == Func::Exit) {
        return;
    }

    timer++;
    if(timer == 3){
        scale = Vec3(1.0fx, 1.0fx, 1.0fx);
    }
    if(timer == 120){
        switchState(&Spike::liftState);
    }

}

bool Spike::updateMain(){
    Player *player = Game::getPlayer(0);

    
    Spike::updateState();

    updateVerticalVelocity(); // updates gravity
    applyMovement(); // applies velocity

    updateCollisionSensors(); // clamps position due to collision

    if(spikeBallHurtBox){
            ActiveColliderInfo activeColliderInfo2 = {
            0.0fx, 28.0fx,
            8.0fx, 8.0fx,
            CollisionGroup::Entity,
            CollisionFlag::None,
            MAKE_GROUP_MASK(CollisionGroup::Player, CollisionGroup::PlayerSpecial, CollisionGroup::Entity),
            MAKE_FLAG_MASK_EX(CollisionFlag::None),
            0,
            activeCallback
        };
        activeCollider2.init(this, activeColliderInfo2, 0);
        activeCollider2.shape = ActiveCollider::Shape::Round;
        activeCollider2.link();
    }
    else{
        activeCollider2.unlink();
    }
    return true;      
}

bool Spike::updateDefeated(){
    updateVerticalVelocity(); // updates gravity
	applyMovement(); // applies velocity
    return true; 
}

bool Spike::updateDefeatedMega(){
    alpha = 0;
    updateVerticalVelocity(); // updates gravity
	applyMovement(); // applies velocity
    return true; 
}

s32 Spike::onDestroy(){
		return 1;
}







    