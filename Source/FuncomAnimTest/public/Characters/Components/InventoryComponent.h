// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "InventoryComponent.generated.h"

class UAnimTableAsset;

enum class ELocomotionMode : uint8;

UENUM(BlueprintType, meta=(Bitflags))
enum class EEquippableItem : uint8
{
	None = 0x00,
	Unarmed = 0x01,
	Pistol = 0x02,
	Invalid = 0x04
};

USTRUCT(BlueprintType)
struct FEquipableItemData
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TMap<ELocomotionMode, TObjectPtr<UAnimTableAsset>> AnimTables;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

public:	
	EEquippableItem GetEquippedItem() const { return EquippedItem; }
	void SetEquippedItem(const EEquippableItem& NewItem);
	
	const FEquipableItemData* GetEquippedItemData() const;
	
	const UAnimTableAsset* GetAnimTableForLocoMode(const ELocomotionMode& Mode) const;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TMap<EEquippableItem, FEquipableItemData> EquippableData;
	
	UPROPERTY(BlueprintReadWrite)
	EEquippableItem EquippedItem = EEquippableItem::Unarmed;
	
private:
	// This is set to mutable exclusively because it's used inside GetEquippedItemData to cache a pointer to the data,
	// so we don't have to find everytime
	// Alternatively one could cache inside SetEquippedItem, thus mutable would no longer be needed
	mutable const FEquipableItemData* CachedEquippedItemData = nullptr;
};
