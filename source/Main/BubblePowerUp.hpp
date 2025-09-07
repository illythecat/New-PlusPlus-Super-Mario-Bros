#pragma once

#include "nsmb.hpp"
#include "objectIds.hpp"

class BubblePowerUp : public StageEntity3D
{
public:

	virtual s32 onCreate() override;
	virtual bool updateMain() override;
	virtual s32 onDestroy() override;
    static bool loadResources();


	static void activeColliderCallback(ActiveCollider& self, ActiveCollider& other);

	int spawnTimer;
	bool doSpawnState;
	bool collected;
    bool itemType;
	u16 codeAnimTimer;


    void theOldSwitcharo();

    static constexpr ObjectBank objectBank = {0, 0};
	static constexpr u16 objectID = ObjectID::BubblePowerUp;

	static constexpr ObjectInfo objectInfo = {
		0, 0,
		32, 32,
		0, 0,
		32, 32,
		CollisionSwitch::None
	};

	static constexpr u16 updatePriority = objectID;
	static constexpr u16 renderPriority = 0x0000;
	static constexpr ActorProfile profile = {&constructObject<BubblePowerUp>, updatePriority, renderPriority, loadResources};


private:

};

