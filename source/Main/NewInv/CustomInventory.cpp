#include "nsmb.hpp"
#include "main/undocumented.hpp"

namespace CustomInventory
{
	static u8 invPowerup[2][3] = { 0 };
	static u8 droppingSlot[2] = { 0 };
	static u8 touchedSlot[2] = { 0 };

	u8 getPowerupOnSlot(int playerNo, int slot) { return invPowerup[playerNo][slot]; }
	void setPowerupOnSlot(int playerNo, int slot, u8 value) { invPowerup[playerNo][slot] = value; }
	u8 getDroppingSlot(int playerNo) { return droppingSlot[playerNo]; }
	void setDroppingSlot(int playerNo, u8 value) { droppingSlot[playerNo] = value; }
}

using namespace CustomInventory;

//u8* reservePowerups[3] = scast<u8*>(0x02088c39)(3);

ncp_repl(0x02012cbc, "nop; nop")
ncp_repl(0x02012ca4, "nop")

ncp_call(0x0211F844, 10)
NTR_NAKED  void itemonselect() {asm(R"(
	STMFD	        SP!, {R0, R1, LR}
	LDR	        R0, =0x04000130
	LDRB	        R0, [R0]
	LSR	        R0, #2
	ANDS	        R0, #1
	LDMNEFD	        SP!, {R0, R1, PC}
	LDR		R0, =0x0208B698
	LDR		R1, =0x009500DA
	STR		R1, [R0]
	LDR		R1, =0x09600D40
	STR		R1, [R0, #0x8]
	LDR		R1, =0x1
	STR		R1, [R0, #0x4]
	STR		R1, [R0, #0xC]
	LDMFD	        SP!, {R0, R1, PC}
)");}

ncp_hook(0x02012cc4)
void resetSomeshit1(){
	invPowerup[0][0] = Save::mainSave.reserved[0];
	invPowerup[0][1] = Save::mainSave.reserved[1];
	invPowerup[0][2] = Save::mainSave.reserved[2];
} 


ncp_hook(0x02012df8)
void writeCustomReserve(){
	Save::mainSave.reserved[0] = invPowerup[0][0];
	Save::mainSave.reserved[1] = invPowerup[0][1];
	Save::mainSave.reserved[2] = invPowerup[0][2];

}
ncp_hook(0x02012ec8)
void writeCustomReserve2(){
	invPowerup[0][0] = Save::mainSave.reserved[0];
	invPowerup[0][1] = Save::mainSave.reserved[1];
	invPowerup[0][2] = Save::mainSave.reserved[2];
}

ncp_repl(0x02020240, "MOV R0, #1\nBX LR") // Replace powerup GET
ncp_repl(0x02020250, "BX LR") // Replace powerup SET

//Allow 3 rectangles to touch and save the touched one
ncp_repl(0x020C058C, 0, "MOV R1, R6")

ncp_call(0x020C0590, 0)
bool repl_020C0590_ov_00(int rect, int playerNo)
{
	if (rect >= 0 && rect < 3)
	{
		touchedSlot[playerNo] = rect;
		return false;
	}
	return true;
}

ncp_repl(0x020C0594, 0, R"(
	CMP     R0, #0
	ADDNE   SP, SP, #0x14
	POPNE   {R4-R7,PC}
	B       0x020C05C8
)")

// Check if the touched powerup slot isn't empty so it can proceed to dropping state
ncp_call(0x020BE1F0, 0)
bool repl_020BE1F0_ov_00(int playerNo)
{
	return getPowerupOnSlot(playerNo, touchedSlot[playerNo]) != 0;
}

// Set the dropping state and check something for the powerup being dropped instead
ncp_call(0x020C06BC, 0)
int repl_020C06BC_ov_00(int playerNo)
{
	setDroppingSlot(playerNo, touchedSlot[playerNo]);
	return getPowerupOnSlot(playerNo, getDroppingSlot(playerNo));
}

// Clear powerup on drop
ncp_call(0x020BF59C, 0)
void repl_020BF59C_ov_00(int playerNo, int zero)
{
	setPowerupOnSlot(playerNo, getDroppingSlot(playerNo), zero);
}

// Change the ReplaceInventoryItem system
ncp_jump(0x020D4A70, 10)
void nsub_020D4A70_ov_0A(Player* player, int playerNo, int newPowerup)
{
	for (int i = 0; i < 3; i++)
	{
		bool isSlotEmpty = getPowerupOnSlot(playerNo, i) == 0;
		if (isSlotEmpty)
		{
			setPowerupOnSlot(playerNo, i, newPowerup);
			setDroppingSlot(playerNo, i);
			UpdateInventoryPowerup(playerNo, i);
			return;
		}
	}

	bool replaced = false;
	for (int i = 0; i < 3; i++)
	{
		bool canReplace = false;
		int oldPowerup = getPowerupOnSlot(playerNo, i);

		switch (oldPowerup)
		{
		case 2:
			if ((newPowerup == 3)||(newPowerup == 4)||(newPowerup == 5)||(newPowerup == 8)||(newPowerup == 9)||(newPowerup == 10))
				canReplace = true;
			break;
		case 3:
			if ((newPowerup == 2)||(newPowerup == 4)||(newPowerup == 5)||(newPowerup == 8)||(newPowerup == 9)||(newPowerup == 10))
				canReplace = true;
			break;
		case 9:
			if ((newPowerup == 3)||(newPowerup == 4)||(newPowerup == 5)||(newPowerup == 8)||(newPowerup == 2)||(newPowerup == 10))
				canReplace = true;
			break;
		case 10:
			if ((newPowerup == 3)||(newPowerup == 4)||(newPowerup == 5)||(newPowerup == 8)||(newPowerup == 9)||(newPowerup == 2))
				canReplace = true;
			break;
		case 4:
			if (newPowerup == 8)
				canReplace = true;
			break;

		default:
			if (oldPowerup == 1 && newPowerup != 1)
				canReplace = true;
			break;
		}

		if (canReplace)
		{
			setPowerupOnSlot(playerNo, i, newPowerup);
			setDroppingSlot(playerNo, i);
			UpdateInventoryPowerup(playerNo, i);
			replaced = true;
			break;
		}
	}

	if (!replaced)
	{
		if (playerNo == Game::localPlayerID)
			SND::playSFX(0x17D);
		StageEntity::getCollectablePoints(4, playerNo);
	}
}

static GXOamAttr oam_noItem[] = {
	
	OAM::getOBJAttr(0, 0, 0, GX_OAM_MODE_NORMAL, false, GX_OAM_EFFECT_NONE, GX_OAM_SHAPE_8x8, GX_OAM_COLOR_256, 358 * 2, 0, 0, 0),
	OAM::getOBJAttr(32, 0, 0, GX_OAM_MODE_NORMAL, false, GX_OAM_EFFECT_NONE, GX_OAM_SHAPE_8x8, GX_OAM_COLOR_256, 358 * 2, 0, 0, 0),
	OAM::getOBJAttr(48, 0, 0, GX_OAM_MODE_NORMAL, false, GX_OAM_EFFECT_NONE, GX_OAM_SHAPE_8x8, GX_OAM_COLOR_256, 358 * 2, 0, 0, 0xFFFF)
	
};

static GXOamAttr oam_mushroomItem[] = {
	OAM::getOBJAttr(0, 0, 0, GX_OAM_MODE_NORMAL, false, GX_OAM_EFFECT_NONE, GX_OAM_SHAPE_32x32, GX_OAM_COLOR_16, 68 * 2, 9, 0, 0xFFFF)
};

static GXOamAttr oam_redStarItem[] = {
    OAM::getOBJAttr(0, 0, 0, GX_OAM_MODE_NORMAL, false, GX_OAM_EFFECT_NONE, GX_OAM_SHAPE_32x32, GX_OAM_COLOR_16, 76 * 2, 9, 0, 0xFFFF)
};

static GXOamAttr oam_miniMushroomItem[] = {
	OAM::getOBJAttr(0, 0, 0, GX_OAM_MODE_NORMAL, false, GX_OAM_EFFECT_NONE, GX_OAM_SHAPE_32x32, GX_OAM_COLOR_16, 196 * 2, 12, 0, 0xFFFF)
};

static GXOamAttr oam_fireflowerItem2[] = {
	OAM::getOBJAttr(0, 0, 0, GX_OAM_MODE_NORMAL, false, GX_OAM_EFFECT_FLIP_H, GX_OAM_SHAPE_32x64, GX_OAM_COLOR_16, 100 * 2, 10, 0, 0xFFFF)
};
static GXOamAttr oam_fireflowerItem[] = {
	OAM::getOBJAttr(0, 0, 0, GX_OAM_MODE_NORMAL, false, GX_OAM_EFFECT_NONE, GX_OAM_SHAPE_32x64, GX_OAM_COLOR_16, 100 * 2, 10, 0, 0xFFFF)
};

static GXOamAttr oam_SuperflowerItem[] = {
	OAM::getOBJAttr(0, 0, 0, GX_OAM_MODE_NORMAL, false, GX_OAM_EFFECT_NONE, GX_OAM_SHAPE_32x64, GX_OAM_COLOR_16, 100 * 2, 14, 0, 0xFFFF)
};
static GXOamAttr oam_SuperflowerItem2[] = {
	OAM::getOBJAttr(0, 0, 0, GX_OAM_MODE_NORMAL, false, GX_OAM_EFFECT_FLIP_H, GX_OAM_SHAPE_32x64, GX_OAM_COLOR_16, 100 * 2, 14, 0, 0xFFFF)
};

static GXOamAttr oam_megaMushroomItem2[] = {
	OAM::getOBJAttr(0, 0, 0, GX_OAM_MODE_NORMAL, false, GX_OAM_EFFECT_FLIP_H, GX_OAM_SHAPE_32x64, GX_OAM_COLOR_16, 116 * 2, 13, 0, 0xFFFF)
};
static GXOamAttr oam_megaMushroomItem[] = {
	OAM::getOBJAttr(0, 0, 0, GX_OAM_MODE_NORMAL, false, GX_OAM_EFFECT_FLIP_H, GX_OAM_SHAPE_32x64, GX_OAM_COLOR_16, 116 * 2, 13, 0, 0xFFFF)
};
static GXOamAttr oam_shellItem[] = {
	OAM::getOBJAttr(0, 0, 0, GX_OAM_MODE_NORMAL, false, GX_OAM_EFFECT_NONE, GX_OAM_SHAPE_64x64, GX_OAM_COLOR_16, 132 * 2, 11, 0, 0xFFFF)
};

static GXOamAttr oam_hammerItem[] = {
	OAM::getOBJAttr(0, 0, 0, GX_OAM_MODE_NORMAL, false, GX_OAM_EFFECT_NONE, GX_OAM_SHAPE_64x64, GX_OAM_COLOR_16, 164 * 2, 14, 0, 0xFFFF)
};
static GXOamAttr* oam_invItems[] = {
	oam_noItem, oam_mushroomItem, oam_fireflowerItem, oam_shellItem, oam_miniMushroomItem, oam_megaMushroomItem, 0, 0, oam_redStarItem, oam_SuperflowerItem, oam_hammerItem
};

static void MyDrawBottomScreenPowerups(int* stageScene, int playerNo)
{
	for (int i = 0; i < 3; i++)
	{
		int invItem = CustomInventory::getPowerupOnSlot(playerNo, i);

		int* stageSceneChild = &stageScene[3 * playerNo];
		int xOff = (stageSceneChild[31] >> 12) - *(int*)0x020CC2C4;
		int yOff = -(stageSceneChild[32] >> 12);

		Vec2* scale;
		s16 rotation;
		int yOffAnim;

		int dropState = ((int*)0x020CC0E0)[playerNo];
		bool somePlayerDropStateCondition = (dropState <= 3);
		if (invItem == 0 ||  (!somePlayerDropStateCondition && i != CustomInventory::getDroppingSlot(playerNo)))
		{
			scale = nullptr;
			rotation = 0;
			yOffAnim = 0;
		}
		else
		{
			bool transitioning = (bool)(Entrance::transitionFlags[playerNo] & EntranceTransitionFlags::SubScreen);


			scale = &((Vec2*)0x020CC14C)[playerNo];
			rotation = *((u16*)0x020CC0D0 + playerNo);
			yOffAnim = *((int*)0x020CC0D8 + playerNo);
		}

		if (invItem >= sizeof(oam_invItems) / sizeof(GXOamAttr*))
			continue;


		if (yOffAnim > -192)
		{
			if(invItem == 1){
        		Vec2 result;
        		result.x = 7000;
        		result.y = 7000;
				drawSub(
				18 + i,
				oam_invItems[invItem],
				OAM::Flags::None,
				0,
				0,
				&result,
				rotation,
				nullptr,
				OAM::Settings::None,
				xOff + 4,
				yOff + 8
				);
			}
			else if(invItem == 8){
        		Vec2 result;
        		result.x = 7000;
        		result.y = 7000;
				drawSub(
				18 + i,
				oam_invItems[invItem],
				OAM::Flags::None,
				0,
				0,
				&result,
				rotation,
				nullptr,
				OAM::Settings::None,
				xOff + 4,
				yOff + 8
				);
			}
			else if(invItem == 2){

				drawSub(
				18 + i,
				oam_invItems[invItem],
				OAM::Flags::None,
				0,
				0,
				scale,
				rotation,
				nullptr,
				OAM::Settings::None,
				xOff,
				yOff-3
				);
				drawSub(
				18 + i,
				oam_fireflowerItem2,
				OAM::Flags::None,
				0,
				0,
				scale,
				rotation,
				nullptr,
				OAM::Settings::None,
				xOff + 32,
				yOff-3
				);
			}
			else if(invItem == 10){

				drawSub(
				18 + i,
				oam_invItems[invItem],
				OAM::Flags::None,
				0,
				0,
				scale,
				rotation,
				nullptr,
				OAM::Settings::None,
				xOff - 2,
				yOff+7
				);
			}
			else if(invItem == 9){

				drawSub(
				18 + i,
				oam_invItems[invItem],
				OAM::Flags::None,
				0,
				0,
				scale,
				rotation,
				nullptr,
				OAM::Settings::None,
				xOff,
				yOff-3
				);
				drawSub(
				18 + i,
				oam_SuperflowerItem2,
				OAM::Flags::None,
				0,
				0,
				scale,
				rotation,
				nullptr,
				OAM::Settings::None,
				xOff + 32,
				yOff - 3
				);
			}
			else if(invItem == 4){
				drawSub(
				18 + i,
				oam_invItems[invItem],
				OAM::Flags::None,
				0,
				0,
				scale,
				rotation,
				nullptr,
				OAM::Settings::None,
				xOff + 16,
				yOff + 24
				);
			}
			else if(invItem == 5){

				drawSub(
				18 + i,
				oam_invItems[invItem],
				OAM::Flags::None,
				0,
				0,
				scale,
				rotation,
				nullptr,
				OAM::Settings::None,
				xOff + 32,
				yOff
				);
				drawSub(
				18 + i,
				oam_megaMushroomItem2,
				OAM::Flags::FlipX,
				0,
				0,
				scale,
				rotation,
				nullptr,
				OAM::Settings::None,
				xOff + 33,
				yOff
				);
			}
			else{
				drawSub(
				18 + i,
				oam_invItems[invItem],
				OAM::Flags::None,
				0,
				0,
				scale,
				rotation,
				nullptr,
				OAM::Settings::None,
				xOff,
				yOff + 6
			);
			}

		}
	}
}

ncp_jump(0x020be828, 0)
void newPowerUpSystemRender(int* stageScene, int playerNo){
	MyDrawBottomScreenPowerups(stageScene, playerNo);
}