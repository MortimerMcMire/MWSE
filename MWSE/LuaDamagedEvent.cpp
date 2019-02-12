#include "LuaDamagedEvent.h"

#include "LuaManager.h"
#include "LuaUtil.h"

#include "TES3MobileActor.h"
#include "TES3Reference.h"

namespace mwse {
	namespace lua {
		namespace event {
			DamagedEvent::DamagedEvent(TES3::MobileActor* mobileActor, float damage) :
				ObjectFilteredEvent("damaged", mobileActor->reference),
				m_MobileActor(mobileActor),
				m_Damage(damage)
			{

			}

			sol::table DamagedEvent::createEventTable() {
				sol::state& state = LuaManager::getInstance().getState();
				sol::table eventData = state.create_table();

				eventData["mobile"] = makeLuaObject(m_MobileActor);
				eventData["reference"] = makeLuaObject(m_MobileActor->reference);
				eventData["damage"] = m_Damage;

				return eventData;
			}
		}
	}
}
