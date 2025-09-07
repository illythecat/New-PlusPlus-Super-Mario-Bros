#pragma once
#include "nsmb/sound.hpp"

namespace SeqArc {

    namespace FirstID {
        enum {
			SAR_LARRY_BASE  = 450,
            YOSHI_SE = 457,
            SAR_ROY_BASE = 465,
            WALUIGI_SE = 474,
            WARIO_SE = 482,
            KAMEK_SE = 491,
            MORTON_SE = 497,
        };
    };

    inline namespace SAR_LARRY_BASE {
		enum {
			SE_LARRY_1 = FirstID::SAR_LARRY_BASE,
            SE_LARRY_2,
            SE_LARRY_3,
            SE_LARRY_4,
            SE_LARRY_5,
            SE_LARRY_6,
            SE_LARRY_7,
		};
	};

    inline namespace YOSHI_SE {
		enum {
			SE_YOSHI_0 = FirstID::YOSHI_SE,
            SE_YOSHI_1,
            SE_YOSHI_2,
            SE_YOSHI_3,
            SE_YOSHI_4,
            SE_YOSHI_5,
            SE_YOSHI_6,
            SE_YOSHI_7,
		};
	};

    inline namespace SAR_ROY_BASE {
		enum {
			SE_CLOWN_CAR_0 = FirstID::SAR_ROY_BASE,
            SE_CLOWN_CAR_1,
            SE_CLOWN_CAR_2,
            SE_ROY_0,
            SE_ROY_1,
            SE_ROY_2,
            SE_ROY_3,
            SE_ROY_4,
            SE_ROY_5,
		};

	};

    inline namespace WALUIGI_SE {
		enum {
			WALUIGI_SE_0 = FirstID::WALUIGI_SE,
            WALUIGI_SE_1,
            WALUIGI_SE_2,
            WALUIGI_SE_3,
            WALUIGI_SE_4,
            WALUIGI_SE_5,
            WALUIGI_SE_6,
            WALUIGI_SE_7,
		};
    }

    inline namespace WARIO_SE {
		enum {
			WARIO_SE_0 = FirstID::WARIO_SE,
            WARIO_SE_1,
            WARIO_SE_2,
            WARIO_SE_3,
            WARIO_SE_4,
            WARIO_SE_5,
            WARIO_SE_6,
            WARIO_SE_7,
		};

    }
    inline namespace KAMEK_SE {
		enum {
            KAMEK_SE_0 = FirstID::KAMEK_SE,
            KAMEK_SE_1,
            KAMEK_SE_2,
            KAMEK_SE_3,
            KAMEK_SE_4,
            KAMEK_SE_5, 
        };
	}

    inline namespace MORTON_SE {
		enum {
            MORTON_SE_0 = FirstID::MORTON_SE,
            MORTON_SE_1,
            MORTON_SE_2,
            MORTON_SE_3,
            MORTON_SE_4,
            MORTON_SE_5, 
        };
	}
    
    enum {
        ExtCount    = Count + 7,
    };
        
}

namespace SND
{

