#pragma once

#include "GameFramework/PlayerController.h"
#include "FCTPlayerController.generated.h"

class UInputMappingContext;

UCLASS()
class FUNCOMANIMTEST_API AFCTPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	virtual void AcknowledgePossession(APawn* PossedPawn) override;
	
private:
	void SetupInputForPawn(APawn* PossedPawn);

protected:
	UPROPERTY(EditDefaultsOnly, Category=Input)
	TObjectPtr<UInputMappingContext> MappingContext;
};
