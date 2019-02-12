#pragma once

#include "LuaObjectFilteredEvent.h"

namespace mwse {
	namespace lua {
		namespace event {
			// Equipped event. Called after something is equipped.
			class EquippedEvent : public ObjectFilteredEvent {
			public:
				EquippedEvent(TES3::Actor* a, TES3::MobileActor* ma, TES3::BaseObject* i, TES3::ItemData* id);
				sol::table createEventTable();

			protected:
				TES3::Actor* m_Actor;
				TES3::MobileActor* m_MobileActor;
				TES3::BaseObject* m_Item;
				TES3::ItemData* m_ItemData;
			};
		}
	}
}
