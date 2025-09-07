#include "nsmb.hpp"

ncp_hook(0x2142544,32) // PipePiranha::onRender
void renderHook(Model* mdl, const VecFx32* scale) {
    NTR_REGISTER(PipePiranha*, pp, r4);
    pp->accel.z = pp->settings>>8;
    if(pp->accel.z == 1){
        NNSG3dResMatData* mat =mdl->getMaterialData(0);
        u32 texParams;
        u32 palParams;

        G3D::getTextureParams(mdl->texture, pp->accel.z, texParams);
        G3D::getPaletteParams(mdl->texture, pp->accel.z, palParams);
        mat->texImageParam = texParams + 1;
        mat->texPlttBase = palParams;
    }
    
}

static void fireballHitHook(PipePiranha* pp) {
    pp->accel.z = pp->settings>>8;
	if (pp->accel.z == 1)
		return;
	rcast<void(*)(PipePiranha*)>(0x214239c)(pp); // original onFireballHit

}

ncp_over(0x2143aac,32) // PipePiranha::vtable
static const auto onFireballHitVFunc = fireballHitHook;