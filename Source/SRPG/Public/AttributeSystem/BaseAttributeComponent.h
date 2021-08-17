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

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Attribute Effects")
	void CheckInEffect(class UAttributeEffect* AttributeEffect);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Attribute Effects")
	void CheckOutEffect(class UAttributeEffect* AttributeEffect);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;



	UPROPERTY()
	TArray<class UAttributeEffect*> ActiveEffects;

};
