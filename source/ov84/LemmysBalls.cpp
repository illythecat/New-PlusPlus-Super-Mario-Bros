#include "LemmysBalls.hpp"

void LemmysBalls::activeCallback(ActiveCollider& self, ActiveCollider& other){
    LemmysBalls* balls = static_cast<LemmysBalls*>(self.owner);
    auto stomped = balls->updatePlayerStomp(self, 9.0fx/32.0fx, 1, true);
    if(other.owner->actorType == ActorType::Player){
        Player* player_ = static_cast<Player*>(other.owner);
        if(stomped == PlayerStompType::MiniStomp)
            return;
        if(stomped == PlayerStompType::Stomp){
            return;
        }
        if(player_->position.x > balls->position.x){
            if(balls->settings == 0x8)
            player_->doBump(Vec2(0x3000, 0x4000));
            else
            player_->doBump(Vec2(0x2000, 0x2000));
        }
        else{
            if(balls->settings == 0x8)
            player_->doBump(Vec2(-0x3000, 0x4000));
            else
            player_->doBump(Vec2(-0x2000, 0x2000));
        }
    }
    if(other.owner->actorType == ActorType::Entity){
        StageEntity* actor = static_cast<StageEntity*>(other.owner);
        if (actor->id == ObjectID::LemmysBalls){
            if(actor->settings == 0x8)
                return;
            else{
            balls->velocity = -balls->velocity;
            balls->position.x += balls->velocity.x;
            if(balls->settings != 0x8)
            SND::playSFX(SeqArc::SAR_PLAYER_BASE::SE_PLY_GLIDING_END);
            if(actor->position.y < balls->position.y){
                balls->velocity.y = 3.0fx;
            }
            }

        }
    }
}

const LineSensorH LemmysBalls::bottomSensor2 = {
    -16.5fx, // Start position from the origin (left)
    16.5fx, // End position from the origin (right)
    -28.0fx // Y Offset (negative = down)
};

const LineSensorH LemmysBalls::TopSensor2 = {
    -16.5fx, // Start position from the origin (left)
    16.5fx, // End position from the origin (right)
    32.0fx // Y Offset (negative = down)
};

const LineSensorV LemmysBalls::sideSensor2 = {
    24.0fx, // Start position from the origin (top)
    -24.0fx, // End position from the origin (bottom)
    32.0fx // X offset (negative = left)
};	

const LineSensorH LemmysBalls::bottomSensor = {
    -8.5fx, // Start position from the origin (left)
    8.5fx, // End position from the origin (right)
    -14.0fx // Y Offset (negative = down)
};

const LineSensorH LemmysBalls::TopSensor = {
    -8.5fx, // Start position from the origin (left)
    8.5fx, // End position from the origin (right)
    14.0fx // Y Offset (negative = down)
};

const LineSensorV LemmysBalls::sideSensor = {
    12.0fx, // Start position from the origin (top)
    -12.0fx, // End position from the origin (bottom)
    12.0fx // X offset (negative = left)
};	

const LineSensorH LemmysBalls::bottomSensor3 = {
    -7.5fx, // Start position from the origin (left)
    5.5fx, // End position from the origin (right)
    -86.0fx, // Y Offset (negative = down)
SensorFlags::ActivateFallingBlocks|SensorFlags::ActivateQuestionBlocks|SensorFlags::ActivateBricksSmall|
SensorFlags::ActivateBricksSuper
};
const LineSensorV LemmysBalls::sideSensor3 = {
    76.0fx, // Start position from the origin (top)
    -76.0fx, // End position from the origin (bottom)
    66.0fx, // X offset (negative = left)
SensorFlags::ActivateFallingBlocks|SensorFlags::ActivateQuestionBlocks|SensorFlags::ActivateBricksSmall|
SensorFlags::ActivateBricksSuper
};	

const ActiveColliderInfo LemmysBalls::activeColliderInfo = {

    -2.0fx, 0.0fx,
    14.0fx, 14.0fx,

    CollisionGroup::Entity,
    CollisionFlag::None,
    MAKE_GROUP_MASK(CollisionGroup::Player, CollisionGroup::PlayerSpecial, CollisionGroup::Entity, CollisionGroup::Fireball),
    MAKE_FLAG_MASK_EX(CollisionFlag::None),
    0,

    LemmysBalls::activeCallback


};

const ActiveColliderInfo LemmysBalls::activeColliderInfo2 = {

    -2.0fx, 0.0fx,
    28.0fx, 28.0fx,

    CollisionGroup::Entity,
    CollisionFlag::None,
    MAKE_GROUP_MASK(CollisionGroup::Player, CollisionGroup::PlayerSpecial, CollisionGroup::Entity, CollisionGroup::Fireball),
    MAKE_FLAG_MASK_EX(CollisionFlag::None),
    0,

    LemmysBalls::activeCallback


};


