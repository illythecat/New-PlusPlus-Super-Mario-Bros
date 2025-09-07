#include "CannonBall.hpp"

void CannonBall::activeCallback(ActiveCollider& self, ActiveCollider& other){

    CannonBall* cb = static_cast<CannonBall*>(self.owner);
    if(other.owner->actorType == ActorType::Player){
        Player* player = static_cast<Player*>(other.owner);
    }
    
    
}

const ActiveColliderInfo CannonBall::activeColliderInfo = {

    0.0fx, 0.0fx,
    7.0fx, 7.0fx,

    CollisionGroup::Entity,
    CollisionFlag::None,
    MAKE_GROUP_MASK(CollisionGroup::Player, CollisionGroup::PlayerSpecial, CollisionGroup::Entity, CollisionGroup::Fireball),
    MAKE_FLAG_MASK_EX(CollisionFlag::None),
    0,

    CannonBall::damagePlayerCallback

};

bool CannonBall::loadResources(){
     FS::Cache::loadFile(2137 - 131, false);
     return 1;
}

void CannonBall::damagePlayer(ActiveCollider& self, Player& player){
    auto stomped = updatePlayerStomp(self, 9.0fx/32.0fx, 1, false);
    if(stomped == PlayerStompType::MiniStomp)
        return;

    if(stomped == PlayerStompType::Stomp){
        setStompCollision(player);
        return;
    }
    player.dealDamage(*this, 0, 4.0fx, PlayerDamageType::Hit);
}

void CannonBall::onStomped(){
    StageEntity::defeat(0, 0.0fx, -9.0fx/32.0fx, 0);
    //defeat(0, 0, 9.0fx/32.0fx, 0);
}

void CannonBall::onFireballHit(){
    
}

void CannonBall::defeat(fx32 velX, fx32 velY, fx32 accelY, u8 unk){
    StageEntity::defeat(velX, velY, accelY, unk);
}

void CannonBall::defeatMega(fx32 velX, fx32 velY, fx32 accelY){
    StageEntity::defeatMega(velX, velY, accelY);
}
 
s32 CannonBall::onCreate(){
    void* cbModel = FS::Cache::loadFile(2137 - 131, false);

    model.create(cbModel, 0, 0);


    fogFlag = false;
	alpha = 31;
	renderOffset = {0, 0};
	rotationTranslation = {0, 0};

    activeCollider.init(this, activeColliderInfo, 0);
    activeCollider.link();

    return 1;

}

bool CannonBall::updateMain(){

    Player *player;
    player = Game::getPlayer(0);
    u16 speed = (this->settings>>0 & 0xf) *1000 ;
    u16 direction = (this->settings>>4);

    switch(direction){
        case 0:{this->position.x += speed; break;}  
        case 1:{this->position.y += speed; break;}
        case 2:{this->position.x -= speed; break;} 
        case 3:{this->position.y -= speed; break;}
    }

    if((this->position.x < player->position.x - 0x200000)||
        (this->position.x > player->position.x + 0x200000)||
        (this->position.y < player->position.y - 0x200000)||
        (this->position.y > player->position.y + 0x200000)){
    
        this->Base::destroy();
    }

    return true;      
}

bool CannonBall::updateDefeated(){
    //this->Base::destroy();
    updateVerticalVelocity(); // updates gravity
	applyMovement(); // applies velocity
    rotation = VecFx16(0, 0, 0);
    return true; 
}

bool CannonBall::updateDefeatedMega(){
    Particle::Handler::createParticle(114, this->position);
    this->Base::destroy();
    return true; 
}

s32 CannonBall::onDestroy(){
		return 1;
}







    