// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InteractableObjectComponent.generated.h"


class UPlayerInteractionSensor;


UCLASS(ClassGroup = (InteractionSystem), Blueprintable)
class SRPG_API UInteractableObjectComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UInteractableObjectComponent();

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "Interaction Triggered"))
		void InteractionTriggered(UPlayerInteractionSensor* InstigatingComponent);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;


};
