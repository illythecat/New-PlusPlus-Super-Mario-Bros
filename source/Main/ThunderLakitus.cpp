#include "nsmb.hpp"


s32 thunderCoolDown = 0;
bool actorSpawned = 0;





ncp_repl(0x02177658, 66, "mov   r0, #0x3000")
ncp_repl(0x0217760C, 66, "mov   r0, #0x3000")


// adds cloud (no lakitu) spawn option
ncp_call(0x02177234,66) // Lakitu::onCreate
static void newLakituSpawnState(Lakitu* lakitu, Lakitu::StateFunction defaultFunc) {
    if ((lakitu->settings >> 8 & 0xf) == 2) {
        lakitu->detachLakitu();
        lakitu->initializeCloud();
        lakitu->skipRenderModels.lakitu = true;
        lakitu->rotation.y = 0;
        lakitu->switchState(&Lakitu::idleState);
    } else {
        lakitu->switchState(defaultFunc);
    }
}




ncp_hook(0x02177260, 66)
void playerthunderLakitu(StageEntity* lakitu){
    Player *player;
    player = Game::getPlayer(0);

    Vec3 pos;
    pos.x = lakitu->position.x;
	pos.y = lakitu->position.y - 0x10000;

    if(thunderCoolDown != 0)
        thunderCoolDown--;

    if((actorSpawned == false) && (Input::playerKeysPressed[player->playerID] & Keys::L) && thunderCoolDown == 0){
        lakitu->velocity = 0;
        Actor::spawnActor(97, 0x00000001, &pos, 0, 0, 0);
        asm("STR     R0, [R0,#0x3FC]");		
        thunderCoolDown = 60;
    }
}



void thunderLakitu(StageEntity* lakitu ){
    if(lakitu->accel.z == 0)
        lakitu->position.x += 7.0fx;
    lakitu->accel.z += 1;
    if(lakitu->accel.z >((lakitu->settings>>4 & 0xf)* 30)-10){
        lakitu->scale.x += 50;
        lakitu->scale.y -= 50;
    }
    else if(lakitu->accel.z >((lakitu->settings>>4 & 0xf)* 30)-60){
        //SND::playSFXHold(SeqArc::SAR_THUNDER::SE_EMY_ELCJ_CHARGE);
        lakitu->scale.x -= 10;
        lakitu->scale.y += 10;
        
    }
    if(lakitu->accel.z > (lakitu->settings>>4 & 0xf)* 30){

        Vec3 pos;
        pos.x = lakitu->position.x;
	    pos.y = lakitu->position.y - 0x10000;

        lakitu->velocity = 0;
        Actor::spawnActor(97, 0x00000001, &pos, 0, 0, 0);
        asm("STR     R0, [R0,#0x3FC]");		
        lakitu->accel.z = 1;
    }

}

ncp_hook(0x02176FC4, 66)
void rotateLakitu(StageEntity* lakitu ){
    Player *player;
    player = Game::getPlayer(0);
    if(lakitu->settings>>4 == 10){
        /*
        lakitu->position = LakiHelper1pos;
        lakitu->scale = LakiHelper1scale;
        lakitu->rotation = LakiHelper1rot;
        */
    }
    else if(lakitu->settings>>4 == 11){
        /*
        lakitu->position = LakiHelper2pos;
        lakitu->scale = LakiHelper1scale;
        lakitu->rotation = LakiHelper1rot;
        */
    }

    else if(lakitu->settings>>4 > 1){
        if(player->position.x != lakitu->position.x){
            thunderLakitu(lakitu);
            if(lakitu->rotation.y <0)
                lakitu->rotation.y += 0.1fx;

        }
    }
}


ncp_jump(0x0217725C, 66)
void EmptyCloud(StageEntity* lakitu ){

    if(lakitu->settings>>4 & 1){
        if((lakitu->settings >> 8 & 0xf) != 2)
            lakitu->position.x += 0x80000;
        asm(".int 0x02178A98");
    }
    else{
        asm(".int   0x02178AA0");
    }
}

ncp_hook(0x021729D8, 60)
void resetCloudSpeed(StageEntity*cloud){
    cloud->accel.z = 0;
}

