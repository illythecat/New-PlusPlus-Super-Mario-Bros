#pragma once
#include "nsmb.hpp"
#include "NSBTX.hpp"

class StageIntroSceneEx : public Scene {

	OSTick start;
	u8* enpg;
	u8* enpg2;


	u8* screenMain;
	u8* screenSub;

	bool selecting;
	bool character;
	bool cursorType;
	bool Bside;
	u8 cursorTimer;


	void defaultCleanup();
	void uploadENPG(u8* screen, u8* palette, bool sub);
	void uploadENPGalt(u8* screen, u8* palette, bool sub);


	void copyENPGTile(u8 srcX, u8 srcY, u8 dstX, u8 dstY, u8 w, u8 h, bool sub);

	inline void renderLives(u8 lives) {
		int units = lives % 10;
		int tens = lives / 10;

		copyENPGTile(units * 8, 192, 64, 154, 8, 16, false);
		if (tens > 0) {
			copyENPGTile(tens * 8, 192, 55, 154, 8, 16, false);
		}
	}
	inline void renderPlayerMario() {
			copyENPGTile(0, 208, 16, 102, 24, 24, false);
	}
    inline void renderPlayerLuigi() {
			copyENPGTile(24, 208, 16, 102, 24, 24, false);
	}

	inline void renderPlayerYoshi() {
			copyENPGTile(96, 208, 16, 102, 24, 24, false);
	}
	inline void renderPlayerYoshiBlue() {
			copyENPGTile(120, 208, 16, 102, 24, 24, false);
	}
	inline void renderPlayerYoshiRed() {
			copyENPGTile(144, 208, 16, 102, 24, 24, false);
	}
	inline void renderPlayerYoshiYellow() {
			copyENPGTile(168, 208, 16, 102, 24, 24, false);
	}
	inline void renderPlayerWario() {
			copyENPGTile(192, 208, 16, 102, 24, 24, false);
	}
	inline void renderPlayerWaluigi() {
			copyENPGTile(216, 208, 16, 102, 24, 24, false);
	}
	inline void renderPlayerNDY() {
			copyENPGTile(238, 208, 16, 102, 24, 24, false);
	}

	inline void renderCross() {
		copyENPGTile(48, 210, 38, 106, 14, 14, false);
	}
	inline void renderArrows() {
		copyENPGTile(64, 210, 75, 106, 8, 16, false);
		copyENPGTile(72, 210, 4, 106, 8, 16, false);
	}
	inline void renderASide(){
		copyENPGTile(96, 234, 20, 140, 100, 16, false);
		copyENPGTile(64, 234, 4, 140, 16, 16, false);
	}
	inline void renderBSide(){
		copyENPGTile(96, 234, 20, 140, 100, 16, false);
		copyENPGTile(80, 234, 4, 140, 16, 16, false);
	}

	inline void renderSelectedArrow(bool left) {
		copyENPGTile(left ? 88 : 80, 210, left ? 4 : 75, 106, 8, 16, false);
	}
	inline void renderSelect() {
		copyENPGTile(8, 232, 32, 124, 38, 8, false);
	}
	inline void renderConfirm() {
		copyENPGTile(16, 240, 32, 124, 38, 8, false);
	}
	inline void renderA() {
		copyENPGTile(8, 240, 24, 124, 8, 8, false);
	}
	inline void renderDot(bool big) {
		copyENPGTile(0, big ? 232 : 240, 24, 124, 8, 8, false);
	}


