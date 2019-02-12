#include "TES3Armor.h"

#include "sol.hpp"

#include "LuaManager.h"

#include "LuaCalcArmorRatingEvent.h"

#include "TES3Util.h"

#include "TES3DataHandler.h"
#include "TES3GameSetting.h"
#include "TES3MobileActor.h"
#include "TES3NPC.h"
#include "TES3Skill.h"

namespace TES3 {
	const auto TES3_Armor_calculateArmorRating = reinterpret_cast<float(__thiscall *)(Armor*, MobileActor*)>(0x4A1120);
	float Armor::calculateArmorRating(MobileActor * actor) {
		// Allow the event to override the value.
		sol::object eventResult = mwse::lua::LuaManager::getInstance().triggerEvent(new mwse::lua::event::CalculateArmorRatingEvent(this, actor));
		if (eventResult.valid()) {
			sol::table eventData = eventResult;

			sol::optional<int> value = eventData["armorRating"];
			sol::optional<bool> block = eventData["block"];
			if (block.value_or(false) && value) {
				return value.value();
			}
		}

		// Figure out armor value for custom slots.
		if (slot < ArmorSlot::First || slot > ArmorSlot::Last) {
			float armorSkillValue = 0.0f;
			switch (getWeightClass()) {
			case TES3::ArmorWeightClass::Light:
				armorSkillValue = actor->getSkillValue(TES3::SkillID::LightArmor);
				break;
			case TES3::ArmorWeightClass::Medium:
				armorSkillValue = actor->getSkillValue(TES3::SkillID::MediumArmor);
				break;
			case TES3::ArmorWeightClass::Heavy:
				armorSkillValue = actor->getSkillValue(TES3::SkillID::HeavyArmor);
				break;
			default:
				return 0.0f;
			}

			return armorRating * armorSkillValue / TES3::DataHandler::get()->nonDynamicData->GMSTs[TES3::GMST::iBaseArmorSkill]->value.asLong;
		}

		return TES3_Armor_calculateArmorRating(this, actor);
	}

	const auto TES3_Armor_calculateArmorRatingForNPC = reinterpret_cast<float(__thiscall *)(Armor*, NPC*)>(0x4A0FD0);
	float Armor::calculateArmorRatingForNPC(NPC * npc) {
		// Allow the event to override the value.
		sol::object eventResult = mwse::lua::LuaManager::getInstance().triggerEvent(new mwse::lua::event::CalculateArmorRatingEvent(this, npc));
		if (eventResult.valid()) {
			sol::table eventData = eventResult;

			sol::optional<int> value = eventData["armorRating"];
			sol::optional<bool> block = eventData["block"];
			if (block.value_or(false) && value) {
				return value.value();
			}
		}

		// Figure out armor value for custom slots.
		if (slot < ArmorSlot::First || slot > ArmorSlot::Last) {
			float armorSkillValue = 0.0f;
			switch (getWeightClass()) {
			case TES3::ArmorWeightClass::Light:
				armorSkillValue = npc->skills[TES3::SkillID::LightArmor];
				break;
			case TES3::ArmorWeightClass::Medium:
				armorSkillValue = npc->skills[TES3::SkillID::MediumArmor];
				break;
			case TES3::ArmorWeightClass::Heavy:
				armorSkillValue = npc->skills[TES3::SkillID::HeavyArmor];
				break;
			default:
				return 0.0f;
			}

			return armorRating * armorSkillValue / TES3::DataHandler::get()->nonDynamicData->GMSTs[TES3::GMST::iBaseArmorSkill]->value.asLong;
		}

		return TES3_Armor_calculateArmorRatingForNPC(this, npc);
	}

	const auto TES3_Armor_getSlotName = reinterpret_cast<const char*(__thiscall *)(Armor*)>(0x4A1270);
	const char* Armor::getSlotName() {
		// If this armor has weight and is of an invalid slot, return straight up armor rating.
		if (slot < ArmorSlot::First || slot > ArmorSlot::Last) {
			TES3::ArmorSlotData * slotData = mwse::tes3::getArmorSlotData(slot);
			if (slotData) {
				return slotData->name.c_str();
			}

			return "Unknown";
		}

		return TES3_Armor_getSlotName(this);
	}

	const auto TES3_Armor_getWeightClass = reinterpret_cast<int(__thiscall *)(Armor*)>(0x4A0F30);
	int Armor::getWeightClass() {
		// Figure out custom slots.
		if (slot < ArmorSlot::First || slot > ArmorSlot::Last) {
			// If we have no custom data, assume light.
			TES3::ArmorSlotData * slotData = mwse::tes3::getArmorSlotData(slot);
			if (slotData == nullptr) {
				return ArmorWeightClass::Light;
			}

			float armorEpsilon = *reinterpret_cast<float*>(0x7483A0);
			auto dataHandler = TES3::DataHandler::get();
			if (slotData->weight * dataHandler->nonDynamicData->GMSTs[TES3::GMST::fLightMaxMod]->value.asFloat + armorEpsilon < weight) {
				return ArmorWeightClass::Light;
			}
			else if (slotData->weight * dataHandler->nonDynamicData->GMSTs[TES3::GMST::fMedMaxMod]->value.asFloat + armorEpsilon < weight) {
				return ArmorWeightClass::Medium;
			}
			else {
				return ArmorWeightClass::Heavy;
			}
		}

		// Finally, we fall back to the vanilla function.
		return TES3_Armor_getWeightClass(this);
	}
}
