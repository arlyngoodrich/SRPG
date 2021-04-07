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
	ValidateSlots();
}

void UPlayerEquipmentManager::GetLifetimeReplicatedProps(TArray<FLifetimeProperty >& OutLifetimeProps) const
{

	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UPlayerEquipmentManager, EquipmentSlots);

}

TArray<FEquipmentSlot> UPlayerEquipmentManager::GetEquipmentSlotInfo() { return EquipmentSlots; }

void UPlayerEquipmentManager::GetSlotItemData(EEquipmentSlots TargetSlot, bool& OutIsOccuiped, FItemData& OutItemData)
{
	int32 SlotIndex;
	if (FindSlot(TargetSlot, SlotIndex))
	{
		OutItemData = EquipmentSlots[SlotIndex].OccupyingItemData;
		OutIsOccuiped = EquipmentSlots[SlotIndex].bIsOccupied;
	}
	else
	{
		UE_LOG(LogInventorySystem, Warning, TEXT("Attempting to obtain equipment slot information from slot that does not exisit"))
	}
}


void UPlayerEquipmentManager::SetOwnerInventories()
{
	AActor* Owner = nullptr;
	Owner = GetOwner();
	OwnerInventories.Empty();
	if (Owner)
	{
		Owner->GetComponents<UInventoryContainer>(OwnerInventories);
	}

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

void UPlayerEquipmentManager::BP_AutoEquipItem(FItemData Item, UInventoryContainer* LosingInventory, int32 PosX, int32 PosY)
{
	AutoEquipItem(Item, LosingInventory, PosX, PosY);
}


void UPlayerEquipmentManager::BP_UnEquipItem(EEquipmentSlots TargetSlot)
{
	Server_UnEquipItem(TargetSlot);
}

void UPlayerEquipmentManager::BP_UnequipItemToPosition(EEquipmentSlots TargetSlot, UInventoryContainer* TargetInventory, int32 PosX, int32 PosY)
{
	Server_UnequipItemToPosition(TargetSlot, TargetInventory, PosX, PosY);
}

void UPlayerEquipmentManager::BP_MoveSlot(EEquipmentSlots CurrentSlot, EEquipmentSlots NewSlot, FItemData ItemData)
{
	Server_MoveSlots(CurrentSlot, NewSlot, ItemData);
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
			OnEquipItem(TargetSlot, Item);
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
				OnUnequipItem(TargetSlot);
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
				OnUnequipItem(TargetSlot);
				UE_LOG(LogInventorySystem, Log, TEXT("Item sucessfully by dropping"))
				return;
			}
			UE_LOG(LogInventorySystem, Log, TEXT("Item was not unequipped by dropping"))
		}
	}
}

bool UPlayerEquipmentManager::Server_UnequipItemToPosition_Validate(EEquipmentSlots TargetSlot, UInventoryContainer* TargetInventory, int32 PosX, int32 PosY)
{
	return true;
}

void UPlayerEquipmentManager::Server_UnequipItemToPosition_Implementation(EEquipmentSlots TargetSlot, UInventoryContainer* TargetInventory, int32 PosX, int32 PosY)
{
	if (TargetInventory == nullptr) { UE_LOG(LogInventorySystem, Warning, TEXT("Attempting to unequip to null inventory")) return; }

	int32 SlotIndex;
	FindSlot(TargetSlot, SlotIndex);
	if (!EquipmentSlots[SlotIndex].bIsOccupied) { UE_LOG(LogInventorySystem, Warning, TEXT("Attempting to unequip item from empty slot")) return; }

	FItemData Item;
	Item = EquipmentSlots[SlotIndex].OccupyingItemData;

	if (TargetInventory->BP_CheckIfItemFitsInPosition(Item, PosX, PosY))
	{
		FItemData NullData;
		
		TargetInventory->BP_AddItem(Item, PosX, PosY);
		EquipmentSlots[SlotIndex].bIsOccupied = false;
		EquipmentSlots[SlotIndex].OccupyingItemData = NullData;
		OnUnequipItem(TargetSlot);
		OnRep_EquipmentSlots();

		return;
	}
	else
	{
		UE_LOG(LogInventorySystem, Log, TEXT("Could not unequip item to position"))
	}

}

bool UPlayerEquipmentManager::Server_MoveSlots_Validate(EEquipmentSlots CurrentSlot, EEquipmentSlots NewSlow, FItemData ItemData)
{
	return true;
}

