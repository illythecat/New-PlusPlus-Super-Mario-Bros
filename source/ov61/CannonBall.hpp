#pragma once

#include "main/objectids.hpp"

#include "nsmb.hpp"


class CannonBall : public StageEntity3D { 

public:

    using StateFunction = void(CannonBall::*)();

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




    static constexpr ObjectBank objectBank = {2, 6};
	static constexpr u16 objectID = ObjectID::CannonBall;

	static constexpr ObjectInfo objectInfo = {
		0, 0,
		16, 16,
		0, 0,
		16, 16,
		CollisionSwitch::None
	};
	static constexpr u16 updatePriority = objectID;
	static constexpr u16 renderPriority = 0x0000;
	static constexpr ActorProfile profile = {&constructObject<CannonBall>, updatePriority, renderPriority, loadResources};

	static void activeCallback(ActiveCollider& self, ActiveCollider& other);
	static const ActiveColliderInfo activeColliderInfo;

    StateFunction updateFunction;
    s8 updateStep;

};
