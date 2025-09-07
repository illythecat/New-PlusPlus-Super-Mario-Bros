#pragma once

#include "objectids.hpp"

#include "nsmb.hpp"


class TimePiece : public StageEntity3D { 

public:

    using StateFunction = void(TimePiece::*)();

    virtual s32 onCreate() override;
    virtual s32 onDestroy() override;

    static bool loadResources();
    bool updateMain() override;



    static constexpr ObjectBank objectBank = {0, 0};
	static constexpr u16 objectID = ObjectID::TimePiece;

	static constexpr ObjectInfo objectInfo = {
		0, 0,
		64, 64,
		0, 0,
		64, 64,
		CollisionSwitch::None
	};
	static constexpr u16 updatePriority = objectID;
	static constexpr u16 renderPriority = 0x0000;
	static constexpr ActorProfile profile = {&constructObject<TimePiece>, updatePriority, renderPriority, loadResources};

	static void activeCallback(ActiveCollider& self, ActiveCollider& other);
	static const ActiveColliderInfo activeColliderInfo;

	void addTime();

	int *TimerUwU = (int*)(0x020CA8B4);

	void AddSecondsToTimerUwU(int amount){
        *TimerUwU += 4096 * amount;
	}

    StateFunction updateFunction;
    s8 updateStep;

};
