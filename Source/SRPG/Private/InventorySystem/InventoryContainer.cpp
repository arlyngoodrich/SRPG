// Fill out your copyright notice in the Description page of Project Settings.


#include "InventorySystem/InventoryContainer.h"
#include "LogFiles.h"

//UE4 Includes
#include "Net/UnrealNetwork.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values for this component's properties
UInventoryContainer::UInventoryContainer()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	InventorySizeX = 1;
	InventorySizeY = 1;
	MaxWeight = 100;
	CurrentWeight = 0;
}


void UInventoryContainer::GetLifetimeReplicatedProps(TArray<FLifetimeProperty >& OutLifetimeProps) const
{

	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UInventoryContainer, InventorySize);
	DOREPLIFETIME(UInventoryContainer, Inventory);
	DOREPLIFETIME(UInventoryContainer, bIsSlotOccupied);
	DOREPLIFETIME(UInventoryContainer, InventorySlots);
	DOREPLIFETIME(UInventoryContainer, MaxWeight);
	DOREPLIFETIME(UInventoryContainer, CurrentWeight);
	DOREPLIFETIME(UInventoryContainer, InventoryName);
	DOREPLIFETIME(UInventoryContainer, EquippedSlot);

}


bool  UInventoryContainer::BP_CheckIfItemFitsInPosition(FItemData Item, int32 PosX, int32 PosY)
{
	return CheckIfItemFitsInPosition(Item, FVector2D(PosX, PosY));
}

bool UInventoryContainer::BP_CheckIfItemCouldBeAdded(FItemData Item)
{
	return CheckIfItemCouldBeAdded(Item);
}


//BP ADD ITEM//

void UInventoryContainer::BP_AddItem(FItemData Item, int32 PosX, int32 PosY)
{
	if (GetOwnerRole() >= ROLE_Authority)
	{
		AddItem(Item, FVector2D(PosX, PosY), false);
	}
	else
	{
		Server_AddItem(Item, PosX, PosY);
	}
}

bool UInventoryContainer::Server_AddItem_Validate(FItemData Item, int32 PosX, int32 PosY)
{
	return true;
}

void UInventoryContainer::Server_AddItem_Implementation(FItemData Item, int32 PosX, int32 PosY)
{
	BP_AddItem(Item, PosX, PosY);
}



// BP Remove Item //

void UInventoryContainer::BP_RemoveItem(FItemData Item, int32 PosX, int32 PosY)
{
	if (GetOwnerRole() >= ROLE_Authority)
	{
		RemoveItem(Item, FVector2D(PosX, PosY));
	}
	else
	{
		Server_RemoveItem(Item, PosX, PosY);
	}
}

bool UInventoryContainer::Server_RemoveItem_Validate(FItemData Item, int32 PosX, int32 PosY)
{
	return true;
}

void UInventoryContainer::Server_RemoveItem_Implementation(FItemData Item, int32 PosX, int32 PosY)
{
	BP_RemoveItem(Item, PosX, PosY);
}


// BP Move Item  // 

void UInventoryContainer::BP_MoveItem(FItemData Item, int32 StartPosX, int32 StartPosY, int32 EndPosX, int32 EndPosY, bool bIsRotated)
{
	if (GetOwnerRole() >= ROLE_Authority)
	{
		MoveItem(Item, FVector2D(StartPosX, StartPosY), FVector2D(EndPosX, EndPosY), bIsRotated);
	}
	else
	{
		Server_MoveItem(Item, StartPosX, StartPosY, EndPosX, EndPosY, bIsRotated);
	}
}


bool UInventoryContainer::Server_MoveItem_Validate(FItemData Item, int32 StartPosX, int32 StartPosY, int32 EndPosX, int32 EndPosY, bool bIsRotated)
{
	return true;
}

void UInventoryContainer::Server_MoveItem_Implementation(FItemData Item, int32 StartPosX, int32 StartPosY, int32 EndPosX, int32 EndPosY, bool bIsRotated)
{
	BP_MoveItem(Item, StartPosX, StartPosY, EndPosX, EndPosY, bIsRotated);
}


// BP Direct Transfer // 

void UInventoryContainer::BP_DirectTransfer(FItemData Item, int32 StartXPos, int32 StartYPos, UInventoryContainer* RecievingInventory, int32 EndPosX, int32 EndPosY, bool bIsRotated)
{
	if (GetOwnerRole() >= ROLE_Authority)
	{
		DirectTransfer(Item, FVector2D(StartXPos, StartYPos), RecievingInventory, FVector2D(EndPosX, EndPosY), bIsRotated);
	}
	else
	{
		Server_DirectTransfer(Item, StartXPos, StartYPos, RecievingInventory, EndPosX, EndPosY, bIsRotated);
	}
}

void UInventoryContainer::BP_AutoTransfer(FItemData Item, int32 StartXPos, int32 StartYPox, UInventoryContainer* RecievingInventory)
{
	if (GetOwnerRole() >= ROLE_Authority)
	{
		AutoTransfer(Item, FVector2D(StartXPos, StartYPox), RecievingInventory);
	}
	else
	{
		Server_AutoTransfer(Item, StartXPos, StartYPox, RecievingInventory);
	}
}

void UInventoryContainer::BP_TransferAll(UInventoryContainer* RecievingInventory)
{
	Server_TransferAll(RecievingInventory);
}