void UPlayerEquipmentManager::Server_MoveSlots_Implementation(EEquipmentSlots CurrentSlot, EEquipmentSlots NewSlot, FItemData ItemData)
{
	int32 CurrentSlotIndex;
	int32 NewSlotIndex;

	if (GetOwnerRole() < ROLE_Authority) { UE_LOG(LogInventorySystem, Warning, TEXT("Attempted to move equipment from client, request ignored")) return; }
	
	//Make sure valid Indexies
	if (FindSlot(CurrentSlot, CurrentSlotIndex) && FindSlot(NewSlot, NewSlotIndex))
	{
		if (EquipmentSlots[CurrentSlotIndex].bIsOccupied)
		{

			if (EquipmentSlots[NewSlotIndex].bIsOccupied)
			{
				//unequipe current item 
				Server_UnEquipItem(NewSlot);
			}

			//equip new item
			FItemData NullItemData;
			EquipmentSlots[CurrentSlotIndex].bIsOccupied = false;
			EquipmentSlots[CurrentSlotIndex].OccupyingItemData = NullItemData;
			OnUnequipItem(CurrentSlot);

			EquipmentSlots[NewSlotIndex].bIsOccupied = true;
			EquipmentSlots[NewSlotIndex].OccupyingItemData = ItemData;
			OnEquipItem(NewSlot, ItemData);
			OnRep_EquipmentSlots();

			UE_LOG(LogInventorySystem, Log, TEXT("Item successfully moved"))
				return;

		}
		else
		{

			UE_LOG(LogInventorySystem, Warning, TEXT("Attempting to move equipment from empty slot"))
			return;
		}
	}

}

void UPlayerEquipmentManager::AutoEquipItem(FItemData Item, UInventoryContainer* LosingInventory, int32 PosX, int32 PosY)
{
	if (!LosingInventory) {	UE_LOG(LogInventorySystem, Warning, TEXT("Attempting to equipment from null inventory")) return;}


	//Find empty eligible slots
	TArray<EEquipmentSlots> EligibleSlots;
	EligibleSlots = Item.EligibleSlots;

	TArray<int32> EligibleSlotIndexies;
	TArray<int32> NonOccupiedEligibleSlotIndexies;

	for (int32 Index = 0; Index != EligibleSlots.Num(); ++Index)
	{
		int32 TargetIndex;
		if (FindSlot(EligibleSlots[Index], TargetIndex))
		{
			//Add eligible slot
			EligibleSlotIndexies.Add(TargetIndex);

			if (!EquipmentSlots[TargetIndex].bIsOccupied)
			{
				//Add eligible empty slot
				NonOccupiedEligibleSlotIndexies.Add(TargetIndex);
			}
		}
	}

	//See if any empty, if 1 or more, pick first to equip
	if (NonOccupiedEligibleSlotIndexies.Num() >= 1)
	{
		int32 NonOccupiedEligibleTargetSlotIndex;
		NonOccupiedEligibleTargetSlotIndex = NonOccupiedEligibleSlotIndexies[0];
		EEquipmentSlots TargetSlot;

		TargetSlot = EquipmentSlots[NonOccupiedEligibleTargetSlotIndex].Slot;

		Server_EquipItem(TargetSlot, Item, LosingInventory, PosX, PosY);
		return;
	}
	else
	{
		if (EligibleSlotIndexies.Num() > 0)
		{
			int32 EligibleTargetSlotIndex;
			EligibleTargetSlotIndex = EligibleSlotIndexies[0];
			EEquipmentSlots TargetSlot;

			TargetSlot = EquipmentSlots[EligibleTargetSlotIndex].Slot;

			Server_EquipItem(TargetSlot, Item, LosingInventory, PosX, PosY);
			return;

		}
	}

	UE_LOG(LogInventorySystem, Log, TEXT("Auto Equip operation failed"))
}


void UPlayerEquipmentManager::ValidateSlots()
{
	for (int32 Index = 0; Index != EquipmentSlots.Num(); ++Index)
	{
		int32 Count = 0;
		EEquipmentSlots ActiveSlot;

		ActiveSlot = EquipmentSlots[Index].Slot;

		for (int32 Index_Count = 0; Index_Count != EquipmentSlots.Num(); ++Index_Count)
		{
			//Check to see if there are more than one of the same slot types
			if (ActiveSlot == EquipmentSlots[Index_Count].Slot) { Count++; }
		}

		if (Count > 1)
		{
			//If there are more than one, then log an error
			UE_LOG(LogInventorySystem,Error,TEXT("Duplicate Slot types in player Equipment Manager.  Equipment system will not work correctly"))
		}
	}
}

bool UPlayerEquipmentManager::ReturnItemToInventory(FItemData Item)
{
	SetOwnerInventories();
	
	for (int32 Index = 0; Index != OwnerInventories.Num(); Index++)
	{
		UInventoryContainer* TargetInventory = nullptr;
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


