#pragma once

#include "main/objectids.hpp"

#include "nsmb.hpp"


class Larry : public StageEntity { 

public:

    using StateFunction = void(Larry::*)();

    virtual s32 onCreate() override;
    virtual s32 onDestroy() override;

    static bool loadResources();
	virtual s32 onRender() override;
	void switchState(StateFunction function);
	void updateState();

    bool updateMain() override;
	void onStomped() override;
	void onGroundPound() override;
	void defeat(fx32 velX, fx32 velY, fx32 accelY, u8 unk) override;
	void defeatMega(fx32 velX, fx32 velY, fx32 accelY) override;
	void damagePlayer(ActiveCollider& self, Player& player) override;


    //bool switchState(StateFunction function);
    //void updateState();

	bool updateDefeated() override;
	bool updateDefeatedMega() override;
    virtual void onFireballHit() override;

    void VictoryAnimation();
	void IntroCutScene();
	void MainState();
	void ShellState();
	void UpdateRotation();


	void ShellCoverAttack();

	s16 timeCrap;

	u8 FightState;
	bool shellDirectionLeft;
	bool shellRecoveryInProgress;
	u16 shellTimer;

	void jumpState();
	void shellCharge();
	void fireState();
	void spawnFire(s8 speed);
	u8   mainStateState;
	u8   maxFireChain;
	u8   FireBallChainNum;
	bool inShell;

    static constexpr ObjectBank objectBank = {5, 8};
	static constexpr u16 objectID = ObjectID::Larry;

	static constexpr ObjectInfo objectInfo = {
		0, 0,
		16, 16,
		0, 0,
		16, 16,
		CollisionSwitch::None
	};
	static constexpr u16 updatePriority = objectID;
	static constexpr u16 renderPriority = 0x0000;
	static constexpr ActorProfile profile = {&constructObject<Larry>, updatePriority, renderPriority, loadResources};

	static void activeCallback(ActiveCollider& self, ActiveCollider& other);
	static const ActiveColliderInfo activeColliderInfo;

	static const LineSensorH bottomSensor;
	static const LineSensorV sideSensor;

    StateFunction updateFunction;
    s8 updateStep;
    s16 BossHealth;
    u16 exitTimer;

	u16 cutSceneTimer;
	u16 Timer;
	s16 recoveryTimer;
	s8 direction;
	bool deathAnim;
	bool startShellSlide;

	u32* ViewStart = (u32*)0x20CAE60;
	u32* ViewEnd = (u32*)0x20CAE64;

	void activateEvent(u32 id){
		u64 *activatorBitmask = (u64*)(0x0208AF3C);
		*activatorBitmask |= 1LL << (id-1);
	}


private:
	BlendModelAnm modelLarry;
    const static u32 LarryID = 2176 - 131;
    const static u32 LarryAnimID = 2203 - 131;
	BlendModelAnm modelLarryShell;
    const static u32 LarryShellID = 2184 - 131;
};

	enum LarryAnimation {
    	_wait = 0,
    	_throw_st_wait,
    	_throw_st,
    	_throw_end_wait,
    	_throw_end,
    	_exitShell,
		_enterShell,
    	_Jump1,
    	_Jump2,
   		_BeginJump,
    	_JumpLand,
   		_WakeUp,
   	    _Defeated,
    	_SlowDefeated,
		_wwait,
		_dummy,
		_Shocked,
		_Suprised,
		_Damaged,
	};


