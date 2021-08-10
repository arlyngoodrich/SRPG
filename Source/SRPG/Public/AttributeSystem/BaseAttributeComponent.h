// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AttributeData.h"
#include "BaseAttributeComponent.generated.h"


UCLASS( ClassGroup=(Attributes))
class SRPG_API UBaseAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UBaseAttributeComponent();


protected:
	// Called when the game starts
	virtual void BeginPlay() override;

};
