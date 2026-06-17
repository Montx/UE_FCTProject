#include "Characters/FCTBaseCharacter.h"
#include "InputActionValue.h"
#include "Characters/FCTCharacterMovementComponent.h"
#include "Characters/Components/InventoryComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

#define DOT_VALUE_35_DEG 0.8192

// Sets default values
AFCTBaseCharacter::AFCTBaseCharacter(const FObjectInitializer& ObjectInitializer) 
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UFCTCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
	PrimaryActorTick.bCanEverTick = true;
	
	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>(TEXT("InventoryComponent"));
	
	CharInfo = CreateDefaultSubobject<UCharInfo>(TEXT("CharInfo"));
	
	CharInfo->MovementComponent = GetCharacterMovement();
	CharInfo->InventoryComponent = InventoryComponent;
}

// Called when the game starts or when spawned
void AFCTBaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	BaseAnimInstance = Cast<UFCTBaseAnimInstance>(GetMesh()->GetAnimInstance());
	
	LandedDelegate.AddDynamic(this, &AFCTBaseCharacter::OnLanding);
}

void AFCTBaseCharacter::IA_Move(const FInputActionValue& InValue)
{
	FVector2D InputValue = InValue.Get<FVector2D>();
	
	FRotator ControlRot = GetControlRotation();
	
	if (!GetCharacterMovement()->IsFlying())
	{
		ControlRot.Pitch = 0.f;
	}
	
	AddMovementInput(ControlRot.Vector(), InputValue.X);
	
	FVector RightVector = ControlRot.RotateVector(FVector::RightVector);
	AddMovementInput(RightVector, InputValue.Y);
}

void AFCTBaseCharacter::IA_Look(const FInputActionValue& InValue)
{
	FVector2D InputValue = InValue.Get<FVector2D>();
	
	AddControllerPitchInput(InputValue.Y);
	AddControllerYawInput(InputValue.X);
}

void AFCTBaseCharacter::IA_Crouch(const FInputActionValue& InValue)
{
	if (!IsValid(CharInfo) || !CharInfo->MovementComponent.IsValid())
	{
		return;
	}
	
	CharInfo->MovementComponent->bWantsToCrouch ? UnCrouch() : Crouch();
	
	if (!BaseAnimInstance.IsValid())
	{
		return;
	}
	
	CharInfo->CurrentLocoMode = CharInfo->MovementComponent->bWantsToCrouch ? ELocomotionMode::Crouching : ELocomotionMode::Standing;
	CharInfo->CurrentGait = CharInfo->MovementComponent->bWantsToCrouch ? EMovementGait::Walking : EMovementGait::Jogging;
	
	BaseAnimInstance->UpdateAnimationSlots();
}

void AFCTBaseCharacter::IA_ToggleWalk(const FInputActionValue& Instance)
{
	if (!IsValid(CharInfo) || !CharInfo->MovementComponent.IsValid())
	{
		return;
	}
	
	if (CharInfo->MovementComponent->bWantsToCrouch)
	{
		return;
	}
	
	CharInfo->CurrentGait = CharInfo->CurrentGait == EMovementGait::Walking ? EMovementGait::Jogging : EMovementGait::Walking;
	
	BaseAnimInstance->UpdateAnimationSlots();
}

void AFCTBaseCharacter::IA_SwitchEquipment(const FInputActionValue& InValue)
{
	if (!IsValid(InventoryComponent))
	{
		return;
	}
	
	EEquippableItem Item = InventoryComponent->GetEquippedItem();
	EEquippableItem NewItem = StaticCast<EEquippableItem>(StaticCast<uint8>(Item) << 1);
	if (NewItem == EEquippableItem::Invalid || NewItem == EEquippableItem::None)
	{
		NewItem = EEquippableItem::Unarmed;
	}
	
	InventoryComponent->SetEquippedItem(NewItem);
	
	BaseAnimInstance->UpdateAnimationSlots();
}

