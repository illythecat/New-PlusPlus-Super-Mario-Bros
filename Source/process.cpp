
// Ogirinall File by MammaMiaTeam, credit them if used

#include "nsmb/entity/object.hpp"


#include "actors/MultiBlocks/TrippleBlock.hpp"

namespace Game {

	const ObjectProfile* mainExtPT[] = {

		//&CheckPoint::profile,
		//&CannonBall::profile,
		//&BlackBowserJR::profile,
		//&TimePiece::profile,
        //&BigCoin::profile,
        //&BroozerBoss::profile,
        //&boolossus::profile,
        //&Fingor::profile,                   //stop putting this off already
        //&Ninji::profile,
        //&BowserHead::profile,
        //&Grinder::profile,                  //fix model
        //&FlyingMario::profile,
        //&Villager::profile,
        //&TorpedoTed::profile,
        //&BeachHut::profile,
        //&Decoration::profile,
        //&ScuttleBuddy::profile,
        //&GimmickController::profile,
        //&FlipBlock::profile,                //faild attempt, to be removed
        //&Larry::profile,      
        //&BowserJRCutscene::profile,
        &TrippleBlock::profile,
        //&KoopalingProjectile::profile,

	};

	constinit const ObjectProfile* const* currentExtPT = mainExtPT;

}


