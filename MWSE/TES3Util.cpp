#include "TES3Util.h"

#include "Log.h"

#include "mwOffsets.h"

#include "TES3Actor.h"
#include "TES3Attachment.h"
#include "TES3Class.h"
#include "TES3Clothing.h"
#include "TES3DataHandler.h"
#include "TES3Enchantment.h"
#include "TES3GameSetting.h"
#include "TES3Misc.h"
#include "TES3MobilePlayer.h"
#include "TES3NPC.h"
#include "TES3Reference.h"
#include "TES3Skill.h"
#include "TES3SoulGemData.h"
#include "TES3Spell.h"
#include "TES3MagicSourceInstance.h"
#include "TES3WorldController.h"

#include "LuaManager.h"
#include "LuaUtil.h"

#include "LuaLoadGameEvent.h"
#include "LuaLoadedGameEvent.h"

#define TES3_general_messagePlayer 0x5F90C0
#define TES3_general_setStringSlot 0x47B410

#define TES3_newGame 0x5FAEA0

#define TES3_data_GMSTs 0x794800

#define TES3_restInterruptHour 0x7BC068
#define TES3_restInterruptCreatures 0x7D7530

namespace mwse {
	namespace tes3 {
		TES3::Reference* getReference(const char* id) {

			bool isplayer = !_stricmp(id, "player") || !_stricmp(id, "playersavegame");
			if (isplayer) {
				TES3::WorldController* worldController = TES3::WorldController::get();
				if (worldController) {
					return worldController->getMobilePlayer()->reference;
				}
			}
			else {
				TES3::DataHandler* dataHandler = TES3::DataHandler::get();
				if (dataHandler) {
					return dataHandler->nonDynamicData->findFirstCloneOfActor(id);
				}
			}

			return NULL;
		}

		TES3::Reference* getReference(std::string& id) {
			return getReference(id.c_str());
		}

		char* setDataString(char** container, const char* string) {
			return reinterpret_cast<char*(__cdecl *)(char**, const char*)>(TES3_general_setStringSlot)(container, string);
		}

		unsigned int* getBaseEffectFlags() {
			return reinterpret_cast<unsigned int*>(TES3_DATA_EFFECT_FLAGS);
		}

		bool getBaseEffectFlag(int index, TES3::EffectFlag::Flag flag) {
			return reinterpret_cast<unsigned int*>(TES3_DATA_EFFECT_FLAGS)[index] & flag;
		}

		void setBaseEffectFlag(int index, TES3::EffectFlag::Flag flag, bool set) {
			if (set) {
				reinterpret_cast<unsigned int*>(TES3_DATA_EFFECT_FLAGS)[index] |= flag;
			}
			else {
				reinterpret_cast<unsigned int*>(TES3_DATA_EFFECT_FLAGS)[index] &= ~flag;
			}
		}

		bool setEffect(TES3::Effect * effects, long index, long effectId,
			long skillAttributeId, long range, long area, long duration,
			long minimumMagnitude, long maximumMagnitude) {
			// Validate effect pointer.
			if (effects == NULL) {
				mwse::log::getLog() << __FUNCTION__ << ": No effect passed." << std::endl;
				return false;
			}

			// Validate index.
			if (index < 1 || index > 8) {
				mwse::log::getLog() << __FUNCTION__ << ": Index must be between [1,8]." << std::endl;
				return false;
			}

			// Validate effect id.
			if (effectId < TES3::EffectID::FirstEffect || effectId > TES3::EffectID::LastEffect) {
				mwse::log::getLog() << __FUNCTION__ << ": Effect id outside bounds." << std::endl;
				return false;
			}

			// Validate that the effect supports the range type.
			const int flags = tes3::getBaseEffectFlags()[effectId];
			const auto effectRange = static_cast<TES3::EffectRange>(range);
			if ((effectRange == TES3::EffectRange::Self && !(flags & TES3::EffectFlag::CanCastSelf)) ||
				(effectRange == TES3::EffectRange::Touch && !(flags & TES3::EffectFlag::CanCastTouch)) ||
				(effectRange == TES3::EffectRange::Target && !(flags & TES3::EffectFlag::CanCastTarget))) {
#if _DEBUG
				mwse::log::getLog() << __FUNCTION__ << ": Effect " << effectId << " (with flags " << std::hex << flags << std::dec << ") does not support given range type of " << range << "." << std::endl;
#endif
				return false;
			}

			// Convert index to zero-based.
			index--;

			// Set basic effect data.
			TES3::Effect& effect = effects[index];
			effect.effectID = effectId;
			effect.rangeType = effectRange;
			effect.radius = area;

			// Set skill.
			if (flags & TES3::EffectFlag::TargetSkill) {
				effect.skillID = skillAttributeId;
			}
			else {
				effect.skillID = TES3::SkillID::Invalid;
			}

			// Set attribute.
			if (flags & TES3::EffectFlag::TargetAttribute) {
				effect.attributeID = skillAttributeId;
			}
			else {
				effect.attributeID = TES3::Attribute::Invalid;
			}

			// Set duration.
			if (flags & TES3::EffectFlag::NoDuration) {
				effect.duration = 0;
			}
			else {
				effect.duration = duration;
			}

			// Set magnitude.
			if (flags & TES3::EffectFlag::NoMagnitude) {
				effect.magnitudeMin = 0;
				effect.magnitudeMax = 0;
			}
			else {
				effect.magnitudeMin = minimumMagnitude;
				effect.magnitudeMax = maximumMagnitude;
			}

			return true;
		}