void AFCTBaseCharacter::IA_Jump(const FInputActionValue& InValue)
{
	BaseAnimInstance->SetIgnoreRootMotionMode();
	
	FVector RootMotionVelocity = GetCharacterMovement()->GetLastUpdateVelocity();
	GetCharacterMovement()->CurrentRootMotion.Clear();
	
	GetCharacterMovement()->Velocity.X = RootMotionVelocity.X;
	GetCharacterMovement()->Velocity.Y = RootMotionVelocity.Y;
	
	Jump();
}

void AFCTBaseCharacter::IA_Swim(const FInputActionValue& InValue)
{
	if (LocomotionTypeData.IsEmpty())
	{
		return;
	}
	
	ELocomotionTypes TargetLocoType = ELocomotionTypes::Swimming;
	if (CharInfo->CurrentLocoType == ELocomotionTypes::Swimming)
	{
		TargetLocoType = ELocomotionTypes::Grounded;
	}
	
	TSubclassOf<UFCTBaseAnimInstance>* AnimClass = LocomotionTypeData.Find(TargetLocoType);
	if (!AnimClass)
	{
		return;
	}
	
	CharInfo->CurrentLocoType = TargetLocoType;
	
	GetMesh()->LinkAnimClassLayers(*AnimClass);
	
	if (CharInfo->CurrentLocoType == ELocomotionTypes::Swimming)
	{
		GetCharacterMovement()->SetMovementMode(MOVE_Flying);
		BaseAnimInstance->SetIgnoreRootMotionMode();
	}
	else
	{
		GetCharacterMovement()->SetMovementMode(MOVE_Walking);
		BaseAnimInstance->RestoreRootMotionMode();
	}
	
	BaseAnimInstance->UpdateAnimationSlots();
}

void AFCTBaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	float Speed = GetVelocity().Size();
	
	UCharacterMovementComponent* MovementComponent = GetCharacterMovement();
	if (!IsValid(MovementComponent))
	{
		return;
	}
	
	CharInfo->bHasSpeed = MovementComponent->GetLastInputVector().SizeSquared() > SMALL_NUMBER;
	CharInfo->bIsMoving = MovementComponent->IsMovingOnGround() && CharInfo->bHasSpeed;
	
	CharInfo->PreviousMovementDir = MovementComponent->GetLastInputVector().SizeSquared2D() > SMALL_NUMBER ? CharInfo->MovementDir : CharInfo->PreviousMovementDir;
	CharInfo->MovementDir = GetCardinalDir(MovementComponent->GetLastInputVector());
	
	CharInfo->LocalVelocity = GetActorRotation().UnrotateVector(GetVelocity());
	
	if (UFCTCharacterMovementComponent* FCTCharMovement = Cast<UFCTCharacterMovementComponent>(GetMovementComponent()))
	{
		CharInfo->LocalOriginalAniRootVelocity = GetActorRotation().UnrotateVector(FCTCharMovement->GetOriginalRootMotionVelocityThisFrame());
	}
}

void AFCTBaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AFCTBaseCharacter::SwitchEquippedItem(EEquippableItem NewItem)
{
	if (!IsValid(InventoryComponent))
	{
		return;	
	}
	
	InventoryComponent->SetEquippedItem(NewItem);
}

ECardinalDirections AFCTBaseCharacter::GetCardinalDir(const FVector& LastInputVector) const
{
	float RightDot = FVector::DotProduct(LastInputVector.GetSafeNormal2D(), GetActorRightVector());
	if (RightDot >= DOT_VALUE_35_DEG)
	{
		return ECardinalDirections::Right;
	}
	else if (RightDot <= -DOT_VALUE_35_DEG)
	{
		return ECardinalDirections::Left;
	}
	else
	{
		float ForwardDot = FVector::DotProduct(LastInputVector.GetSafeNormal2D(), GetActorForwardVector());
		return ForwardDot >= 0 ? ECardinalDirections::Forward : ECardinalDirections::Backward;
	}
}

void AFCTBaseCharacter::OnLanding(const FHitResult& HitResult)
{
	BaseAnimInstance->RestoreRootMotionMode();
}
