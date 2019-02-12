#pragma once

#include "LuaGenericEvent.h"

#include "TES3Defines.h"

namespace mwse {
	namespace lua {
		namespace event {
			class JournalEvent : public GenericEvent {
			public:
				JournalEvent(TES3::Dialogue * topic, int oldIndex, int newIndex);
				sol::table createEventTable();

			protected:
				TES3::Dialogue * m_Topic;
				int m_OldIndex;
				int m_NewIndex;
			};
		}
	}
}
