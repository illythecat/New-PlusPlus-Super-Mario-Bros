#pragma once

#include "objectids.hpp"

#include "nsmb.hpp"


class TrippleBlock : public StageEntity { 

public:


    virtual s32 onCreate() override;
    virtual s32 onDestroy() override;
    virtual s32 onRender() override;

    static void topColliderCallback(StageActor& self, StageActor& other);
	
	static void sideColliderCallback(StageActor& self, StageActor& other);
	
	static void bottomColliderCallback(StageActor& self, StageActor& other);


    static bool loadResources();
    bool updateMain() override;

	void onHit();
	
	void textureAnim();

	void doubleBlockSpawn();
	void trippleBlockSpawn();


    int offsetTimer;
    int offsetOffsetTimer;
	int movetimer;
	bool groundPounded;


	static constexpr ObjectInfo objectInfo = {
		0, 0,
		64, 64,
		0, 0,
		64, 64,
		CollisionSwitch::None
	};


    static constexpr ObjectBank objectBank = {0, 0};
	static constexpr u16 objectID = ObjectID::TrippleBlock;

	static constexpr u16 updatePriority = objectID;
	static constexpr u16 renderPriority = 0x0000;
	static constexpr ActorProfile profile = {&constructObject<TrippleBlock>, updatePriority, renderPriority, loadResources};



    private:
	    Model model;
        Collider collider;
        bool hit;
		bool hitAnim;

};
