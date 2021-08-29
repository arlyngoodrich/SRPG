// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ItemData.h"
#include "InventoryContainer.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInventoryUpdated);

UENUM(BlueprintType)
enum class EInventoryFilterType : uint8 {

	EIT_None			UMETA(DisplayName = "None"),
	EIT_BlackList		UMETA(DisplayName = "BlackList"),
	EIT_WhiteList		UMETA(DisplayName = "WhiteList"),

};




UCLASS(ClassGroup = (Inventory), blueprintable, meta = (BlueprintSpawnableComponent))
class SRPG_API UInventoryContainer : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UInventoryContainer();

	UFUNCTION(BlueprintCallable, Category = "Inventory", DisplayName = "Check If Item Fits")
	bool BP_CheckIfItemFitsInPosition(FItemData Item, int32 PosX, int32 PosY);

	UFUNCTION(BlueprintCallable, Category = "Inventory", DisplayName = "Check If Item Could be added")
	bool BP_CheckIfItemCouldBeAdded(FItemData Item);

	UFUNCTION(BlueprintCallable, Category = "Inventory", DisplayName = "Check If Valid Item")
	bool BP_CheckIfValidItem(FItemData Item, int32 PosX, int32 PosY);

	UFUNCTION(BlueprintCallable, Category = "Inventory", DisplayName = "Add Item")
	void BP_AddItem(FItemData Item, int32 PosX, int32 PosY);

	UFUNCTION(BlueprintCallable, Category = "Inventory", DisplayName = "Remove Item")
	void BP_RemoveItem(FItemData Item, int32 PosX, int32 PosY);

	UFUNCTION(BlueprintCallable, Category = "Inventory", DisplayName = "Move Item")
	void BP_MoveItem(FItemData Item, int32 StartPosX, int32 StartPosY, int32 EndPosX, int32 EndPosY, bool bIsRotated);

	UFUNCTION(BlueprintCallable, Category = "Inventory", DisplayName = "Direct Transfer Item")
	void BP_DirectTransfer(FItemData Item, int32 StartXPos, int32 StartYPos, UInventoryContainer* RecievingInventory, int32 EndPosX, int32 EndPosY, bool bIsRotated);

	UFUNCTION(BlueprintCallable, Category = "Inventory", DisplayName = "Auto Transfer Item")
	void BP_AutoTransfer(FItemData Item, int32 StartXPos, int32 StartYPox, UInventoryContainer* RecievingInventory);

	UFUNCTION(BlueprintCallable, Category = "Inventory", DisplayName = "Transfer All Items")
	void BP_TransferAll(UInventoryContainer* RecievingInventory);

	//Must be run on server to receive callback of reamining items
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Inventory", DisplayName = "Server Only Auto Add Item")
	void BP_AutoAddItem(FItemData Item, bool& OutbHasLeftOver, FItemData& OutRemainingItem);

	//Returns true if item fully added, false if not added or partially added
	UFUNCTION(BlueprintCallable, Category = "Inventory", DisplayName = "Same Inventory Direct Stack Item")
	void BP_SameInventoryDirectStack(FItemData IncomingItem, int32 IncomingPosX, int32 IncomingPosY, FItemData ReceivingItem, int32 RecPosX, int32 RecPosY);

	UFUNCTION(BlueprintCallable, Category = "Inventory", DisplayName = "Different Inventory Direct Stack Item")
	void BP_DifferentInventoryStack(FItemData IncomingItem, int32 IncomingItemPosX, int32 IncomingItemPosY, UInventoryContainer* RecievingInventory, FItemData ReceivingItem, int32 TargetPosX, int32 TargetPosY);

	UFUNCTION(BlueprintCallable, Category = "Inventory", DisplayName = "Split Stack")
	void BP_SplitStack(FItemData OriginalItem, int32 PositionX, int32 PositionY, int32 NewStackAmount);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Inventory", DisplayName = "Remove Quantity of Item")
	void BP_RemoveQtyOfItem(int32 Quantity, FItemData ItemData, int32 PositionX, int32 PositionY);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Inventory")
	void SetInventoryFromAbstract(TArray<FInventoryItemData> NewInventory, int32 NewAbstractInventoryPairID);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Inventory")
	void GetInventoryForAbstract(TArray<FInventoryItemData>& OutInventoryData, int32& OutAbstractInventoryPaidID);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Inventory")
	void RemoveQuantityOfClass(TSubclassOf<class AItemBase> Class, int32 QuantityToRemove, int32& QuantityRemaining);

	UPROPERTY(BlueprintReadOnly, Category = "Inventory")
	int32 Inventory_AbstractInventoryPairID;

