#pragma once

#include "Animation/CharInfoDefinitions.h"
#include "GameFramework/Character.h"
#include "Animation/FCTBaseAnimInstance.h"
#include "FCTBaseCharacter.generated.h"

class UCharacterMovementComponent;
class UInventoryComponent;

struct FEquipableItemData;

// Centralized place to keep some extra data that is shared between gameplay and animation
UCLASS()
class FUNCOMANIMTEST_API UCharInfo : public UObject
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadOnly)
	TWeakObjectPtr<const UCharacterMovementComponent> MovementComponent;
	
	UPROPERTY(BlueprintReadOnly)
	TWeakObjectPtr<const UInventoryComponent> InventoryComponent;
	
	UPROPERTY(BlueprintReadOnly)
	FVector LocalVelocity = FVector::ZeroVector;
	
	UPROPERTY(BlueprintReadOnly)
	FVector LocalOriginalAniRootVelocity = FVector::ZeroVector;
	
	UPROPERTY(BlueprintReadOnly)
	ECardinalDirections MovementDir = ECardinalDirections::Forward;
	
	UPROPERTY(BlueprintReadOnly)
	ECardinalDirections PreviousMovementDir = ECardinalDirections::Forward;
	
	UPROPERTY(BlueprintReadOnly)
	ELocomotionTypes CurrentLocoType = ELocomotionTypes::Grounded;
	
	UPROPERTY(BlueprintReadOnly)
	ELocomotionMode CurrentLocoMode = ELocomotionMode::Standing;
	
	UPROPERTY(BlueprintReadOnly)
	EMovementGait CurrentGait = EMovementGait::Jogging;
	
	UPROPERTY(BlueprintReadOnly)
	bool bIsMoving = false;
	
	UPROPERTY(BlueprintReadOnly)
	bool bHasSpeed = false;
};

UCLASS()
class FUNCOMANIMTEST_API AFCTBaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AFCTBaseCharacter(const FObjectInitializer& ObjectInitializer);
	
	UFUNCTION(BlueprintCallable)
	void SwitchEquippedItem(EEquippableItem NewItem);
	
	virtual void Tick(float DeltaTime) override;
	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	const UCharInfo* GetCharInfo() const { return CharInfo; }

protected:
	virtual void BeginPlay() override;
	
	UFUNCTION()
	void IA_Move(const FInputActionValue& InValue);
	
	UFUNCTION()
	void IA_Look(const FInputActionValue& InValue);
	
	UFUNCTION()
	void IA_Crouch(const FInputActionValue& InValue);
	
	UFUNCTION()
	void IA_ToggleWalk(const FInputActionValue& InValue);
	
	UFUNCTION()
	void IA_SwitchEquipment(const FInputActionValue& InValue);
	
	UFUNCTION()
	void IA_Jump(const FInputActionValue& InValue);
	
	UFUNCTION()
	void IA_Swim(const FInputActionValue& InValue);

private:
	UFUNCTION()
	void OnLanding(const FHitResult& HitResult);
	
	ECardinalDirections GetCardinalDir(const FVector& LastInputVector) const;
	
protected:	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UInventoryComponent> InventoryComponent;
	
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UCharInfo> CharInfo;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TMap<ELocomotionTypes, TSubclassOf<UFCTBaseAnimInstance>> LocomotionTypeData;

	TWeakObjectPtr<UFCTBaseAnimInstance> BaseAnimInstance;
	
	uint16 RootMotionSourceID = 0;
};