bool UInventoryContainer::Server_DirectTransfer_Validate(FItemData Item, int32 StartXPos, int32 StartYPos, UInventoryContainer* RecievingInventory, int32 EndPosX, int32 EndPosY, bool bIsRotated)
{
	return true;
}

void UInventoryContainer::Server_DirectTransfer_Implementation(FItemData Item, int32 StartXPos, int32 StartYPos, UInventoryContainer* RecievingInventory, int32 EndPosX, int32 EndPosY, bool bIsRotated)
{
	BP_DirectTransfer(Item, StartXPos, StartYPos, RecievingInventory, EndPosX, EndPosY, bIsRotated);
}

bool UInventoryContainer::Server_AutoTransfer_Validate(FItemData Item, int32 StartXPos, int32 StartYPox, UInventoryContainer* RecievingInventory)
{
	return true;
}

void UInventoryContainer::Server_AutoTransfer_Implementation(FItemData Item, int32 StartXPos, int32 StartYPox, UInventoryContainer* RecievingInventory)
{
	BP_AutoTransfer(Item, StartXPos, StartYPox, RecievingInventory);
}

bool UInventoryContainer::Server_TransferAll_Validate(UInventoryContainer* RecievingInventory)
{
	return true;
}

void UInventoryContainer::Server_TransferAll_Implementation(UInventoryContainer* RecievingInventory)
{
	if (RecievingInventory)
	{

		TArray<bool> InventoryIndexiesToRemove;
		TArray<FItemData> LeftOverItemDataArray;

	for (int32 Index = 0; Index != Inventory.Num(); Index++)
		{
			FItemData LeftOverItemData;
			InventoryIndexiesToRemove.Add(RecievingInventory->AutoAddItem(Inventory[Index].ItemData, true, LeftOverItemData));
			LeftOverItemDataArray.Add(LeftOverItemData);
		}

		
	for (int32 Index = Inventory.Num() - 1; Index > -1; Index--)
		{
			if (InventoryIndexiesToRemove[Index])
			{//was fully removed

				RemoveItem(Inventory[Index].ItemData, Inventory[Index].Position);
			}
			else
			{//Was not fully removed

				Inventory[Index].ItemData = LeftOverItemDataArray[Index];
		
			}
		}

	Internal_OnInventoryUpdate();

	}


}

// BP AutoAdd Item //

void UInventoryContainer::BP_AutoAddItem(FItemData Item, bool& bOutWasFullyAdded, FItemData& OutRemainingItem)
{
	if (GetOwnerRole() >= ROLE_Authority)
	{
		bOutWasFullyAdded = AutoAddItem(Item, true, OutRemainingItem);
	}
}

// BP Same Inventory Direct Stack

void UInventoryContainer::BP_SameInventoryDirectStack(FItemData IncomingItem, int32 IncomingPosX, int32 IncomingPosY, FItemData ReceivingItem, int32 RecPosX, int32 RecPosY)
{
	if (GetOwnerRole() >= ROLE_Authority)
	{
		FItemData OutRemainingItem;
		SameInventoryStack(IncomingItem, FVector2D(IncomingPosX, IncomingPosY), ReceivingItem, FVector2D(RecPosX, RecPosY), OutRemainingItem);
	}
	else
	{
		Server_SameInventoryDirectStack(IncomingItem, IncomingPosX, IncomingPosY, ReceivingItem, RecPosX, RecPosY);
	}
}


bool UInventoryContainer::Server_SameInventoryDirectStack_Validate(FItemData IncomingItem, int32 IncomingPosX, int32 IncomingPosY, FItemData ReceivingItem, int32 RecPosX, int32 RecPosY)
{
	return true;
}

void UInventoryContainer::Server_SameInventoryDirectStack_Implementation(FItemData IncomingItem, int32 IncomingPosX, int32 IncomingPosY, FItemData ReceivingItem, int32 RecPosX, int32 RecPosY)
{
	BP_SameInventoryDirectStack(IncomingItem, IncomingPosX, IncomingPosY, ReceivingItem, RecPosX, RecPosY);
}


// BP DifferentInventoryStack

void UInventoryContainer::BP_DifferentInventoryStack(FItemData IncomingItem, int32 IncomingItemPosX, int32 IncomingItemPosY, UInventoryContainer* RecievingInventory, FItemData ReceivingItem, int32 TargetPosX, int32 TargetPosY)
{
	if (GetOwnerRole() >= ROLE_Authority)
	{
		FItemData OutLeftOverItemData;
		DifferentInventoryStack(IncomingItem, FVector2D(IncomingItemPosX, IncomingItemPosY), RecievingInventory, ReceivingItem, FVector2D(TargetPosX, TargetPosY), OutLeftOverItemData);
	}
	else
	{
		Server_DifferentInventoryStack(IncomingItem, IncomingItemPosX, IncomingItemPosY, RecievingInventory, ReceivingItem, TargetPosX, TargetPosY);
	}
}



bool UInventoryContainer::Server_DifferentInventoryStack_Validate(FItemData IncomingItem, int32 IncomingItemPosX, int32 IncomingItemPosY, UInventoryContainer* RecievingInventory, FItemData ReceivingItem, int32 TargetPosX, int32 TargetPosY)
{
	return true;
}

