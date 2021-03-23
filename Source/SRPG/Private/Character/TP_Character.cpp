// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/TP_Character.h"

// Sets default values
ATP_Character::ATP_Character()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATP_Character::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATP_Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ATP_Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

