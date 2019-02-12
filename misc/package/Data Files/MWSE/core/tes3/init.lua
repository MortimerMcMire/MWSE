local tes3 = {}

-- Game constants.
tes3.aiPackage = require("tes3.aiPackage")
tes3.animationState = require("tes3.animationState")
tes3.apparatusType = require("tes3.apparatusType")
tes3.armorSlot = require("tes3.armorSlot")
tes3.attachmentType = require("tes3.attachmentType")
tes3.attribute = require("tes3.attribute")
tes3.attributeName = require("tes3.attributeName")
tes3.bookType = require("tes3.bookType")
tes3.clothingSlot = require("tes3.clothingSlot")
tes3.creatureType = require("tes3.creatureType")
tes3.effect = require("tes3.effect")
tes3.effectRange = require("tes3.effectRange")
tes3.enchantmentType = require("tes3.enchantmentType")
tes3.gmst = require("tes3.gmst")
tes3.keybind = require("tes3.keybind")
tes3.keyTransition = require("tes3.keyTransition")
tes3.magicSchool = require("tes3.magicSchool")
tes3.musicSituation = require("tes3.musicSituation")
tes3.niType = require("tes3.niType")
tes3.objectType = require("tes3.objectType")
tes3.scanCode = require("tes3.scanCode")
tes3.scanCodeToNumber = require("tes3.scanCodeToNumber")
tes3.skill = require("tes3.skill")
tes3.skillName = require("tes3.skillName")
tes3.skillType = require("tes3.skillType")
tes3.soundGenType = require("tes3.soundGenType")
tes3.soundMix = require("tes3.soundMix")
tes3.specialization = require("tes3.specialization")
tes3.specializationName = require("tes3.specializationName")
tes3.spellOrigin = require("tes3.spellOrigin")
tes3.spellState = require("tes3.spellState")
tes3.spellType = require("tes3.spellType")
tes3.uiProperty = require("tes3.uiProperty")
tes3.voiceover = require("tes3.voiceover")
tes3.weaponType = require("tes3.weaponType")
tes3.weather = require("tes3.weather")

-- Second pass game constants, declared based on the above constants.
tes3.magicSchoolSkill = {
	[tes3.magicSchool.alteration] = tes3.skill.alteration,
	[tes3.magicSchool.conjuration] = tes3.skill.conjuration,
	[tes3.magicSchool.destruction] = tes3.skill.destruction,
	[tes3.magicSchool.illusion] = tes3.skill.illusion,
	[tes3.magicSchool.mysticism] = tes3.skill.mysticism,
	[tes3.magicSchool.restoration] = tes3.skill.restoration,
}

-------------------------------------------------
-- Functions
-------------------------------------------------

-- Translate an attribute constant to a readable name.
function tes3.getAttributeName(attributeId)
	return tes3.attributeName[attributeId] or "invalid"
end

-- Translate an skill constant to a readable name.
function tes3.getSkillName(skillId)
	return tes3.skillName[skillId] or "invalid"
end

-- Translate an specialization constant to a readable name.
function tes3.getSpecializationName(specializationId)
	return tes3.specializationName[specializationId] or "invalid"
end

-- Return an attachment from a reference.
function tes3.getAttachment(reference, attachment)
	return reference and reference.attachments and reference.attachments[attachment]
end

-- Get a reference's owner.
function tes3.getOwner(reference)
	local vars = tes3.getAttachment(reference, "variables")
	if (vars) then
		return vars.owner
	end
end

-- Iterator to use TES3::Iterator in a for loop.
function tes3.iterate(iterator)
	local node = iterator.head
	return function()
		if (node) then
			local data = node.nodeData
			node = node.nextNode
			return data
		end
	end
end

-- Iterator to use TES3::TArray in a for loop.
function tes3.loopTArray(tarray)
	local index = 0
	local length = #tarray
	return function()
		index = index + 1
		if (index <= length) then
			return tarray[index]
		end
	end
end

-- Shortcut for the current MenuMode state.
function tes3.menuMode()
	return tes3.getWorldController().flagMenuMode
end

-- Shortcut to check if we're in the main menu.
function tes3.onMainMenu()
	return tes3.worldController.charGenState.value == 0
end

return tes3