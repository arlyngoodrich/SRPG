// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSystem/AttributeEffect.h"
#include "ConsumeEffect.generated.h"

/**
 * 
 */
UCLASS(blueprintable, BlueprintType)
class SRPG_API UConsumeEffect : public UAttributeEffect
{
	GENERATED_BODY()


protected:

	virtual void Internal_OnEffectTriggered() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Consumption Effect")
	float CarbsChange;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Consumption Effect")
	float VitaminsChange;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Consumption Effect")
	float ProteinChange;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Consumption Effect")
	float WaterChange;

};
