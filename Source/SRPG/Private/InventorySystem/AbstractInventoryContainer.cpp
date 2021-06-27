// Fill out your copyright notice in the Description page of Project Settings.


#include "InventorySystem/AbstractInventoryContainer.h"
#include "LogFiles.h"

void UAbstractInventoryContainer::SetInventory(TArray<FInventoryData> NewInventory, int32 NewAbstractInventoryPairID)
{
    Inventory = NewInventory;
    Abstract_AbstractInventoryPairID = NewAbstractInventoryPairID;
}

void UAbstractInventoryContainer::GetInventory(TArray<FInventoryData>& OutInventory, int32& OutAbstractInventoryPairID)
{
    OutInventory = Inventory;
    OutAbstractInventoryPairID = Abstract_AbstractInventoryPairID;

}

void UAbstractInventoryContainer::BeginDestroy()
{
    Super::BeginDestroy();
    UE_LOG(LogInventorySystem,Log,TEXT("Abstract Destroyed"))
}

