#include "BowserHead.hpp"

void BowserHead::activeCallback(ActiveCollider& self, ActiveCollider& other){

    BowserHead* bh = static_cast<BowserHead*>(self.owner);
    if(other.owner->actorType == ActorType::Player){
        Player* player = static_cast<Player*>(other.owner);
    }
}

const ActiveColliderInfo BowserHead::activeColliderInfo = {

    0.0fx, 0.0fx,
    10.0fx, 10.fx,

    CollisionGroup::Entity,
    CollisionFlag::None,
    MAKE_GROUP_MASK(CollisionGroup::Player, CollisionGroup::PlayerSpecial),
    MAKE_FLAG_MASK_EX(CollisionFlag::None),
    0,

    BowserHead::activeCallback

};
bool BowserHead::loadResources(){
     FS::Cache::loadFile(2103 - 131, false);
     FS::Cache::loadFile(2104 - 131, false);
     FS::Cache::loadFile(1531 - 131, false);
     FS::Cache::loadFile(1532 - 131, false);
     FS::Cache::loadFile(1533 - 131, false);
     FS::Cache::loadFile(1534 - 131, false);

     return 1;
}

 
s32 BowserHead::onCreate(){
    void* bhmodel = FS::Cache::loadFile(2104 - 131, false);
    void* bhmodel2 = FS::Cache::loadFile(2103 - 131, false);

    if(this->settings>>0 & 0x1 == 1){
        model.create(bhmodel2, 0, 0);
    }
    else{
        model.create(bhmodel, 0, 0);
    }

    fogFlag = false;
	alpha = 0xff;
	renderOffset = {0, 0};
	rotationTranslation = {0, 0};
    this->scale += {0x1000, 0x1000, 0x1000};
    BowserHead::fireBallTimer = 0;


    activeCollider.init(this, activeColliderInfo, 0);
    activeCollider.shape = ActiveCollider::Shape::Round;
    activeCollider.link();
    
    return 1;

}

bool BowserHead::updateMain(){
    Player *player;
    player = Game::getPlayer(0);

    if((this->position.x < player->position.x - 0x100000)||
        (this->position.x > player->position.x + 0x100000)||
        (this->position.y < player->position.y - 0x100000)||
        (this->position.y > player->position.y + 0x100000)){}
    else{

    BowserHead::fireBallTimer++;

    if(BowserHead::fireBallTimer == 100||BowserHead::fireBallTimer == 110||BowserHead::fireBallTimer == 120){
        Vec3 FireBallParticalSpawnPos;
        FireBallParticalSpawnPos.y = this->position.y;
        FireBallParticalSpawnPos.x = this->position.x;

        Particle::Handler::createParticle(87, FireBallParticalSpawnPos);
        
    }
    if(BowserHead::fireBallTimer == 120){
        if(this->settings>>4 == 0){
            Vec3 FireBallSpawnPos;
            FireBallSpawnPos.y = this->position.y - 0x8000;
            FireBallSpawnPos.x = this->position.x + 0x10000;
            SND::playSFX(360, 0);
            Actor::spawnActor(78, 0x00003, &FireBallSpawnPos);
        }
        if(this->settings>>4 == 1){
            Vec3 FireBallSpawnPos;
            FireBallSpawnPos.y = this->position.y - 0x8000;
            FireBallSpawnPos.x = this->position.x - 0x10000;
            SND::playSFX(360, 0);
            Actor::spawnActor(78, 0x80003, &FireBallSpawnPos);
        }
        if(this->settings>>4 == 2){
            Vec3 FireBallSpawnPos;
            FireBallSpawnPos.y = this->position.y - 0x8000;
            FireBallSpawnPos.x = this->position.x + 0x10000;
            SND::playSFX(360, 0);
            Actor::spawnActor(78, 0x00001, &FireBallSpawnPos);
        }        
        if(this->settings>>4 == 3){
            Vec3 FireBallSpawnPos;
            FireBallSpawnPos.y = this->position.y - 0x8000;
            FireBallSpawnPos.x = this->position.x - 0x10000;
            SND::playSFX(360, 0);
            Actor::spawnActor(78, 0x80001, &FireBallSpawnPos);
        }
        BowserHead::fireBallTimer = 0;
    }
    }

    return true;      
}

s32 BowserHead::onDestroy(){
		return 1;
}
