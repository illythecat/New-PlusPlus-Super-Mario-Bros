#pragma once

#include "Main/objectids.hpp"

#include "nsmb.hpp"


class kingBill : public StageEntity3DAnm { 

public:
    using StateFunction = void(kingBill::*)();

    virtual s32 onCreate() override;
    virtual s32 onDestroy() override;

    static bool loadResources();

    bool updateMain() override;
	void defeat(fx32 velX, fx32 velY, fx32 accelY, u8 unk) override;
	void defeatMega(fx32 velX, fx32 velY, fx32 accelY) override;
	void damagePlayer(ActiveCollider& self, Player& player) override;


	virtual void onFireballHit() override;

	void switchState(StateFunction function);


	void updateState();

	s8 codeAnimTimer;

	bool updateDefeated() override;
	bool updateDefeatedMega() override;

	bool doJump;
	s32 estimatedPos;
	s32 timeCrap;



    static constexpr ObjectBank objectBank = {0, 0};
	static constexpr u16 objectID = ObjectID::kingBill;

	static constexpr ObjectInfo objectInfo = {
		0, 0,
		20, 80,
		0, 0,
		0, 0,
		CollisionSwitch::None
	};
	static constexpr u16 updatePriority = objectID;
	static constexpr u16 renderPriority = 0x0000;
	static constexpr ActorProfile profile = {&constructObject<kingBill>, updatePriority, renderPriority, loadResources};

	static const LineSensorV sideSensor;
	static const LineSensorV sideSensor2;
	static const LineSensorV sideSensor3;


	static void activeCallback(ActiveCollider& self, ActiveCollider& other);
	static const ActiveColliderInfo activeColliderInfo;

    StateFunction updateFunction;
    s8 updateStep;
};
