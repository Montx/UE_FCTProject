#include "Player/FCTPlayerController.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"

void AFCTPlayerController::AcknowledgePossession(APawn* PossedPawn)
{
	Super::AcknowledgePossession(PossedPawn);
		
	if (PossedPawn->IsLocallyControlled())
	{
		SetupInputForPawn(PossedPawn);
	}
}

void AFCTPlayerController::SetupInputForPawn(APawn* PossedPawn)
{
	if (!IsValid(InputComponent))
	{
		return;
	}
	
	if (!IsValid(MappingContext))
	{
		return;
	}
	
	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent);
	if (!IsValid(EnhancedInputComponent))
	{
		return;
	}
	
	if (ULocalPlayer* LocalPlayer = Cast<ULocalPlayer>(Player))
	{
		if (UEnhancedInputLocalPlayerSubsystem* InputSystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			FModifyContextOptions Options;
			InputSystem->AddMappingContext(MappingContext, 0, Options);
			
			for (const FEnhancedActionKeyMapping& Mapping : MappingContext->GetMappings())
			{
				// Looks for implemented UFUNCTION with the same name as the action and binds to it
				if (UFunction* Function = PossedPawn->FindFunction(Mapping.Action.GetFName()))
				{
					EnhancedInputComponent->BindAction(Mapping.Action, ETriggerEvent::Triggered, PossedPawn, Mapping.Action.GetFName());
				}
			}
		}
	}
}
