// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "AttributeEffect.generated.h"

/**
 * 
 */
UCLASS(blueprintable, BlueprintType)
class SRPG_API UAttributeEffect : public UObject
{
	GENERATED_BODY()
	
public:

	void InitalizeEffect(class UBaseAttributeComponent* SpawningComponent);

	void OnEffectDeactivated();


protected:
	UBaseAttributeComponent* OriginatingComponent;

};