ncp_hook(0x02172CC0, 60)
void CloudHook(StageEntity* cloud){
    s16 HorzAmplitude  = cloud->settings>>12 & 0xf;
    s16 VertAmplitude  = cloud->settings>>16 & 0xf;
    s16 Speed          = cloud->settings>>20 & 0xf;
    s16 Period         = cloud->settings>>24 & 0xf;
    s16 OffsetH        = cloud->settings>>28 & 0xf;
    cloud->accel.z    += Period*180;
    cloud->position.x += HorzAmplitude*Math::cos(Speed*cloud->accel.z + (OffsetH*2.0fx)) ;
    cloud->position.y += VertAmplitude*Math::sin(Speed*cloud->accel.z);
}






class LakithunderExt : public StageEntity3DAnm {
public:
    u8 lakithunder[0x5B0 /* sizeof(Lakithunder) */ - sizeof(StageEntity3DAnm)];
    Vec3 LakiHelperOnePosition;
    Vec3 LakiHelperTwoPosition;
    Vec3 LakiHelperThreePosition;
    Vec3 LakiHelperFourPosition;
    Vec3s LakiHelperRotation;
    Vec3 LakiHelperScale;
    Player* player;
    Player* player2;
    Player* player3;
    Player* player4;
    Player* Lightning1;
    Player* Lightning2;
    Player* Lightning3;
    Player* Lightning4;
    bool helpersSpawned;
    bool Test;
    bool ShowHelpers;
    int timer;
    int helperTimer;
    
};

ncp_jump(0x02133004, 17)
NTR_NAKED static void Lakithunder_ctorSize_hook() {
    asm(R"(
        LDR     R0, =%0
        B       0x02133008
    )" : : "i" (sizeof(LakithunderExt)) : "r0");
}



ncp_repl(0x0213276C, 17, "addeq r1, r1, #30")
ncp_repl(0x02132B2C, 17, "addeq r1, r1, #30")

ncp_call(0x0213196C, 17)
void newLakithynderBossMusic(){
        SND::playBGM(11, 0);

}
//ncp_repl(0x0213196C, 17, "mov  r0, #0x7")   // LakiThunder Boss music

ncp_hook(0x02130D4C, 17)
void LightningSpawned(LakithunderExt* laki){
    Vec3 pos1 = laki->player->position;
    Vec3 pos2 = laki->player2->position;

    laki->timer = 50;

    Particle::Handler::createParticle(0x520, laki->player->position);
    laki->Lightning1 = (Player *) Actor::spawnActor(97, 0x00000001, &pos1, 0, 0, 0);
    asm("STR     R0, [R0,#0x3FC]");	
    Particle::Handler::createParticle(0x520, laki->player2->position);
    laki->Lightning2 = (Player *) Actor::spawnActor(97, 0x00000001, &pos2, 0, 0, 0);
    asm("STR     R0, [R0,#0x3FC]");	

     if (Stage::challengeModeEnabled){
        Vec3 pos3 = laki->player3->position;
        Vec3 pos4 = laki->player4->position;

        Particle::Handler::createParticle(0x520, laki->player->position);
        laki->Lightning3 = (Player *) Actor::spawnActor(97, 0x00000001, &pos3, 0, 0, 0);
        asm("STR     R0, [R0,#0x3FC]");	

        Particle::Handler::createParticle(0x520, laki->player->position);
        laki->Lightning4 = (Player *) Actor::spawnActor(97, 0x00000001, &pos4, 0, 0, 0);
        asm("STR     R0, [R0,#0x3FC]");	


     }



}

ncp_hook(0x02131124, 17)
void SwoopEnd(LakithunderExt* laki){
    //laki->Timer = 100;
}
ncp_hook(0x021311E4, 17)
void BeginSwoop(LakithunderExt* laki){
    //laki->Timer = 100;

}
ncp_hook(0x021314DC, 17)
void StartofFight(LakithunderExt* laki){
    laki->ShowHelpers = true;

}



