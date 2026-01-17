#pragma once

#include "main/objectids.hpp"

#include "nsmb.hpp"
#include "main/NSBTX.hpp"



class Grinder : public StageEntity { 

public:


    virtual s32 onCreate() override;
    virtual s32 onDestroy() override;
    virtual s32 onRender() override;
    static bool loadResources();

	virtual void onMegaGroundPound() override;
    bool updateMain() override;

	static constexpr ObjectInfo objectInfo = {
		0, 0,
		16, 16,
		0, 0,
		16, 16,
		CollisionSwitch::None
	};


    static constexpr ObjectBank objectBank = {2, 6};
	static constexpr u16 objectID = ObjectID::Grinder;
	static void activeCallback(ActiveCollider& self, ActiveCollider& other);
	static const ActiveColliderInfo activeColliderInfo;
	static const ActiveColliderInfo activeColliderInfo2;
	static const ActiveColliderInfo activeColliderInfo3;

	static constexpr u16 updatePriority = objectID;
	static constexpr u16 renderPriority = 0x0000;
	static constexpr ActorProfile profile = {&constructObject<Grinder>, updatePriority, renderPriority, loadResources};
    Vec3 newPos;
    Vec2 pos;
    VecFx16 spinzies;
    private:
	    Model Blades;
    	Model Core;
        LineController lineCtrl;

};
