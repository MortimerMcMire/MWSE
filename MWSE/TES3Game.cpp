#include "TES3Game.h"

namespace TES3 {
	Game * Game::get() {
		return *reinterpret_cast<TES3::Game**>(0x7C6CDC);
	}

	const auto TES3_Game_initialize = reinterpret_cast<bool(__thiscall *)(Game*)>(0x417880);
	bool Game::initialize() {
		return TES3_Game_initialize(this);
	}
}
