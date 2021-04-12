// Fill out your copyright notice in the Description page of Project Settings.


#include "InventorySystem/AbstractInventoryContainer.h"
#include "LogFiles.h"

void UAbstractInventoryContainer::SetInventory(TArray<FInventoryData> NewInventory)
{
    Inventory = NewInventory;
}

TArray<FInventoryData> UAbstractInventoryContainer::GetInventory()
{
    return Inventory;
}

void UAbstractInventoryContainer::BeginDestroy()
{
    Super::BeginDestroy();
    UE_LOG(LogInventorySystem,Log,TEXT("Abstract Destroyed"))
}

