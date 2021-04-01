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

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "EquipmentData")
	TArray<FEquipmentSlot> GetEquipmentSlotInfo();

	// Adds Item to slot and removes from losing inventory
	// Does not check if item was removed or not from inventory, just calls BP implementation of RemoveItem
	//TODO make internal access to inventory 
	UFUNCTION(BlueprintCallable, Category = "Equipment Data", meta = (DisplayName = "Equip Item"))
	void BP_EquipItem(EEquipmentSlots TargetSlot, FItemData Item, class UInventoryContainer* LosingInventory, int32 PosX, int32 PosY);

	UFUNCTION(BlueprintCallable, Category = "Equipment Data", meta = (DisplayName = "Unequip Item"))
	void BP_UnEquipItem(EEquipmentSlots TargetSlot);

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

	void SetOwnerInventories();

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_EquipItem(EEquipmentSlots TargetSlot, FItemData Item, class UInventoryContainer* LosingInventory, int32 PosX, int32 PosY);
	bool Server_EquipItem_Validate(EEquipmentSlots TargetSlot, FItemData Item, class UInventoryContainer* LosingInventory, int32 PosX, int32 PosY);
	void Server_EquipItem_Implementation(EEquipmentSlots TargetSlot, FItemData Item, class UInventoryContainer* LosingInventory, int32 PosX, int32 PosY);

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_UnEquipItem(EEquipmentSlots TargetSlot);
	bool Server_UnEquipItem_Validate(EEquipmentSlots TargetSlot);
	void Server_UnEquipItem_Implementation(EEquipmentSlots TargetSlot);

	bool ReturnItemToInventory(FItemData Item);

	bool FindSlot(EEquipmentSlots TargetSlot, int32& Index);

};