    constexpr SFXEntry extSFXTable[] =
    {
        { SeqArc::Count + 0, 0 }, //SE_LARRY_1
        { SeqArc::Count + 0, 1 }, //SE_LARRY_2
        { SeqArc::Count + 0, 2 }, //SE_LARRY_3
        { SeqArc::Count + 0, 3 }, //SE_LARRY_4
        { SeqArc::Count + 0, 4 }, //SE_LARRY_5
        { SeqArc::Count + 0, 5 }, //SE_LARRY_6
        { SeqArc::Count + 0, 6 }, //SE_LARRY_7
        { SeqArc::Count + 1, 0 }, //SE_YOSHI_0
        { SeqArc::Count + 1, 1 }, //SE_YOSHI_1
        { SeqArc::Count + 1, 2 }, //SE_YOSHI_2
        { SeqArc::Count + 1, 3 }, //SE_YOSHI_3
        { SeqArc::Count + 1, 4 }, //SE_YOSHI_4
        { SeqArc::Count + 1, 5 }, //SE_YOSHI_5
        { SeqArc::Count + 1, 6 }, //SE_YOSHI_6
        { SeqArc::Count + 1, 7 }, //SE_YOSHI_7
        { SeqArc::Count + 2, 0 }, //SE_CLOWN_CAR_0
        { SeqArc::Count + 2, 1 }, //SE_CLOWN_CAR_1
        { SeqArc::Count + 2, 2 }, //SE_CLOWN_CAR_2
        { SeqArc::Count + 2, 3 }, //SE_ROY_0 
        { SeqArc::Count + 2, 4 }, //SE_ROY_1
        { SeqArc::Count + 2, 5 }, //SE_ROY_2
        { SeqArc::Count + 2, 6 }, //SE_ROY_3
        { SeqArc::Count + 2, 7 }, //SE_ROY_4
        { SeqArc::Count + 2, 8 }, //SE_ROY_5
        { SeqArc::Count + 3, 0 }, //WALUIGI_SE_0
        { SeqArc::Count + 3, 1 }, //WALUIGI_SE_1
        { SeqArc::Count + 3, 2 }, //WALUIGI_SE_2
        { SeqArc::Count + 3, 3 }, //WALUIGI_SE_3
        { SeqArc::Count + 3, 4 }, //WALUIGI_SE_4
        { SeqArc::Count + 3, 5 }, //WALUIGI_SE_5
        { SeqArc::Count + 3, 6 }, //WALUIGI_SE_6
        { SeqArc::Count + 3, 7 }, //WALUIGI_SE_7
        { SeqArc::Count + 4, 0 }, //WARIO_SE_0
        { SeqArc::Count + 4, 1 }, //WARIO_SE_0
        { SeqArc::Count + 4, 2 }, //WARIO_SE_0
        { SeqArc::Count + 4, 3 }, //WARIO_SE_0
        { SeqArc::Count + 4, 4 }, //WARIO_SE_0
        { SeqArc::Count + 4, 5 }, //WARIO_SE_0
        { SeqArc::Count + 4, 6 }, //WARIO_SE_0
        { SeqArc::Count + 4, 7 }, //WARIO_SE_0
        { SeqArc::Count + 4, 8 }, //WARIO_SE_0
        { SeqArc::Count + 5, 0 },  //KAMEK_SE_0
        { SeqArc::Count + 5, 1 },//KAMEK_SE_1
        { SeqArc::Count + 5, 2 },//KAMEK_SE_2
        { SeqArc::Count + 5, 3 },//KAMEK_SE_3
        { SeqArc::Count + 5, 4 },//KAMEK_SE_4
        { SeqArc::Count + 5, 5 },//KAMEK_SE_5
        { SeqArc::Count + 6, 0 },
        { SeqArc::Count + 6, 1 },
        { SeqArc::Count + 6, 2 },
        { SeqArc::Count + 6, 3 },
        { SeqArc::Count + 6, 4 },
        { SeqArc::Count + 6, 5 },


    };

    NTR_INLINE bool playExtSFX(s32 sfxID, const Vec3* pos = nullptr) {
        sfxID -= SeqArc::SfxCount + 1;
        return playSFX(extSFXTable[sfxID].seqArcNo, extSFXTable[sfxID].seqArcIndex, pos, 0x7F, SFXMode::Retrigger);
    }

    NTR_INLINE bool playExtSFXContinuous(s32 sfxID, const Vec3* pos = nullptr) {
        sfxID -= SeqArc::SfxCount + 1;
        return playSFX(extSFXTable[sfxID].seqArcNo, extSFXTable[sfxID].seqArcIndex, pos, 0x7F, SFXMode::Continuous);
    }

    NTR_INLINE bool playExtSFXUnique(s32 sfxID, const Vec3* pos = nullptr) {
        sfxID -= SeqArc::SfxCount + 1;
        return playSFX(extSFXTable[sfxID].seqArcNo, extSFXTable[sfxID].seqArcIndex, pos, 0x7F, SFXMode::Unique);
    }

}