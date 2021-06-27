// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "ItemData.generated.h"


UENUM(BlueprintType)
enum class EEquipmentSlots : uint8 {

	ESE_None		UMETA(DisplayName = "None"),
	ESE_Torso		UMETA(DisplayName = "Torso"),
	ESE_Head		UMETA(DisplayName = "Head"),
	ESE_Hands		UMETA(DisplayName = "Hands"),
	ESE_Legs		UMETA(DisplayName = "Legs"),
	ESE_Feet		UMETA(DisplayName = "Feet"),
	ESE_Back		UMETA(DisplayName = "Back"),
	ESE_Shoulder	UMETA(DisplayName = "Shoulder"),
	ESE_LeftHip		UMETA(DisplayName = "LeftHip"),
	ESE_RightHip	UMETA(DisplayName = "RightHip"),
};


UENUM(BlueprintType)
enum class EItemType : uint8 {

	EIT_None			UMETA(DisplayName = "None"),
	EIT_Equipment		UMETA(DisplayName = "Equipment"),
	EIT_Consumable		UMETA(DisplayName = "Consumable"),
	EIT_Seed			UMETA(DisplayName = "Seed"),

};


USTRUCT(BlueprintType)
struct FItemData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Info")
	FName DisplayName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Info")
	FText Discription;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Info")
	EItemType ItemType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory Info")
	int32 SizeX;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory Info")
	int32 SizeY;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory Info")
	UTexture2D* InventoryItemIcon = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory Info")
	UTexture2D* SmallItemIcon = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory Info")
	TSubclassOf<class AItemBase> InWorldActorClass;

	//This causes a bug for some reason... just set max stack 1
	bool bCanBeStacked = true;

	UPROPERTY(BlueprintReadWrite, Category = "Inventory Info")
	bool bIsRotated;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory Info", meta = (EditCondition = "bCanBeStacked"))
	int32 StackQuantity = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory Info", meta = (EditCondition = "bCanBeStacked"))
	int32 MaxStackQuantity = 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory Info", meta = (ClampMin = 0))
	float PerItemWeight;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Equipment Data")
	bool bIsEquipable;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Equipment Data", meta = (EditCondition = "bIsEquipable"))
	TArray<EEquipmentSlots> EligibleSlots;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipment Data", meta = (EditCondition = "bIsEquipable"))
	bool bIsMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipment Data", meta = (EditCondition = "bIsMesh"))
	USkeletalMesh* SkeletalMesh = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipment Data", meta = (EditCondition = "bIsEquipable"))
	bool bHasInventory;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory Configuration", meta = (EditCondition = "bHasInventory"))
	FString InventoryName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory Configuration", meta = (ClampMin = 1), meta = (EditCondition = "bHasInventory"))
	int32 InventorySizeX;

	//Set in editor
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory Configuration", meta = (ClampMin = 1), meta = (EditCondition = "bHasInventory"))
	int32 InventorySizeY;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory Configuration", meta = (EditCondition = "bHasInventory"))
	float MaxWeight;

	UPROPERTY(BlueprintReadWrite, Category = "Inventory Configuration")
	class UAbstractInventoryContainer* AbstractInventory = nullptr;

	UPROPERTY(BlueprintReadWrite, Category = "Inventory Configuration")
	int32 Inventory_AbstractInventoryPairID;

	UPROPERTY(BlueprintReadWrite, Category = "Abstract Data")
	class UAbstractDataContainer* AbstractData = nullptr;

	UPROPERTY(BlueprintReadWrite, Category = "Abstract Data")
	int32 AbstractDataContainerPairID;

};



USTRUCT(BlueprintType)
struct FEquipmentSlot
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Equipment Data", meta = (ClampMin = 0), meta = (ClampMax = 100))
	EEquipmentSlots Slot;

	UPROPERTY(BlueprintReadOnly, Category = "Equipment Data")
	bool bIsOccupied;

	UPROPERTY(BlueprintReadOnly, Category = "Equipment Data")
	FItemData OccupyingItemData;

};
