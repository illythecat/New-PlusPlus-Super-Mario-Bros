#pragma once

#include "main/objectids.hpp"

#include "nsmb.hpp"


class BigCoin : public StageEntity3D { 

public:

    using StateFunction = void(BigCoin::*)();

    virtual s32 onCreate() override;
    virtual s32 onDestroy() override;

    static bool loadResources();
    bool updateMain() override;
	int bobTimer;
	int coinAddTimer;
	int coinAddTimer1;
	int coinAddTimer2;
	int coinAddTimer3;





    static constexpr ObjectBank objectBank = {0,0};
	static constexpr u16 objectID = ObjectID::BigCoin;

	static constexpr ObjectInfo objectInfo = {
		0, 0,
		32, 32,
		0, 0,
		32, 32,
		CollisionSwitch::None
	};
	static constexpr u16 updatePriority = objectID;
	static constexpr u16 renderPriority = 0x0000;
	static constexpr ActorProfile profile = {&constructObject<BigCoin>, updatePriority, renderPriority, loadResources};

	static void activeCallback(ActiveCollider& self, ActiveCollider& other);
	static const ActiveColliderInfo activeColliderInfo;

    StateFunction updateFunction;
    s8 updateStep;

};