void UInventoryContainer::Server_DifferentInventoryStack_Implementation(FItemData IncomingItem, int32 IncomingItemPosX, int32 IncomingItemPosY, UInventoryContainer* RecievingInventory, FItemData ReceivingItem, int32 TargetPosX, int32 TargetPosY)
{
	BP_DifferentInventoryStack(IncomingItem, IncomingItemPosX, IncomingItemPosY, RecievingInventory, ReceivingItem, TargetPosX, TargetPosY);
}


// BP Split Stack

void UInventoryContainer::BP_SplitStack(FItemData OriginalItem, int32 PositionX, int32 PositionY, int32 NewStackAmount)
{

	if (GetOwnerRole() >= ROLE_Authority)
	{
		SplitStack(OriginalItem, FVector2D(PositionX, PositionY), NewStackAmount);
	}
	else
	{
		Server_SplitStack(OriginalItem, PositionX, PositionY, NewStackAmount);
	}
}

void UInventoryContainer::SetInventoryFromAbstract(TArray<FInventoryData> NewInventory)
{
	Inventory = NewInventory;
	RefreshSlotOccupancy();
	Client_InventoryUpdate();
	Internal_OnInventoryUpdate();

	UE_LOG(LogInventorySystem,Log,TEXT("Inventory overwritten from abstract"))
}

TArray<FInventoryData> UInventoryContainer::GetInventoryForAbstract()
{
	return Inventory;
}

bool UInventoryContainer::Server_SplitStack_Validate(FItemData OriginalItem, int32 PositionX, int32 PositionY, int32 NewStackAmount)
{
	return true;
}

void UInventoryContainer::Server_SplitStack_Implementation(FItemData OriginalItem, int32 PositionX, int32 PositionY, int32 NewStackAmount)
{
	BP_SplitStack(OriginalItem, PositionX, PositionY, NewStackAmount);
}


void UInventoryContainer::GetInventorySize(int32& OutSizeX, int32& OutSizeY)
{
	OutSizeX = InventorySizeX;
	OutSizeY = InventorySizeY;

}

void UInventoryContainer::GetSlotInformation(TArray<int32>& OutXPositions, TArray<int32>& OutYPositions, TArray<bool>& OutbIsOccupied)
{
	TArray<int32> XPositions;
	TArray<int32> YPositions;

	for (int32 Index = 0; Index != InventorySlots.Num(); Index++)
	{
		XPositions.Add(InventorySlots[Index].X);
		YPositions.Add(InventorySlots[Index].Y);
	}

	OutXPositions = XPositions;
	OutYPositions = YPositions;
	OutbIsOccupied = bIsSlotOccupied;

}

void UInventoryContainer::GetInventoryData(TArray<FItemData>& OutItems, TArray<int32>& OutXPositions, TArray<int32>& OutYPositions)
{
	TArray<int32> XPositions;
	TArray<int32> YPositions;
	TArray<FItemData> Items;

	for (int32 Index = 0; Index != Inventory.Num(); Index++)
	{
		XPositions.Add(Inventory[Index].Position.X);
		YPositions.Add(Inventory[Index].Position.Y);
		Items.Add(Inventory[Index].ItemData);
	}


	OutItems = Items;
	OutXPositions = XPositions;
	OutYPositions = YPositions;

}

FItemData UInventoryContainer::GetItemAtPosition(FVector2D Position)
{
	for (int32 Index = 0; Index != Inventory.Num(); Index++)
	{
		if (Inventory[Index].Position == Position)
		{
			return Inventory[Index].ItemData;
		}
	}
return FItemData();
}

void UInventoryContainer::RemoveQuantityOfItem(FItemData Item, int32 RequestedQuantity, int32& OutQuantityRemoved)
{
	if (GetOwnerRole() < ROLE_Authority) { UE_LOG(LogInventorySystem, Warning, TEXT("Canno run 'Remove QTY Of Items Function' on client")) return; }

	int32 FoundQuantity;
	FindTotalQuantityOfItem(Item, FoundQuantity);
	if (FoundQuantity == 0)
	{
		OutQuantityRemoved = 0;
		UE_LOG(LogInventorySystem, Log, TEXT("Could not find any %s to remove"), *Item.DisplayName.ToString())
		return;
	}

	TArray<int32> ItemIndexices;
	FindAllItemIndexices(Item, ItemIndexices);
	TArray<FInventoryData> InventoryDataToRemove;

	int32 QuantityNeeded = RequestedQuantity;

	for (int32 i = 0; i < ItemIndexices.Num(); i++)
	{
		int32 ActiveItemIndex = ItemIndexices[i];
		FInventoryData ActiveInventoryData = Inventory[ActiveItemIndex];

		int32 ItemAmount;
		ItemAmount = ActiveInventoryData.ItemData.StackQuantity;
		
		//If not enough
		if (ItemAmount <= QuantityNeeded)
		{
			QuantityNeeded -= ItemAmount;
			InventoryDataToRemove.Add(ActiveInventoryData);
			if (QuantityNeeded == 0) { break; }
		}
		//If more than needed
		if (ItemAmount > QuantityNeeded)
		{
			Inventory[ActiveItemIndex].ItemData.StackQuantity = ItemAmount - QuantityNeeded;
			QuantityNeeded = 0;
			break;
		}
	}

	OutQuantityRemoved = RequestedQuantity - QuantityNeeded;

	//Remove Data
	for (int32 Index = InventoryDataToRemove.Num() - 1; Index > -1; Index--)
	{
		RemoveItem(InventoryDataToRemove[Index].ItemData, InventoryDataToRemove[Index].Position);
	}
}

