#include "Animation/FCTBaseAnimInstance.h"

#include "Animation/AnimTableAsset.h"
#include "Animation/LocoAnimTableAsset.h"
#include "Characters/FCTBaseCharacter.h"
#include "Characters/Components/InventoryComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

void UFCTBaseAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	
	SetupInstance();
}

const UAnimTableAsset* UFCTBaseAnimInstance::GetAnimTableForLocomotionMode(const ELocomotionMode& LocomotionMode, const EMovementGait& MovementGait)
{
	if (!CharInfo.IsValid() || !CharInfo->InventoryComponent.IsValid())
	{
		return nullptr;
	}
	
	return CharInfo->InventoryComponent->GetAnimTableForLocoMode(LocomotionMode);
}

void UFCTBaseAnimInstance::UpdateAnimationSlots()
{
	EvaluateSlotsForAnimTable(GetAnimTableForLocomotionMode(CharInfo->CurrentLocoMode, CharInfo->CurrentGait));
	
	// This is usually called by gameplay
	// If it's the main instance it will call UpdateAnimationSlots for every linked anim graph of the same anim class
	if (IsMainInstance())
	{
		GetOwningComponent()->ForEachAnimInstance([this](UAnimInstance* AnimInstance)
		{
			if (AnimInstance == this)
			{
				return;
			}
			
			if (UFCTBaseAnimInstance* FCTAnimInstance = Cast<UFCTBaseAnimInstance>(AnimInstance))
			{
				FCTAnimInstance->UpdateAnimationSlots();
			}
		});
	}
	
}

void UFCTBaseAnimInstance::SetIgnoreRootMotionMode()
{
	RootMotionMode = ERootMotionMode::IgnoreRootMotion;
}

void UFCTBaseAnimInstance::RestoreRootMotionMode()
{
	RootMotionMode = DefaultRootMotionMode;
}

void UFCTBaseAnimInstance::EvaluateSlotsForAnimTable_Implementation(const UAnimTableAsset* AnimTable)
{
}

void UFCTBaseAnimInstance::EvaluateAnimSlot(const UAnimTableAsset* AnimTableAsset, UPARAM(ref) FAnimSlotData& AnimSlotData)
{
	if (!IsValid(AnimTableAsset))
	{
		return;
	}
	
	if (const TObjectPtr<UAnimSequenceBase>* AnimSequence = AnimTableAsset->AnimTable.Find(AnimSlotData.AnimSlot))
	{
		AnimSlotData.AnimSequence = *AnimSequence;
	}
}

void UFCTBaseAnimInstance::EvaluateLocoAnimSlot(const UAnimTableAsset* AnimTableAsset, FLocoAnimSlotData& AnimSlotData)
{
	const ULocoAnimTableAsset* LocoAnimTable = Cast<ULocoAnimTableAsset>(AnimTableAsset);
	if (!IsValid(LocoAnimTable) || !CharInfo.IsValid())
	{
		return;
	}
	
	if (const FMovementGaitAnimData* AnimData = LocoAnimTable->LocoAnimTable.Find(AnimSlotData.AnimSlot))
	{
		if (const FDirectionalAnimData* DirAnimData = AnimData->MovementAnimTable.Find(CharInfo->CurrentGait))
		{
			AnimSlotData.AnimSequences = DirAnimData->DirectionalAnimTable;
		}
	}
}

void UFCTBaseAnimInstance::SetupInstance()
{
	AFCTBaseCharacter* Character = Cast<AFCTBaseCharacter>(GetOwningActor());
	if (!IsValid(Character))
	{
		return;
	}
	
	CharInfo = Character->GetCharInfo();
	if (!CharInfo.IsValid())
	{
		return;
	}
	
	EvaluateSlotsForAnimTable(GetAnimTableForLocomotionMode(CharInfo->CurrentLocoMode, CharInfo->CurrentGait));
	DefaultRootMotionMode = RootMotionMode;
}

bool UFCTBaseAnimInstance::IsMainInstance() const
{
	return GetOwningComponent()->GetAnimInstance() == this;
}
