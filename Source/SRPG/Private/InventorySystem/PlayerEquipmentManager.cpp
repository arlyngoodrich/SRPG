// Fill out your copyright notice in the Description page of Project Settings.


#include "InventorySystem/PlayerEquipmentManager.h"
#include "InventorySystem/InventoryContainer.h"
#include "LogFiles.h"


#include "Net/UnrealNetwork.h"

// Sets default values for this component's properties
UPlayerEquipmentManager::UPlayerEquipmentManager()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	



}


// Called when the game starts
void UPlayerEquipmentManager::BeginPlay()
{
	Super::BeginPlay();
	

	
}

void UPlayerEquipmentManager::GetLifetimeReplicatedProps(TArray<FLifetimeProperty >& OutLifetimeProps) const
{

	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UPlayerEquipmentManager, EquipmentSlots);

}

TArray<FEquipmentSlot> UPlayerEquipmentManager::GetEquipmentSlotInfo() { return EquipmentSlots; }


void UPlayerEquipmentManager::SetOwnerInventories()
{
	AActor* Owner = GetOwner();
	Owner->GetComponents<UInventoryContainer>(OwnerInventories);

}


void UPlayerEquipmentManager::BP_EquipItem(EEquipmentSlots TargetSlot, FItemData Item, class UInventoryContainer* LosingInventory, int32 PosX, int32 PosY)
{

	if (LosingInventory)
	{
		Server_EquipItem(TargetSlot, Item, LosingInventory, PosX, PosY);
	}
	else
	{
		UE_LOG(LogInventorySystem,Error,TEXT("Attempted to equip from invalid inventory"))
		return;
	}
}


void UPlayerEquipmentManager::BP_UnEquipItem(EEquipmentSlots TargetSlot)
{
	Server_UnEquipItem(TargetSlot);
}

bool UPlayerEquipmentManager::Server_EquipItem_Validate(EEquipmentSlots TargetSlot, FItemData Item, class UInventoryContainer* LosingInventory, int32 PosX, int32 PosY)
{
	return true;
}

void UPlayerEquipmentManager::Server_EquipItem_Implementation(EEquipmentSlots TargetSlot, FItemData Item, class UInventoryContainer* LosingInventory, int32 PosX, int32 PosY)
{
	int32 SlotIndex;
	if (FindSlot(TargetSlot, SlotIndex))
	{
		FEquipmentSlot TargetEquipmentSlot = EquipmentSlots[SlotIndex];

		if (!TargetEquipmentSlot.bIsOccupied && TargetEquipmentSlot.Slot == TargetSlot)
		{
			EquipmentSlots[SlotIndex].bIsOccupied = true;
			EquipmentSlots[SlotIndex].OccupyingItemData = Item;

			LosingInventory->BP_RemoveItem(Item, PosX, PosY);
			OnRep_EquipmentSlots();
			UE_LOG(LogInventorySystem, Log, TEXT("Item equipped"))
			return;
		}
		else
		{

			UE_LOG(LogInventorySystem, Log, TEXT("Could not item, equipment slot is already full"))
				return;
		}

	}

}


bool UPlayerEquipmentManager::Server_UnEquipItem_Validate(EEquipmentSlots TargetSlot)
{
	return true;
}

void UPlayerEquipmentManager::Server_UnEquipItem_Implementation(EEquipmentSlots TargetSlot)
{
	int32 SlotIndex;
	if (FindSlot(TargetSlot, SlotIndex))
	{
		FEquipmentSlot EquipmentSlot = EquipmentSlots[SlotIndex];
		if (EquipmentSlot.bIsOccupied)
		{
			if (ReturnItemToInventory(EquipmentSlot.OccupyingItemData))
			{
				FItemData NullData;
				EquipmentSlots[SlotIndex].bIsOccupied = false;
				EquipmentSlots[SlotIndex].OccupyingItemData = NullData;
				UE_LOG(LogInventorySystem, Log, TEXT("Item sucessfully unequiped by transfring back to inventory"))
					OnRep_EquipmentSlots();
					return;
			}

			UE_LOG(LogInventorySystem, Log, TEXT("Item was not unequiped, please drop from BP"))
				return;
						
			if (DropItem(EquipmentSlot.OccupyingItemData))
			{
				FItemData NullData;
				EquipmentSlots[SlotIndex].bIsOccupied = false;
				EquipmentSlots[SlotIndex].OccupyingItemData = NullData;
				OnRep_EquipmentSlots();
				UE_LOG(LogInventorySystem, Log, TEXT("Item sucessfully by dropping"))
			}
			UE_LOG(LogInventorySystem, Log, TEXT("Item was not unequipped by dropping"))
		}
	}
}

bool UPlayerEquipmentManager::ReturnItemToInventory(FItemData Item)
{
	SetOwnerInventories();
	
	for (int32 Index = 0; Index != OwnerInventories.Num(); Index++)
	{
		UInventoryContainer* TargetInventory;
		TargetInventory = OwnerInventories[Index];
		
		if (TargetInventory)
		{
			if (TargetInventory->BP_CheckIfItemCouldBeAdded(Item))
			{
				FItemData OutData;
				bool OutBool;
				TargetInventory->BP_AutoAddItem(Item, OutBool, OutData);
				return true;
			}

		}
	}
	return false;
}


bool UPlayerEquipmentManager::DropItem_Implementation(FItemData Item)
{
	return true;
}


bool UPlayerEquipmentManager::FindSlot(EEquipmentSlots TargetSlot, int32& Index)
{


	for (int32 LoopIndex = 0; LoopIndex != EquipmentSlots.Num(); LoopIndex++)
	{
		if (EquipmentSlots[LoopIndex].Slot == TargetSlot)
		{
			Index = LoopIndex;
			return true;
		}
	}
	return false;
}


