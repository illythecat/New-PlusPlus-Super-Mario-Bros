#include "Crates.hpp"





bool Crates::onPrepareResources(){
    void* nsbtxFile = FS::Cache::loadFile(2351 - 131, false);

    u8 length = settings>>0 & 0xf;
    u8 width = settings>>4 & 0xf;



	CrateNsbtx[0].setup(nsbtxFile, Vec2(16, 16), Vec2(0, 0), 0, 0);
	CrateNsbtx[1].setup(nsbtxFile, Vec2(16*(length), 16), Vec2(0, 0), 1, 1);
	CrateNsbtx[2].setup(nsbtxFile, Vec2(16, 16), Vec2(0, 0), 2, 2);
	CrateNsbtx[3].setup(nsbtxFile, Vec2(16, 16*(width)), Vec2(0, 0), 3, 3);
	CrateNsbtx[4].setup(nsbtxFile, Vec2(16*(length), 16*(width)), Vec2(0, 0), 4, 4);
	CrateNsbtx[5].setup(nsbtxFile, Vec2(16, 16*(width)), Vec2(0, 0), 5, 5);
	CrateNsbtx[6].setup(nsbtxFile, Vec2(16, 16), Vec2(0, 0), 6, 6);
	CrateNsbtx[7].setup(nsbtxFile, Vec2(16*(length), 16), Vec2(0, 0), 7, 7);
	CrateNsbtx[8].setup(nsbtxFile, Vec2(16, 16), Vec2(0, 0), 8, 8);



    return 1;
}

bool Crates::loadResources() {
    return true;
}

void Crates::onMegaGroundPound(){
    return;
}

s32 Crates::onCreate(){
    void* crate = FS::Cache::loadFile(2351 - 131, false);


    
    onPrepareResources();
	loadResources();

    return 1;

}


bool Crates::updateMain(){
    if(breakAnim){
        Vec3 Position;
        Position.x = this->position.x + 0x10000;
        Position.y = this->position.y - 0x8000;
        Position.z = 0;
        if(settings>>8 & 0xf != 0){
            Actor::spawnActor(31, (settings>>12 & 0xf)*0x111100000 + ((settings>>8 & 0xf) - 0x100),  &this->position);
        }
        this->Base::destroy();

    }

    ColliderInfo colliderCrates = {
	0.0fx,  //lefr
	32.0fx + 16.0fx*(settings>>4 & 0xf), //top
	32.0fx + 16.0fx*(settings>>0 & 0xf),//right
	0.0fx, //bottom
	
	topColliderCallback,
	bottomColliderCallback,
	sideColliderCallback
    };

    ColliderInfo colliderInfo;
    colliderInfo = colliderCrates;
    collider.init(this, colliderInfo, 0, 0);
    collider.link();

    LineSensorH bottomSensor = {
    2.0fx, // Start position from the origin (left)
    30.0fx + 15.0fx*(settings>>0 & 0xf), // End position from the origin (right)
    0.0fx // Y Offset (negative = down)
    };

    collisionMgr.init(this, &bottomSensor, nullptr, nullptr); 

    updateVerticalVelocity(); // updates gravity
	applyMovement(); // applies velocity
	updateCollisionSensors(); // clamps position due to collision 
    return true;      
}

s32 Crates::onRender()
{
    u8 length = settings>>0 & 0xf;
    u8 width = settings>>4 & 0xf;

     newPos[6] = Vec3(position.x, position.y, position.z+0x10);
     newPos[7] = Vec3(position.x+16.0fx, position.y, position.z+0x10);
     newPos[8] = Vec3(position.x+16.0fx*(length+1), position.y, position.z+0x20);
     newPos[3] = Vec3(position.x, position.y+16.0fx, position.z-0x10);
     newPos[4] = Vec3(position.x+16.0fx, position.y+16.0fx, position.z-0x10);
     newPos[5] = Vec3(position.x+16.0fx*(length+1), position.y+16.0fx, position.z-0x10);
     newPos[0] = Vec3(position.x, position.y+16.0fx*(width+1), position.z+0x10);
     newPos[1] = Vec3(position.x+16.0fx, position.y+16.0fx*(width+1), position.z+0x10);
     newPos[2] = Vec3(position.x+16.0fx*(length+1), position.y+16.0fx*(width+1), position.z+0x20);

    Vec3 scale(1fx);
    for (int i = 0; i < 10; i++) {
        CrateNsbtx[i].render(newPos[i], scale);
    
    }
	return 1;
}

s32 Crates::onDestroy(){
        activateEvent(settings>>16 & 0xff);
        for (int i = 0; i < 10; i+=2) {
            Particle::Handler::createParticle(107, newPos[i]);
        }
        SND::playSFXHold(363, 0);


		return 1;
}



void Crates::topColliderCallback(StageActor& self, StageActor& other)
{
	Player& player = static_cast<Player&>(other);
    StageEntity& actor = static_cast<StageEntity&>(other);
    Crates& c = static_cast<Crates&>(self);

    if ((player.subActionFlag.groundPounding||player.subActionFlag.drillSpin)||(player.currentPowerup == PowerupState::Mega)){
		if((actor.id == 21))c.breakAnim = true;
    }
    if((actor.id == 37)||(actor.id == 38)||(actor.id == 0x182 + 0x1c))c.breakAnim = true;
	return;
}

void Crates::sideColliderCallback(StageActor& self, StageActor& other)
{
    Player& player = static_cast<Player&>(other);
    StageEntity& actor = static_cast<StageEntity&>(other);
    Crates& c = static_cast<Crates&>(self);
	if((actor.id == 0x182 + 0x1c))c.breakAnim = true;
    if ((player.actionFlag.inShell)||(player.currentPowerup == PowerupState::Mega)){
		if((actor.id == 21))
            c.breakAnim = true;
    }
    if (bool(other.collisionMgr.sideSensor->flags & CollisionMgr::SensorFlags::ActivateQuestionBlocks))
        c.breakAnim = true;


	return;
}

void Crates::bottomColliderCallback(StageActor& self, StageActor& other)
{
    Player& player = static_cast<Player&>(other);
    StageEntity& actor = static_cast<StageEntity&>(other);
    Crates& c = static_cast<Crates&>(self);
	if((actor.id == 21)||(actor.id == 0x182 + 0x1c))c.breakAnim = true;
    
	return;
}
