#pragma once

#include "objectids.hpp"

#include "nsmb.hpp"


class KoopalingProjectile : public StageEntity3DAnm { 

public:

    using StateFunction = void(KoopalingProjectile::*)();

    virtual s32 onCreate() override;
    virtual s32 onDestroy() override;

    static bool loadResources();
    static bool prepareResources();

    bool updateMain() override;



    static constexpr ObjectBank objectBank = {0, 0};
	static constexpr u16 objectID = ObjectID::KoopalingProjectile;

	static constexpr ObjectInfo objectInfo = {
		0, 0,
		128, 128,
		-128, -128,
		128, 128,
		CollisionSwitch::None
	};
	static constexpr u16 updatePriority = objectID;
	static constexpr u16 renderPriority = 0x0000;
	static constexpr ActorProfile profile = {&constructObject<KoopalingProjectile>, updatePriority, renderPriority, loadResources};

	static void activeCallback(ActiveCollider& self, ActiveCollider& other);
	static const ActiveColliderInfo activeColliderInfo;

    AnimationCtrl animctrl;
    StateFunction updateFunction;
    s8 updateStep;
	s8 particalTimer;
	VecFx32 d;
	s32 magnitude;
	u16 homeTimer;
 

	

};
