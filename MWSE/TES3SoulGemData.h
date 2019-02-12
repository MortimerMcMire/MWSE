#pragma once

#include "TES3Defines.h"

namespace TES3 {
	struct SoulGemData {
		const char * id; // 0x0
		const char * name; // 0x4
		const char * model; // 0x8
		const char * texture; // 0xC
		int value; // 0x10
		float weight; // 0x14
		Misc * item; // 0x18
	};
}
