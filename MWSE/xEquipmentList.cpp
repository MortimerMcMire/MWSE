/************************************************************************
	
	xEquipmentList.cpp - Copyright (c) 2008 The MWSE Project
	https://github.com/MWSE/MWSE/

	This program is free software; you can redistribute it and/or
	modify it under the terms of the GNU General Public License
	as published by the Free Software Foundation; either version 2
	of the License, or (at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program; if not, write to the Free Software
	Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

**************************************************************************/

#include "VMExecuteInterface.h"
#include "Stack.h"
#include "InstructionInterface.h"
#include "TES3Util.h"

#include "TES3Armor.h"
#include "TES3Clothing.h"
#include "TES3Enchantment.h"
#include "TES3ItemData.h"
#include "TES3NPC.h"
#include "TES3Reference.h"
#include "TES3Weapon.h"

using namespace mwse;

namespace mwse {
	class xEquipmentList : mwse::InstructionInterface_t {
	public:
		xEquipmentList();
		virtual float execute(VMExecuteInterface &virtualMachine);
		virtual void loadParameters(VMExecuteInterface &virtualMachine);

	private:
		bool nodeMatchesFilter(TES3::IteratorNode<TES3::EquipmentStack>* node, long typeFilter, long subtypeFilter);
		long getItemSubType(TES3::BaseObject* object);
	};

	static xEquipmentList xEquipmentListInstance;

	xEquipmentList::xEquipmentList() : mwse::InstructionInterface_t(OpCode::xEquipmentList) {}

	void xEquipmentList::loadParameters(mwse::VMExecuteInterface &virtualMachine) {}