// Called when the game starts
void UInventoryContainer::BeginPlay()
{
	Super::BeginPlay();
	InitalizeSlots();

}



void UInventoryContainer::InitalizeSlots()
{

	if (GetOwnerRole() >= ROLE_Authority)
	{
		InventorySize = FVector2D(InventorySizeX, InventorySizeY);

		for (int32 IndexX = 0; IndexX != InventorySize.X; IndexX++)
		{
			for (int32 IndexY = 0; IndexY != InventorySize.Y; IndexY++)
			{
				InventorySlots.Add(FVector2D(IndexX, IndexY));
				bIsSlotOccupied.Add(false);
				UE_LOG(LogInventorySystem, Verbose, TEXT("Slot added at position(%s,%s)"), *FString::FromInt(IndexX), *FString::FromInt(IndexY))

			}
		}
	}

}

bool UInventoryContainer::CheckIfItemFitsInPosition(FItemData Item, FVector2D Position)
{

	int32 InitalPositionIndex;

	if(!CheckIfWeightOK(Item))
	{
		return false;
	}


	if (!InventorySlots.Find(Position, InitalPositionIndex))
	{
		// If position not found in inventory, then return false
		return false;
	}
	else
	{
		// If inital position is occupied, then return false
		if (bIsSlotOccupied[InitalPositionIndex])
		{
			return false;
		}

	}

	TArray<bool> bIsSlotValid;

	//Check if slots are occupied
	for (int32 IndexX = 0; IndexX != Item.SizeX; IndexX++)
	{
		for (int32 IndexY = 0; IndexY != Item.SizeY; IndexY++)
		{
			int32 PositionIndex;
			if (InventorySlots.Find(FVector2D(IndexX + Position.X, IndexY + Position.Y), PositionIndex))
			{
				//Add true if slot is NOT occupied
				bIsSlotValid.Add(!bIsSlotOccupied[PositionIndex]);
			}
			else
			{
				// If slot not found, return false for status
				bIsSlotValid.Add(false);
			}
		}
	}

	if (bIsSlotValid.Contains(false))
	{
		return false;
	}
	else
	{
		return true;
	}

}

bool UInventoryContainer::CheckIfItemCouldBeAdded(FItemData Item)
{
	
	if (CheckIfWeightOK(Item))
	{


		for (int32 Index = 0; Index != InventorySlots.Num(); Index++)
		{
			if (CheckIfItemFitsInPosition(Item, InventorySlots[Index]))
			{
				//Return true for first slot that it can fit
				return true;
			}
		}
		//If not slots will fit, then return false
	}
	return false;
}

bool UInventoryContainer::AddItem(FItemData Item, FVector2D Position, bool bCheckWeight)
{
	FItemData ItemData;
	ItemData = GetServerVersionOfItem(Item, this, Position);


	if (CheckIfWeightOK(ItemData) || !bCheckWeight)
	{
		if (CheckIfItemFitsInPosition(ItemData, Position))
		{
			//Set slots to occupied
			SetSlotsAsOccupied(ItemData.SizeX, ItemData.SizeY, Position, true);

			FInventoryData InventoryData;
			InventoryData.ItemData = ItemData;
			InventoryData.Position = Position;

			Inventory.Add(InventoryData);


			UE_LOG(LogInventorySystem, Log, TEXT("%s added to positon (%s,%s)"), *ItemData.DisplayName.ToString(), *FString::SanitizeFloat(Position.X), *FString::SanitizeFloat(Position.Y))
			Internal_OnInventoryUpdate();
			return true;
		}
		else
		{
			UE_LOG(LogInventorySystem, Log, TEXT("%s could not fit"), *ItemData.DisplayName.ToString())
				return false;
		}
	}
	else
	{
		UE_LOG(LogInventorySystem, Log, TEXT("%s weighed too much"), *Item.DisplayName.ToString())
			return false;
	}

}

bool UInventoryContainer::RemoveItem(FItemData Item, FVector2D Position)
{

	for (int32 Index = 0; Index != Inventory.Num(); Index++)
	{
		// If item in inventory
		if (Inventory[Index].Position == Position && Inventory[Index].ItemData.DisplayName == Item.DisplayName)
		{
			//Set slots to empty

			SetSlotsAsOccupied(Item.SizeX, Item.SizeY, Position, false);

			//Remove from inventory
			Inventory.RemoveAt(Index);

			UE_LOG(LogInventorySystem, Log, TEXT("%s removed from (%s,%s)"), *Item.DisplayName.ToString(), *FString::SanitizeFloat(Position.X), *FString::SanitizeFloat(Position.Y))
			Internal_OnInventoryUpdate();
			return true;
		}

	}

	UE_LOG(LogInventorySystem, Log, TEXT("Failed to remove %s"), *Item.DisplayName.ToString())

		return false;
}

