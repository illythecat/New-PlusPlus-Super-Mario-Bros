#pragma once

#include "main/objectids.hpp"

#include "nsmb.hpp"


class Kameck : public StageEntity3DAnm { 

public:

    using StateFunction = void(Kameck::*)();

    virtual s32 onCreate() override;
    virtual s32 onDestroy() override;
    virtual s32 onRender() override;


    static bool loadResources();

    bool updateMain() override;
	void onStomped() override;
	void defeat(fx32 velX, fx32 velY, fx32 accelY, u8 unk) override;
	void defeatMega(fx32 velX, fx32 velY, fx32 accelY) override;
	bool updateDefeated() override;
	bool updateDefeatedMega() override;
	virtual void onFireballHit() override;


	void switchState(StateFunction function);
	void updateState();
	void idleState();
	void fireAttack();
	void spawnEnemyAttack();
	void dashLeftAttack();
	void dashRightAttack();
	void resetState();
	void randomFire();
	void deathState();





    static constexpr ObjectBank objectBank = {9, 9};
	static constexpr u16 objectID = ObjectID::Kameck;

	static constexpr ObjectInfo objectInfo = {
		0, 0,
		16, 16,
		0, 0,
		16, 16,
		CollisionSwitch::NoBlueShell | CollisionSwitch::NoSliding
	};
	static constexpr u16 updatePriority = objectID;
	static constexpr u16 renderPriority = 0x0000;
	static constexpr ActorProfile profile = {&constructObject<Kameck>, updatePriority, renderPriority, loadResources};

	static void activeCallback(ActiveCollider& self, ActiveCollider& other);
	static const ActiveColliderInfo activeColliderInfo;

	static const LineSensorH bottomSensor;


    StateFunction updateFunction;
    s8 updateStep;

    s32 timeCrap;

	u32* ViewStart = (u32*)0x20CAE60;
	u32* ViewEnd = (u32*)0x20CAE64;
	u32* ViewTop = (u32*)0x20CAE70;
	u32* ViewBottom = (u32*)0x20CAE74;

	bool isScroling;

	Vec3 spawnPos;
	s16 health;

private:
    BlendModelAnm modelKameck;
    const static u32 kamckID = 2173 - 131;
    const static u32 kameckAnimId = 2174 - 131;
    Vec3 kameckScale;
	Vec3 kameckPosition;

    ModelAnm modelbroom;
    const static u32 broomID = 2175 - 131;
    const static u32 broomAnimId = 2176 - 131;
    Vec3 broomScale;
	Vec3 broomPosition;




};

enum KameckAnimation {
    Apear = 0,
	idle,
	hit,
	CutsceneIntro,
	defeat,
	startRecovery,
	disapear,
	startAttack,
	flying,
	flyIdle,
	finishAttack,
	attackEndHold,
	attackChargeHold,


};
