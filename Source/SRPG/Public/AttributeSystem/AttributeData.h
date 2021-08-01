// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "AttributeData.generated.h"


USTRUCT(BlueprintType)
struct FAttributeData
{
	GENERATED_USTRUCT_BODY()

	//Clamps the max amount of for an attribute
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = " Atttribute Data", meta = (ClampMin = "0.0"))
	float AttributeMaxAmount = 100;

};