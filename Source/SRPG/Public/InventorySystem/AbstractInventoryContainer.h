// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InventoryContainer.h"
#include "UObject/NoExportTypes.h"
#include "AbstractInventoryContainer.generated.h"

/**
 * 
 */
UCLASS(blueprintable, BlueprintType)
class SRPG_API UAbstractInventoryContainer : public UObject
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Inventory Data")
	void SetInventory(TArray<FInventoryData> NewInventory);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Inventory Data")
	TArray<FInventoryData> GetInventory();

protected:
	UPROPERTY()
	TArray<FInventoryData> Inventory;

	virtual void BeginDestroy() override;
	
};
