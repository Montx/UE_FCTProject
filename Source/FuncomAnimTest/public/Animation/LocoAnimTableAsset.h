#pragma once

#include "Animation/AnimTableAsset.h"
#include "LocoAnimTableAsset.generated.h"

enum class ECardinalDirections : uint8;
enum class EMovementGait : uint8;

USTRUCT(BlueprintType)
struct FDirectionalAnimData
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TMap<ECardinalDirections, TObjectPtr<UAnimSequenceBase>> DirectionalAnimTable;
	
};

USTRUCT(BlueprintType)
struct FMovementGaitAnimData
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TMap<EMovementGait, FDirectionalAnimData> MovementAnimTable;
	
};

/**
 * Locomotion Type Anim Slot to animations sequence table
 */
UCLASS()
class FUNCOMANIMTEST_API ULocoAnimTableAsset : public UAnimTableAsset
{
	GENERATED_BODY()
	
public:
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TMap<TObjectPtr<UAnimSlotAsset>, FMovementGaitAnimData> LocoAnimTable;
};
