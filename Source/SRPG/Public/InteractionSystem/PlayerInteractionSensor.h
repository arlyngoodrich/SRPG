// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerInteractionSensor.generated.h"

class UInteractableObjectComponent;


UCLASS( ClassGroup=(InteractionSystem), Blueprintable)
class SRPG_API UPlayerInteractionSensor : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UPlayerInteractionSensor();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category = "Interaction System")
		void TriggerInteraction(UInteractableObjectComponent* InteractableComponent);

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_TriggerInteraction(UInteractableObjectComponent* InteractableComponent);
	bool Server_TriggerInteraction_Validate(UInteractableObjectComponent* InteractableComponent);
	void Server_TriggerInteraction_Implementation(UInteractableObjectComponent* InteractableComponent);

};
