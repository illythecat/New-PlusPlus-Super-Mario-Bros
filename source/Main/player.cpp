/*============================================================\
|  This file was made by TheGameratorT & Ed_IT.               |
|                                                             |
|  This code is meant to work with the following template:    |
|  https://github.com/MammaMiaTeam/NSMB-Code-Reference        |
|                                                             |
|  You may modify this file and use it for whatever you want  |
|  just be sure to credit me (TheGameratorT).                 |
|                                                             |
|  Hope you like it just as much as I had fun coding this!    |
|                                                             |
|  ---------------------------------------------------------  |
|                                                             |
|  NWAV player integration into NSMB DS.                      |
|  This is where the wav player is installed in the game.     |
\============================================================*/

#include "nsmb.hpp"

#include "core.hpp"

extern "C" {

	//Implement enemy dance animation events
	void updateMusicEvents()
	{
		if (!NWAV::getPaused())
		{
			if (SND::bgmVariables[2])
			{
				static bool remove = false;
				if (remove)
				{
					SND::bgmVariables[2] = false;
					remove = false;
				}
				else
				{
					remove = true;
				}
			}
		}
	}

	void mainEventHandler(int eventID)
	{
		switch (eventID)
		{
			//"bah" event
		case 0:
			if (Game::getPlayerCount() == 1)
				SND::bgmVariables[2] = 1;
			break;
			//Add your events here
		default:
			break;
		}
	}

	// hook into SND::Internal::createHeap
	ncp_jump(0x0204F2F0)
	[[gnu::naked]] void repl_0204F2F0() {asm(R"(
	
		bl	NNS_SndInit									@ Keep replaced instruction
	
		bl	_ZN4NWAV4initEv
		ldr	r0, =mainEventHandler
		@bl	_ZN4NWAV15setEventHandlerEPFviE
	
		b	0x0204F2F0 + 4
	
	)");}
	
	// hook into SND::Internal::updateSound
	ncp_jump(0x0204EC74)
	[[gnu::naked]] void repl_0204EC74() {asm(R"(
	
		bl	NNS_SndMain									@ Keep replaced instruction
	
		bl	_ZN4NWAV10updateFadeEv
		bl	updateMusicEvents
	
		b	0x0204EC74 + 4
	
	)");}

	//NSMB uses stereo by default
	ncp_call(0x02012FC0)
	__attribute__((naked)) void repl_02012FC0()
	{
		asm("STR R0, [R4,#0xC]"); //Keep replaced instruction
		asm("MOV R0, #2");
		asm("STR R0, [R4,#0x344]"); //Set sound mode
		asm("BX  LR");
	}

	//FS uses DMA channel 3.
	ncp_call(0x02009444)
	int repl_02009444() { return 3; }


	constexpr int firstWavID = 2203; 


	//Backup the original functions
	[[gnu::naked]] static void playSeq(int seqID, int sfxSetID) { asm("STMFD SP!, {R4,LR}\nB 0x02011E80"); }
	[[gnu::naked]] static void loadSeq(int seqID) { asm("STMFD SP!, {R4,LR}\nB 0x0204F198"); }
	[[gnu::naked]] static void setTempo(int ratio) { asm("STMFD SP!, {R4,LR}\nB 0x0204E530"); }
	[[gnu::naked]] static void setVolume(int targetVolume, int frames) { asm("STMFD SP!, {R4,LR}\nB 0x0204DCB8"); }
	[[gnu::naked]] static void pauseSeq(bool flag) { asm("STMFD SP!, {R4,LR}\nB 0x0204E480"); }
	[[gnu::naked]] static void stopSeq(int fadeFrame) { asm("STMFD SP!, {R4,LR}\nB 0x0204E424"); }

	static bool GetIfSequenced(int seqID)
	{
		int wavID = firstWavID + seqID;

		FSFile file;
		FS_InitFile(&file);
		if (FS_OpenFileFast(&file, {(FSArchive*)0x02096114, wavID})) 
		{
			int magic;
			int readSize = FS_ReadFile(&file, &magic, 4);
			if (readSize)
			{
				if (magic == NWAV_MAGIC)
				{
					FS_CloseFile(&file);
					return false;
				}
			}
			FS_CloseFile(&file);
		}
		return true;
	}

	//Replace play
	ncp_jump(0x02011E7C)
	void nsub_02011E7C(int seqID, int sfxSetID)
	{
		int wavID = firstWavID + seqID;

		int& currSeq = *(int*)0x02088B64;
		if (GetIfSequenced(seqID))
		{
			if (currSeq != seqID)
				NWAV::stop(0);
			playSeq(seqID, sfxSetID);
		}
		else
		{
			if (currSeq != seqID) {
				stopSeq(0);

				NWAV::play(wavID);

				currSeq = seqID;
			}
		}
	}


	//Replace load
	ncp_jump(0x0204F194)
	void nsub_0204F194(int seqID)
	{
		if (GetIfSequenced(seqID))
			loadSeq(seqID);
	}

	//Replace set tempo
	ncp_jump(0x0204E52C)
	void nsub_0204E52C(int ratio)
	{
		setTempo(ratio);
		NWAV::setSpeed(ratio << 12 >> 8);
	}

	//Replace set volume
	ncp_jump(0x0204DCB4)
	void nsub_0204DCB4(int targetVolume, int frames)
	{
		setVolume(targetVolume, frames);
		NWAV::setVolume(targetVolume, frames);
	}

	//Replace pause
	ncp_jump(0x0204E47C)
	void nsub_0204E47C(bool flag)
	{
		pauseSeq(flag);
		NWAV::setPaused(flag);
	}

	//Replace stop
	ncp_jump(0x0204E420)
	void nsub_0204E420(int fadeFrame)
	{
		stopSeq(fadeFrame);
		NWAV::stop(fadeFrame);
	}

}
