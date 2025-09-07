#include "nsmb.hpp"

class smallBurnerExt : public StageEntity3DAnm {
public:
    u8 smallBurner[0x5ec /* sizeof(smallBurner) */ - sizeof(StageEntity3DAnm)];
    LineController lineCtrl;
};

class largeBurnerExt : public StageEntity3DAnm {
public:
    u8 largeBurner[0x5f4 /* sizeof(largeBurner) */ - sizeof(StageEntity3DAnm)];
    LineController lineCtrl;
};

ncp_jump(0x02185E38, 101)
NTR_NAKED static void smallBurner_ctorSize_hook() {
    asm(R"(
        LDR     R0, =%0
        B       0x02185E38 + 4
    )" : : "i" (sizeof(smallBurnerExt)) : "r0");
}

ncp_jump(0x02186B60, 101)
NTR_NAKED static void largeBurner_ctorSize_hook() {
    asm(R"(
        LDR     R0, =%0
        B       0x02186B60 + 4
    )" : : "i" (sizeof(largeBurnerExt)) : "r0");
}

ncp_hook(0x021859A4, 101)
void smallBurnerOnCreateHook(smallBurnerExt* self){
    u8 speed = self->settings>>12 & 0xf;
    if(self->settings>>8 & 0x1){
        Vec2 pos;
        pos = Vec2(self->position.x,self->position.y);
        self->lineCtrl.create(pos,0.5fx*speed);
        self->lineCtrl.start();
    }
}

ncp_hook(0x0218665C, 101)
void largeBurnerOnCreateHook(largeBurnerExt* self){
    u8 speed = self->settings>>12 & 0xf;
    if(self->settings>>8 & 0x1){
        Vec2 pos;
        pos = Vec2(self->position.x,self->position.y);
        self->lineCtrl.create(pos,0.5fx*speed);
        self->lineCtrl.start();
    }
}
ncp_hook(0x021857AC, 101)
void smallBurnerOnUpdateHook(smallBurnerExt* self){
    if(self->settings>>8 & 0x1){
        Vec2 pos;
        self->lineCtrl.update();
        pos = self->lineCtrl.getPosition();
        self->position.x = pos.x;
        self->position.y = pos.y;
    }  
}

ncp_hook(0x021863F4, 101)
void largeBurnerOnUpdateHook(largeBurnerExt* self){
    if(self->settings>>8 & 0x1){
        Vec2 pos;
        self->lineCtrl.update();
        pos = self->lineCtrl.getPosition();
        self->position.x = pos.x;
        self->position.y = pos.y;
    }  
}