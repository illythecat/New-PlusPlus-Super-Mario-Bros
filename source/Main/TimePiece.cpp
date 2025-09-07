#include "TimePiece.hpp"






void TimePiece::activeCallback(ActiveCollider& self, ActiveCollider& other){

    TimePiece* tp = static_cast<TimePiece*>(self.owner);
    if(other.owner->actorType == ActorType::Player){
        Player* player = static_cast<Player*>(other.owner);
    }
    tp->addTime();
}

const ActiveColliderInfo TimePiece::activeColliderInfo = {

    0.0fx, 0.0fx,
    10.0fx, 10.fx,

    CollisionGroup::Entity,
    CollisionFlag::None,
    MAKE_GROUP_MASK(CollisionGroup::Player, CollisionGroup::PlayerSpecial),
    MAKE_FLAG_MASK_EX(CollisionFlag::None),
    0,

    TimePiece::activeCallback

};
bool TimePiece::loadResources(){
     FS::Cache::loadFile(2125 - 131, false);
     FS::Cache::loadFile(2126 - 131, false);

     return 1;
}

 
s32 TimePiece::onCreate(){
    void* tpmodel = FS::Cache::loadFile(2125 - 131, false);
    void* tpmodel2 = FS::Cache::loadFile(2126 - 131, false);

    if(this->settings>>0 == 1){
        model.create(tpmodel2, 0, 0);
    }
    else{
        model.create(tpmodel, 0, 0);
		
    }

    fogFlag = false;
	alpha = 0xff;
	renderOffset = {0, 0};
	rotationTranslation = {0, 0};

    if(this->settings>>0 == 1){
        this->scale.x = 0xe00;
        this->scale.y = 0xe00;
        this->scale.z = 0xe00;
    }
    else{     
        this->scale.x = 0xc00;
        this->scale.y = 0xc00;
        this->scale.z = 0xc00;
    }
    this->position.x += 0x8000;
    this->position.y -= 0x8000;


    activeCollider.init(this, activeColliderInfo, 0);
    activeCollider.shape = ActiveCollider::Shape::Round;
    activeCollider.link();

    return 1;

}

void TimePiece::addTime(){
    SND::playSFX(17, 0);
    Particle::Handler::createParticle(107, position);
    if(this->settings>>0 == 1){
        AddSecondsToTimerUwU(100);
    }
    else{
        AddSecondsToTimerUwU(10);    
    }
    Base::destroy();
}

bool TimePiece::updateMain(){
    this->rotation.y += 0x500;
    return true;      
}

s32 TimePiece::onDestroy(){
		return 1;
}
