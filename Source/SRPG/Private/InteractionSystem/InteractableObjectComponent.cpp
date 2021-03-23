// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractionSystem/InteractableObjectComponent.h"
#include "InteractionSystem/PlayerInteractionSensor.h"

// Sets default values for this component's properties
UInteractableObjectComponent::UInteractableObjectComponent()
{

	PrimaryComponentTick.bCanEverTick = true;

}


// Called when the game starts
void UInteractableObjectComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...

}