		void checkForLevelUp(long progress) {
			TES3::NonDynamicData* nonDynamicData = TES3::DataHandler::get()->nonDynamicData;
			if (progress >= nonDynamicData->GMSTs[TES3::GMST::iLevelupTotal]->value.asLong) {
				const char* levelUpMessage = nonDynamicData->GMSTs[TES3::GMST::sLevelUpMsg]->value.asString;
				messagePlayer(levelUpMessage);
			}
		}

		void messagePlayer(const char* message) {
			reinterpret_cast<void(__cdecl *)(const char*, int, int)>(TES3_general_messagePlayer)(message, 0, 1);
		}


		int getSkillNameGMST(int id) {
			return reinterpret_cast<int*>(0x794430)[id];
		}

		int getAttributeNameGMST(int id) {
			return reinterpret_cast<int*>(0x794410)[id];
		}

		int getCastRangeNameGMST(int id) {
			return reinterpret_cast<int*>(0x7947C8)[id];
		}

		static std::unordered_map<TES3::Misc*, TES3::SoulGemData*> customSoulGems;

		bool isSoulGem(TES3::Object* objectOrReference) {
			if (reinterpret_cast<bool(__cdecl *)(TES3::Object*)>(0x49ABE0)(objectOrReference)) {
				return true;
			}

			// If we were given a reference, look at the base object.
			if (objectOrReference->objectType == TES3::ObjectType::Reference) {
				objectOrReference = reinterpret_cast<TES3::Reference*>(objectOrReference)->baseObject;
			}

			if (objectOrReference->objectType == TES3::ObjectType::Misc) {
				auto searchResult = customSoulGems.find(reinterpret_cast<TES3::Misc*>(objectOrReference));
				if (searchResult != customSoulGems.end()) {
					return true;
				}
			}

			return false;
		}

		TES3::SoulGemData * addCustomSoulGem(TES3::Misc * item) {
			if (isSoulGem(item)) {
				return nullptr;
			}

			auto data = new TES3::SoulGemData();
			data->item = item;
			data->id = item->objectID;
			data->name = item->name;
			data->model = item->model;
			data->texture = item->icon;
			data->value = item->value;
			data->weight = item->weight;

			customSoulGems[item] = data;
			return data;
		}

		TES3::SoulGemData * getSoulGemData(TES3::Misc * item) {
			TES3::SoulGemData * vanillaSoulGems = reinterpret_cast<TES3::SoulGemData*>(0x791C98);
			for (size_t i = 0; i < 6; i++) {
				if (vanillaSoulGems[i].item == item) {
					return &vanillaSoulGems[i];
				}
			}

			auto searchResult = customSoulGems.find(item);
			if (searchResult != customSoulGems.end()) {
				return searchResult->second;
			}

			return nullptr;
		}

		static std::map<int, TES3::ArmorSlotData*> customArmorSlots;

		TES3::ArmorSlotData * getArmorSlotData(int slot) {
			auto searchResult = customArmorSlots.find(slot);
			if (searchResult != customArmorSlots.end()) {
				return searchResult->second;
			}

			return nullptr;
		}

		void setArmorSlotData(TES3::ArmorSlotData * data) {
			auto searchResult = customArmorSlots.find(data->slot);
			if (searchResult != customArmorSlots.end()) {
				delete searchResult->second;
				customArmorSlots.erase(data->slot);
			}

			customArmorSlots[data->slot] = data;
		}

		TES3::Reference* exteriorRefs[9] = { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL };

		void clearExteriorRefs() {
			for (size_t i = 0; i < 9; i++) {
				exteriorRefs[i] = NULL;
			}
		}

		void startNewGame() {
			// Call our load event.
			mwse::lua::LuaManager& luaManager = mwse::lua::LuaManager::getInstance();
			luaManager.triggerEvent(new mwse::lua::event::LoadGameEvent(NULL, false, true));

			// Call original function.
			reinterpret_cast<void(__stdcall *)()>(TES3_newGame)();

			// Clear any timers.
			luaManager.clearTimers();

			// Call our post-load event.
			luaManager.triggerEvent(new mwse::lua::event::LoadedGameEvent(NULL, false, true));
		}

		int getRestHoursInterrupted() {
			return *reinterpret_cast<int*>(TES3_restInterruptHour);
		}

		void setRestHoursInterrupted(int hour) {
			if (hour < -1) {
				hour = -1;
			}

			*reinterpret_cast<int*>(TES3_restInterruptHour) = hour;
		}

		int getRestInterruptCount() {
			return *reinterpret_cast<int*>(TES3_restInterruptCreatures);
		}

		void setRestInterruptCount(int count) {
			*reinterpret_cast<int*>(TES3_restInterruptCreatures) = count;
		}

		const auto TES3_ResolveAssetPath = reinterpret_cast<int(__cdecl*)(const char *, char *)>(0x47A960);
		int resolveAssetPath(const char* path, char * out_buffer) {
			return TES3_ResolveAssetPath(path, out_buffer);
		}

		void * _new(size_t size) {
			return reinterpret_cast<void*(__cdecl*)(size_t)>(0x727692)(size);
		}

		ExternalRealloc _realloc = NULL;
		void* realloc(void* address, size_t size) {
			return _realloc(address, size);
		}

		ExternalMalloc _malloc = NULL;
		void* malloc(size_t size) {
			return _malloc(size);
		}

		ExternalFree _free = NULL;
		void free(void* address) {
			_free(address);
		}
	}
}
