#include "Clone.hpp"


const ActiveColliderInfo Clone::activeColliderInfo = {

    0.0fx, 5.0fx,
    5.0fx, 5.0fx,

    CollisionGroup::Entity,
    CollisionFlag::None,
    MAKE_GROUP_MASK(CollisionGroup::Player, CollisionGroup::PlayerSpecial, CollisionGroup::Fireball),
    MAKE_FLAG_MASK_EX(CollisionFlag::None),
    0,

    Clone::damagePlayerCallback

};

bool Clone::loadResources() {
	// Load model and animation files
	FS::Cache::loadFile(1887 - 131, true); // mario_model_LZ.bin
	FS::Cache::loadFile(1893 - 131, true); // pl_LZ.bin
	FS::Cache::loadFile(1895 - 131, true); // pl_mario_LZ.bin
	FS::Cache::loadFile(1878 - 131, true); // pl_mario_LZ.bin


	return true;
}

s32 Clone::onCreate() {
	void* modelFile = FS::Cache::getFile(1887 - 131);
	void* animFile  = FS::Cache::getFile(1893 - 131);
	void* headmodelw  = FS::Cache::getFile(1878- 131);
	smokeTimer = 0;

	modelPlayer.create(modelFile, animFile, 0, 0, 0);
	headModel.create(headmodelw, 0, 0);

	modelPlayer.pushAnimation(0, 5, FrameCtrl::Looping, 1.0fx, 0); // Default anim

	scale = Vec3(1.6fx, 1.6fx, 1.6fx);
	rotation = {0, 0, 0};

	targetPlayer = Game::getPlayer(0);

	for (int i = 0; i < 60; i++)
		positionHistory[i] = position;

	activeCollider.init(this, activeColliderInfo, 0);
    activeCollider.link();

	lastAnimationID = 0xFFFF;

	return 1;
}

