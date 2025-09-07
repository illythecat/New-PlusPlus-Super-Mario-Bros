#include "FlyGuys.hpp"

void FlyGuys::activeCallback(ActiveCollider& self, ActiveCollider& other){

    FlyGuys* guy = static_cast<FlyGuys*>(self.owner);
    if(other.owner->actorType == ActorType::Player){
        Player* player = static_cast<Player*>(other.owner);
    }
    
    
}

const ActiveColliderInfo FlyGuys::activeColliderInfo = {

    0.0fx, 7.0fx,
    7.0fx, 7.0fx,

    CollisionGroup::Entity,
    CollisionFlag::None,
    MAKE_GROUP_MASK(CollisionGroup::Player, CollisionGroup::PlayerSpecial, CollisionGroup::Entity, CollisionGroup::Fireball),
    MAKE_FLAG_MASK_EX(CollisionFlag::None),
    0,

    FlyGuys::damagePlayerCallback

};

bool FlyGuys::loadResources(){
     FS::Cache::loadFile(2146 - 131, false);
     FS::Cache::loadFile(2380 - 131, false);
     FS::Cache::loadFile(2381 - 131, false);
     FS::Cache::loadFile(2382 - 131, false);
     FS::Cache::loadFile(2365 - 131, false);
     FS::Cache::loadFile(2366 - 131, false);
     FS::Cache::loadFile(2367 - 131, false);

     return 1;
}

void FlyGuys::damagePlayer(ActiveCollider& self, Player& player){
    auto stomped = updatePlayerStomp(self, 9.0fx/32.0fx, 1, false);
    if(stomped == PlayerStompType::MiniStomp)
        return;

    if(stomped == PlayerStompType::Stomp){
        setStompCollision(player);
        return;
    }
    player.dealDamage(*this, 0, 4.0fx, PlayerDamageType::Hit);
}

void FlyGuys::onStomped(){
    Player* player = Game::getPlayer(0);

    defeat(player->velocity.x*0.5, 0, 9.0fx/32.0fx, 0);
}

void FlyGuys::onFireballHit(){
    
}

void FlyGuys::defeat(fx32 velX, fx32 velY, fx32 accelY, u8 unk){
    StageEntity::defeat(velX, velY, -accelY, unk);
}

void FlyGuys::defeatMega(fx32 velX, fx32 velY, fx32 accelY){
    StageEntity::defeatMega(velX, velY, accelY);
}
 
s32 FlyGuys::onCreate(){
    void* guyanim = FS::Cache::loadFile(2146 - 131, false);



    if((this->settings>>0 & 0xf) == 1){
        void* guymodelb = FS::Cache::loadFile(2381 - 131, false);
        model.create(guymodelb, guyanim, 0, 0, 0);
    }
    else if((this->settings>>0 & 0xf) == 2){
        void* guymodely = FS::Cache::loadFile(2382 - 131, false);
        model.create(guymodely, guyanim, 0, 0, 0);
    }
    else if((this->settings>>0 & 0xf) == 3){
        void* guymodely = FS::Cache::loadFile(2365 - 131, false);
        model.create(guymodely, guyanim, 0, 0, 0);
    }
    else if((this->settings>>0 & 0xf) == 4){
        void* guymodely = FS::Cache::loadFile(2366 - 131, false);
        model.create(guymodely, guyanim, 0, 0, 0);
    }
    else if((this->settings>>0 & 0xf) == 5){
        void* guymodely = FS::Cache::loadFile(2367 - 131, false);
        model.create(guymodely, guyanim, 0, 0, 0);
    }
    else{
        void* guymodelr = FS::Cache::loadFile(2380 - 131, false);
        model.create(guymodelr, guyanim, 0, 0, 0);
    }

    model.init(0, FrameCtrl::Looping, 1.0fx, 0);
    position.x += 0x8000;
    timeCrap = 0;

    fogFlag = false;
	alpha = 31;
	renderOffset = {0, 0};
	rotationTranslation = {0, 0};

    activeCollider.init(this, activeColliderInfo, 0);
    activeCollider.link();

    return 1;

}

void FlyGuys::UpdateRotation(){
    Player* player = Game::getPlayer(0);
    if(position.x > player->position.x){
        if(rotation.y > -2.0fx)
            rotation.y -=  0.2fx;
    }
    if(position.x < player->position.x){
        if(rotation.y < 2.0fx)
            rotation.y +=  0.2fx;
    }
}

void FlyGuys::updateTrigMovment(){
    s16 HorzAmplitude  = settings>>8 & 0xf;
    s16 VertAmplitude  = settings>>12 & 0xf;
    s16 Speed          = settings>>16 & 0xf;
    s16 Period         = settings>>20 & 0xf;
    s16 OffsetH        = settings>>24 & 0xf;
    s16 OffsetV        = settings>>28 & 0xf;
    timeCrap    += Period*180;
    position.x += HorzAmplitude*Math::cos(Speed*timeCrap + (OffsetH*1.0fx));
    position.y += VertAmplitude*Math::sin(Speed*timeCrap + (OffsetV*1.0fx));
}

bool FlyGuys::updateMain(){
    Player *player;
    player = Game::getPlayer(0);
    updateAnimation();
    UpdateRotation();
    if(settings>>4 & 0x1){
        updateTrigMovment();
    }
    return true;      
}

bool FlyGuys::updateDefeated(){
    updateVerticalVelocity(); // updates gravity
	applyMovement(); // applies velocity
    updateAnimation();
    rotation.x += 0.3fx;

    return true; 
}

bool FlyGuys::updateDefeatedMega(){
    Particle::Handler::createParticle(114, this->position);
    this->Base::destroy();
    return true; 
}

s32 FlyGuys::onDestroy(){
		return 1;
}







    