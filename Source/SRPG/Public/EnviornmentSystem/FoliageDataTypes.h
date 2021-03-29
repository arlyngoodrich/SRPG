// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "FoliageDataTypes.generated.h"

class ACustomFoliageActor;
class UStaticMesh;

UENUM(BlueprintType)
enum class EVegitationStatus : uint8 {

	VSE_NewGrowth       UMETA(DisplayName = "New Growth"),
	VSE_MediumGrowth	UMETA(DisplayName = "Medium Growth"),
	VSE_OldGrowth	    UMETA(DisplayName = "Old Growth"),
};


USTRUCT(BlueprintType)
struct FResourceRewardData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Foliage Data")
	FName ResourceName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Foliage Data", meta = (ClampMin = 0))
	int32 Quantity;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Foliage Data", meta = (ClampMin = 0))
	int32 MinAmount;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Foliage Data", meta = (ClampMin = 0))
	int32 MaxAmount;

};



USTRUCT(BlueprintType)
struct FFoliageInformation : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Foliage Data")
	bool bCanDamageForHarvest;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Foliage Data", meta = (EditCondition = "bCanDamageForHarvest"))
	TArray<FResourceRewardData> RewardsOnDamage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Foliage Data", meta = (EditCondition = "bCanDamageForHarvest"))
	float MinDamageAmountForHarvest;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Foliage Data", meta = (EditCondition = "bCanDamageForHarvest"))
	int32 MaxNumberOfDamageHarvests;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Foliage Data", meta = (EditCondition = "bCanInteractForHarvest"))
	bool bCanInteractForHarvest;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Foliage Data", meta = (EditCondition = "bCanInteractForHarvest"))
	FText InteractionPrompt;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Foliage Data", meta = (EditCondition = "bCanInteractForHarvest"))
	bool bShouldOutline;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Foliage Data", meta = (EditCondition = "bCanInteractForHarvest"))
	TArray<FResourceRewardData> RewardsOnInteract;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Foliage Data", meta = (EditCondition = "bCanInteractForHarvest"))
	int32 MaxNumberOfInteractHarvests;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Foliage Data")
	TArray<UStaticMesh*> FolliageMeshes;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Foliage Data")
	TSubclassOf<ACustomFoliageActor> FoliageActor;

};
