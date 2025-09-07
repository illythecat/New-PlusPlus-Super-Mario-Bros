#include "nsmb/entity/object.hpp"

#include "CheckPoint.hpp"
#include "ov61/CannonBall.hpp"
#include "ov83/BlackBowserJR.hpp"
#include "TimePiece.hpp"
#include "BigCoin.hpp"
#include "ov84/BroozerBoss.hpp"
#include "ov83/boolossus.hpp"
#include "ov61/Fingor.hpp"
#include "Ninji.hpp"
#include "ov61/BowserHead.hpp"
#include "BubblePowerUp.hpp"
#include "ov84/Villager.hpp"
#include "ov61/TorpedoTed.hpp"
#include "ov61/BeachHut.hpp"
#include "3dDecoration.hpp"
#include "ov84/ScuttleBuddy.hpp"
#include "GimmickController.hpp"
#include "ov83/Larry.hpp"
#include "ov61/BowserJRCutscene.hpp"
#include "TrippleBlock.hpp"
#include "KoopalingProjectile.hpp"
#include "waterBubble.hpp"
#include "RedStar.hpp"
#include "ov85/Roy.hpp"
#include "SuperBall.hpp"
#include "Hammer.hpp"
#include "ov84/Lemmy.hpp"
#include "ov84/LemmysBalls.hpp"
#include "FlyGuys.hpp"
#include "Crates.hpp"
#include "Kameck.hpp"
#include "ov76/Morton.hpp"
#include "ov62/Goonie.hpp"
#include "ov62/Clone.hpp"
#include "PowBlock.hpp"
#include "Collectable.hpp"
#include "TalkingFlower.hpp"
#include "SubNohara.hpp"
#include "ov61/hoppyCat.hpp"
#include "Spike.hpp"
#include "kingBill.hpp"





namespace Game {
	const ObjectProfile* mainExtPT[] = {
		&CheckPoint::profile,
		&CannonBall::profile,
		&BlackBowserJR::profile,
		&TimePiece::profile,
        &BigCoin::profile,
        &BroozerBoss::profile,
        &boolossus::profile,
        &Fingor::profile,                   //stop putting this off already
        &Ninji::profile,
        &BowserHead::profile,       
        0,      
        &BubblePowerUp::profile,
        &Villager::profile,
        &TorpedoTed::profile,
        &BeachHut::profile,
        &Decoration::profile,
        &ScuttleBuddy::profile,
        &GimmickController::profile,
        0,                //faild attempt, to be removed
        &Larry::profile,      
        &BowserJRCutscene::profile,
        &TrippleBlock::profile,
        &KoopalingProjectile::profile,
        &waterBubble::profile,
        &RedStar::profile,
        0,
        &Roy::profile,
        &SuperBall::profile,
        &Hammer::profile,
        &Lemmy::profile,
        &LemmysBalls::profile,
        &FlyGuys::profile,
        &Crates::profile,
        &Kameck::profile,
        &Morton::profile,
        &Goonie::profile,
        &Clone::profile,
        &PowBlock::profile,
        &Collectable::profile,
        &TalkingFlower::profile,
        &SubNohara::profile,
        &hoppyCat::profile,
        &Spike::profile,
        &kingBill::profile,

	};

	constinit const ObjectProfile* const* currentExtPT = mainExtPT;

}