	float xEquipmentList::execute(mwse::VMExecuteInterface &virtualMachine) {
		// Get parameters.
		TES3::IteratorNode<TES3::EquipmentStack>* node = reinterpret_cast<TES3::IteratorNode<TES3::EquipmentStack>*>(mwse::Stack::getInstance().popLong());
		long typeFilter = mwse::Stack::getInstance().popLong();
		long subtypeFilter = mwse::Stack::getInstance().popLong() - 1;

		// Get reference.
		TES3::Reference* reference = virtualMachine.getReference();
		if (reference == NULL) {
			mwse::log::getLog() << "xEquipmentList: Called without refrence." << std::endl;
			mwse::Stack::getInstance().pushLong(0);
			mwse::Stack::getInstance().pushLong(0);
			mwse::Stack::getInstance().pushLong(0);
			mwse::Stack::getInstance().pushLong(0);
			mwse::Stack::getInstance().pushLong(0);
			mwse::Stack::getInstance().pushFloat(0.0f);
			mwse::Stack::getInstance().pushLong(0);
			mwse::Stack::getInstance().pushLong(0);
			mwse::Stack::getInstance().pushLong(0);
			return 0.0f;
		}

		// Verify actor state.
		TES3::Actor* actor = reinterpret_cast<TES3::Actor*>(reference->baseObject);
		TES3::ObjectType::ObjectType objectType = actor->objectType;
		if (objectType != TES3::ObjectType::NPC && objectType != TES3::ObjectType::Creature) {
			mwse::log::getLog() << "xEquipmentList: Called on non-actor." << std::endl;
			mwse::Stack::getInstance().pushLong(0);
			mwse::Stack::getInstance().pushLong(0);
			mwse::Stack::getInstance().pushLong(0);
			mwse::Stack::getInstance().pushLong(0);
			mwse::Stack::getInstance().pushLong(0);
			mwse::Stack::getInstance().pushFloat(0.0f);
			mwse::Stack::getInstance().pushLong(0);
			mwse::Stack::getInstance().pushLong(0);
			mwse::Stack::getInstance().pushLong(0);
			return 0.0f;
		}

		// Make sure the object isn't a base actor.
		if (actor->actorFlags & TES3::ActorFlag::IsBase) {
			mwse::log::getLog() << "xEquipmentList: Called on base actor. Must be used on instance." << std::endl;
			mwse::Stack::getInstance().pushLong(0);
			mwse::Stack::getInstance().pushLong(0);
			mwse::Stack::getInstance().pushLong(0);
			mwse::Stack::getInstance().pushLong(0);
			mwse::Stack::getInstance().pushLong(0);
			mwse::Stack::getInstance().pushFloat(0.0f);
			mwse::Stack::getInstance().pushLong(0);
			mwse::Stack::getInstance().pushLong(0);
			mwse::Stack::getInstance().pushLong(0);
			return 0.0f;
		}

		// Results.
		char* id = NULL;
		long count = 0;
		long type = 0;
		long subtype = -1;
		long value = 0;
		float weight = 0;
		char* name = NULL;
		char* enchantId = NULL;
		TES3::IteratorNode<TES3::EquipmentStack>* next = NULL;

		// If we aren't given a node, get the first one.
		if (node == NULL) {
			node = actor->equipment.head;

			// Pass over any records that don't match the current filters.
			while (node && !nodeMatchesFilter(node, typeFilter, subtypeFilter)) {
				node = node->next;
			}
		}

		// Validate the node we've obtained.
		if (node && node->data) {
			TES3::BaseObject* object = node->data->object;

			id = reinterpret_cast<TES3::PhysicalObject*>(object)->objectID;
			type = object->objectType;
			value = object->vTable.object->getValue(object);
			weight = object->vTable.object->getWeight(object);
			name = object->vTable.object->getName(object);

			// Get subtype. We can't directly use the vtable here to get the type,
			// because types are zero-indexed.
			subtype = getItemSubType(object) + 1;

			// Get count. Right now we hardcode this to 1, but ammo might actually have a count.
			if (node->data->variables) {
				count = node->data->variables->count;
			}
			else {
				count = 1;
			}

			// Get enchantment id.
			TES3::Enchantment* enchantment = object->vTable.object->getEnchantment(object);
			if (enchantment) {
				enchantId = enchantment->objectID;
			}

			// Pass over any records that don't match the current filters.
			next = node->next;
			while (next && !nodeMatchesFilter(next, typeFilter, subtypeFilter)) {
				next = next->next;
			}
		}

		// Push values to the stack.
		mwse::Stack::getInstance().pushLong((long)next);
		mwse::Stack::getInstance().pushString(enchantId);
		mwse::Stack::getInstance().pushString(name);
		mwse::Stack::getInstance().pushFloat(weight);
		mwse::Stack::getInstance().pushLong(value);
		mwse::Stack::getInstance().pushLong(subtype);
		mwse::Stack::getInstance().pushLong(type);
		mwse::Stack::getInstance().pushLong(count);
		mwse::Stack::getInstance().pushString(id);

		return 0.0f;
	}

	bool xEquipmentList::nodeMatchesFilter(TES3::IteratorNode<TES3::EquipmentStack>* node, long typeFilter, long subtypeFilter) {
		TES3::BaseObject* object = node->data->object;

		if (typeFilter != 0 && object->objectType != typeFilter) {
			return false;
		}

		if (subtypeFilter != -1 && getItemSubType(object) != subtypeFilter) {
			return false;
		}

		return true;
	}

	long xEquipmentList::getItemSubType(TES3::BaseObject* object) {
		if (object == NULL) {
			return -2;
		}

		long subtype = -2;

		long type = object->objectType;
		if (type == TES3::ObjectType::Armor || type == TES3::ObjectType::Apparatus ||
			type == TES3::ObjectType::Clothing || type == TES3::ObjectType::Weapon)
		{
			subtype = object->vTable.object->getType(object);
			log::getLog() << "xEquipmentList::getItemSubType : " << object->vTable.object->getObjectID(object) << " = " << subtype - 1 << std::endl;
		}

		return subtype;
	}
}