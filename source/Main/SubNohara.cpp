#include "SubNohara.hpp"


const LineSensorH SubNohara::bottomSensor = {
    -9.5fx, // Start position from the origin (left)
    9.5fx, // End position from the origin (right)
    0.0fx // Y Offset (negative = down)
};

bool SubNohara::loadResources(){

     return 1;
}


s32 SubNohara::onCreate(){

    if(settings == 1){
        FS::Cache::loadFile(2425 - 131, false);
	    void* modelFile = FS::Cache::getFile(2425 - 131);
	    model.create(modelFile, 0, 0);
    }
    else{
        FS::Cache::loadFile(2424 - 131, false);
	    void* modelFile = FS::Cache::getFile(2424 - 131);
	    model.create(modelFile, 0, 0);
    }

    fogFlag = false;
	alpha = 31;
	renderOffset = {0, 0};
	rotationTranslation = {0, 0};


	collisionMgr.init(this, &bottomSensor, nullptr, nullptr); 
    return 1;

}


bool SubNohara::updateMain(){
    Player *player;
    player = Game::getPlayer(0);
    this->scale = {460.0fx, 105.0fx, 105.0fx};
    rotation.y = -4.0fx;
    position.z = -300.0fx;
    updateVerticalVelocity(); // updates gravity
	applyMovement(); // applies velocity
	updateCollisionSensors(); // clamps position due to collision  
    return true;      
}



s32 SubNohara::onDestroy(){
		return 1;
}






    