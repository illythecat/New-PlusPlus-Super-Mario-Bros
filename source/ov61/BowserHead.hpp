#pragma once

#include "main/objectids.hpp"

#include "nsmb.hpp"


class BowserHead : public StageEntity3D { 

public:

    using StateFunction = void(BowserHead::*)();

    virtual s32 onCreate() override;
    virtual s32 onDestroy() override;

    static bool loadResources();
    bool updateMain() override;

    int fireBallTimer;



    static constexpr ObjectBank objectBank = {2, 6};
	static constexpr u16 objectID = ObjectID::BowserHead;

	static constexpr ObjectInfo objectInfo = {
		0, 0,
		64, 64,
		0, 0,
		64, 64,
		CollisionSwitch::None
	};
	static constexpr u16 updatePriority = objectID;
	static constexpr u16 renderPriority = 0x0000;
	static constexpr ActorProfile profile = {&constructObject<BowserHead>, updatePriority, renderPriority, loadResources};

	static void activeCallback(ActiveCollider& self, ActiveCollider& other);
	static const ActiveColliderInfo activeColliderInfo;

    StateFunction updateFunction;
    s8 updateStep;

};
