#include "BigCoin.hpp"

bool add10coin = 0;
bool add30coin = 0;
bool add50coin = 0;
bool add100coin = 0;

void checkCoin(BigCoin* coin){
    if(coin->settings>>0 == 0){
        add10coin = 1;
    }
    if(coin->settings>>0 == 1){
        add30coin = 1;
    }
    if(coin->settings>>0 == 2){
        add50coin = 1;
    }
    if(coin->settings>>0 == 3){
        add100coin = 1;
    }

}


void BigCoin::activeCallback(ActiveCollider& self, ActiveCollider& other){

    BigCoin* coin = static_cast<BigCoin*>(self.owner);
    if(other.owner->actorType == ActorType::Player){
        Player* player = static_cast<Player*>(other.owner);
    }
    checkCoin(coin);
}

const ActiveColliderInfo BigCoin::activeColliderInfo = {

    0.0fx, 0.0fx,
    14.0fx, 14.fx,

    CollisionGroup::Entity,
    CollisionFlag::None,
    MAKE_GROUP_MASK(CollisionGroup::Player, CollisionGroup::PlayerSpecial),
    MAKE_FLAG_MASK_EX(CollisionFlag::None),
    0,

    BigCoin::activeCallback

};
bool BigCoin::loadResources(){
     FS::Cache::loadFile(2138 - 131, false);
     FS::Cache::loadFile(2139 - 131, false);
     FS::Cache::loadFile(2140 - 131, false);
     FS::Cache::loadFile(2141 - 131, false);

     return 1;
}

 
s32 BigCoin::onCreate(){
    void* coinmodel = FS::Cache::loadFile((2138 + this->settings>>0) - 131, false);

    model.create(coinmodel, 0, 0);

    this->scale.x = 0x2000;
    this->scale.y = 0x2000;
    this->scale.z = 0x2000;

    BigCoin::bobTimer = 0;
    BigCoin::coinAddTimer = 0;
     add10coin = 0;
     add30coin = 0;
     add50coin = 0;
     add100coin = 0;




    fogFlag = false;
	alpha = 0xff;
	renderOffset = {0, 0};
	rotationTranslation = {0, 0};

    this->position.y -= 0x5000;


    activeCollider.init(this, activeColliderInfo, 0);
    activeCollider.shape = ActiveCollider::Shape::Round;
    activeCollider.link();

    return 1;

}


bool BigCoin::updateMain(){
    bobTimer++;


    if(bobTimer == 60){
        bobTimer = 0;
    }
    if (bobTimer < 30){
        this->position.y += 0x100;
    }
    if (bobTimer > 30){
        this->position.y -=0x100;
    }

    if(add10coin == 1){
        BigCoin::coinAddTimer++;
        if(this->settings>>0 == 0){
            SND::playSFX(364, 0);
            this->scale.x = 0;
            this->scale.y = 0;
            if(BigCoin::coinAddTimer <= 10){
                Game::addPlayerCoin(0);

             }
            if(BigCoin::coinAddTimer ==11){
                this->Base::destroy();
                add10coin = 0;
                BigCoin::coinAddTimer = 0;
            }
        }
    }

    if(add30coin == 1){
        BigCoin::coinAddTimer1++;
        if(this->settings>>0 == 1){
            SND::playSFX(364, 0);
            this->scale.x = 0;
            this->scale.y = 0;
            if(BigCoin::coinAddTimer1 <= 30){
                Game::addPlayerCoin(0);

             }
            if(BigCoin::coinAddTimer1 ==31){
                this->Base::destroy();
                add30coin = 0;
                BigCoin::coinAddTimer1 = 0;
            }
        }
    }
    if(add50coin == 1){
        BigCoin::coinAddTimer2++;
        if(this->settings>>0 == 2){
            SND::playSFX(364, 0);
            this->scale.x = 0;
            this->scale.y = 0;
            if(BigCoin::coinAddTimer2 <= 50){
                Game::addPlayerCoin(0);
             }
            if(BigCoin::coinAddTimer2 ==51){
                
                this->Base::destroy();
                add50coin = 0;
                BigCoin::coinAddTimer2 = 0;

            }
        }
    }

    if(add100coin == 1){
        BigCoin::coinAddTimer3++;
        if(this->settings>>0 == 3){
            this->scale.x = 0;
            this->scale.y = 0;
            SND::playSFX(364, 0);
            if(BigCoin::coinAddTimer3 <= 100){
                Game::addPlayerCoin(0);

             }
            if(BigCoin::coinAddTimer3 ==101){
                this->Base::destroy();
                add100coin = 0;
                BigCoin::coinAddTimer3 = 0;
            }
        }
    }
    return true;    

}

s32 BigCoin::onDestroy(){
		return 1;
}
