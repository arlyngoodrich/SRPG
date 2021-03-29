// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FoliageInstancedStaticMeshComponent.h"
#include "CustomFoliageISMComponent.generated.h"

class UStaticMesh;

/**
 * 
 */
UCLASS()
class SRPG_API UCustomFoliageISMComponent : public UFoliageInstancedStaticMeshComponent
{
	GENERATED_BODY()


public:

	UFUNCTION(BlueprintCallable, Category = "Foliage Data")
	void GetInstancesInRange(FVector TargetLocaiton, float Range, TArray<int32>& OutInstanceIndexies, TArray<FTransform>& OutInstanceTransforms, UStaticMesh*& OutMeshObject);
	
};