bool UInventoryContainer::MoveItem(FItemData Item, FVector2D StartingPosition, FVector2D EndingPosition, bool bPerformRotationOnSet)
{
	FItemData ItemData;
	ItemData = GetServerVersionOfItem(Item, this, StartingPosition);

	//Find Item Index
	int32 ItemIndex;

	if (FindInventoryItemIndex(ItemData, StartingPosition, ItemIndex))
	{
		//Set inital slot as empty so they are not part of the 'If It Fitz' check
		SetSlotsAsOccupied(ItemData.SizeX, ItemData.SizeY, StartingPosition, false);

		if (bPerformRotationOnSet)
		{
			bool bOriginalRotation = ItemData.bIsRotated;
			ItemData.bIsRotated = !bOriginalRotation;

			ItemData.SizeX = Item.SizeY;
			ItemData.SizeY = Item.SizeX;
		}
	

		if (CheckIfItemFitsInPosition(ItemData, EndingPosition))
		{
			//Move item to new position
			Inventory[ItemIndex].Position = EndingPosition;
			Inventory[ItemIndex].ItemData = ItemData;
			SetSlotsAsOccupied(ItemData.SizeX, ItemData.SizeY, EndingPosition, true);
			UE_LOG(LogInventorySystem, Log, TEXT("%s has been moved to (%s,%s)"), *ItemData.DisplayName.ToString(), *FString::SanitizeFloat(EndingPosition.X), *FString::SanitizeFloat(EndingPosition.Y))
			Internal_OnInventoryUpdate();
			return true;

		}
		else
		{
			//Item will not fit in new position
			//Return inital slots to being occupied
			SetSlotsAsOccupied(Item.SizeX, Item.SizeY, StartingPosition, true);
			UE_LOG(LogInventorySystem, Log, TEXT("Could not move %s, item will not fit"), *Item.DisplayName.ToString())
				Internal_OnInventoryUpdate();
			return false;
		}

	}
	else
	{
		UE_LOG(LogInventorySystem, Log, TEXT("Could not move %s, item not found in inventory"), *Item.DisplayName.ToString())
			return false;
	}



}

bool UInventoryContainer::SplitStack(FItemData OriginalItem, FVector2D StartingPosition, int32 NewStackAmount)
{

	if (IsValidItem(OriginalItem, StartingPosition))
	{

		FItemData NewStack;
		NewStack = OriginalItem;
		NewStack.StackQuantity = NewStackAmount;

		int32 OriginalItemIndex;
		FindInventoryItemIndex(OriginalItem, StartingPosition, OriginalItemIndex);
		int32 OriginalStackAmount = Inventory[OriginalItemIndex].ItemData.StackQuantity;



		for (int32 Index = 0; Index != InventorySlots.Num(); Index++)
		{
			if (CheckIfItemFitsInPosition(NewStack, InventorySlots[Index]))
			{

				if (AddItem(NewStack, InventorySlots[Index], false))
				{
					Inventory[OriginalItemIndex].ItemData.StackQuantity = OriginalStackAmount - NewStackAmount;
					Internal_OnInventoryUpdate();
					return true;
				}
			}
		}

		//No space for stack
		return false;

	}
	else
	{
		return false;
	}

}

bool UInventoryContainer::AutoAddItem(FItemData Item, bool bShouldStackItem, FItemData& OutRemainingItem)
{

	bool bWasFullyStacked = false;
	FItemData RemainingItem;

	if (bShouldStackItem == true)
	{
		if (CheckIfWeightOK(Item))
		{
			AutoStackItem(Item, bWasFullyStacked, RemainingItem);
		}
		else
		{
			UE_LOG(LogInventorySystem, Log, TEXT("%s is too heavy, failed to stack"), *Item.DisplayName.ToString())
				OutRemainingItem = Item;
			return false;
		}
	}

	if (bWasFullyStacked)
	{
		UE_LOG(LogInventorySystem, Log, TEXT("%s added into existing stack"), *Item.DisplayName.ToString())
			return true;
	}
	else
	{

		//check all slots to see if it fits
		for (int32 Index = 0; Index != InventorySlots.Num(); Index++)
		{
			if (CheckIfItemFitsInPosition(RemainingItem, InventorySlots[Index]))
			{
				//Add Item to first availabe slot
				if (AddItem(RemainingItem, InventorySlots[Index], true))
				{

					UE_LOG(LogInventorySystem, Log, TEXT("%s added as new item"), *Item.DisplayName.ToString())
					return true;
				}
			}
		}


		//TODO Try adding with rotated orientation



		//Item could not be fully stacked or placed as new item

		UE_LOG(LogInventorySystem, Log, TEXT("%s was not fully added, %d quantity remains"), *Item.DisplayName.ToString(), RemainingItem.StackQuantity)
			OutRemainingItem = RemainingItem;
		return false;
	}

}

