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
	void SetInventory(TArray<FInventoryData> NewInventory, int32 NewAbstractInventoryPairID);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Inventory Data")
	void GetInventory(TArray<FInventoryData>& OutInventory, int32& OutAbstractInventoryPairID);

	UPROPERTY(BlueprintReadOnly, Category = "Inventory Data")
	int32 Abstract_AbstractInventoryPairID;

protected:
	UPROPERTY()
	TArray<FInventoryData> Inventory;

	virtual void BeginDestroy() override;
	
};
