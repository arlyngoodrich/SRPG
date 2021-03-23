// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "ItemData.generated.h"


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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory Item Data")
		bool bCanBeStacked;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory Item Data")
		bool bIsRotated;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory Item Data", meta = (EditCondition = "bCanBeStacked"))
		int32 StackQuantity = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory Item Data", meta = (EditCondition = "bCanBeStacked"))
		int32 MaxStackQuantity = 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory Item Data", meta = (ClampMin = 0))
		float PerItemWeight;

};