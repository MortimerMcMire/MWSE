#pragma once

#include "TES3Defines.h"

namespace TES3 {
	struct Statistic_vTable {
		float (__thiscall * getCurrentValue)(Statistic*);
	};

	struct Statistic {
		Statistic_vTable * vTable;
		float base;
		float current;

		//
		// Virtual table function wrappers.
		//

		float getCurrent();

		//
		// Other related this-call functions.
		//

		float getBase();
		float getCurrentRaw();
		float getNormalized();

		void modBaseCapped(float delta, bool capAt0, bool capAt100);
		void modCurrentCapped(float delta, bool capAt0, bool capAtBase, bool capAt100);

		void setBase(float value);
		void setBaseAndCurrent(float value);
		void setBaseToCurrent();
		void setCurrentCapped(float value, bool applyCaps);

	};
	static_assert(sizeof(Statistic) == 0xC, "TES3::Statistic failed size validation");


	struct SkillStatistic : Statistic {
		unsigned int type; // 0xC // Not specialization?
	};
	static_assert(sizeof(SkillStatistic) == 0x10, "TES3::SkillStatistic failed size validation");
}