bool Clone::updateMain() {
	void* modelFile = FS::Cache::getFile(1887 - 131);
	void* animFile  = FS::Cache::getFile(1893 - 131);
	void* animFile2  = FS::Cache::getFile(1895 - 131);

	smokeTimer++;
	if(smokeTimer == 10){
	    Particle::Handler::createParticle(136 , Vec3(position.x, position.y + 10.0fx, position.z - 10.0fx));
		smokeTimer = 0;
	}

	if (!targetPlayer)
		return true;

	// Update position history
	for (int i = 59; i > 0; --i)
		positionHistory[i] = positionHistory[i - 1];
	positionHistory[0] = targetPlayer->position;
	position = positionHistory[45];
	for (int i = 59; i > 0; --i)
		animationHistory[i] = animationHistory[i - 1];
	animationHistory[0] = targetPlayer->animID;
	assumedAnimID = animationHistory[45];

	// Sync scale and rotation
	rotation = targetPlayer->rotation;

	// Try to detect animation changes (fallback to default animID 0)

	if (assumedAnimID != lastAnimationID) {
		if(assumedAnimID == 2){
			modelPlayer.create(modelFile, animFile2, 0, 0, 0);
			modelPlayer.pushAnimation(0, 5, FrameCtrl::Looping, 1.0fx, 0);
		}
		else if(assumedAnimID == 1){
			modelPlayer.create(modelFile, animFile2, 0, 0, 0);
			modelPlayer.pushAnimation(2, 5, FrameCtrl::Looping, 1.0fx, 0);
		}
		else if(assumedAnimID == 0){
			modelPlayer.create(modelFile, animFile2, 0, 0, 0);
			modelPlayer.pushAnimation(1, 5, FrameCtrl::Looping, 1.0fx, 0);
		}
		else if(assumedAnimID == 3){
			modelPlayer.create(modelFile, animFile, 0, 0, 0);
			modelPlayer.pushAnimation(3, 5, FrameCtrl::Looping, 1.0fx, 0);
		}
		else if(assumedAnimID == 4){
			modelPlayer.create(modelFile, animFile, 0, 0, 0);
			modelPlayer.pushAnimation(4, 5, FrameCtrl::Looping, 1.0fx, 0);
		}
		else if(assumedAnimID == 5){
			modelPlayer.create(modelFile, animFile, 0, 0, 0);
			modelPlayer.pushAnimation(9, 5, FrameCtrl::Looping, 1.0fx, 0);
		}
		else if(assumedAnimID == 6){
			modelPlayer.create(modelFile, animFile, 0, 0, 0);
			modelPlayer.pushAnimation(10, 5, FrameCtrl::Looping, 1.0fx, 0);
		}
		else if(assumedAnimID == 7){
			modelPlayer.create(modelFile, animFile, 0, 0, 0);
			modelPlayer.pushAnimation(11, 5, FrameCtrl::Looping, 1.0fx, 0);
		}
		else if(assumedAnimID == 8){
			modelPlayer.create(modelFile, animFile, 0, 0, 0);
			modelPlayer.pushAnimation(0, 5, FrameCtrl::Looping, 1.0fx, 0);
		}
		else if(assumedAnimID == 9){
			modelPlayer.create(modelFile, animFile, 0, 0, 0);
			modelPlayer.pushAnimation(1, 5, FrameCtrl::Looping, 1.0fx, 0);
		}
		else if(assumedAnimID == 0xa){
			modelPlayer.create(modelFile, animFile, 0, 0, 0);
			modelPlayer.pushAnimation(2, 5, FrameCtrl::Looping, 1.0fx, 0);
		}
			else if(assumedAnimID == 0xb){
			modelPlayer.create(modelFile, animFile, 0, 0, 0);
			modelPlayer.pushAnimation(12, 5, FrameCtrl::Looping, 1.0fx, 0);
		}
			else if(assumedAnimID == 0xc){
			modelPlayer.create(modelFile, animFile, 0, 0, 0);
			modelPlayer.pushAnimation(15, 5, FrameCtrl::Looping, 1.0fx, 0);
		}
		else if(assumedAnimID == 0xd){
			modelPlayer.create(modelFile, animFile, 0, 0, 0);
			modelPlayer.pushAnimation(16, 5, FrameCtrl::Looping, 1.0fx, 0);
		}
		else if(assumedAnimID == 0xe){
			modelPlayer.create(modelFile, animFile, 0, 0, 0);
			modelPlayer.pushAnimation(8, 5, FrameCtrl::Looping, 1.0fx, 0);
		}
		else if(assumedAnimID == 0xf){
			modelPlayer.create(modelFile, animFile, 0, 0, 0);
			modelPlayer.pushAnimation(6, 5, FrameCtrl::Looping, 1.0fx, 0);
		}
		else if(assumedAnimID == 0x10){
			modelPlayer.create(modelFile, animFile, 0, 0, 0);
			modelPlayer.pushAnimation(7, 5, FrameCtrl::Looping, 1.0fx, 0);
		}
		else if(assumedAnimID == 0x11){
			modelPlayer.create(modelFile, animFile, 0, 0, 0);
			modelPlayer.pushAnimation(14, 5, FrameCtrl::Looping, 1.0fx, 0);
		}		
		else if(assumedAnimID == 0x12){
			modelPlayer.create(modelFile, animFile, 0, 0, 0);
			modelPlayer.pushAnimation(5, 5, FrameCtrl::Looping, 1.0fx, 0);
		}
		else if(assumedAnimID == 0x13){
			modelPlayer.create(modelFile, animFile, 0, 0, 0);
			modelPlayer.pushAnimation(17, 5, FrameCtrl::Looping, 1.0fx, 0);
		}
		else if(assumedAnimID == 0x15){
			modelPlayer.create(modelFile, animFile, 0, 0, 0);
			modelPlayer.pushAnimation(18, 5, FrameCtrl::Looping, 1.0fx, 0);
		}
		else if(assumedAnimID == 0x2f){
			modelPlayer.create(modelFile, animFile, 0, 0, 0);
			modelPlayer.pushAnimation(29, 5, FrameCtrl::Looping, 1.0fx, 0);
		}
		else if(assumedAnimID == 0x56){
			modelPlayer.create(modelFile, animFile, 0, 0, 0);
			modelPlayer.pushAnimation(13, 5, FrameCtrl::Looping, 1.0fx, 0);
		}
		else if(assumedAnimID == 0x57){
			modelPlayer.create(modelFile, animFile, 0, 0, 0);
			modelPlayer.pushAnimation(14, 5, FrameCtrl::Looping, 1.0fx, 0);
		}
		else{
			modelPlayer.create(modelFile, animFile, 0, 0, 0);
			modelPlayer.pushAnimation(0, 5, FrameCtrl::Looping, 1.0fx, 0);
		}
		lastAnimationID = assumedAnimID;
	}

	modelPlayer.update();
	return true;
}

s32 Clone::onRender() {
	MTX::identity(modelPlayer.matrix);
	MTX::translate(modelPlayer.matrix, position);
	MTX::rotate(modelPlayer.matrix, rotation);
	Game::modelMatrix = modelPlayer.matrix;

	modelPlayer.render(&scale);

    modelPlayer.getNodeMatrix(15, &headModel.matrix);
	Vec3 headScale = Vec3(1.2fx, 1.2fx, 1.2fx);
    headModel.render(&headScale);
	return true;
}

s32 Clone::onDestroy() {
	return 1;
}
