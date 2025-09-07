#include "nsmb.hpp"

ncp_hook(0x0213DBA8, 23)
bool PokeyLoadMoreCrap(){
    FS::Cache::loadFile(2429 - 131, false);
    FS::Cache::loadFile(2428 - 131, false);

    return true;
}

static void pokeyOnStomped(Actor* self) {    
    int *world = (int*)(0x02088BFC);
    if(*world == 0) asm("B  0x0213C75C");
}

static void pokeyDamagePlayer(ActiveCollider& selfAC, ActiveCollider& otherAC) {
    auto pokey = static_cast<Pokey*>(selfAC.owner);
    int *world = (int*)(0x02088BFC);
    if (otherAC.owner->actorType == ActorType::Player) {
        Player* player = static_cast<Player*>(otherAC.owner);

        if(*world == 0){
        auto stomped = pokey->updatePlayerStomp(selfAC, 9.0fx/32.0fx, 1, false);
        if(stomped == PlayerStompType::MiniStomp)
            return;

        if(stomped == PlayerStompType::Stomp){
            pokey->setStompCollision(*player);
            return;
        }
        }
        player->dealDamage(*pokey, 0, 4.0fx, PlayerDamageType::Hit);

    }
}


ncp_over(0x0213ECC8,23)
static const auto pokeyOnStompedVFunc = pokeyOnStomped;

ncp_over(0x0213ECF8, 23)
static const auto pokeyDamagePlayerVFunc = pokeyDamagePlayer;

ncp_over(0x0213EAF8, 23)
static const ActiveColliderInfo activeColliderInfo = {
    0.0fx, 0.0fx,
    8.0fx, 6.0fx,
    CollisionGroup::Entity,
    CollisionFlag::None,
    MAKE_GROUP_MASK(CollisionGroup::Player, CollisionGroup::PlayerSpecial, CollisionGroup::Fireball),
    MAKE_FLAG_MASK_EX(CollisionFlag::None),
    0,
    pokeyDamagePlayer
};

ncp_jump(0x0213DB14,23)
NTR_NAKED static void pokeyHead() {asm(R"(
	LDR		R0, =0x02088BFC
	LDR		R0, [R0]
	CMP		R0, #0
	LDREQ           R0, =2429 - 131 
    BEQ             0x0213DB14 + 4
	LDR             R0, =0x5cc
    B               0x0213DB14 + 4

)");}


ncp_jump(0x0213DB54,23)
NTR_NAKED static void pokeyBoobs() {asm(R"(
	LDR		R7, =0x02088BFC
	LDR		R7, [R7]
	CMP		R7, #0
	LDREQ           R7, =2428 - 131 
    BEQ             0x0213DB54 + 4
	LDR             R7, =0x5cb
    B               0x0213DB54 + 4
)");}

