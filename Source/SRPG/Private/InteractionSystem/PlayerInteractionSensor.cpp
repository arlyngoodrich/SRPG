// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractionSystem/PlayerInteractionSensor.h"
#include "InteractionSystem/InteractableObjectComponent.h"

//UE4 Includes
#include "Net/UnrealNetwork.h"

// Sets default values for this component's properties
UPlayerInteractionSensor::UPlayerInteractionSensor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;


	// ...
}


// Called when the game starts
void UPlayerInteractionSensor::BeginPlay()
{
	Super::BeginPlay();

	// ...

}

void UPlayerInteractionSensor::TriggerInteraction(UInteractableObjectComponent* InteractableComponent, int32 InteractionIndex)
{
	//If one of the components is not valid, exit function
	if (!InteractableComponent) { return; }

	if (GetOwnerRole() < ROLE_Authority)
	{
		Server_TriggerInteraction(InteractableComponent, InteractionIndex);
	}
	else
	{

		InteractableComponent->InteractionTriggered(this, InteractionIndex);

	}


}

bool UPlayerInteractionSensor::Server_TriggerInteraction_Validate(UInteractableObjectComponent* InteractableComponent, int32 InteractionIndex)
{
	return true;
}

void UPlayerInteractionSensor::Server_TriggerInteraction_Implementation(UInteractableObjectComponent* InteractableComponent, int32 InteractionIndex)
{
	TriggerInteraction(InteractableComponent, InteractionIndex);
}