ncp_jump(0x0204C9FC) // Base::spawn
NTR_NAKED void extendBaseSpawn() {asm(R"(
    print    "Spawning base (ID: %r0% Settings: %r5%)\n"
    cmp        r0, #0x180
    ldrls    r0, =0x0208FB98
    ldrgt    r0, =_ZN4Game12currentExtPTE
    subgt    r7, r7, #0x180
    subgt    r7, r7, #0x2
    b        0x0204C9FC + 4
)");}

ncp_jump(0x0204D378) // Base::Base
NTR_NAKED void extendBaseCtor() {asm(R"(
    ldr        r1, [r4, #0xC] 
    cmp        r1, #0x180
    ldrlt    r0, =0x0208FB98
    ldrgt    r0, =_ZN4Game12currentExtPTE
    subgt    r1, r1, #0x180
    subgt    r1, r1, #0x2
    b        0x0204D380
)");}

ncp_jump(0x0201F424) // StageScene::loadObjectsResources
NTR_NAKED void extendObjectsResourcesLoading() {asm(R"(
    print    "Loading resources (StageObj %r0% "
    bl        _ZN11StageEntity10getActorIDEt
    cmp        r0, #0x180
    ldrlt    r7, =0x0208FB98
    ldrgt    r7, =_ZN4Game12currentExtPTE
    subgt    r0, r0, #0x180
    subgt    r0, r0, #0x2
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
NSMB_OBJDECL(0x020C22CC, 0x020C5364, 0x020C5024, 10, CheckPoint); // Stage object 10 will now spawn CheckPoint

NSMB_OBJDECL(0x020C22CE, 0x020C5378, 0x020C5026, 11, CannonBall); // Here all the other unused slots you can use
NSMB_OBJDECL(0x020C22D0, 0x020C538C, 0x020C5028, 12, BlackBowserJR);
NSMB_OBJDECL(0x020C22D2, 0x020C53A0, 0x020C502A, 13, TimePiece);

NSMB_OBJDECL(0x020C22D4, 0x020C53B4, 0x020C502C, 14, BigCoin);
NSMB_OBJDECL(0x020C22D6, 0x020C53C8, 0x020C502E, 15, BroozerBoss);
NSMB_OBJDECL(0x020C22D8, 0x020C53DC, 0x020C5030, 16, boolossus);
NSMB_OBJDECL(0x020C22DA, 0x020C53F0, 0x020C5032, 17, Fingor);
NSMB_OBJDECL(0x020C22DC, 0x020C5404, 0x020C5034, 18, Ninji);
NSMB_OBJDECL(0x020C22DE, 0x020C5418, 0x020C5036, 19, BowserHead);
//NSMB_OBJDECL(0x020C2330, 0x020C574C, 0x020C5088, 60, );
NSMB_OBJDECL(0x020C2332, 0x020C5760, 0x020C508A, 61, BubblePowerUp);
NSMB_OBJDECL(0x020C23CE, 0x020C5D78, 0x020C5126, 139, TrippleBlock);
NSMB_OBJDECL(0x020C23CA, 0x020C5D50, 0x020C5122, 137, Larry);
NSMB_OBJDECL(0x020C235A, 0x020C58F0, 0x020C50B2, 81, Villager)
NSMB_OBJDECL(0x020C2366, 0x020C5968, 0x020C50BE, 87, TorpedoTed);
NSMB_OBJDECL(0x020C237A, 0x020C5A30, 0x020C50D2, 97, BeachHut);
NSMB_OBJDECL(0x020C2398, 0x020C5B5C, 0x020C50F0, 112, Decoration);
NSMB_OBJDECL(0x020C23AA, 0x020C5C10, 0x020C5102, 121, ScuttleBuddy);
NSMB_OBJDECL(0x020C23C2, 0x020C5D00, 0x020C511A, 133, GimmickController);
//NSMB_OBJDECL(0x020C23C4, 0x020C5D14, 0x020C511C, 134, );
//NSMB_OBJDECL(0x020C23C6, 0x020C5D28, 0x020C511E, 135, );
NSMB_OBJDECL(0x020C23CC, 0x020C5D64, 0x020C5124, 138, BowserJRCutscene);
NSMB_OBJDECL(0x020C23E6, 0x020C5E68, 0x020C513E, 151, KoopalingProjectile);
NSMB_OBJDECL(0x020C23EA, 0x020C5E90, 0x020C5142, 153, waterBubble);
NSMB_OBJDECL(0x020C23EC, 0x020C5EA4, 0x020C5144, 154, RedStar);
//NSMB_OBJDECL(0x020C23F0, 0x020C5ECC, 0x020C5148, 156, );
NSMB_OBJDECL(0x020C23F6, 0x020C5F08, 0x020C514E, 159, Roy);
NSMB_OBJDECL(0x020C23F8, 0x020C5F1C, 0x020C5150, 160, Lemmy);
NSMB_OBJDECL(0x020C23FA, 0x020C5F30, 0x020C5152, 161, LemmysBalls);
NSMB_OBJDECL(0x020C23FE, 0x020C5F58, 0x020C5156, 163, FlyGuys);
NSMB_OBJDECL(0x020C240A, 0x020C5FD0, 0x020C5162, 169, Crates);
NSMB_OBJDECL(0x020C240C, 0x020C5FE4, 0x020C5164, 170, Kameck);
NSMB_OBJDECL(0x020C240E, 0x020C5FF8, 0x020C5166, 171, Morton);
NSMB_OBJDECL(0x020C2410, 0x020C600C, 0x020C5168, 172, Goonie);
NSMB_OBJDECL(0x020C2418, 0x020C605C, 0x020C5170, 176, Clone);
NSMB_OBJDECL(0x020C241A, 0x020C6070, 0x020C5172, 177, PowBlock);
NSMB_OBJDECL(0x020C241C, 0x020C6084, 0x020C5174, 178, Collectable);
NSMB_OBJDECL(0x020C241E, 0x020C6098, 0x020C5176, 179, TalkingFlower);
NSMB_OBJDECL(0x020C2422, 0x020C60C0, 0x020C517A, 181, SubNohara);
NSMB_OBJDECL(0x020C2424, 0x020C60D4, 0x020C517C, 182, hoppyCat);
NSMB_OBJDECL(0x020C2428, 0x020C60FC, 0x020C5180, 184, Spike);
NSMB_OBJDECL(0x020C2430, 0x020C614C, 0x020C5188, 188, kingBill);
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


/*
0x020c22b8
u16 objectIDTable[326] = {
    0x15, 0x15, 0x15, 0x15, 0x15, 0x15, 0x15, 0x15,
    0x15, 0x15, 0x145, 0x145, 0x145, 0x145, 0x145, 0x145,
    0x145, 0x145, 0x145, 0x145, 0x16, 0xEF, 0x45, 0x17,
    0x18, 0x19, 0x1A, 0xF8, 0x23, 0x73, 0x2B, 0x1C,
    0x24, 0xED, 0x97, 0x74, 0x25, 0x28, 0x29, 0x72,
    0x27, 0x109, 0x2C, 0x2D, 0x2E, 0x10C, 0x68, 0x81,
    0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x39, 0x3A,
    0x41, 0x42, 0x83, 0x44, 0x145, 0x145, 0x85, 0x84,
    0x49, 0x87, 0xF4, 0x4A, 0xA1, 0xA2, 0xA4, 0xA5,
    0xA6, 0xA9, 0xAA, 0xAB, 0xAC, 0xAD, 0xAE, 0xAF,
    0xB1, 0x145, 0xB3, 0xF0, 0x31, 0x2F, 0xB4, 0x145,
    0xF5, 0x5B, 0x6F, 0xB6, 0x4B, 0x4D, 0x5A, 0xFE,
    0x12D, 0x145, 0x10E, 0xB7, 0x10F, 0x14, 0x67, 0x4F,
    0xE1, 0xE2, 0x50, 0xF2, 0xF6, 0x6E, 0xF7, 0xE3,
    0x145, 0x70, 0xEA, 0xB9, 0x51, 0x105, 0xEB, 0xE4,
    0x63, 0x145, 0x64, 0x65, 0x66, 0xE5, 0xE6, 0xBA,
    0x10D, 0x10A, 0x88, 0x9E, 0x9F, 0x145, 0x145, 0x145,
    0x62, 0x145, 0x145, 0x145, 0x5D, 0xE7, 0xE8, 0xFB,
    0xFC, 0xFD, 0xBB, 0xBC, 0x53, 0x5E, 0x5F, 0x145,
    0x56, 0x145, 0x145, 0x57, 0x145, 0x58, 0x59, 0x145,
    0x145, 0x145, 0xBD, 0x145, 0x89, 0x8A, 0x8B, 0x8C,
    0x8D, 0xFF, 0x145, 0x100, 0x145, 0xC4, 0xC3, 0xE9,
    0x145, 0x145, 0x145, 0x145, 0x60, 0x145, 0x145, 0x3B,
    0x145, 0x3C, 0x54, 0xC6, 0x145, 0xCA, 0x145, 0xFA,
    0x5C, 0x38, 0x26, 0x110, 0x111, 0xCE, 0x112, 0x113,
    0x145, 0x145, 0x145, 0x69, 0x6A, 0x6B, 0xCF, 0x1D,
    0x145, 0x46, 0x21, 0x8F, 0x90, 0x91, 0x145, 0x145,
    0x145, 0x145, 0x114, 0x93, 0x71, 0x145, 0x55, 0x2F,
    0x30, 0x145, 0x75, 0x76, 0x101, 0x77, 0x145, 0x116,
    0x102, 0xB8, 0x117, 0x43, 0xB5, 0x78, 0xBE, 0xBF,
    0x145, 0x106, 0xC1, 0x7B, 0xC2, 0xCC, 0xD0, 0x3D,
    0x2A, 0xD1, 0x94, 0x4C, 0xF9, 0x145, 0x95, 0x118,
    0xD2, 0x1E, 0x96, 0x119, 0xDC, 0xDD, 0xDE, 0xDF,
    0x3E, 0x98, 0x99, 0x145, 0x7C, 0xE0, 0xD3, 0x9A,
    0xD4, 0x9B, 0xC8, 0x103, 0x115, 0x7D, 0xC7, 0xD5,
    0x11A, 0x7E, 0xC5, 0x9D, 0x7F, 0x3F, 0x8E, 0x40,
    0x145, 0x52, 0x104, 0xF3, 0x107, 0x11B, 0x11C, 0x80,
    0xA3, 0xA7, 0xA8, 0xD6, 0xD7, 0x86, 0x108, 0x82,
    0xD8, 0x11D, 0x11E, 0xD9, 0xDA, 0xDB, 0x11F, 0x123,
    0xC0, 0x124, 0x125, 0x120, 0x121, 0x122, 0x126, 0x127,
    0x128, 0x129, 0x12A, 0xC9, 0x12B, 0x12C
};
*/
