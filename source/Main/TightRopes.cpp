#include "nsmb.hpp"

ncp_hook(0x02187D94, 98)
void tightRopeHook(StageEntity* self){
    u8 settingsCrap = self->settings>>0 & 0xf;
    switch (settingsCrap){
        case 1:
            self->position.y += Math::sin(self->accel.z);
            break;
        case 2:
            self->position.y += -1*Math::sin(self->accel.z);
            break;
        case 3:
            self->position.y += 2*Math::sin(self->accel.z);
            break;
        case 4:         
            self->position.y += -2*Math::sin(self->accel.z);
            break;
        case 5:
            self->position.x += Math::cos(self->accel.z);
            break;
        case 6:
            self->position.x += -1*Math::cos(self->accel.z);

            break;
        case 7:
            self->position.x += 2*Math::cos(self->accel.z);
            break;
        case 8:   
            self->position.x += -2*Math::cos(self->accel.z);
            break;
        case 9:
            self->position.y += 0x800;
            break;



    }
    self->accel.z += 360;
}


ncp_hook(0x02188514, 98)
void tightRopeHook2(StageEntity* self){
        self->accel.z = 0;
}