protected:
	//Blueprint server functions

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_AddItem(FItemData Item, int32 PosX, int32 PosY);
	bool Server_AddItem_Validate(FItemData Item, int32 PosX, int32 PosY);
	void Server_AddItem_Implementation(FItemData Item, int32 PosX, int32 PosY);

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_RemoveItem(FItemData Item, int32 PosX, int32 PosY);
	bool Server_RemoveItem_Validate(FItemData Item, int32 PosX, int32 PosY);
	void Server_RemoveItem_Implementation(FItemData Item, int32 PosX, int32 PosY);

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_MoveItem(FItemData Item, int32 StartPosX, int32 StartPosY, int32 EndPosX, int32 EndPosY, bool bIsRotated);
	bool Server_MoveItem_Validate(FItemData Item, int32 StartPosX, int32 StartPosY, int32 EndPosX, int32 EndPosY, bool bIsRotated);
	void Server_MoveItem_Implementation(FItemData Item, int32 StartPosX, int32 StartPosY, int32 EndPosX, int32 EndPosY, bool bIsRotated);

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_DirectTransfer(FItemData Item, int32 StartXPos, int32 StartYPos, UInventoryContainer* RecievingInventory, int32 EndPosX, int32 EndPosY, bool bIsRotated);
	bool Server_DirectTransfer_Validate(FItemData Item, int32 StartXPos, int32 StartYPos, UInventoryContainer* RecievingInventory, int32 EndPosX, int32 EndPosY, bool bIsRotated);
	void Server_DirectTransfer_Implementation(FItemData Item, int32 StartXPos, int32 StartYPos, UInventoryContainer* RecievingInventory, int32 EndPosX, int32 EndPosY, bool bIsRotated);

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_AutoTransfer(FItemData Item, int32 StartXPos, int32 StartYPox, UInventoryContainer* RecievingInventory);
	bool Server_AutoTransfer_Validate(FItemData Item, int32 StartXPos, int32 StartYPox, UInventoryContainer* RecievingInventory);
	void Server_AutoTransfer_Implementation(FItemData Item, int32 StartXPos, int32 StartYPox, UInventoryContainer* RecievingInventory);

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_TransferAll(UInventoryContainer* RecievingInventory);
	bool Server_TransferAll_Validate(UInventoryContainer* RecievingInventory);
	void Server_TransferAll_Implementation(UInventoryContainer* RecievingInventory);

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_SameInventoryDirectStack(FItemData IncomingItem, int32 IncomingPosX, int32 IncomingPosY, FItemData ReceivingItem, int32 RecPosX, int32 RecPosY);
	bool Server_SameInventoryDirectStack_Validate(FItemData IncomingItem, int32 IncomingPosX, int32 IncomingPosY, FItemData ReceivingItem, int32 RecPosX, int32 RecPosY);
	void Server_SameInventoryDirectStack_Implementation(FItemData IncomingItem, int32 IncomingPosX, int32 IncomingPosY, FItemData ReceivingItem, int32 RecPosX, int32 RecPosY);

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_DifferentInventoryStack(FItemData IncomingItem, int32 IncomingItemPosX, int32 IncomingItemPosY, UInventoryContainer* RecievingInventory, FItemData ReceivingItem, int32 TargetPosX, int32 TargetPosY);
	bool Server_DifferentInventoryStack_Validate(FItemData IncomingItem, int32 IncomingItemPosX, int32 IncomingItemPosY, UInventoryContainer* RecievingInventory, FItemData ReceivingItem, int32 TargetPosX, int32 TargetPosY);
	void Server_DifferentInventoryStack_Implementation(FItemData IncomingItem, int32 IncomingItemPosX, int32 IncomingItemPosY, UInventoryContainer* RecievingInventory, FItemData ReceivingItem, int32 TargetPosX, int32 TargetPosY);


	UFUNCTION(Server, Reliable, WithValidation)
	void Server_SplitStack(FItemData OriginalItem, int32 PositionX, int32 PositionY, int32 NewStackAmount);
	bool Server_SplitStack_Validate(FItemData OriginalItem, int32 PositionX, int32 PositionY, int32 NewStackAmount);
	void Server_SplitStack_Implementation(FItemData OriginalItem, int32 PositionX, int32 PositionY, int32 NewStackAmount);
	
