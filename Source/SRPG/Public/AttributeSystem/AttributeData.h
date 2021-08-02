// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "AttributeData.generated.h"

USTRUCT(BlueprintType)
struct FAttributeEffectTrigger
{
	GENERATED_USTRUCT_BODY()

		//Attribute effect that will be applied
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = " Atttribute Trigger")
	TSubclassOf<class UAttributeEffect> AttributeEffect;


	//Should this trigger the attribute effect if it is greater than an amount
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = " Atttribute Trigger")
	bool bApplyWhenGreater;

	//Trigger amount for the attribute effect if greater than
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = " Atttribute Trigger")
	float GreaterThanAmount;

	//Should this trigger the attribute effect if it is less than an amount
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = " Atttribute Trigger")
	bool bApplyWhenLessThan;

	//Trigger amount for the attribute effect if less than
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = " Atttribute Trigger")
	float LessThanAmount;

};



USTRUCT(BlueprintType)
struct FAttributeData
{
	GENERATED_USTRUCT_BODY()

	//Clamps the max amount of for an attribute
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = " Atttribute Data", meta = (ClampMin = "0.0"))
	float AttributeMaxAmount = 100;

	//Current ammount of the attribute
	UPROPERTY(BlueprintReadOnly, Category = " Atttribute Data", meta = (ClampMin = "0.0"))
	float AttributeCurrentAmount; 

	//Attribute Effect Triggers
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = " Atttribute Data")
	TArray<FAttributeEffectTrigger> AtributeTriggers;

};