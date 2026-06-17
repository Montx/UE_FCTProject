#include "Characters/FCTCharacterMovementComponent.h"

/*
 * This function is the easiest way to steer the character with root motion towards player input, it overrides velocity after root motion applies to it.
 * A better approach might be using ProcessRootMotionPreConvertToWorld to actually rotate the extract root motion before
 * its converted into world space, or creating custom FRootMotionSource that will adjust the extract root motion every turn.
 */
void UFCTCharacterMovementComponent::ApplyRootMotionToVelocity(float deltaTime)
{
	Super::ApplyRootMotionToVelocity(deltaTime);
	
	if (HasAnimRootMotion())
	{
		OriginalRootMotionVelocity = Velocity;
		
		float CurrentSpeed = Velocity.Size2D();
		if (CurrentSpeed < SMALL_NUMBER)
		{
			return;
		}
		
		FVector InputDir = GetLastInputVector().GetSafeNormal();
		if (InputDir.Size2D() < SMALL_NUMBER)
		{
			InputDir = PreviousInput;
		}
			
		FVector VelocityOverride = InputDir * CurrentSpeed;
		Velocity.X = VelocityOverride.X;
		Velocity.Y = VelocityOverride.Y;
		
		PreviousInput = InputDir;
	}
	else
	{
		OriginalRootMotionVelocity = FVector::ZeroVector;
	}
	
	
	
}
