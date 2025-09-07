#pragma once

#include "objectids.hpp"

#include "nsmb.hpp"


class Hammer : public StageEntity3D { 

public:

    using StateFunction = void(Hammer::*)();

    virtual s32 onCreate() override;
    virtual s32 onDestroy() override;

    static bool loadResources();
    bool updateMain() override;
	bool updateDefeated();




    static constexpr ObjectBank objectBank = {0, 0};
	static constexpr u16 objectID = ObjectID::Hammer;

	static constexpr ObjectInfo objectInfo = {
		0, 0,
		64, 64,
		0, 0,
		64, 64,
		CollisionSwitch::None
	};
	static constexpr u16 updatePriority = objectID;
	static constexpr u16 renderPriority = 0x0000;
	static constexpr ActorProfile profile = {&constructObject<Hammer>, updatePriority, renderPriority, loadResources};



	static void activeCallback(ActiveCollider& self, ActiveCollider& other);
	static const ActiveColliderInfo activeColliderInfo;

	Vec2 BallAccel;


	u8 timer;
	u16 exsistenceTimer;
	u16 exsistenceTimer2;

    StateFunction updateFunction;
    s8 updateStep;

};
