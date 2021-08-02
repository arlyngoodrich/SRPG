// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AttributeData.h"
#include "BaseAttributeComponent.generated.h"


UCLASS( ClassGroup=(Attributes), meta=(BlueprintSpawnableComponent) )
class SRPG_API UBaseAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UBaseAttributeComponent();

	void CheckOutEffect(class UAttributeEffect* Effect);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	void ChangeAttributeAmount(FAttributeData InAttribute, float ChangeAmount, FAttributeData& OutAttribute);

	//Returns true if EffectClass is found in TArray member 'AppliedEffects'
	bool CheckIfEffectIsAlreadyActive(UClass* EffectClass);

	bool bIsAuthority;
	
	UPROPERTY(BlueprintReadOnly, Category = "Attribute Data")
	TArray<class UAttributeEffect*> AppliedEffects;
};
