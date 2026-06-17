#include "Characters/Components/InventoryComponent.h"

void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	CachedEquippedItemData = GetEquippedItemData();
}

void UInventoryComponent::SetEquippedItem(const EEquippableItem& NewItem)
{
	EquippedItem = NewItem;
	CachedEquippedItemData = nullptr;
}

const FEquipableItemData* UInventoryComponent::GetEquippedItemData() const
{
	if (CachedEquippedItemData)
	{
		return CachedEquippedItemData;
	}
	
	if (EquippableData.IsEmpty())
	{
		return nullptr;
	}
	
	CachedEquippedItemData = EquippableData.Find(EquippedItem);
	return CachedEquippedItemData;
}

const UAnimTableAsset* UInventoryComponent::GetAnimTableForLocoMode(const ELocomotionMode& Mode) const
{
	if (!CachedEquippedItemData)
	{
		CachedEquippedItemData = GetEquippedItemData();
	}
	
	if (const TObjectPtr<UAnimTableAsset>* AnimTable = CachedEquippedItemData->AnimTables.Find(Mode))
	{
		return *AnimTable;
	}
	
	return nullptr;
}

