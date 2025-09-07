#pragma once

#include "objectids.hpp"

#include "nsmb.hpp"


class TalkingFlower : public StageEntity3DAnm { 

public:

    using StateFunction = void(TalkingFlower::*)();

    virtual s32 onCreate() override;
    virtual s32 onDestroy() override;

    static bool loadResources();

    bool updateMain() override;
	void switchState(StateFunction function);

	void updateState();


	bool updateDefeated() override;
	bool updateDefeatedMega() override;

	bool spoken;
	s16 speachTimer;




    static constexpr ObjectBank objectBank = {0, 0};
	static constexpr u16 objectID = ObjectID::TalkingFlower;

	static constexpr ObjectInfo objectInfo = {
		0, 0,
		2, 2,
		0, 0,
		2, 2,
		CollisionSwitch::None
	};
	static constexpr u16 updatePriority = objectID;
	static constexpr u16 renderPriority = 0x0000;
	static constexpr ActorProfile profile = {&constructObject<TalkingFlower>, updatePriority, renderPriority, loadResources};

	static const LineSensorH bottomSensor;
	static const LineSensorH topSensor;

	static const LineSensorV sideSensor;

	static void activeCallback(ActiveCollider& self, ActiveCollider& other);
	static const ActiveColliderInfo activeColliderInfo;

    StateFunction updateFunction;
    s8 updateStep;

};
