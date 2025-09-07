#pragma once

#include "main/objectids.hpp"

#include "nsmb.hpp"


class Morton : public StageEntity3DAnm { 

public:

    using StateFunction = void(Morton::*)();

    virtual s32 onCreate() override;
    virtual s32 onDestroy() override;
	virtual s32 onRender() override;


    static bool loadResources();

    bool updateMain() override;
	void onStomped() override;
	void defeat(fx32 velX, fx32 velY, fx32 accelY, u8 unk) override;
	void defeatMega(fx32 velX, fx32 velY, fx32 accelY) override;
	void damagePlayer(ActiveCollider& self, Player& player) override;


	virtual void onFireballHit() override;

	void switchState(StateFunction function);


	void introCutScene();
	void updateState();

	void updateJumpState();
	void updateFireState();
	void recoveryState();
	void recoverAttck();
	void invertedJump();
	void resetState();
	void fakeDeath();
	void groundPound();

	s16 timeCrap;
	s16 health;
	bool phase2;

	bool updateDefeated() override;
	bool updateDefeatedMega() override;


	s16 JumpTimer;
	u32 currentTex;
	bool landAnimation;
	bool fliped;
	bool targetLocked;
	s32 target;
	u32 resultTileBeavior;
	u8 recoverAttacksCompleated;
	u8 shockCooldown;
	u8 colourchange;




    static constexpr ObjectBank objectBank = {5, 1};
	static constexpr u16 objectID = ObjectID::Morton;

	static constexpr ObjectInfo objectInfo = {
		0, 0,
		2, 2,
		0, 0,
		2, 2,
		CollisionSwitch::None
	};
	static constexpr u16 updatePriority = objectID;
	static constexpr u16 renderPriority = 0x0000;
	static constexpr ActorProfile profile = {&constructObject<Morton>, updatePriority, renderPriority, loadResources};

	static const LineSensorH bottomSensor;
	static const LineSensorH topSensor;
	static const LineSensorV sideSensor;

	u32* ViewTop = (u32*)0x20CAE70;
	u32* ViewBottom = (u32*)0x20CAE74;




	static void activeCallback(ActiveCollider& self, ActiveCollider& other);
	static void activeCallback2(ActiveCollider& self, ActiveCollider& other);
	static const ActiveColliderInfo activeColliderInfo;
	static const ActiveColliderInfo activeColliderInfo2;


    StateFunction updateFunction;
    s8 updateStep;

private:
    BlendModelAnm modelMorton;
    const static u32 MortonID = 2178 - 131;
    const static u32 MortonAnimID = 2203 - 131;
	ActiveCollider activeCollider2;
	ActiveCollider activeCollider;


	Model shellmet;
	const static u32 spinyModel = 2170 - 131;



};

enum MortonAnim {
	Idle = 0,
	ChargeAttack,
	startingAttack,
	EndAttackHold,
	EndAttack,
	ExitShell,
	EnterShell,
	Jumping,
	StartJump,
	Landearthqaukeing,
	earthqaukefalling,
	earthqaukeJumping,
	startEarthqaukeJump,
	GetUp,
	DeathBounce,
	DeathFall,
	PreFightPos,
	FightCutsceneNotice,
	FightCutsceneEngage,
	hit1,
	hit2

};
