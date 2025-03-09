#pragma once

#include "objectids.hpp"

#include "nsmb.hpp"
#include "NSBTX.hpp"



class Crates : public StageEntity { 

public:


    virtual s32 onCreate() override;
    virtual s32 onDestroy() override;
    virtual s32 onRender() override;

    static void topColliderCallback(StageActor& self, StageActor& other);
	
	static void sideColliderCallback(StageActor& self, StageActor& other);
	
	static void bottomColliderCallback(StageActor& self, StageActor& other);

	virtual void onMegaGroundPound() override;

    static bool loadResources();
    virtual bool onPrepareResources();
    bool updateMain() override;



	
	bool groundPounded;


	static constexpr ObjectInfo objectInfo = {
		0, 0,
		64, 64,
		0, 0,
		64, 64,
		CollisionSwitch::None
	};


    static constexpr ObjectBank objectBank = {0, 0};
	static constexpr u16 objectID = ObjectID::Crates;



	static constexpr u16 updatePriority = objectID;
	static constexpr u16 renderPriority = 0x0000;
	static constexpr ActorProfile profile = {&constructObject<Crates>, updatePriority, renderPriority, loadResources};

    LineSensorH bottomSensor;

	Vec3 newPos[9];

	void activateEvent(u32 id){
		u64 *activatorBitmask = (u64*)(0x0208AF3C);
		*activatorBitmask |= 1LL << (id-1);
	}

    private:
	    Model model;
        Collider collider;
        bool hit;
		bool breakAnim;
        NSBTX CrateNsbtx[9];



};