const ActiveColliderInfo LemmysBalls::activeColliderInfo3 = {

    -2.0fx, 0.0fx,
    88.0fx, 88.0fx,

    CollisionGroup::Entity,
    CollisionFlag::None,
    MAKE_GROUP_MASK(CollisionGroup::Player, CollisionGroup::PlayerSpecial, CollisionGroup::Entity, CollisionGroup::Fireball),
    MAKE_FLAG_MASK_EX(CollisionFlag::None),
    0,

    LemmysBalls::activeCallback


};

bool LemmysBalls::loadResources(){
    FS::Cache::loadFile(2374 - 131, false);
    FS::Cache::loadFile(2375 - 131, false);
    return 1;
}

void LemmysBalls::damagePlayer(ActiveCollider& self, Player& player){

}

void LemmysBalls::onGroundPound(){
}

void LemmysBalls::onStomped(){
    StageEntity::defeat(0, 0.0fx, -9.0fx/32.0fx, 0);
    //defeat(0, 0, 9.0fx/32.0fx, 0);
}

void LemmysBalls::onFireballHit(){
    
}

void LemmysBalls::defeat(fx32 velX, fx32 velY, fx32 accelY, u8 unk){
    StageEntity::defeat(velX, velY, accelY, unk);
}

void LemmysBalls::defeatMega(fx32 velX, fx32 velY, fx32 accelY){
    StageEntity::defeatMega(velX, velY, accelY);
}
 
s32 LemmysBalls::onCreate(){
    void* BallAnim = FS::Cache::loadFile(2374 - 131, false);
    void* BallModel = FS::Cache::loadFile(2375 - 131, false);

    model.create(BallModel, BallAnim, 0, 0, 0);



    fogFlag = false;
	alpha = 31;
	renderOffset = {0, 0};
	rotationTranslation = {0, 0};
    if(settings & 0x1)
        velocity.x = 1.0fx;
    else
        velocity.x = -1.0fx;

    //
    bottomAnimTimer = 30;



    if(settings>>4 & 0x1){
        scale = Vec3(1.6fx, 2.4fx, 1.6fx);
        collisionMgr.init(this, &bottomSensor2, &TopSensor2, &sideSensor2); 
        activeCollider.init(this, activeColliderInfo2, 0);
        activeCollider.link();
    }
    else if(settings>>4 & 0x2){
        scale = Vec3(6.66fx, 10.0fx, 6.66fx);
        collisionMgr.init(this, &bottomSensor3, &TopSensor2, &sideSensor3); 
        activeCollider.init(this, activeColliderInfo3, 0);
        activeCollider.shape = ActiveCollider::Shape::Round;
        activeCollider.link();
    }
    else{
        scale = Vec3(0.8fx, 1.2fx, 0.8fx);
        collisionMgr.init(this, &bottomSensor, &TopSensor, &sideSensor); 
        activeCollider.init(this, activeColliderInfo, 0);
        activeCollider.link();
    }
    return 1;

}

void LemmysBalls::bottomCollisonAnim(){
    if(bottomAnimTimer <= 21){
        if(bottomAnimTimer < 11){
            scale.y -= 0x20;
            scale.x += 0x20;

        }
        else{
            scale.y += 0x20;
            scale.x -= 0x20;
        }
    }

}

bool LemmysBalls::updateMain(){

    if(settings == 0x8){
        return 1;
    }
    updateVerticalVelocity(); // updates gravity
    updateAnimation();
	applyMovement(); // applies velocity
	updateCollisionSensors(); // clamps position due to collision

    const CollisionMgrResult result = collisionMgr.collisionResult;
    if (bool(result & CollisionMgrResult::WallAny)){

        if(settings>>4 & 0x2){

        }
        else{
        velocity.x = -velocity.x;
        bottomAnimTimer = 0;
        }

    }

    if (bool(result & CollisionMgrResult::Ceiling)){
        velocity.y -= 3.0fx;
    }

    if(bool(result & CollisionMgrResult::GroundTile)){

        if(settings>>4 & 0x2){

        }
        else{
        velocity.y += 5.0fx;
            bottomAnimTimer = 0;
        

        }
    }
    bottomAnimTimer++;
    if(settings>>4 & 0x2){
        rotation.z -= velocity.x/5;
    }
    else{
        bottomCollisonAnim();

    }


    Player *player;
    player = Game::getPlayer(0);
    if(position.y < player->position.y - 0x1000000)
    rotation.z += velocity.x/10;

    u32* ViewBottom = (u32*)0x20CAE74;


    if(position.y < *ViewBottom - 0x80000){
        Base::destroy();
    }



    return true;      
}

bool LemmysBalls::updateDefeated(){
    Particle::Handler::createParticle(46, position);
    Particle::Handler::createParticle(107, position);
    Particle::Handler::createParticle(277, position);
    Base::destroy();
    return true; 
}

bool LemmysBalls::updateDefeatedMega(){
    Particle::Handler::createParticle(46, position);
    Particle::Handler::createParticle(107, position);
    Particle::Handler::createParticle(277, position);
    Base::destroy();
    return true; 
}

s32 LemmysBalls::onDestroy(){
		return 1;
}







    