#pragma once

#include "CharInfoDefinitions.h"
#include "Animation/AnimInstance.h"
#include "FCTBaseAnimInstance.generated.h"

class UAnimSlotAsset;
class UAnimTableAsset;
class ULocoAnimTableAsset;
class UCharInfo;

enum class ELocomotionMode : uint8;
enum class ECardinalDirections : uint8;

// Generic Animation Slot Data that maps to an anim sequence
USTRUCT(BlueprintType)
struct FAnimSlotData
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UAnimSlotAsset> AnimSlot;
	
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UAnimSequenceBase> AnimSequence;
};

// Locomotion Type Anim slot data which maps anim sequences to directions
USTRUCT(BlueprintType)
struct FLocoAnimSlotData
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UAnimSlotAsset> AnimSlot;
	
	UPROPERTY(BlueprintReadOnly)
	TMap<ECardinalDirections, TObjectPtr<UAnimSequenceBase>> AnimSequences;
};

/**
 * AnimInstance that supports anim slots
 */
UCLASS()
class FUNCOMANIMTEST_API UFCTBaseAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	virtual void NativeInitializeAnimation() override;
	
	const UAnimTableAsset* GetAnimTableForLocomotionMode(const ELocomotionMode& LocomotionMode, const EMovementGait& MovementGait);
	
	void UpdateAnimationSlots();
	
	void SetIgnoreRootMotionMode();
	void RestoreRootMotionMode();

	UFUNCTION(BlueprintNativeEvent)
	void EvaluateSlotsForAnimTable(const UAnimTableAsset* AnimTable);
	void EvaluateSlotsForAnimTable_Implementation(const UAnimTableAsset* AnimTable);
	
	UFUNCTION(BlueprintCallable)
	void EvaluateAnimSlot(const UAnimTableAsset* AnimTableAsset, UPARAM(ref) FAnimSlotData& AnimSlotData);
	
	UFUNCTION(BlueprintCallable)
	void EvaluateLocoAnimSlot(const UAnimTableAsset* AnimTableAsset, UPARAM(ref) FLocoAnimSlotData& AnimSlotData);
	
private:
	void SetupInstance();
	
	bool IsMainInstance() const;

protected:
	UPROPERTY(BlueprintReadOnly)
	TWeakObjectPtr<const UCharInfo> CharInfo;
	
private:
	ERootMotionMode::Type DefaultRootMotionMode;
};
