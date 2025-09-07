#include "Collectable.hpp"

bool Collectable::onPrepareResources(){
        void* nsbtxFile1 = FS::Cache::getFile(2388 - 131);
        void* nsbtxFile2 = FS::Cache::getFile(2389 - 131);
        text[0].setup(nsbtxFile1, Vec2(128, 16), Vec2(0, 0), 0, 0);
        text[1].setup(nsbtxFile2, Vec2(64, 16), Vec2(0, 0), 0, 0);
    return 1;
}


void Collectable::activeCallback(ActiveCollider& self, ActiveCollider& other){

    Collectable* tp = static_cast<Collectable*>(self.owner);
    if(other.owner->actorType == ActorType::Player){
        Player* player = static_cast<Player*>(other.owner);
        tp->switchState(&Collectable::collectedState);

    }
}

const ActiveColliderInfo Collectable::activeColliderInfo = {

    0.0fx, 0.0fx,
    10.0fx, 10.fx,

    CollisionGroup::Entity,
    CollisionFlag::None,
    MAKE_GROUP_MASK(CollisionGroup::Player, CollisionGroup::PlayerSpecial),
    MAKE_FLAG_MASK_EX(CollisionFlag::None),
    0,

    Collectable::activeCallback

};
bool Collectable::loadResources(){
     FS::Cache::loadFile(2387 - 131, false);
     FS::Cache::loadFile(2388 - 131, false);
     FS::Cache::loadFile(2389 - 131, false);

     return 1;
}

 
s32 Collectable::onCreate(){
    void* tpmodel = FS::Cache::getFile(2387 - 131);

    
    Casette.create(tpmodel, 0, 0);
		
    scale = Vec3(180fx, 200fx, 180fx);
    rotation.x = 8.0fx;
    switchState(&Collectable::mainState);

    renderText = false;


    activeCollider.init(this, activeColliderInfo, 0);
    activeCollider.link();
    onPrepareResources();
    if ((static_cast<u8>(Save::mainSave.nodeFlags[Game::currentWorldNode * Game::stageGroup] ) & 0x20) == 0){

        NNS_G3dMdlSetMdlAlpha(Casette.data, 0, 31);
    }
    else    
        NNS_G3dMdlSetMdlAlpha(Casette.data, 0, 10);
    textPosOffset = -0xf0000;

    return 1;

}

void Collectable::switchState(StateFunction function) {
    if (updateFunction == function && !updateFunction) {
    }

    if (updateFunction) {
        updateStep = Func::Exit;
        (this->*updateFunction)();
    }

    updateFunction = function;
    updateStep = Func::Init;
    (this->*updateFunction)();

}

s32 Collectable::onRender()
{
    Player *player = Game::getPlayer(0); 

    if(renderText){
        Vec3 scale2 =  Vec3(1.0fx, 1.0fx, 1.0fx);
        textpos[0] = Vec3(Stage::cameraX[0] + 0x20000 + textPosOffset, player->position.y - 0x20000, 0x3f2000);
        textpos[1] = Vec3(Stage::cameraX[0] + 0xa0000 + textPosOffset,player->position.y - 0x20000, 0x3f2000);
        text[0].render(textpos[0],scale2);
        text[1].render(textpos[1],scale2);
    }
    MTX::identity(Casette.matrix);
    MTX::translate(Casette.matrix, position);
    MTX::rotate(Casette.matrix, rotation);
    Game::modelMatrix = Casette.matrix;
    Casette.render(&scale);


    
	return 1;
}

void Collectable::updateState(){
    (this->*updateFunction)();


}

void Collectable::mainState(){
    Player *player = Game::getPlayer(0); 
    if (updateStep == Func::Init) {
        timer = 0;
        updateStep = Func::Step(0);
        // Initializing state
        return;
    }
    if (updateStep == Func::Exit) {
        // Exiting state
        return;
    }
    timer++;
    if(timer < 64){
        rotation.y += 0x200;

    }
    else if(timer ==128){
        timer = 0;
    }

    Game::setPlayerLookAtPosition(position);
    Game::setPlayerLookingAtTarget(true);

}

void Collectable::exitState(){
    Player *player = Game::getPlayer(0); 
    if (updateStep == Func::Init) {
        timer = 0;
        updateStep = Func::Step(0);
        // Initializing state
        return;
    }
    if (updateStep == Func::Exit) {
        // Exiting state
        return;
    }
    if(timer != 100)
        timer++;
    if(timer < 59){
        textPosOffset += 0x8000;   
        player->position.y = storePlayerPos.y;
        player->position.x = storePlayerPos.x;    

    }
    if(timer == 60){
        Game::fader.fadingStopped = false;
        Game::fader.fadeFrames = 60;
		Game::fader.fadeMaskShape[playerID] = 7;
    	StageEntrance* respawnEntrance = &Entrance::overriddenEntrance[playerID];
    	Entrance::spawnEntranceID[playerID] = settings; // This is how the game does it at Entrance::overrideEntrance
		player->switchTransitionState(&Player::viewTransitState);

        SND::playBGM(SND::bgmSeqID, false);
    }
    if(timer == 62){
        position.x = player->position.x;
        position.y = player->position.y + 10.0fx;
        position.z = player->position.z - 10.0fx;

    }
    if(timer == 90){
        Save::mainSave.nodeFlags[Game::currentWorldNode * Game::stageGroup] |= 0x20;
 
        player->endCutscene();
        player->velocity.x = 0;
        player->velocity.y = 0;

    }
    else if (timer > 90){
        position.y += 6.0fx;
        rotation.y += 0.5fx;
    }
}

void Collectable::collectedState(){
    Player *player = Game::getPlayer(0); 
    if (updateStep == Func::Init) {
        SND::playSFX(0x11A, nullptr);
        activeCollider.unlink();
        player->beginCutscene(0);
        Game::fader.fadeFrames = 60;
        timer = 60;
		Game::fader.fadeMaskShape[playerID] = 7;
		Game::fader.fadingState[playerID] |= 0x28;
        player->position.z = 0x3f2000;
        position.z = 0x3f3350;
        rotation.y = 0;

        player->setAnimation(5, true,Player::FrameMode::Continue,0xb00,0);
        storePlayerPos = player->position;
        updateStep = Func::Step(0);
        SND::stopBGM(20);
        // Initializing state
        return;
    }
    if (updateStep == Func::Exit) {
        // Exiting state
        return;
    }
    timer--;

    if(timer == -2){
        Game::fader.fadingStopped = true;
        renderText = true;

    }
    if((renderText) && (textPosOffset < 0)){
        textPosOffset += 0x8000;

    }
    else {
        if(player->keysPressed & Keys::ABXY)
             switchState(&Collectable::exitState);
    }


    player->position.y = storePlayerPos.y;
    player->position.x = storePlayerPos.x;

    //rotation.y += 0x400;
    //position.y += 4.0fx;


}
/*
ncp_repl(0x020b1b48, 0, "b 0x020b1b94")
ncp_repl(0x01fffd08, "ldr r2, =0x400003c")
*/

		
		
		
		
		
		
		


bool Collectable::updateMain(){
    updateState();

    return true;      
}

s32 Collectable::onDestroy(){
		return 1;
}
