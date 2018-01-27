/************************************************************************
	
	xNextStack.cpp - Copyright (c) 2008 The MWSE Project
	http://www.sourceforge.net/projects/mwse

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

using namespace mwse;

namespace mwse
{
	class xNextStack : mwse::InstructionInterface_t
	{
	public:
		xNextStack();
		virtual float execute(VMExecuteInterface &virtualMachine);
		virtual void loadParameters(VMExecuteInterface &virtualMachine);
	};

	static xNextStack xNextStackInstance;

	xNextStack::xNextStack() : mwse::InstructionInterface_t(OpCode::xNextStack) {}

	void xNextStack::loadParameters(mwse::VMExecuteInterface &virtualMachine) {}

	float xNextStack::execute(mwse::VMExecuteInterface &virtualMachine)
	{
		// Get the passed node.
		ListNode_t<InventoryNode_t>* node = reinterpret_cast<ListNode_t<InventoryNode_t>*>(mwse::Stack::getInstance().popLong());
		if (node == NULL) {
			mwse::Stack::getInstance().pushLong(0);
			mwse::Stack::getInstance().pushLong(0);
			mwse::Stack::getInstance().pushLong(0);
			return 0.0f;
		}

		mwse::Stack::getInstance().pushLong((mwLong_t)node->nextNode);
		mwse::Stack::getInstance().pushLong(node->dataPtr->itemCount);
		mwse::Stack::getInstance().pushString(reinterpret_cast<TES3DefaultTemplate_t*>(node->dataPtr->recordAddress)->objectId);

		return 0.0f;
	}
}