#pragma once

#include "TES3Defines.h"

#include "TES3Item.h"
#include "TES3Vectors.h"

namespace TES3 {
	namespace AttachmentType {
		enum AttachmentType {
			Animation = 0x0,
			Lock = 0x3,
			TravelDestination = 0x5,
			Variables = 0x6,
			ActorData = 0x8,
			Action = 0x9,
			NewOrientation = 0xA
		};
	}

	struct Attachment {
		AttachmentType::AttachmentType type;
		Attachment * next;
	};
	static_assert(sizeof(Attachment) == 0x8, "TES3::Attachment failed size validation");

	template <typename T>
	struct AttachmentWithNode : Attachment {
		T * data;
	};
	static_assert(sizeof(AttachmentWithNode<void>) == 0xC, "TES3::AttachmentWithNode failed size validation");

	//
	// Animation data.
	//

	typedef AttachmentWithNode<AnimationData> AnimationAttachment;

	//
	// Locks
	//

	struct LockAttachmentNode {
		int lockLevel; // 0x00
		Misc * key; // 0x04
		Spell * trap; // 0x08
		bool locked; // 0x0C
	};
	static_assert(sizeof(LockAttachmentNode) == 0x10, "TES3::LockAttachmentNode failed size validation");

	typedef AttachmentWithNode<LockAttachmentNode> LockAttachment;

	//
	// TravelDestination
	//

	struct TravelDestination {
		Cell * cell; // 0x0
		char * cellName; // 0x4
		Reference * destination; // 0x8
	};
	static_assert(sizeof(TravelDestination) == 0xC, "TES3::TravelDestination failed size validation");

	typedef AttachmentWithNode<TravelDestination> TravelDestinationAttachment;

	//
	// Variables
	//

	typedef AttachmentWithNode<ItemData> ItemDataAttachment;

	//
	// Ownership
	//

	struct OwnershipAttachmentNode {
		int unknown_0x00;
		BaseObject * owner; // 0x04
		union {
			long rank;
			void * variable;
		} rankVar; // 0x08
	};
	static_assert(sizeof(OwnershipAttachmentNode) == 0x0C, "TES3::OwnershipAttachmentNode failed size validation");

	typedef AttachmentWithNode<OwnershipAttachmentNode> OwnershipAttachment;

	//
	// Actor Data / Mobile Object
	//

	typedef AttachmentWithNode<MobileActor> MobileActorAttachment;

	//
	// Action
	//

	namespace ActionFlags {
		typedef unsigned int value_type;

		enum Flag : value_type {
			UseEnabled = 0x1,
			OnActivate = 0x2,
			OnDeath = 0x10,
			OnKnockout = 0x20,
			OnMurder = 0x40,
			DoorOpening = 0x100,
			DoorClosing = 0x200,
			DoorJammedOpening = 0x400,
			DoorJammedClosing = 0x800
		};
	}

	struct ActionAttachment : Attachment {
		ActionFlags::value_type flags;
		Reference * reference;
	};
	static_assert(sizeof(ActionAttachment) == 0x10, "TES3::ActionAttachment failed size validation");
	
	//
	// New Orientation
	//

	struct NewOrientationAttachment : Attachment {
		Vector3 position;
		Vector3 orientation;
	};
	static_assert(sizeof(NewOrientationAttachment) == 0x20, "TES3::NewOrientationAttachment failed size validation");
}
