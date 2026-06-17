// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/DataAsset.h"
#include "AnimTableAsset.generated.h"

class UAnimSlotAsset;
class AnimSequenceBase;

/**
 * 
 */
UCLASS(BlueprintType)
class FUNCOMANIMTEST_API UAnimTableAsset : public UDataAsset
{
	GENERATED_BODY()
	
public:
	
	UPROPERTY(EditDefaultsOnly)
	TMap<TObjectPtr<UAnimSlotAsset>, TObjectPtr<UAnimSequenceBase>> AnimTable;
	
};
