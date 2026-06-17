#pragma once

#include "GameFramework/CharacterMovementComponent.h"
#include "FCTCharacterMovementComponent.generated.h"

UCLASS()
class FUNCOMANIMTEST_API UFCTCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()
	
public:
	FVector GetOriginalRootMotionVelocityThisFrame() const { return OriginalRootMotionVelocity; };
	
protected:
	virtual void ApplyRootMotionToVelocity(float deltaTime) override;
	
private:
	FVector OriginalRootMotionVelocity = FVector::ZeroVector;
	FVector PreviousInput = FVector::ZeroVector;
	
};
