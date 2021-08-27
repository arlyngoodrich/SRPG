// Fill out your copyright notice in the Description page of Project Settings.


#include "EnviornmentSystem/FoliageManager.h"

//UE4 Includes
#include "Net/UnrealNetwork.h"

// Sets default values
AFoliageManager::AFoliageManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AFoliageManager::BeginPlay()
{
	Super::BeginPlay();
	
}

void AFoliageManager::UpdateClientInstances(FFoliageInstanceData InstanceData)
{
	Client_InternaUpdateClientInstances(InstanceData);
	UE_LOG(LogTemp,Log,TEXT("Update Client Instances Called"))
}

void AFoliageManager::Client_InternaUpdateClientInstances_Implementation(FFoliageInstanceData InstanceData)
{
	OnUpdateClientInstances(InstanceData);
	UE_LOG(LogTemp, Log, TEXT("On Update Clieint Instances Called"))
}

// Called every frame
void AFoliageManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

