#pragma once

#include "objectids.hpp"

#include "nsmb.hpp"


class SubNohara : public StageEntity3D { 

public:

    using StateFunction = void(SubNohara::*)();

    virtual s32 onCreate() override;
    virtual s32 onDestroy() override;

    static bool loadResources();

    bool updateMain() override;

    static constexpr ObjectBank objectBank = {0, 0};
	static constexpr u16 objectID = ObjectID::SubNohara;

	static constexpr ObjectInfo objectInfo = {
		0, 0,
		1000, 1000,
		0, 0,
		2, 2,
        CollisionSwitch::NoLevelBeaten
	};
	static constexpr u16 updatePriority = objectID;
	static constexpr u16 renderPriority = 0x0000;
	static constexpr ActorProfile profile = {&constructObject<SubNohara>, updatePriority, renderPriority, loadResources};

	static const LineSensorH bottomSensor;


};
