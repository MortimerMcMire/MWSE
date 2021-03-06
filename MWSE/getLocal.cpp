/************************************************************************
               getLocal.cpp - Copyright (c) 2008 The MWSE Project
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

using namespace mwse;

namespace mwse
{
	//----------------------------------------
	class getLocal : public InstructionInterface_t
	{
	public:
		getLocal();
		virtual float execute(VMExecuteInterface &virtualMachine);
		virtual void loadParameters(VMExecuteInterface &virtualMachine);
	};

	static getLocal getLocalInstance;

	getLocal::getLocal(): InstructionInterface_t(OpCode::_GetLocal){}

	void getLocal::loadParameters(mwse::VMExecuteInterface &virtualMachine){}

	float getLocal::execute(mwse::VMExecuteInterface &virtualMachine)
	{
		char type = Stack::getInstance().popByte();
		short index = Stack::getInstance().popShort();

		switch(type)
		{
		case 's':
			{
				Stack::getInstance().pushShort(virtualMachine.getShortVariable(index));
			}
			break;
		case 'l':
			{
				Stack::getInstance().pushLong(virtualMachine.getLongVariable(index));
			}
			break;
		case 'f':
			{
				Stack::getInstance().pushFloat(virtualMachine.getFloatVariable(index));
			}
			break;
		default:
			{
				//throw exception
			}
			break;
		}
		return 0.0f;
	}
	//----------------------------------------

	//----------------------------------------
	class setLocal : public InstructionInterface_t
	{
	public:
		setLocal();
		virtual float execute(VMExecuteInterface &virtualMachine);
		virtual void loadParameters(VMExecuteInterface &virtualMachine);
	private:
	};

	static setLocal setLocalInstance;

	setLocal::setLocal(): InstructionInterface_t(OpCode::_SetLocal){}

	void setLocal::loadParameters(mwse::VMExecuteInterface &virtualMachine){}

	float setLocal::execute(mwse::VMExecuteInterface &virtualMachine)
	{
		char type = Stack::getInstance().popByte();
		char index = Stack::getInstance().popByte();

		switch(type)
		{
		case 's':
			{
				virtualMachine.setShortVariable(index, Stack::getInstance().popShort() );	//get value of stack
			}
			break;
		case 'l':
			{
				virtualMachine.setLongVariable(index, Stack::getInstance().popLong() );
			}
			break;
		case 'f':
			{
				virtualMachine.setFloatVariable(index, Stack::getInstance().popFloat() );
			}
			break;
		default:
			{
				//throw exception
			}
			break;
		}
		return 0.0f;
	}
	//----------------------------------------
}