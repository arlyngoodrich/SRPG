// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventorySystem/ItemData.h"
#include "PlayerEquipmentManager.generated.h"


UCLASS( ClassGroup=(Custom), blueprintable, meta=(BlueprintSpawnableComponent) )
class SRPG_API UPlayerEquipmentManager : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPlayerEquipmentManager();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Equipment Data")
	TArray<FEquipmentSlot> GetEquipmentSlotInfo();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Equipment Data")
	void GetSlotItemData(EEquipmentSlots TargetSlot, bool& OutIsOccuiped, FItemData& OutItemData);

	// Adds Item to slot and removes from losing inventory
	// Does not check if item was removed or not from inventory, just calls BP implementation of RemoveItem
	//TODO make internal access to inventory 
	UFUNCTION(BlueprintCallable, Category = "Equipment Data", meta = (DisplayName = "Equip Item"))
	void BP_EquipItem(EEquipmentSlots TargetSlot, FItemData Item, class UInventoryContainer* LosingInventory, int32 PosX, int32 PosY);

	UFUNCTION(BlueprintCallable, Category = "Equipment Data", meta = (DisplayName = "Auto Equip Item"))
	void BP_AutoEquipItem(FItemData Item, class UInventoryContainer* LosingInventory, int32 PosX, int32 PosY);

	UFUNCTION(BlueprintCallable, Category = "Equipment Data", meta = (DisplayName = "Unequip Item"))
	void BP_UnEquipItem(EEquipmentSlots TargetSlot);

	UFUNCTION(BlueprintCallable, Category = "Equipment Data", meta = (DisplayName = "Unequip Item to Position"))
	void BP_UnequipItemToPosition(EEquipmentSlots TargetSlot, class UInventoryContainer* TargetInventory, int32 PosX, int32 PosY);

	UFUNCTION(BlueprintCallable, Category = "Equipment Data", meta = (DisplayName = "Move Slot"))
	void BP_MoveSlot(EEquipmentSlots CurrentSlot, EEquipmentSlots NewSlot, FItemData ItemData);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_EquipmentSlots, EditDefaultsOnly, Category = "Equipment Data" )
	TArray<FEquipmentSlot> EquipmentSlots;
	
	UPROPERTY(BlueprintReadOnly, Category = "Equpiment Data")
	TArray<class UInventoryContainer*> OwnerInventories;

	UFUNCTION(BlueprintImplementableEvent, Category = "Equipment Data", meta = (DisplayName = "Equipment Update"))
	void OnRep_EquipmentSlots();

	UFUNCTION(BlueprintNativeEvent, Category = "Equipment Data", meta = (DisplayName = "Unequip by Dop"))
	bool DropItem(FItemData Item);

	UFUNCTION(BlueprintImplementableEvent, Category = "Equipment Data")
	void OnEquipItem(EEquipmentSlots Slot, FItemData ItemData);

	UFUNCTION(BlueprintImplementableEvent, Category = "Equipment Data")
	void OnUnequipItem(EEquipmentSlots Slot);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Equipment Data", meta = (DisplayName = "Multicast Call Set Skeletal Mesh"))
	void BP_CallSetSkeletalMesh(EEquipmentSlots TargetSlot, FItemData ItemData);

	UFUNCTION(BlueprintImplementableEvent, Category = "Equipment Data", meta = (DisplayName = "Set Skeletal Mesh"))
	void BP_SetSkeletalMesh(EEquipmentSlots TargetSlot, FItemData ItemData);

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_EquipItem(EEquipmentSlots TargetSlot, FItemData Item, class UInventoryContainer* LosingInventory, int32 PosX, int32 PosY);
	bool Server_EquipItem_Validate(EEquipmentSlots TargetSlot, FItemData Item, class UInventoryContainer* LosingInventory, int32 PosX, int32 PosY);
	void Server_EquipItem_Implementation(EEquipmentSlots TargetSlot, FItemData Item, class UInventoryContainer* LosingInventory, int32 PosX, int32 PosY);

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_UnEquipItem(EEquipmentSlots TargetSlot);
	bool Server_UnEquipItem_Validate(EEquipmentSlots TargetSlot);
	void Server_UnEquipItem_Implementation(EEquipmentSlots TargetSlot);

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_MoveSlots(EEquipmentSlots CurrentSlot, EEquipmentSlots NewSlot, FItemData ItemData);
	bool Server_MoveSlots_Validate(EEquipmentSlots CurrentSlot, EEquipmentSlots NewSlot, FItemData ItemData);
	void Server_MoveSlots_Implementation(EEquipmentSlots CurrentSlot, EEquipmentSlots NewSlot, FItemData ItemData);

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_UnequipItemToPosition(EEquipmentSlots TargetSlot, class UInventoryContainer* TargetInventory, int32 PosX, int32 PosY);
	bool Server_UnequipItemToPosition_Validate(EEquipmentSlots TargetSlot, class UInventoryContainer* TargetInventory, int32 PosX, int32 PosY);
	void Server_UnequipItemToPosition_Implementation(EEquipmentSlots TargetSlot, class UInventoryContainer* TargetInventory, int32 PosX, int32 PosY);

	UFUNCTION(NetMulticast, Reliable)
	void Server_SetSkeletalMesh(EEquipmentSlots TargetSlot, FItemData ItemData);

	void AutoEquipItem(FItemData Item, class UInventoryContainer* LosingInventory, int32 PosX, int32 PosY);
	
	void ValidateSlots();

	void SetOwnerInventories();

	bool ReturnItemToInventory(FItemData Item);

	bool FindSlot(EEquipmentSlots TargetSlot, int32& Index);

};
