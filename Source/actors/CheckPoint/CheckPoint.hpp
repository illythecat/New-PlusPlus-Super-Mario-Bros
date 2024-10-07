#pragma once

#include "objectids.hpp"

#include "nsmb.hpp"


class CheckPoint : public StageEntity3D {

public:

	virtual s32 onCreate() override;
	virtual s32 onDestroy() override;
	static bool loadResources();

	bool updateMain() override;
	void modelUpdated();
	bool checkpointCollected;

	static constexpr ObjectBank objectBank = {0, 0};

	static constexpr u16 objectID = ObjectID::CheckPoint;
	static constexpr ObjectInfo objectInfo = {
		0, 0,
		64, 64,
		0, 0,
		64, 64,
		CollisionSwitch::None
	};

	static constexpr u16 updatePriority = objectID;
	static constexpr u16 renderPriority = 0x0000;
	static constexpr ActorProfile profile = {&constructObject<CheckPoint>, updatePriority, renderPriority, loadResources};

	static const LineSensorH bottomSensor;
	static const LineSensorV sideSensor;

	static void activeCallback(ActiveCollider& self, ActiveCollider& other);
	static const ActiveColliderInfo activeColliderInfo;

	bool SetCheckPoint;

	int *Timer = (int*)(0x020CA8B4);

	void AddSecondsToTimer(int amount){
        *Timer += 4096 * amount;
	}
	

};
