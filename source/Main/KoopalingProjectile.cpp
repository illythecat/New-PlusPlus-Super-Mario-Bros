#include "KoopalingProjectile.hpp"



void KoopalingProjectile::activeCallback(ActiveCollider& self, ActiveCollider& other){

    KoopalingProjectile* tp = static_cast<KoopalingProjectile*>(self.owner);
    if(other.owner->actorType == ActorType::Player){
        Player* player = static_cast<Player*>(other.owner);
        player->dealDamage(*tp, 0, 4.0fx, PlayerDamageType::Hit);
    }
}

const ActiveColliderInfo KoopalingProjectile::activeColliderInfo = {

    0.0fx, 0.0fx,
    6.0fx, 6.fx,

    CollisionGroup::Entity,
    CollisionFlag::None,
    MAKE_GROUP_MASK(CollisionGroup::Player, CollisionGroup::PlayerSpecial),
    MAKE_FLAG_MASK_EX(CollisionFlag::None),
    0,

    KoopalingProjectile::activeCallback

};
bool KoopalingProjectile::loadResources(){
     return 1;
}

bool KoopalingProjectile::prepareResources(){
    int *world = (int*)(0x02088BFC);

    if(*world == 7){
     FS::Cache::loadFile(2177 - 131, false);
    }
    else if(*world == 1){
     FS::Cache::loadFile(2350 - 131, false);
    }
    else if(*world == 5){
     FS::Cache::loadFile(2411 - 131, false);
    }
    else{
     FS::Cache::loadFile(2185 - 131, false);
    }
     FS::Cache::loadFile(2186 - 131, false);
    return 1;
}

 
s32 KoopalingProjectile::onCreate(){
    int *world = (int*)(0x02088BFC);

    prepareResources();
    void* FireModel;
    if(*world == 7){
        FireModel = FS::Cache::loadFile(2177 - 131, false);
    } 
    else if(*world == 1){
        FireModel = FS::Cache::loadFile(2350 - 131, false);
    } 
    else if(*world == 5){
        FireModel = FS::Cache::loadFile(2411 - 131, false);
    } 
    else{
        FireModel = FS::Cache::loadFile(2185 - 131, false);
    }
    void* FireModelAnm = FS::Cache::loadFile(2186 - 131, false);
    model.create(FireModel, FireModelAnm, 0, 0, 0);
    animctrl.createCtrl(&model.renderObj, FireModelAnm, 0, nullptr);
    model.init(0, FrameCtrl::Looping, 1.0fx, 0);

    fogFlag = false;
	alpha = 0xff;
	renderOffset = {0, 0};
	rotationTranslation = {0, 0};
    homeTimer = 0;
    Particle::Handler::createParticle(87, this->position);
    particalTimer = 0;

    activeCollider.init(this, activeColliderInfo, 0);
    activeCollider.shape = ActiveCollider::Shape::Round;
    activeCollider.link();

    return 1;

}

bool KoopalingProjectile::updateMain(){
    animctrl.update();
    int *world = (int*)(0x02088BFC);

    Player *player;
    player = Game::getPlayer(0);
    u16 speed = (this->settings>>0 & 0xf) *1000;
    u16 direction = (this->settings>>4);

    switch(direction){
        case 0:{
            this->position.x -= speed; 
            break;
            }  
        case 1:{
            this->position.x += speed; 
            this->rotation.z = 8.0fx;
            break;
            }      
        case 3:{
            this->position.x += speed; 
            this->position.y += 0.5fx; 
            this->rotation.z = -7.5fx;
            break;
        }
        case 4:{
            this->position.x -= speed; 
            this->position.y += 0.5fx; 
            this->rotation.z = -0.5fx;
            break;
        }
        case 5:{
            this->position.x += speed; 
            this->position.y -= 0.5fx; 
            this->rotation.z = 7.5fx;

            break;
        }
        case 6:{
            this->position.x -= speed; 
            this->position.y -= 0.5fx; 
            this->rotation.z =  0.5fx;
            break;
        }
        case 7:{
            this->position.x -= speed; 
            this->position.y -= speed; 
            this->rotation.z =  3.0fx;
            break;
        }
        case 8:{
            this->position.x += speed; 
            this->position.y -= speed; 
            this->rotation.z =  5.0fx;
            break;
        }
        case 9:{
            this->position.y -= speed; 
            this->rotation.z =  4.0fx;
            break;
        }

        case 10:{
            homeTimer++;
            if(homeTimer < 180){
                    d.x = this->position.x -  player->position.x;
                     d.y = this->position.y - player->position.y;
                    magnitude = FX_Sqrt(FX_Mul(d.x, d.x) + FX_Mul(d.y, d.y));   
                    magnitude += 0x3000;
            }
                this->position.x -= FX_Div(d.x, magnitude);
                this->position.y -= FX_Div(d.y, magnitude);
                this->rotation.z = 1.0fx + Math::atan2(d.y, d.x); // if using radians
            break;

        }
        
    }

    particalTimer++;
    if(particalTimer == 10){
        if(*world == 1){
            Particle::Handler::createParticle(155, position);
        }
        if(*world == 1){
            Particle::Handler::createParticle(155, position);
        }
        if(*world == 0){
            Particle::Handler::createParticle(149, position);
        }
        particalTimer = 0;
    }

    if(Game::stageID != 0x3 ){
    if((this->position.x < player->position.x - 0x220000)||
        (this->position.x > player->position.x + 0x220000)||
        (this->position.y < player->position.y - 0x220000)||
        (this->position.y > player->position.y + 0x220000)){
    
        this->Base::destroy();
    }
    }

    return true;      
}

s32 KoopalingProjectile::onDestroy(){
		return 1;
}