	inline void renderCurser() {
	
		copyENPGTile(80, 193, 16, 14, 2, 2, false);
		copyENPGTile(80, 193, 16, 16, 2, 2, false);
		copyENPGTile(80, 193, 18, 14, 2, 2, false);


		copyENPGTile(80, 193, 16, 14 + 16, 2, 2, false);
		copyENPGTile(80, 193, 16, 16 + 16, 2, 2, false);
		copyENPGTile(80, 193, 18, 16 + 16, 2, 2, false);


		copyENPGTile(80, 193, 16+ 16, 14, 2, 2, false);
		copyENPGTile(80, 193, 18+ 16, 16, 2, 2, false);
		copyENPGTile(80, 193, 18+ 16, 14, 2, 2, false);


		copyENPGTile(80, 193, 18+ 16, 14 + 16, 2, 2, false);
		copyENPGTile(80, 193, 16+ 16, 16 + 16, 2, 2, false);
		copyENPGTile(80, 193, 18+ 16, 16 + 16, 2, 2, false);


	}

	inline u32 getTopScreenFileID() {
		static const u32 enpgAssignment[8][24] = {
			//	    Arrow  1     2     3     4     5     6     7     8     A     B     C     Ghosthouse Tower Castle Pipe Cannon Red  Green Yellow Fork Tower2 FinalCastle BGChooser
					{0, 1312, 1312, 1312, 1312, 1312, 1312,    0,    0, 1312,    0,    0,             0, 1312,  1312,   0,  1312, 1312, 1312, 1312,   0,     0,          0,     1312}, 		 //World 1
					{0, 1312, 1312, 1312, 1312, 1312, 1312,    0,    0, 1312,    0,    0,             0, 1312,  1312,   0,  1312, 1312, 1312, 1312,   0,     0,          0,        0},  	 //World 2	 
					{0, 1312, 1312, 1312, 1312, 1312, 1312,    0,    0, 1312, 1312, 1312,          1312, 1312,  1312,   0,  1312, 1312, 1312, 1312,   0,     0,          0,        0},  	 //World 3	 
					{0, 1312, 1312, 1312, 1312, 1312, 1312,    0,    0, 1312,    0,    0,          1312, 1312,  1312,   0,  1312, 1312, 1312, 1312,   0,     0,          0,        0},  	 //World 4	
					{0, 1312, 1312, 1312, 1312, 1312, 1312, 1312,    0, 1312, 1312, 1312,          1312, 1312,  1312,   0,  1312, 1312, 1312, 1312,   0,     0,          0,        0},  	 //World 5	 
					{0, 1312, 1312, 1312, 1312, 1312, 1312, 1312,    0, 1312, 1312,    0,             0, 1312,  1312,   0,     0, 1312, 1312, 1312,   0,  1312,          0,        0},  	 //World 6	 
					{0, 1312, 1312, 1312, 1312, 1312, 1312, 1312,    0, 1312,    0,    0,          1312, 1312,  1312,   0,     0, 1312, 1312, 1312,   0,     0,          0,        0},  	 //World 7	
					{0, 1312, 1312, 1312, 1312, 1312, 1312, 1312, 1312, 1312,    0,    0,          1312, 1312,  1312,   0,     0, 1312, 1312, 1312,   0,  1312,       1312,        0} 		 //World 8
		};
		s32 file = enpgAssignment[Save::mainSave.currentWorld][Game::currentWorldNode] - 131;
		return file >= 0 ? file : (1029 - 131);
	}
	inline u32 getBottomScreenFileID() {
		s32 file = 1315 - 131;
		return file >= 0 ? file : (1029 - 131);
	}

public:

	StageIntroSceneEx() {}
	virtual ~StageIntroSceneEx() {}

	virtual s32 onCreate() override;
	virtual s32 onUpdate() override;
	virtual s32 onRender() override;
	virtual s32 onDestroy() override;

	
	virtual bool onPrepareResources();


	virtual void onCleanupResources() override {}

	void LevelStartCheck();

	u32 MenuCharecter;
		u16 creepyThing;


	static constexpr u16 objectID = 0x000d;

	static const ObjectProfile profile;
	static constexpr u16 updatePriority = objectID;
	static constexpr u16 renderPriority = 0x0014;
private:
	NSBTX Nsbtx;
	const static u32 curserModel = 2351 - 131;
	Vec3 curserPosition;
	Vec3 curserScale;
};
