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
	ESE_LeftHip		UMETA(DisplayName = "LeftHip"),
	ESE_RightHip	UMETA(DisplayName = "RightHip"),
};



USTRUCT(BlueprintType)
struct FEquipmentData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

		UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Equipment Data", meta = (ClampMin = 0), meta = (ClampMax = 100))
		float Heatlh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Equipment Data")
		TArray<EEquipmentSlots> EligibleSlots;

};






USTRUCT(BlueprintType)
struct FItemData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory Item Data")
		FName DisplayName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory Item Data")
		FText Discription;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory Item Data")
		int32 SizeX;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory Item Data")
		int32 SizeY;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory Item Data")
		UTexture2D* InventoryItemIcon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory Item Data")
		UTexture2D* SmallItemIcon;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory Item Data")
		TSubclassOf<AActor> InWorldActorClass;

	//This causes a bug for some reason... just set max stack 1
		bool bCanBeStacked = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory Item Data")
		bool bIsRotated;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory Item Data", meta = (EditCondition = "bCanBeStacked"))
		int32 StackQuantity = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory Item Data", meta = (EditCondition = "bCanBeStacked"))
		int32 MaxStackQuantity = 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory Item Data", meta = (ClampMin = 0))
		float PerItemWeight;

	// Equipment Data 

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Equipment Data")
	bool bIsEquipable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipment Data", meta = (EditCondition = "bIsEquipable"))
	FEquipmentData EquipmentData;

};

USTRUCT(BlueprintType)
struct FEquipmentSlot
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Equipment Data", meta = (ClampMin = 0), meta = (ClampMax = 100))
	EEquipmentSlots Slot;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Equipment Data")
	bool bIsOccupied;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Equipment Data")
	FItemData OccupyingItemData;

};
