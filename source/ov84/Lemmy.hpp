#pragma once

#include "main/objectids.hpp"
#include "nsmb.hpp"

class Lemmy : public StageEntity3DAnm {
public:
    using StateFunction = void(Lemmy::*)();

    virtual s32 onCreate() override;
    virtual s32 onDestroy() override;
    virtual s32 onRender() override;

    static bool loadResources();

    bool updateMain() override;
    void onStomped() override;
    void switchState(StateFunction function);
    void updateState();
    void onGroundPound() override;
    void defeat(fx32 velX, fx32 velY, fx32 accelY, u8 unk) override;
    void defeatMega(fx32 velX, fx32 velY, fx32 accelY) override;
    void damagePlayer(ActiveCollider& self, Player& player) override;

    bool updateDefeated() override;
    bool updateDefeatedMega() override;
    virtual void onFireballHit() override;

    void VictoryAnimation();
    void IntroCutScene();
    void MainState();
    void ShellState();
    void UpdateRotation();
    void phase2mainState();
    void jumpState();
    void fireState();
    void fireState2();
    void fakeDeath();
    void switchSide();

    void spawnFire(s8 speed, bool fire);

    static constexpr ObjectBank objectBank = {5, 9};
    static constexpr u16 objectID = ObjectID::Lemmy;

    static constexpr ObjectInfo objectInfo = {
        0, 0,
        16, 16,
        0, 0,
        16, 16,
        CollisionSwitch::None
    };

    static constexpr u16 updatePriority = objectID;
    static constexpr u16 renderPriority = 0x0000;
    static constexpr ActorProfile profile = {&constructObject<Lemmy>, updatePriority, renderPriority, loadResources};

    static void activeCallback(ActiveCollider& self, ActiveCollider& other);
    static const ActiveColliderInfo activeColliderInfoMain;
    static const ActiveColliderInfo activeColliderInfo2;
    static const LineSensorH bottomSensor;
    static const LineSensorV sideSensor;

    StateFunction updateFunction;
    s8 updateStep;
    s16 BossHealth;
    s16 BossHealth2;
    u16 exitTimer;
    u16 cutSceneTimer;
    s16 recoveryTimer;
    s8 direction;
    bool startShellSlide;

    Vec3 spawnPos;
    bool phase2;
    bool deathAnim;
    bool updateModelPos;
    bool posLocked;

    bool inShell;
    bool onRight;
    bool cutscene;

    u8 FightState;
    u8 mainStateState;
    u8 maxFireChain;
    u8 FireBallChainNum;
    s16 timeCrap;

    u32* ViewStart = (u32*)0x20CAE60;
    u32* ViewEnd = (u32*)0x20CAE64;

    void activateEvent(u32 id) {
        u64* activatorBitmask = (u64*)(0x0208AF3C);
        *activatorBitmask |= 1LL << (id - 1);
    }

    enum LemmyAnimation_ {
        Wait,
        Through_Wait,
        Through_Start,
        Through_End_Wait,
        Through_End,
        Shell_Out,
        Shell_In,
        Jump_Start,
        idk,
        Jump_End,
        Jump_A,
        Demo_Escape_3,
        Demo_Escape_2,
        Demo_Escape_1,
        Demo_Battle_A_Wait,
        Demo_Battle_A_Start,
        Demo_Battle_A,
        Damaged,
        Damaged_Start,
        Chase,
    };

private:
    static constexpr u32 LemmyID = 2373 - 131;
    static constexpr u32 LemmyShellId = 2410 - 131;
    static constexpr u32 LemmyAnimationID = 2374 - 131;
    static constexpr u32 Ball1ModelID = 2375 - 131;
    static constexpr u32 Ball1AnimID = 2379 - 131;
    static constexpr u32 carId = 2412 - 131;
    static constexpr u32 carAnmId = 2413 - 131;

    Model LemmyShell;
    ModelAnm modelLemmy;
    ModelAnm modelCar;

    Vec3 LemmyScale;
    Vec3 LemmyPosition;
    Vec3 carPos;

    Actor* ball;

    ActiveCollider activeColliderMain;
    ActiveCollider activeCollider2;
};
