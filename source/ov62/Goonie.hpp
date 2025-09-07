#pragma once

#include "main/objectids.hpp"

#include "nsmb.hpp"


class Goonie : public StageEntity3DAnm { 

public:

    using StateFunction = void(Goonie::*)();

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


	void swoopState();
	void updateState();



	bool updateDefeated() override;
	bool updateDefeatedMega() override;

	bool shyGuyDroped;





    static constexpr ObjectBank objectBank = {0, 0};
	static constexpr u16 objectID = ObjectID::Goonie;

	static constexpr ObjectInfo objectInfo = {
		0, 0,
		2, 2,
		0, 0,
		2, 2,
		CollisionSwitch::None
	};
	static constexpr u16 updatePriority = objectID;
	static constexpr u16 renderPriority = 0x0000;
	static constexpr ActorProfile profile = {&constructObject<Goonie>, updatePriority, renderPriority, loadResources};

	static const LineSensorH bottomSensor;
	static const LineSensorH topSensor;

	static const LineSensorV sideSensor;

	static void activeCallback(ActiveCollider& self, ActiveCollider& other);
	static const ActiveColliderInfo activeColliderInfo;



    StateFunction updateFunction;
    s8 updateStep;

private:

	ModelAnm modelGoonie;
    const static u32 GoonieId = 2396 - 131;
    const static u32 GoonieHoldId = 2395 - 131;
    const static u32 BoneGoonieId = 2397 - 131;
    const static u32 BoneGoonieHoldId = 2398 - 131;

    const static u32 GoonieAnimId = 2394 - 131;

	ModelAnm modelShyGuy;
    const static u32 ShyGuyID = 2149 - 131;
    const static u32 ShyGuyAnimId = 1918 - 131;

};