ncp_jump(0x0204C9FC) // Base::spawn
NTR_NAKED void extendBaseSpawn() {asm(R"(
    print    "Spawning base (ID: %r0% Settings: %r5%)\n"
    cmp        r0, #0x180
    ldrls    r0, =0x0208FB98
    ldrhi    r0, =_ZN4Game12currentExtPTE
    subhi    r7, r7, #0x180
    subhi    r7, r7, #0x2
    b        0x0204C9FC + 4
)");}

ncp_jump(0x0204D378) // Base::Base
NTR_NAKED void extendBaseCtor() {asm(R"(
    ldr        r1, [r4, #0xC] 
    cmp        r1, #0x180
    ldrls    r0, =0x0208FB98
    ldrhi    r0, =_ZN4Game12currentExtPTE
    subhi    r1, r1, #0x180
    subhi    r1, r1, #0x2
    b        0x0204D380
)");}

ncp_jump(0x0201F424) // StageScene::loadObjectsResources
NTR_NAKED void extendObjectsResourcesLoading() {asm(R"(
    print    "Loading resources (StageObj %r0% "
    bl        _ZN11StageEntity10getActorIDEt
    cmp        r0, #0x180
    ldrls    r7, =0x0208FB98
    ldrhi    r7, =_ZN4Game12currentExtPTE
    subhi    r0, r0, #0x180
    subhi    r0, r0, #0x2
    bls        noext
    print    "Extended %r0%)\n"
    b        0x0201F424 + 4
noext:
    print    "Standard %r0%)\n"
    b        0x0201F424 + 4
)");}


// Replace unused stage object entries

#define NSMB_OBJDECL(idAddr, infoAddr, bankAddr, id, class) \
ncp_over(idAddr, 0) static const u16 class##ObjID = class::objectID; \
ncp_over(infoAddr, 0) static const ObjectInfo class##ObjInfo = class::objectInfo; \
ncp_over(bankAddr, 0) static const ObjectBank class##ObjBank = class::objectBank;

#define NSMB_OBJDECL_EX(idAddr, bankAddr, class, id) \
ncp_over(idAddr, 0) static const u16 class##ObjID = class::objectID; \
ncp_over(bankAddr, 0) static const ObjectBank class##ObjBank = class::objectBank;

//         0x020C22B8  0x020C529C  0x020C5010  00
//NSMB_OBJDECL(0x020C22CC, 0x020C5364, 0x020C5024, 10, CheckPoint); // Stage object 10 will now spawn CheckPoint
//NSMB_OBJDECL(0x020C22CE, 0x020C5378, 0x020C5026, 11, CannonBall); // Here all the other unused slots you can use
//NSMB_OBJDECL(0x020C22D0, 0x020C538C, 0x020C5028, 12, BlackBowserJR);
//NSMB_OBJDECL(0x020C22D2, 0x020C53A0, 0x020C502A, 13, TimePiece);
//NSMB_OBJDECL(0x020C22D4, 0x020C53B4, 0x020C502C, 14, BigCoin);
//NSMB_OBJDECL(0x020C22D6, 0x020C53C8, 0x020C502E, 15, BroozerBoss);
//NSMB_OBJDECL(0x020C22D8, 0x020C53DC, 0x020C5030, 16, boolossus);
//NSMB_OBJDECL(0x020C22DA, 0x020C53F0, 0x020C5032, 17, Fingor);
//NSMB_OBJDECL(0x020C22DC, 0x020C5404, 0x020C5034, 18, Ninji);
//NSMB_OBJDECL(0x020C22DE, 0x020C5418, 0x020C5036, 19, BowserHead);
//NSMB_OBJDECL(0x020C2330, 0x020C574C, 0x020C5088, 60, Grinder);
//NSMB_OBJDECL(0x020C2332, 0x020C5760, 0x020C508A, 61, FlyingMario);
//NSMB_OBJDECL(0x020C235A, 0x020C58F0, 0x020C50B2, 81, Villager);
//NSMB_OBJDECL(0x020C2366, 0x020C5968, 0x020C50BE, 87, TorpedoTed);
//NSMB_OBJDECL(0x020C237A, 0x020C5A30, 0x020C50D2, 97, BeachHut);
//NSMB_OBJDECL(0x020C2398, 0x020C5B5C, 0x020C50F0, 112, Decoration);
//NSMB_OBJDECL(0x020C23AA, 0x020C5C10, 0x020C5102, 121, ScuttleBuddy);
//NSMB_OBJDECL(0x020C23C2, 0x020C5D00, 0x020C511A, 133, GimmickController);
//NSMB_OBJDECL(0x020C23C4, 0x020C5D14, 0x020C511C, 134, );
//NSMB_OBJDECL(0x020C23C6, 0x020C5D28, 0x020C511E, 135, FlipBlock);
//NSMB_OBJDECL(0x020C23CA, 0x020C5D50, 0x020C5122, 137, Larry);
//NSMB_OBJDECL(0x020C23CC, 0x020C5D64, 0x020C5124, 138, BowserJRCutscene);
NSMB_OBJDECL(0x020C23CE, 0x020C5D78, 0x020C5126, 139, TrippleBlock);
//NSMB_OBJDECL(0x020C23E6, 0x020C5E68, 0x020C513E, 151, KoopalingProjectile);
//NSMB_OBJDECL(0x020C23EA, 0x020C5E90, 0x020C5142, 153, );
//NSMB_OBJDECL(0x020C23EC, 0x020C5EA4, 0x020C5144, 154, );
//NSMB_OBJDECL(0x020C23F0, 0x020C5ECC, 0x020C5148, 156, );
//NSMB_OBJDECL(0x020C23F6, 0x020C5F08, 0x020C514E, 159, );
//NSMB_OBJDECL(0x020C23F8, 0x020C5F1C, 0x020C5150, 160, );
//NSMB_OBJDECL(0x020C23FA, 0x020C5F30, 0x020C5152, 161, );
//NSMB_OBJDECL(0x020C23FE, 0x020C5F58, 0x020C5156, 163, );
//NSMB_OBJDECL(0x020C240A, 0x020C5FD0, 0x020C5162, 169, );
//NSMB_OBJDECL(0x020C240C, 0x020C5FE4, 0x020C5164, 170, );
//NSMB_OBJDECL(0x020C240E, 0x020C5FF8, 0x020C5166, 171, );
//NSMB_OBJDECL(0x020C2410, 0x020C600C, 0x020C5168, 172, );
//NSMB_OBJDECL(0x020C2418, 0x020C605C, 0x020C5170, 176, );
//NSMB_OBJDECL(0x020C241A, 0x020C6070, 0x020C5172, 177, );
//NSMB_OBJDECL(0x020C241C, 0x020C6084, 0x020C5174, 178, );
//NSMB_OBJDECL(0x020C241E, 0x020C6098, 0x020C5176, 179, );
//NSMB_OBJDECL(0x020C2422, 0x020C60C0, 0x020C517A, 181, );
//NSMB_OBJDECL(0x020C2424, 0x020C60D4, 0x020C517C, 182, );
//NSMB_OBJDECL(0x020C2428, 0x020C60FC, 0x020C5180, 184, );
//NSMB_OBJDECL(0x020C2430, 0x020C614C, 0x020C5188, 188, );
//NSMB_OBJDECL(0x020C2434, 0x020C6174, 0x020C518C, 190, );
//NSMB_OBJDECL(0x020C2448, 0x020C623C, 0x020C51A0, 200, );
//NSMB_OBJDECL(0x020C244A, 0x020C6250, 0x020C51A2, 201, );
//NSMB_OBJDECL(0x020C244C, 0x020C6264, 0x020C51A4, 202, );
//NSMB_OBJDECL(0x020C2458, 0x020C62DC, 0x020C51B0, 208, );
//NSMB_OBJDECL(0x020C2464, 0x020C6354, 0x020C51BC, 214, );
//NSMB_OBJDECL(0x020C2466, 0x020C6368, 0x020C51BE, 215, );
//NSMB_OBJDECL(0x020C2468, 0x020C637C, 0x020C51C0, 216, );
//NSMB_OBJDECL(0x020C246A, 0x020C6390, 0x020C51C2, 217, );
//NSMB_OBJDECL(0x020C2472, 0x020C63E0, 0x020C51CA, 221, );
//NSMB_OBJDECL(0x020C247A, 0x020C6430, 0x020C51D2, 225, );
//NSMB_OBJDECL(0x020C2484, 0x020C6494, 0x020C51DC, 230, );
//NSMB_OBJDECL(0x020C2498, 0x020C655C, 0x020C51F0, 240, );
//NSMB_OBJDECL(0x020C24B2, 0x020C6660, 0x020C520A, 253, );
//NSMB_OBJDECL(0x020C24CE, 0x020C6778, 0x020C5226, 267, );
//NSMB_OBJDECL(0x020C24F8, 0x020C691C, 0x020C5250, 288, );
