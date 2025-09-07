#pragma once

#include "Main/objectids.hpp"

#include "nsmb.hpp"


class Spike : public StageEntity3DAnm { 

public:

    using StateFunction = void(Spike::*)();

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


	void IdleState();
	void updateState();
	void holdState();
	void throughState();


	void liftState();
	bool updateDefeated() override;
	bool updateDefeatedMega() override;
	bool renderBall;
	bool spikeBallHurtBox;

	s16 timer;
	Vec3 BallScale;
	Vec3 BallPos;

	s8 direction[2];
	


    static constexpr ObjectBank objectBank = {0, 0};
	static constexpr u16 objectID = ObjectID::Spike;

	static constexpr ObjectInfo objectInfo = {
		0, 0,
		2, 2,
		0, 0,
		2, 2,
		CollisionSwitch::None
	};
	static constexpr u16 updatePriority = objectID;
	static constexpr u16 renderPriority = 0x0000;
	static constexpr ActorProfile profile = {&constructObject<Spike>, updatePriority, renderPriority, loadResources};

	static const LineSensorH bottomSensor;
	static const LineSensorH topSensor;

	static const LineSensorV sideSensor;

	static void activeCallback(ActiveCollider& self, ActiveCollider& other);
	static void activeCallback2(ActiveCollider& self, ActiveCollider& other);

	static const ActiveColliderInfo activeColliderInfo;


    StateFunction updateFunction;
    s8 updateStep;

private:
    BlendModelAnm modelSpike;
	Model	  modelEyes;
	Model	  modelHair;
	Model 	  modelBall;
    Actor* ball;

	ActiveCollider activeCollider;
    ActiveCollider activeCollider2;


    const static u32 SpikeID = 2430 - 131;
    const static u32 SpikeAnimID = 2431 - 131;
    const static u32 SpikeEyesID = 2432 - 131;
    const static u32 HairID = 2433 - 131;
	const static u32 BallID = 1504 - 131;

};
