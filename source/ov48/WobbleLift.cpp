#include "nsmb.hpp"

ncp_hook(0x02146F50, 48)
void wobbleLiftHook(StageEntity* self){
    s16 HorzAmplitude  = self->settings>>12 & 0xf;
    s16 VertAmplitude  = self->settings>>16 & 0xf;
    s16 Speed          = self->settings>>20 & 0xf;
    s16 Period         = self->settings>>24 & 0xf;
    s16 OffsetH        = self->settings>>28 & 0xf;
    self->accel.z    += Period*180;
    self->position.x += HorzAmplitude*Math::cos(Speed*self->accel.z + (OffsetH*2.0fx)) ;
    self->position.y += VertAmplitude*Math::sin(Speed*self->accel.z);
}