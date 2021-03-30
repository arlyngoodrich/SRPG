// Fill out your copyright notice in the Description page of Project Settings.


#include "EnviornmentSystem/CustomFoliageActor.h"

// Sets default values
ACustomFoliageActor::ACustomFoliageActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACustomFoliageActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACustomFoliageActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

