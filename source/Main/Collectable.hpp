#pragma once

#include "objectids.hpp"

#include "nsmb.hpp"
#include "NSBTX.hpp"



class Collectable : public StageEntity3D { 

public:

    using StateFunction = void(Collectable::*)();

    virtual s32 onCreate() override;
    virtual s32 onDestroy() override;
    virtual s32 onRender() override;


    static bool loadResources();
    bool updateMain() override;

    void switchState(StateFunction function);
    void updateState();
    void mainState();
    void collectedState();
    void exitState();
    Vec3 storePlayerPos;
    virtual bool onPrepareResources();






    static constexpr ObjectBank objectBank = {0, 0};
	static constexpr u16 objectID = ObjectID::Collectable;

	static constexpr ObjectInfo objectInfo = {
		0, 0,
		64, 64,
		0, 0,
		64, 64,
		CollisionSwitch::None
	};
	static constexpr u16 updatePriority = objectID;
	static constexpr u16 renderPriority = 0x0000;
	static constexpr ActorProfile profile = {&constructObject<Collectable>, updatePriority, renderPriority, loadResources};

	static void activeCallback(ActiveCollider& self, ActiveCollider& other);
	static const ActiveColliderInfo activeColliderInfo;
    s32 timer;


    StateFunction updateFunction;
    s8 updateStep;
    bool renderText;
    s32  textPosOffset;
private:
        Model  Casette;
        NSBTX text[2];
        Vec3 textpos[2];



};