public:


	UFUNCTION(BlueprintPure, Category = "Inventory")
	void GetInventorySize(int32& OutSizeX, int32& OutSizeY);

	UFUNCTION(BlueprintPure, Category = "Inventory")
	void GetSlotInformation(TArray<int32>& OutXPositions, TArray<int32>& OutYPositions, TArray<bool>& OutbIsOccupied);

	UFUNCTION(BlueprintPure, Category = "Inventory")
	void GetInventoryData(TArray<FItemData>& OutItems, TArray<int32>& OutXPositions, TArray<int32>& OutYPositions);

	UPROPERTY(BlueprintAssignable)
	FOnInventoryUpdated InventoryUpdated_Onupdate;

	UFUNCTION(BlueprintImplementableEvent, Category = "Inventory")
	void InventoryUpdated();

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadOnly, Category = "Inventory Configuration", meta = (ExposeOnSpawn = "true"))
	EEquipmentSlots EquippedSlot;

	FItemData GetItemAtPosition(FVector2D Position);

	//Must be run on server
	void RemoveQuantityOfItem(FItemData Item, int32 RequestedQuantity, int32& QuantityRemoved);

	void Internal_OnInventoryUpdate();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadOnly, Category = "Inventory Configuration", meta = (ExposeOnSpawn = "true"))
	FString InventoryName;

	//Set in editor
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory Configuration", meta = (ClampMin = 1), meta = (ExposeOnSpawn = "true"))
	int32 InventorySizeX;

	//Set in editor
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory Configuration", meta = (ClampMin = 1), meta = (ExposeOnSpawn = "true"))
	int32 InventorySizeY;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory Configuration", meta = (ExposeOnSpawn = "true"))
	EInventoryFilterType FilterType = EInventoryFilterType::EIT_None;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory Configuration", meta = (ExposeOnSpawn = "true"))
	TArray<TSubclassOf<class AItemBase>> FilterClasses;

	UPROPERTY(Replicated)
	FVector2D InventorySize;

	UPROPERTY(Replicated)
	TArray<FVector2D> InventorySlots;

	UPROPERTY(Replicated)
	TArray<bool> bIsSlotOccupied;

	UPROPERTY(ReplicatedUsing = OnRep_InventoryUpdated)
	TArray<FInventoryItemData> Inventory;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadOnly, Category = "Inventory Configuration", meta = (ExposeOnSpawn = "true"))
	float MaxWeight;

	UPROPERTY(Replicated, BlueprintReadOnly)
	float CurrentWeight;

	void InitalizeSlots();

	bool CheckIfItemFitsInPosition(FItemData Item, FVector2D Position);

	bool CheckIfItemCouldBeAdded(FItemData Item);

	bool AddItem(FItemData Item, FVector2D Position, bool bCheckWeight, bool bDeferUIUpdate);

	bool RemoveItem(FItemData Item, FVector2D Position, bool bDeferUIUpdate);

	bool MoveItem(FItemData Item, FVector2D StartingPosition, FVector2D EndingPosition, bool bIsRotated);

	bool SplitStack(FItemData OriginalItem, FVector2D StartingPosition, int32 NewStackAmount);

	bool AutoAddItem(FItemData Item, bool bShouldStackItem, FItemData& OutRemainingItem, bool bDeferUIIpdate);

	void AutoStackItem(FItemData Item, bool& OutItemFullyStacked, FItemData& OutLeftOverItemData,bool bDeferUIUpdate);

	bool DirectTransfer(FItemData Item, FVector2D StartingPosition, UInventoryContainer* RecievingInventory, FVector2D EndingPosition, bool bIsRotated);

	bool AutoTransfer(FItemData Item, FVector2D StartingPosition, UInventoryContainer* ReceivingInventory, bool UpdateUI);

	bool SameInventoryStack(FItemData IncomingItem, FVector2D IncomingItemPos, FItemData ReceivingItem, FVector2D TargetPosition, FItemData& OutLefOverItemData);

	bool DifferentInventoryStack(FItemData IncomingItem, FVector2D IncomingItemPos, UInventoryContainer* RecievingInventory, FItemData ReceivingItem, FVector2D TargetPosition, FItemData& OutLefOverItemData);

	bool DirectStack(FItemData IncomingItem, FItemData ReceivingItem, FVector2D TargetPosition, FItemData& OutLefOverItemData);

	void RemoveQuantityOfItemFromStack(int32 QtyToRemove,FItemData Item, FVector2D StartingPosition);

	bool PerformFilterCheck(FItemData Item);

	bool IsItemInFilterList(FItemData Item);

	bool IsItemValidSubClass(FItemData Item, TSubclassOf<class AItemBase> FilterClass);

	//TODO StackTransfer

	//Helper function to change status of slots given start position and size, false is that the slot is not occupied
	void SetSlotsAsOccupied(int32 SizeX, int32 SizeY, FVector2D Position, bool bIsOccupied);

	bool FindInventoryItemIndex(FItemData Item, FVector2D Position, int32& OutIndex);

	bool FindAllItemIndexices(FItemData Item, TArray<int32>& OutItemIndexices);

	//returns false if no quantity was found
	bool FindTotalQuantityOfItem(FItemData Item, int32& OutQuantity);

	bool IsValidItem(FItemData Item, FVector2D Position);

	bool CheckIfWeightOK(FItemData Item);

	void UpdateWeight();

	void RefreshSlotOccupancy();

	bool CheckIfSlotIsOccupied(FVector2D Position);

	UFUNCTION(Client, Reliable)
	void Client_InventoryUpdate();
	void Client_InventoryUpdate_Implementation();

	UFUNCTION()
	void OnRep_InventoryUpdated();

	FItemData GetServerVersionOfItem(FItemData ClientItem, UInventoryContainer* TargetInventory, FVector2D Position);

	UInventoryContainer* ReceivingInventoryTemp;

	void UpdateUIDelay();
};