void UInventoryContainer::AutoStackItem(FItemData Item, bool& OutItemFullyStacked, FItemData& OutLeftOverItemData)
{
	TArray<int32>EligibleIndexices;
	FItemData ItemToAdd;
	ItemToAdd = Item;

	if (Item.bCanBeStacked && FindAllItemIndexices(Item, EligibleIndexices))
	{
		//Attempt a single stack
		for (int32 Index = 0; Index != EligibleIndexices.Num(); Index++)
		{

			int32 InventoryIndex;
			InventoryIndex = EligibleIndexices[Index];

			int32 MaxStackQuantity;
			int32 CurrentStackAmount;
			int32 QuantityToAdd;

			MaxStackQuantity = Inventory[InventoryIndex].ItemData.MaxStackQuantity;
			CurrentStackAmount = Inventory[InventoryIndex].ItemData.StackQuantity;
			QuantityToAdd = ItemToAdd.StackQuantity;

			if (MaxStackQuantity >= CurrentStackAmount + QuantityToAdd)
			{
				//OK to Stack fully
				Inventory[InventoryIndex].ItemData.StackQuantity = CurrentStackAmount + QuantityToAdd;
				UE_LOG(LogInventorySystem, Log, TEXT("Item successfully stacked"))

				OutItemFullyStacked = true;
				Internal_OnInventoryUpdate();
				return;
			}

		}

		UE_LOG(LogInventorySystem, Log, TEXT("Full stack failed, attempting to overflow stack"))

			//Find Stack with greatest space available
			TArray<int32> ItemQuantities;

		//Get all Item Quantities
		for (int32 Index = 0; Index != EligibleIndexices.Num(); Index++)
		{
			int32 InventoryIndex;
			InventoryIndex = EligibleIndexices[Index];

			ItemQuantities.Add(Inventory[InventoryIndex].ItemData.StackQuantity);

		}

		//Find Min of Quantity Array

		int32 IndexOfMin;
		int32 MinValue;
		UKismetMathLibrary::MinOfIntArray(ItemQuantities, IndexOfMin, MinValue);

		if (ItemToAdd.MaxStackQuantity > MinValue)
		{

			Inventory[EligibleIndexices[IndexOfMin]].ItemData.StackQuantity = ItemToAdd.MaxStackQuantity;

			int32 QuantityAdded;
			int32 RemainingQuantity;

			QuantityAdded = ItemToAdd.MaxStackQuantity - MinValue;
			RemainingQuantity = ItemToAdd.StackQuantity - QuantityAdded;

			ItemToAdd.StackQuantity = RemainingQuantity;

			OutItemFullyStacked = false;
			OutLeftOverItemData = ItemToAdd;

			UE_LOG(LogInventorySystem, Log, TEXT("Partially stacked %s, %d remain"), *ItemToAdd.DisplayName.ToString(), RemainingQuantity)
				Internal_OnInventoryUpdate();
			return;


		}
		else
		{
			//All stacks already at Max Quantity
			OutItemFullyStacked = false;
			OutLeftOverItemData = ItemToAdd;

			UE_LOG(LogInventorySystem, Log, TEXT("Failed to stack %s, all stacks are already max"), *ItemToAdd.DisplayName.ToString())
				return;
		}

	}
	else
	{
		//Item Cannot be stacked OR No stacking option is available
		OutItemFullyStacked = false;
		OutLeftOverItemData = ItemToAdd;
		UE_LOG(LogInventorySystem, Log, TEXT("Failed to stack %s, no option available"), *ItemToAdd.DisplayName.ToString())
			return;
	}

}



bool UInventoryContainer::DirectTransfer(FItemData Item, FVector2D StartingPosition, UInventoryContainer* RecievingInventory, FVector2D EndingPosition, bool bIsRotated)
{

	//Need to save reference to original item in case it is rotated during transit.  
	FItemData OriginalItem;
	OriginalItem = Item;

	FItemData ItemData;
	ItemData = GetServerVersionOfItem(Item, this, StartingPosition);

	if (bIsRotated)
	{
		int32 ItemSizeX;
		int32 ItemSizeY;

		ItemSizeX = ItemData.SizeX;
		ItemSizeY = ItemData.SizeY;

		ItemData.bIsRotated = bIsRotated;
		ItemData.SizeX = ItemSizeY;
		ItemData.SizeY = ItemSizeX;
	}

	//Add items already checks if it can fit so no need to add here

	int32 ItemIndex;
	if (!FindInventoryItemIndex(ItemData, StartingPosition, ItemIndex))
	{
		UE_LOG(LogInventorySystem, Log, TEXT("Transfer of %s failed, could not find in inventory"), *ItemData.DisplayName.ToString())
			return false;
	}

	if (RecievingInventory == nullptr)
	{
		UE_LOG(LogInventorySystem, Log, TEXT("Transfer of %s failed, receiving inventory not valid"), *ItemData.DisplayName.ToString())
			return false;
	}


	if (RecievingInventory->AddItem(ItemData, EndingPosition, true) && RemoveItem(OriginalItem, StartingPosition))
	{
		UE_LOG(LogInventorySystem, Log, TEXT("Transfer of %s succeeded"), *ItemData.DisplayName.ToString())
			return true;
	}
	else
	{
		UE_LOG(LogInventorySystem, Log, TEXT("Transfer of %s failed"), *ItemData.DisplayName.ToString())
			Internal_OnInventoryUpdate();
		RecievingInventory->Internal_OnInventoryUpdate();

		return false;
	}

}

