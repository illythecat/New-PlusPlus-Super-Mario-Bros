#pragma once
#include "main/objectids.hpp"
#include "nsmb.hpp"

class Clone : public StageEntity3D {

public:
	virtual s32 onCreate() override;
	virtual s32 onDestroy() override;
	virtual s32 onRender() override;

	bool updateMain() override;
	static bool loadResources();

	static constexpr ObjectBank objectBank = {2, 7};
	static constexpr u16 objectID = ObjectID::Clone;

	static constexpr ObjectInfo objectInfo = {
		0, 0,
		8, 8,
		0, 0,
		8, 8,
		CollisionSwitch::None
	};

	static void activeCallback(ActiveCollider& self, ActiveCollider& other);
	static const ActiveColliderInfo activeColliderInfo;
	
	static constexpr u16 updatePriority = objectID;
	static constexpr u16 renderPriority = 0;

	static constexpr ActorProfile profile = {
		&constructObject<Clone>,
		updatePriority,
		renderPriority,
		loadResources
	};

private:
	BlendModelAnm modelPlayer;
	Vec3 positionHistory[60];
	u32 animationHistory[60];
	u32 assumedAnimID;

	Player* targetPlayer = nullptr;
    u32 lastAnimationID;
	u8 smokeTimer;
    Model headModel;


	NNSG3dAnmObj* bodyAnm = nullptr;
	u16* animationMap = nullptr;
	FrameCtrl frameCtrl{};
};
