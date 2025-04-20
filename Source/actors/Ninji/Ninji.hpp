#pragma once

#include "objectids.hpp"

#include "nsmb.hpp"


class Ninji : public StageEntity3D { 

public:

    using StateFunction = void(Ninji::*)();

    virtual s32 onCreate() override;
    virtual s32 onDestroy() override;

    static bool loadResources();

    bool updateMain() override;
	void onStomped() override;
	void defeat(fx32 velX, fx32 velY, fx32 accelY, u8 unk) override;
	void defeatMega(fx32 velX, fx32 velY, fx32 accelY) override;
	void damagePlayer(ActiveCollider& self, Player& player) override;


	virtual void onFireballHit() override;

	void switchState(StateFunction function);


	void codeAnim();
	void updateState();

	void updateJumpState();
	s8 codeAnimTimer;

	bool updateDefeated() override;
	bool updateDefeatedMega() override;

	bool doJump;

	s16 jumpCoolDown;
	s16 JumpTimer;
	u32 currentTex;



    static constexpr ObjectBank objectBank = {0, 0};
	static constexpr u16 objectID = ObjectID::Ninji;

	static constexpr ObjectInfo objectInfo = {
		0, 0,
		2, 2,
		0, 0,
		2, 2,
		CollisionSwitch::None
	};
	static constexpr u16 updatePriority = objectID;
	static constexpr u16 renderPriority = 0x0000;
	static constexpr ActorProfile profile = {&constructObject<Ninji>, updatePriority, renderPriority, loadResources};

	static const LineSensorH bottomSensor;
	static const LineSensorV sideSensor;

	static void activeCallback(ActiveCollider& self, ActiveCollider& other);
	static const ActiveColliderInfo activeColliderInfo;

    StateFunction updateFunction;
    s8 updateStep;

};