ncp_hook(0x0212F9BC, 17)
void extraLakithunderCrap(LakithunderExt* laki){
    laki->LakiHelperOnePosition.x = laki->position.x + 0x18000;
    laki->LakiHelperOnePosition.y = laki->position.y;
    laki->LakiHelperTwoPosition.x = laki->position.x - 0x18000;
    laki->LakiHelperTwoPosition.y = laki->position.y;
    if (Stage::challengeModeEnabled){
        laki->LakiHelperThreePosition.x = laki->position.x + 0x50000;
        laki->LakiHelperThreePosition.y = laki->position.y;
        laki->LakiHelperFourPosition.x = laki->position.x - 0x50000;
        laki->LakiHelperFourPosition.y = laki->position.y;
    }
    laki->LakiHelperRotation = laki->rotation;
    laki->LakiHelperScale = laki->scale;

    if(laki->Test == true){
        laki->player->scale = {0, 0, 0};
        laki->player2->scale = {0, 0, 0};
        if (Stage::challengeModeEnabled){
             laki->player3->scale = {0, 0, 0};
             laki->player4->scale = {0, 0, 0};
        }
        //laki->Timer +=1;
        if(laki->ShowHelpers){
            Particle::Handler::createParticle(87, laki->player->position);
            Particle::Handler::createParticle(87, laki->player2->position);
            if (Stage::challengeModeEnabled){
                Particle::Handler::createParticle(87, laki->player3->position);
                Particle::Handler::createParticle(87, laki->player4->position);
            }
            laki->Test = false;
        }
    }

    //player->position = laki->LakiHelperOnePosition;


    if(laki->helpersSpawned == 0){
        laki->player = (Player *) Actor::spawnActor(39, 0xA0, &laki->LakiHelperOnePosition);
        laki->player2 = (Player *) Actor::spawnActor(39, 0xA0, &laki->LakiHelperTwoPosition);
        if (Stage::challengeModeEnabled){
        laki->player3 = (Player *) Actor::spawnActor(39, 0xA0, &laki->LakiHelperThreePosition);
        laki->player4 = (Player *) Actor::spawnActor(39, 0xA0, &laki->LakiHelperFourPosition);
        }

        laki->timer = 0;
        laki->helperTimer = 0;
        laki->helpersSpawned = true;
        laki->ShowHelpers = false;
        laki->Test = true;
        
    }
    else if (laki->helpersSpawned == 1){
        laki->player->position = laki->LakiHelperOnePosition;
        laki->player->rotation = laki->LakiHelperRotation;
        laki->player2->position = laki->LakiHelperTwoPosition;
        laki->player2->rotation = laki->LakiHelperRotation;
        if (Stage::challengeModeEnabled){
            laki->player3->position = laki->LakiHelperThreePosition;
            laki->player3->rotation = laki->LakiHelperRotation;
            laki->player4->position = laki->LakiHelperFourPosition;
            laki->player4->rotation = laki->LakiHelperRotation;
        }
    }
    laki->helperTimer++;

    if(laki->helperTimer == 120){
        laki->player->destroy();
        laki->player = (Player *) Actor::spawnActor(39, 0xA0, &laki->LakiHelperOnePosition);
        laki->player2->destroy();
        laki->player2 = (Player *) Actor::spawnActor(39, 0xA0, &laki->LakiHelperTwoPosition);
        if (Stage::challengeModeEnabled){
            laki->player3->destroy();
            laki->player3 = (Player *) Actor::spawnActor(39, 0xA0, &laki->LakiHelperThreePosition);
            laki->player4->destroy();
            laki->player4 = (Player *) Actor::spawnActor(39, 0xA0, &laki->LakiHelperFourPosition);
            Particle::Handler::createParticle(87, laki->player3->position);
            Particle::Handler::createParticle(87, laki->player4->position);
        }
        Particle::Handler::createParticle(87, laki->player->position);
        Particle::Handler::createParticle(87, laki->player2->position);
        laki->helperTimer = 0;

    }

    if(laki->ShowHelpers){
       laki->player->scale = laki->LakiHelperScale;
       laki->player2->scale =  laki->LakiHelperScale;
       if (Stage::challengeModeEnabled){
            laki->player3->scale = laki->LakiHelperScale;
            laki->player4->scale =  laki->LakiHelperScale;
       }
    }
    
    if(laki->timer != 0){
        laki->timer--;
        laki->Lightning2->position.x = laki->player2->position.x;
        laki->Lightning1->position.x = laki->player->position.x;
        if (Stage::challengeModeEnabled){
            laki->Lightning3->position.x = laki->player3->position.x;
            laki->Lightning4->position.x = laki->player4->position.x;
        }

    }

}

ncp_hook(0x02132888, 17)
void LakiThunerOnStompHook(LakithunderExt* laki){
        laki->player->destroy();
        laki->player2->destroy();
        if (Stage::challengeModeEnabled){
            laki->player3->destroy();
            laki->player4->destroy();
            Particle::Handler::createParticle(87, laki->player3->position);
            Particle::Handler::createParticle(87, laki->player4->position);
        }
        Particle::Handler::createParticle(87, laki->player->position);
        Particle::Handler::createParticle(87, laki->player2->position);
        laki->helperTimer = 0;
}




                 

