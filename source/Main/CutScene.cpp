#include "nsmb.hpp"
#include "NSBTX.hpp"
#include "core.hpp"
/*
ModelAnm newTStest;
ModelAnm playerModel;

NSBTX TestNSBTX;
const static u32 testID = 2403 - 131;
const static u32 playerID = 1887 - 131;
const static u32 playerAnmID = 1893 - 131;
Vec3 playerPosition_ts;

AnimationCtrl UwUanimctrl;

static bool flip_flag = true;
static bool swap_ready = false;

ncp_hook(0x02013238)
static void additionalBootScene() {
	if ((Scene::nextSceneSettings == 0) && (Scene::nextSceneID == 4)) {
		Scene::switchScene(8);
	}
	return;
}

void newCodeTest2(Actor* self){
	self->position = {0, 0, 0};
	self->scale = {0x1000, 0x1000, 0x1000};
	self->rotation = {0, 0, 0};
	playerPosition_ts = Vec3(0x00, -0x28000, 0xf000);

	void* nsbtxFile = FS::Cache::loadFile(2402 - 131, false);
	TestNSBTX.setup(nsbtxFile, Vec2(256, 128), Vec2(128, 0), 0, 0);

	void* TestModel = FS::Cache::loadFile(testID, 0);
	void* FireModelAnm = FS::Cache::loadFile(2404 - 131, false);

	void* playerMod = FS::Cache::loadFile(playerID, true);
	void* playerAnm   = FS::Cache::loadFile(playerAnmID, true);
	playerModel.create(playerMod, playerAnm, 0, 0, 0);
	newTStest.create(TestModel, FireModelAnm, 0, 0, 0);
	UwUanimctrl.createCtrl(&newTStest.renderObj, FireModelAnm, 0, nullptr);
	newTStest.init(0, FrameCtrl::Looping, 1.0fx, 0);
	playerModel.init(21, FrameCtrl::Looping, 1.0fx, 0);
	//3 = run
	SND::initialize;
	NWAVPlayer::play(2401);
	self->rotation.y = 12.0fx;
	self->rotation.z = 0.21fx;
}

ncp_hook(0x020CD040, 8)
void moreShitUwU() {
	NNS_G3dGlbInit();
}

ncp_over(0x020CD8D0, 8)
bool nukeWorldCreate(Actor* self){
	newCodeTest2(self);
	return true;
}



ncp_over(0x020CD86C, 8)
bool nononono() {
	return true;
}

void newRenderTest(Actor* self) {
	//#define PLTT_SUB_BG  ((u16*)0x05000400)
	//MIi_CpuClearFast(0, PLTT_SUB_BG, 0x400);
		GX_SetDispSelect(GX_DISP_SELECT_MAIN_SUB);

		Vec3 nsbtxScale = Vec3(1.0fx, 13.0fx, 13.0fx);
		MTX::identity(newTStest.matrix);
		MTX::translate(newTStest.matrix, self->position);
		MTX::rotate(newTStest.matrix, self->rotation);
		Game::modelMatrix = newTStest.matrix;
		newTStest.render(&nsbtxScale);
		UwUanimctrl.update();


		MTX::identity(playerModel.matrix);
		MTX::translate(playerModel.matrix, playerPosition_ts);
		MTX::rotate(playerModel.matrix, self->rotation);
		Game::modelMatrix = playerModel.matrix;
		playerModel.render(&self->scale);
		playerModel.update();

		Vec3 nsbtxPos = Vec3(self->position.x - 0x80000, self->position.y - 0x8000, self->position.z + 0xf00);
		Vec3 scale = Vec3(0.43fx, 0.43fx, 0.42fx);
		TestNSBTX.render(nsbtxPos, scale);
		

	
}

ncp_jump(0x020CD668, 8)
bool nukeIconRingUpdate(Actor* self) {

	return true;
}

ncp_over(0x020CD6E8, 8)
bool nukeIconRingRender(Actor* self) {
	newRenderTest(self);
	return true;
}
*/