bool UInventoryContainer::AutoTransfer(FItemData Item, FVector2D StartingPosition, UInventoryContainer* ReceivingInventory)
{
	if (IsValidItem(Item, StartingPosition) && ReceivingInventory)
	{
		FItemData ItemData;
		ItemData = GetServerVersionOfItem(Item, this, StartingPosition);

		FItemData LeftOverItem;
		if (ReceivingInventory->AutoAddItem(ItemData, ItemData.bCanBeStacked, LeftOverItem))
		{
			RemoveItem(ItemData, StartingPosition);
			Internal_OnInventoryUpdate();
			UE_LOG(LogInventorySystem, Log, TEXT("Transfer of %s succeeded"), *ItemData.DisplayName.ToString())
			return true;
		}
		else
		{

			//Failed or leftoverItem
			int32 ItemIndex;
			FindInventoryItemIndex(ItemData, StartingPosition, ItemIndex);
			Inventory[ItemIndex].ItemData = LeftOverItem;
			Internal_OnInventoryUpdate();
			return true;

		}
	}
	
	return false;
}

bool UInventoryContainer::SameInventoryStack(FItemData IncomingItem, FVector2D IncomingItemPos, FItemData ReceivingItem, FVector2D ReceivingItemPos, FItemData& OutLefOverItemData)
{
	//If both items valid in same inventory
	if (IsValidItem(IncomingItem, IncomingItemPos) && IsValidItem(ReceivingItem, ReceivingItemPos))
	{

		FItemData RemainingItemData;
		int32 IncomingItemIndex;
		FindInventoryItemIndex(IncomingItem, IncomingItemPos, IncomingItemIndex);

		if (DirectStack(IncomingItem, ReceivingItem, ReceivingItemPos, RemainingItemData))
		{
			//Item fully stacked into receiving item, OK to remove incoming item
			RemoveItem(IncomingItem, IncomingItemPos);
			Internal_OnInventoryUpdate();
			return true;
		}
		else
		{

			//Item not fully stacked, update to remaining item
			Inventory[IncomingItemIndex].ItemData.StackQuantity = RemainingItemData.StackQuantity;
			Internal_OnInventoryUpdate();
			return false;

		}
	}



	return false;
}

bool UInventoryContainer::DifferentInventoryStack(FItemData IncomingItem, FVector2D IncomingItemPos, UInventoryContainer* RecievingInventory, FItemData ReceivingItem, FVector2D TargetPosition, FItemData& OutLefOverItemData)
{

	int32 IncomingItemIndex;
	if (FindInventoryItemIndex(IncomingItem, IncomingItemPos, IncomingItemIndex) == false)
	{
		UE_LOG(LogInventorySystem, Log, TEXT("Cannot execute stack transfer, incoming item not valid"))
			return false;
	}

	if (RecievingInventory == nullptr)
	{
		UE_LOG(LogInventorySystem, Log, TEXT("Cannot execute stack transfer, receiving invetory not valid"))
			return false;
	}

	int32 RecievingItemIndex;
	if (RecievingInventory->FindInventoryItemIndex(ReceivingItem, TargetPosition, RecievingItemIndex) == false)
	{
		UE_LOG(LogInventorySystem, Log, TEXT("Cannot execute stack transfer, receiving item not valid"))
			return false;
	}


	FItemData RemainingItemData;
	if (RecievingInventory->DirectStack(IncomingItem, ReceivingItem, TargetPosition, RemainingItemData))
	{
		//Item fully stacked into receiving item, OK to remove incoming item
		RemoveItem(IncomingItem, IncomingItemPos);
		Internal_OnInventoryUpdate();
		return true;
	}
	else
	{
		//Item not fully stacked, update to remaining item
		Inventory[IncomingItemIndex].ItemData.StackQuantity = RemainingItemData.StackQuantity;
		Internal_OnInventoryUpdate();
		return false;
	}


}

bool UInventoryContainer::DirectStack(FItemData IncomingItem, FItemData ReceivingItem, FVector2D ReceivingItemPos, FItemData& OutLefOverItemData)
{

	OutLefOverItemData = IncomingItem;

	//Make sure receiving item is valid
	if (IsValidItem(ReceivingItem, ReceivingItemPos) && IncomingItem.DisplayName == ReceivingItem.DisplayName)
	{

		//See if can full stack
		int32 IncomingQty;
		int32 CurrentQty;
		int32 MaxQty;

		int32 ItemIndex;
		FindInventoryItemIndex(ReceivingItem, ReceivingItemPos, ItemIndex);

		IncomingQty = IncomingItem.StackQuantity;
		CurrentQty = ReceivingItem.StackQuantity;
		MaxQty = ReceivingItem.MaxStackQuantity;

		if (MaxQty >= IncomingQty + CurrentQty)
		{
			//OK to full stack

			Inventory[ItemIndex].ItemData.StackQuantity = IncomingQty + CurrentQty;
			OutLefOverItemData.StackQuantity = 0;
			Internal_OnInventoryUpdate();
			OnRep_InventoryUpdated();
			return true;

		}
		else
		{
			//must do partial stack or no stack
			Inventory[ItemIndex].ItemData.StackQuantity = MaxQty;

			int32 RemainingQty;
			RemainingQty = (IncomingQty + CurrentQty) - MaxQty;

			OutLefOverItemData.StackQuantity = RemainingQty;
			Internal_OnInventoryUpdate();
			OnRep_InventoryUpdated();
			return false;
		}

	}
	else
	{
		UE_LOG(LogInventorySystem, Log, TEXT("Direct stack failed, Receiving Item not valid"))
			return false;
	}

}



