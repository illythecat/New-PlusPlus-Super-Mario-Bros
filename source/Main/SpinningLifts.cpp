#include "nsmb.hpp"

ncp_hook(0x0217EBE8, 88)
void liftSteroids(StageEntity3D* lifts){
    lifts->accel.z += 180;
    u16 rotSpeed = lifts->settings & 0xf;
    if(lifts->settings>>4 & 0x1){
        if(lifts->settings>>16 & 0x1){
            lifts->rotation.z += 0x80;
            lifts->rotation.z -= 0.1*Math::sin(lifts->accel.z*(rotSpeed + 1));
        }
        else{
            lifts->rotation.z -= 0x80;
            lifts->rotation.z += 0.1*Math::sin(lifts->accel.z*(rotSpeed + 1));
        }
    }
    else{
        if(lifts->settings>>16 & 0x1)
            lifts->rotation.z -= 0x20 * rotSpeed;
        else
            lifts->rotation.z += 0x20 * rotSpeed;
    }
    
}

