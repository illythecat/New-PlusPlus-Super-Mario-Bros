#include "Grinder.hpp"

void Grinder::activeCallback(ActiveCollider& self, ActiveCollider& other){

    Grinder* tp = static_cast<Grinder*>(self.owner);
    if(other.owner->actorType == ActorType::Player){
        Player* player = static_cast<Player*>(other.owner);
        player->dealDamage(*self.owner, 0, 4.0fx, PlayerDamageType::Hit);
    }
}

const ActiveColliderInfo Grinder::activeColliderInfo = {

    0.0fx, 0.0fx,
    8.0fx, 8.fx,

    CollisionGroup::Entity,
    CollisionFlag::None,
    MAKE_GROUP_MASK(CollisionGroup::Player, CollisionGroup::PlayerSpecial),
    MAKE_FLAG_MASK_EX(CollisionFlag::None),
    0,

    Grinder::activeCallback

};

const ActiveColliderInfo Grinder::activeColliderInfo2 = {

    0.0fx, 0.0fx,
    16.0fx, 16.fx,

    CollisionGroup::Entity,
    CollisionFlag::None,
    MAKE_GROUP_MASK(CollisionGroup::Player, CollisionGroup::PlayerSpecial),
    MAKE_FLAG_MASK_EX(CollisionFlag::None),
    0,

    Grinder::activeCallback

};


const ActiveColliderInfo Grinder::activeColliderInfo3 = {

    0.0fx, 0.0fx,
    24.0fx, 24.fx,

    CollisionGroup::Entity,
    CollisionFlag::None,
    MAKE_GROUP_MASK(CollisionGroup::Player, CollisionGroup::PlayerSpecial),
    MAKE_FLAG_MASK_EX(CollisionFlag::None),
    0,

    Grinder::activeCallback

};


bool Grinder::loadResources() {
    FS::Cache::loadFile(2506 - 131, false);
    FS::Cache::loadFile(2507 - 131, false);
    return true;
}

void Grinder::onMegaGroundPound(){
    return;
}

s32 Grinder::onCreate(){
    void* modelBlade = FS::Cache::loadFile(2506 - 131, false);
    void* modelCore = FS::Cache::loadFile(2507 - 131, false);
    spinzies = VecFx16(rotation.x, rotation.y, rotation.z);

    Blades.create(modelBlade, 0, 0);
    rotation.x = 8fx;
    Core.create(modelCore, 0, 0);
    position.x += 0x8000;
    position.y -= 0x8000;

    if(settings>>4 != 1){
        pos = Vec2(position.x, position.y);
        lineCtrl.create(pos,0.5fx * (1+(settings>>8) & 0xf));
        lineCtrl.start();
    }

    if((settings >> 0) & 0x1){
        scale = Vec3(2.0fx, 2.0fx, 2.0fx);
        activeCollider.init(this, activeColliderInfo2, 0);
        activeCollider.shape = ActiveCollider::Shape::Round;
        activeCollider.link();
        return 1;

    }
    if((settings >> 0) & 0x2){
        scale = Vec3(3.0fx, 3.0fx, 3.0fx);
        activeCollider.init(this, activeColliderInfo3, 0);
        activeCollider.shape = ActiveCollider::Shape::Round;
        activeCollider.link();
        return 1;

    }
    activeCollider.init(this, activeColliderInfo, 0);
    activeCollider.shape = ActiveCollider::Shape::Round;
    activeCollider.link();
    

    return 1;

}


bool Grinder::updateMain(){

   // updateVerticalVelocity(); // updates gravity
	//applyMovement(); // applies velocity
    rotation.z += 0.5fx;
    if(settings>>4 != 1){
        lineCtrl.update();
        Vec2 pos = lineCtrl.getPosition();
        position.x = pos.x;
        position.y = pos.y;
    }
    SND::playSFXHold(SeqArc::SE_OBJ_BIG_ROCK_ROLL, nullptr);

    return true;      
}

s32 Grinder::onRender()
{
    spinzies.x = 4.0fx;

    MTX::identity(Blades.matrix);
    MTX::translate(Blades.matrix, position);
    MTX::rotate(Blades.matrix, rotation);
    Game::modelMatrix = Blades.matrix;
    Blades.render(&scale);

    MTX::identity(Core.matrix);
    newPos = Vec3(position.x, position.y, position.z + 10fx);
    MTX::translate(Core.matrix, newPos);
    MTX::rotate(Core.matrix, rotation);
    Game::modelMatrix = Core.matrix;
    Core.render(&scale);
	return 1;
}

s32 Grinder::onDestroy(){
    return 1;
}