void UInventoryContainer::SetSlotsAsOccupied(int32 SizeX, int32 SizeY, FVector2D Position, bool bIsOccupied)
{

	for (int32 IndexX = 0; IndexX != SizeX; IndexX++)
	{
		for (int32 IndexY = 0; IndexY != SizeY; IndexY++)
		{
			int32 PositionIndex;
			if (InventorySlots.Find(FVector2D(IndexX + Position.X, IndexY + Position.Y), PositionIndex))
			{
				bIsSlotOccupied[PositionIndex] = bIsOccupied;

			}
			else
			{
				UE_LOG(LogInventorySystem, Warning, TEXT("Could not find slot to change if occupied (%s,%s)"), *FString::SanitizeFloat(IndexX + Position.X), *FString::SanitizeFloat(IndexY + Position.Y))
			}
		}
	}


}

bool UInventoryContainer::FindInventoryItemIndex(FItemData Item, FVector2D Position, int32& OutIndex)
{

	for (int32 Index = 0; Index != Inventory.Num(); Index++)
	{
		// If item in inventory
		if (Inventory[Index].Position == Position && Inventory[Index].ItemData.DisplayName == Item.DisplayName)
		{
			OutIndex = Index;
			return true;
		}
	}

	//Item was not found at position
	return false;
}

bool UInventoryContainer::FindAllItemIndexices(FItemData Item, TArray<int32>& OutItemIndexices)
{

	TArray<int32>ItemIndexices;

	for (int32 Index = 0; Index != Inventory.Num(); Index++)
	{
		if (Inventory[Index].ItemData.DisplayName == Item.DisplayName)
		{
			ItemIndexices.Add(Index);
		}
	}

	if (ItemIndexices.Num() > 0)
	{
		OutItemIndexices = ItemIndexices;
		return true;
	}
	else
	{
		return false;
	}

}

bool UInventoryContainer::FindTotalQuantityOfItem(FItemData Item, int32& OutQuantity)
{

	TArray<int32>ValidIndexces;
	int32 Quantity = 0;

	if (FindAllItemIndexices(Item, ValidIndexces))
	{
		for (int32 Index = 0; Index != ValidIndexces.Num(); Index++)
		{
			int32 InventoryIndex;
			InventoryIndex = ValidIndexces[Index];

			Quantity += Inventory[InventoryIndex].ItemData.StackQuantity;
		}

		OutQuantity = Quantity;
		return true;

	}
	else
	{
		return false;
	}

}

bool UInventoryContainer::IsValidItem(FItemData Item, FVector2D Position)
{
	int32 Index;
	return FindInventoryItemIndex(Item, Position, Index);
}

bool UInventoryContainer::CheckIfWeightOK(FItemData Item)
{
	UpdateWeight();

	float IncomingWeight;
	IncomingWeight = Item.StackQuantity * Item.PerItemWeight;

	float NewWeight = CurrentWeight + IncomingWeight;

	if (MaxWeight >= NewWeight)
	{

		UE_LOG(LogInventorySystem, Log, TEXT("Weight OK"))
			return true;
	}
	else
	{
		UE_LOG(LogInventorySystem, Log, TEXT("Item stack too heavy"))
			return false;
	}

}

void UInventoryContainer::UpdateWeight()
{
	CurrentWeight = 0;

	for (int32 Index = 0; Index != Inventory.Num(); Index++)
	{
		float StackWeight;
		StackWeight = Inventory[Index].ItemData.StackQuantity * Inventory[Index].ItemData.PerItemWeight;
		CurrentWeight += StackWeight;
	}

}

void UInventoryContainer::RefreshSlotOccupancy()
{
	for (int32 Index = 0; Index != InventorySlots.Num(); Index++)
	{
		bIsSlotOccupied[Index] = CheckIfSlotIsOccupied(InventorySlots[Index]);
	}

}

bool UInventoryContainer::CheckIfSlotIsOccupied(FVector2D Position)
{
	if (Inventory.Num() == 0)
	{
		return false;
	}
	else
	{
		for (int32 Index = 0; Index != Inventory.Num(); ++Index)
		{
			if(Inventory[Index].Position == Position)
			{
				return true;
			}
		}
	}

	return false;
}

void UInventoryContainer::Client_InventoryUpdate_Implementation()
{
	InventoryUpdated();
	InventoryUpdated_Onupdate.Broadcast();
}

void UInventoryContainer::OnRep_InventoryUpdated()
{
	InventoryUpdated();
	InventoryUpdated_Onupdate.Broadcast();
}

void UInventoryContainer::Internal_OnInventoryUpdate()
{

	if (GetOwnerRole() >= ROLE_Authority)
	{

		UpdateWeight();
		InventoryUpdated();
		InventoryUpdated_Onupdate.Broadcast();
		Client_InventoryUpdate();
	}
}

FItemData UInventoryContainer::GetServerVersionOfItem(FItemData ClientItem, UInventoryContainer* TargetInventory, FVector2D Position)
{

	if (TargetInventory)
	{
		FItemData NewItem;
		NewItem = TargetInventory->GetItemAtPosition(Position);
		if (NewItem.DisplayName == ClientItem.DisplayName)
		{
			return NewItem;
		}
	}
	return ClientItem;

}

