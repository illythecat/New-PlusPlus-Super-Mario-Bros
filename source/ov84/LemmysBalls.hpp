#pragma once

#include "main/objectids.hpp"

#include "nsmb.hpp"


class LemmysBalls : public StageEntity3DAnm { 

public:

    using StateFunction = void(LemmysBalls::*)();

    virtual s32 onCreate() override;
    virtual s32 onDestroy() override;

    static bool loadResources();

    bool updateMain() override;
	void onStomped() override;
	void defeat(fx32 velX, fx32 velY, fx32 accelY, u8 unk) override;
	void defeatMega(fx32 velX, fx32 velY, fx32 accelY) override;
	void damagePlayer(ActiveCollider& self, Player& player) override;

	bool updateDefeated() override;
	bool updateDefeatedMega() override;
	virtual void onFireballHit() override;
	void onGroundPound() override;
	void bottomCollisonAnim();
	u8   bottomAnimTimer;
    bool doBallBump;


    static constexpr ObjectBank objectBank = {0, 0};
	static constexpr u16 objectID = ObjectID::LemmysBalls;

	static constexpr ObjectInfo objectInfo = {
		0, 0,
		8, 8,
		0, 0,
		8, 8,
		CollisionSwitch::None
	};
	static constexpr u16 updatePriority = objectID;
	static constexpr u16 renderPriority = 0x0000;
	static constexpr ActorProfile profile = {&constructObject<LemmysBalls>, updatePriority, renderPriority, loadResources};

	static void activeCallback(ActiveCollider& self, ActiveCollider& other);
	static const ActiveColliderInfo activeColliderInfo;
	static const ActiveColliderInfo activeColliderInfo2;
	static const ActiveColliderInfo activeColliderInfo3;



    static const LineSensorH bottomSensor;
    static const LineSensorH TopSensor;
	static const LineSensorV sideSensor;

	static const LineSensorH bottomSensor2;
    static const LineSensorH TopSensor2;
	static const LineSensorV sideSensor2;
	static const LineSensorH bottomSensor3;
	static const LineSensorV sideSensor3;

    StateFunction updateFunction;
    s8 updateStep;

};
