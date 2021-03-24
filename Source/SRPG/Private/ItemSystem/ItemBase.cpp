// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemSystem/ItemBase.h"

//UE4 Includes
#include "Net/UnrealNetwork.h"

// Sets default values
AItemBase::AItemBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;

}

// Called when the game starts or when spawned
void AItemBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void AItemBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty >& OutLifetimeProps) const
{

	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AItemBase, ItemData);

}

void AItemBase::UpdateItemInformation(FItemData NewItemData)
{
	ItemData = NewItemData;
}
