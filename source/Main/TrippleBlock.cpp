#include "TrippleBlock.hpp"


static ColliderInfo colliderTrippleBlock = {
	-24.0fx,
	8.0fx,
	24.0fx,
	-8.0fx,
	
	TrippleBlock::topColliderCallback,
	TrippleBlock::bottomColliderCallback,
	TrippleBlock::sideColliderCallback
};

static ColliderInfo colliderDoubleBlock = {
	-16.0fx,
	8.0fx,
	16.0fx,
	-8.0fx,
	
	TrippleBlock::topColliderCallback,
	TrippleBlock::bottomColliderCallback,
	TrippleBlock::sideColliderCallback
};

bool TrippleBlock::loadResources(){
     FS::Cache::loadFile(2119 - 131, false);
     FS::Cache::loadFile(2120 - 131, false);
     FS::Cache::loadFile(2136 - 131, false);
     FS::Cache::loadFile(2179 - 131, false);
     FS::Cache::loadFile(2180 - 131, false);
     FS::Cache::loadFile(2181 - 131, false);
     return 1;
}

 
s32 TrippleBlock::onCreate(){
    void* DBmodel = FS::Cache::loadFile(2119 - 131, false);
    void* TBmodel = FS::Cache::loadFile(2136 - 131, false);
    void* BDBmodel = FS::Cache::loadFile(2180 - 131, false);
    void* BTBmodel = FS::Cache::loadFile(2179 - 131, false);

    ColliderInfo colliderInfo;
    SND::Internal::loadSeq(18);


    if((this->settings>>12) & 0x1 == 1){
        colliderInfo = colliderTrippleBlock;
        if((this->settings>>16) & 0x1 == 1)
            model.create(BTBmodel, 0, 0);
        else
            model.create(TBmodel, 0, 0);
    }
    else{
        colliderInfo = colliderDoubleBlock;
        if((this->settings>>16) & 0x1 == 1)
            model.create(BDBmodel, 0, 0);
        else
            model.create(DBmodel, 0, 0);
    }


    offsetTimer = 0;
    offsetOffsetTimer = 7;
    hit = true;
    hitAnim = false;
    movetimer = 0;
    groundPounded = false;
    
    if((this->settings>>12) & 0x1)
        this->scale = {0, 0x8000, 0x15000};
    else
        this->scale = {0, 0x8000, 0xDDC2};

    this->rotation.y = 9.0fx;
    this->position += {7.9fx, -7.6fx, 0.0fx};
    if((this->settings>>20) & 0x1)
        this->position.x += 0x8000;

    collider.init(this, colliderInfo, 0, 0);
    collider.link();

    return 1;

}
void TrippleBlock::textureAnim(){
    offsetOffsetTimer++;
    
    if(offsetOffsetTimer == 8){
        offsetTimer -= 1;
        offsetOffsetTimer = 0;
    }
    
    if(offsetTimer == -1)
        offsetTimer = 7;
    
    if(hit == false){
        if((this->settings>>16) & 0x1 == 1){
            void* TBmodel = FS::Cache::loadFile(2181 - 131, false);
            model.create(TBmodel, 0, 0);
        }
        else{
            void* TBmodel = FS::Cache::loadFile(2120 - 131, false);
            model.create(TBmodel, 0, 0);
        } 
    }

    NNSG3dResMatData* mat = model.getMaterialData(0);
    u32 texParams;
    u32 palParams;

    G3D::getTextureParams(model.texture, offsetTimer*hit, texParams);
    G3D::getPaletteParams(model.texture, offsetTimer*hit, palParams);
    mat->texImageParam = texParams;
    mat->texPlttBase = palParams;

}

void TrippleBlock::trippleBlockSpawn(){
    Vec3 Position;
    Position.x = this->position.x + 0x10000;
    Position.y = this->position.y - 0x8000;
    Position.z = 0;

    u32 groundPoundOffset = 0;
    if (groundPounded) 
        groundPoundOffset = 0x000C0000;

    for (int i = 0; i < 3; i++)
    {
        Actor::spawnActor(31, (((this->settings >> (4*i)) & 0xF)| groundPoundOffset), &Position, 0, 0, 0);
        Position.x -= 0x10000;
        if (((this->settings >> (4*i)) & 0xF) == 2) 
           SND::playSFX(SeqArc::SE_OBJ_GET_COIN, &Position);
        else 
            SND::playSFX(SeqArc::SE_OBJ_ITEM_APPEAR, &Position);
    }
    movetimer = 0;
    hit = false;
    hitAnim = false;
}

void TrippleBlock::doubleBlockSpawn(){
    Vec3 Position;
    Position.x = this->position.x + 0x8000;
    Position.y = this->position.y - 0x8000;
    Position.z = 0;

    u32 groundPoundOffset = 0;
    if (groundPounded) 
        groundPoundOffset = 0x000C0000;

    for (int i = 0; i < 2; i++)
    {
        Actor::spawnActor(31, (((this->settings >> (4*i)) & 0xF)| groundPoundOffset), &Position, 0, 0, 0);
        Position.x -= 0x10000;
        if (((this->settings >> (4*i)) & 0xF) == 2) 
           SND::playSFX(SeqArc::SE_OBJ_GET_COIN, &Position);
        else 
            SND::playSFX(SeqArc::SE_OBJ_ITEM_APPEAR, &Position);
    }
    movetimer = 0;
    hit = false;
    hitAnim = false;
}

void TrippleBlock::onHit(){
    movetimer++;

    if(movetimer < 6){
         if (groundPounded)
            this->position.y -= 1.0fx; 
         else
            this->position.y += 1.0fx; 
    }
    else {
        if (groundPounded)
            this->position.y += 1.0fx; 
        else
            this->position.y -= 1.0fx; 
    }

    if (movetimer == 10) 
    {
        if((this->settings>>12) & 0x1)
            trippleBlockSpawn();
        else
            doubleBlockSpawn();

    }
}


bool TrippleBlock::updateMain(){
    textureAnim();

    if(hitAnim)
        onHit();

    return true;      
}

s32 TrippleBlock::onRender()
{

	MTX::identity(model.matrix);
	MTX::translate(model.matrix, position.x, position.y, position.z);
	MTX::rotate(model.matrix, rotation.x, rotation.y, rotation.z);
	Game::modelMatrix = model.matrix;
	model.render(&scale);
	return 1;
}

s32 TrippleBlock::onDestroy(){
		return 1;
}



void TrippleBlock::topColliderCallback(StageActor& self, StageActor& other)
{
	TrippleBlock& tb = static_cast<TrippleBlock&>(self);
	Player& player = static_cast<Player&>(other);
    if ((player.subActionFlag.groundPounding||player.subActionFlag.drillSpin) && tb.hit == true){
		tb.hitAnim = true;
        tb.groundPounded = true;
    }
	return;
}

void TrippleBlock::bottomColliderCallback(StageActor& self, StageActor& other)
{
	TrippleBlock& tb = static_cast<TrippleBlock&>(self);
    if(tb.hit == true)
        tb.hitAnim = true;
	return;
}

void TrippleBlock::sideColliderCallback(StageActor& self, StageActor& other)
{
	return;
}
