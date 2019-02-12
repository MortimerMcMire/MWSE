#pragma once

#include "LuaObjectFilteredEvent.h"

namespace mwse {
	namespace lua {
		namespace event {
			class InfoResponseEvent : public ObjectFilteredEvent {
			public:
				InfoResponseEvent(const char* command, TES3::Reference * reference, TES3::ScriptVariables * variables, TES3::Dialogue * dialogue, TES3::DialogueInfo * info);
				sol::table createEventTable();

			protected:
				const char* m_Command;
				TES3::Reference* m_Reference;
				TES3::ScriptVariables* m_ScriptVariables;
				TES3::Dialogue* m_Dialogue;
				TES3::DialogueInfo* m_DialogueInfo;
			};
		}
	}
}
