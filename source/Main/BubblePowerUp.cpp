#include "BubblePowerUp.hpp"
#include "main/undocumented.hpp"
#include "main/NewInv/CustomInventory.hpp"

using namespace CustomInventory;

u8 CurrentReservePowerup;

ncp_call(0x020bf590, 0)
void itemThingTest(){

    FS::Cache::loadFile(2351 + CurrentReservePowerup - 131, false);

	CurrentReservePowerup = getPowerupOnSlot(0, getDroppingSlot(0));

	Player* player;
    player = Game::getPlayer(0);
	Vec3 spawnPos;
	spawnPos.x = player->position.x;
	spawnPos.y = player->position.y + 0x30000;

	Actor::spawnActor(0x182 + 0xb, CurrentReservePowerup, &spawnPos);
}

static ActiveColliderInfo collisionInfo = {

    0, 
    0x8000, 
    0x7000,
    0x8000,
    
    CollisionGroup::Entity, // your actor's group
	CollisionFlag::None,  // your actor's flag
    MAKE_GROUP_MASK(CollisionGroup::Player, CollisionGroup::PlayerSpecial), // groups it collides with
    MAKE_FLAG_MASK(CollisionFlag::None), // other colliders with flags it collides with
	
	0,
    
    BubblePowerUp::activeColliderCallback,

};

bool BubblePowerUp::loadResources()
{
	FS::Cache::loadFile(2351 + CurrentReservePowerup - 131, false);
	return 1;
}

s32 BubblePowerUp::onCreate()
{
    void* modelFile = FS::Cache::loadFile(2351 + CurrentReservePowerup - 131, false);
    model.create(modelFile, 0, 0);

	fogFlag = false;
	alpha = 0;
	renderOffset = {0, 0};
	rotationTranslation = {0, 0};

	
	activeCollider.init(this, collisionInfo, 0);
	activeCollider.link();
	
    this->scale = {0x2800, 0x2800, 0x2800};
	
	return 1;
}

bool BubblePowerUp::updateMain()
{
	if(alpha != 31)
		alpha++;

	codeAnimTimer++;
    if(codeAnimTimer < 30){
        this->scale.x -= 8;
        this->scale.y -= 32;

    }
    if(codeAnimTimer > 30){
        this->scale.x += 8;
        this->scale.y += 32;
    }
    if(codeAnimTimer == 60){
        codeAnimTimer = 0;
    }

	return 1;	
}

s32 BubblePowerUp::onDestroy()
{
	return 1;
}

void BubblePowerUp::theOldSwitcharo(){
	Player *player;
    player = Game::getPlayer(0);
		if((player->currentPowerup == PowerupState::Super)){
		}
		if((player->currentPowerup == PowerupState::Fire)){
			replacePowerup(player, 0, 2);
		}
		else if((player->currentPowerup == PowerupState::Shell)){
			replacePowerup(player, 0, 3);
		}
		else if((player->currentPowerup== PowerupState::Test1)){
			replacePowerup(player, 0, 8);

		}
		else if((player->currentPowerup== PowerupState::Test2)){
			replacePowerup(player, 0, 9);
		}
		else if((player->currentPowerup== PowerupState::Test3)){
			replacePowerup(player, 0, 10);
		}
}


void BubblePowerUp::activeColliderCallback(ActiveCollider& self, ActiveCollider& other)
{
	if (other.owner->actorType != ActorType::Player || (((other.owner->settings >> 8) & 0xF) == 1))
			return;
	
    BubblePowerUp* bp = static_cast<BubblePowerUp*>(self.owner);
    Player* player = static_cast<Player*>(other.owner);
	
	if (bp->collected == false && (player->runtimePowerup != PowerupState::Mega))
	{
		player->powerupSwitchTimer = 1;
		player-> powerupScaleStep = 10;
		
		bp->collected = true;



		if(bp->settings == 1){
			bp->theOldSwitcharo();
			player->applyPowerup(PowerupState::Super);
		}
		else if(bp->settings == 2){
			bp->theOldSwitcharo();
			player->applyPowerup(PowerupState::Fire);
		}
		else if(bp->settings == 3){
			bp->theOldSwitcharo();
			player->applyPowerup(PowerupState::Shell);
		}
		else if(bp->settings == 4){
			bp->theOldSwitcharo();
			player->applyPowerup(PowerupState::Mini);
		}
		else if(bp->settings == 5){
			bp->theOldSwitcharo();
			player->applyPowerup(PowerupState::Mega);
		}
		else if(bp->settings == 8){
			bp->theOldSwitcharo();
			player->applyPowerup(PowerupState::Test1);
		}
		else if(bp->settings == 9){
			bp->theOldSwitcharo();
			player->applyPowerup(PowerupState::Test2);
		}
		else if(bp->settings == 0xa){
			bp->theOldSwitcharo();
			player->applyPowerup(PowerupState::Test3);
		}
		


		
	}
	self.owner->destroy();
}