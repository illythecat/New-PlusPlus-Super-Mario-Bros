#include "nsmb.hpp"

class bills : public StageEntity3DAnm {
public:
    static const LineSensorH bottomSensor;
    void loadSensor();
    void updateHook();
};

const LineSensorH bills::bottomSensor = {
    -5.5fx, // Start position from the origin (left)
    5.5fx, // End position from the origin (right)
    0.0fx // Y Offset (negative = down)
};

ncp_hook(0x02147B4c, 42)
void bills::loadSensor(){
        collisionMgr.init(this, &bottomSensor, nullptr, nullptr); 
}

ncp_hook(0x02147AF0, 42)
void bills::updateHook(){
    if(this->settings>>0 & 0x1){
        applyMovement(); 
        updateVerticalVelocity(); 
        updateCollisionSensors(); 
    }
}

ncp_hook(0x0214B370, 42)
void bulletBlasterTurretCreateHook() {
    register StageEntity* self asm("r4");
        static LineSensorH bottomSensor{ -8.0fx, 7.0fx };
        self->collisionMgr.init(self,
            &bottomSensor,
            nullptr,
            nullptr
        );
}

ncp_hook(0x0214AF98, 42)
void bulletBlasterTurretUpdateHook(StageEntity* self) {
        self->updateVerticalVelocity();
        self->applyMovement();
        self->updateBottomSensor();
}
