#include "nsmb.hpp"

//bombDropKoopas
ncp_hook(0x020e04dc, 10)
void bombdrop(StageEntity* koops){
    if((koops->settings >> 16) == 0x1){
        koops->accel.z++;
        if(koops->accel.z > 120){
            Actor::spawnActor(35, 0x20000100, &koops->position, 0, 0, 0);
	        koops->accel.z =0;
            }
	    }
	if((koops->settings >> 16) == 0x2){
        koops->position.y += 0x1500;
    }
		return;
}

ncp_call(0x020DDF2C, 10)
NTR_NAKED  void noblueshellparakoopa() {asm(R"(
BL 0x20DDFAC
BX LR
)");}

ncp_jump(0x020DDE70, 10)
NTR_NAKED  void noblueshellfromkoopa1() {asm(R"(
B  0x20DF754
)");}

//itemtex   
ncp_call(0x020D123C, 10)                                           
NTR_NAKED  void item1() {asm(R"(
    MOV     R1, R6
    MOV     R6, #8
    STR     R6, [SP]
    MOV     R6, R1
    BX      LR
)");}

ncp_call(0x0215F27C, 54)                                                
NTR_NAKED  void item2() {asm(R"(
    MOV     R5, #8
    STR     R5, [SP]
    MOV     R5, #0
    BX      LR
)");}

ncp_call(0x02157BF4, 54)                                              
NTR_NAKED  void RouletteBlock() {asm(R"(      
    MOV     R2, #8
    STR     R2, [SP]
    MOV     R2, #0
    BX      LR
)");}


//Music Limits
ncp_repl(0x02011CF8, "cmp  r5,#0xff")
ncp_repl(0x02011ebc, "cmp  r0,#0xff")

//ncp_repl(0x02011f18, "ands       r1 ,r4 ,#0xff")
ncp_repl(0x02011f18, "ands       r1 ,r4 ,#0x0")

ncp_repl(0x02118840, 10, "cmp        r0, #0xff")
ncp_repl(0x0201e798, "cmp        r1 ,#0xff")

//bobombSpeedSetting
ncp_call(0x0214D008, 0x2a)
NTR_NAKED  void bombspeed() {asm(R"(      
    LDR     R0, [R4, #8]
    LSR     R0, R0, #8
    ANDS    R0, R0, #0xF
    MOVEQ   R3, #0xF0
    MOVNE   R3, #0x25
    MULNE   R3, R0
    BX      LR
)");}

//movingLog
ncp_hook(0x0217DCA4, 87)
void spinlog(StageEntity* log){
    if((log->settings >> 16) & 0x1){
		log->position.x +=0x800;
		log->rotation.x += 0x100;
	}

}

ncp_over(0x021445A0, 33)
u32 newWiglerSegmentDistences[5] = {
     0xa, 0x1a, 0x2a, 0x3c, 0x0
};


ncp_over(0x021445B4, 33)
u32 newWiglerSegmentDistences2[4] = {
     0x6, 0x11, 0x1a, 0x24
};
