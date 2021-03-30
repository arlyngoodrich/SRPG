// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Runtime/Engine/Classes/Engine/EngineTypes.h"
#include "FoliageDataTypes.generated.h"

class ACustomFoliageActor;
class UStaticMesh;
class UDamageType;

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
	int32 MinAmount;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Foliage Data", meta = (ClampMin = 0))
	int32 MaxAmount;

};



USTRUCT(BlueprintType)
struct FFoliageInformation : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Foliage General Settings")
	TArray<UStaticMesh*> FolliageMeshes;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Foliage General Settings")
	TSubclassOf<ACustomFoliageActor> FoliageActor;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Foliage General Settings")
	bool bUseDestructableMesh;

	// Destructible Meshes need to have the same array index as FoliageMeshes
	// You must also add a Destructible Mesh Component to your Foliage Actor Blueprint!
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Foliage General Settings", meta = (EditCondition = "bUseDestructableMesh"))
	TArray<class UDestructibleMesh*> FoliageDestructibleMeshes;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Foliage General Settings")
	bool bCanDamageForHarvest;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Foliage General Settings")
	bool bCanInteractForHarvest;



	// On Damage Settings //

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "On Damage Settings", meta = (EditCondition = "bCanDamageForHarvest"))
	TArray <TSubclassOf<UDamageType>> ValidDamangeTypes;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "On Damage Settings", meta = (EditCondition = "bCanDamageForHarvest"))
	TArray<FResourceRewardData> RewardsOnDamage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "On Damage Settings", meta = (EditCondition = "bCanDamageForHarvest"))
	float MinDamageAmountForHarvest;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "On Damage Settings", meta = (EditCondition = "bCanDamageForHarvest"))
	int32 MaxNumberOfDamageHarvests;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "On Damage Settings", meta = (EditCondition = "bCanDamageForHarvest"))
	bool bRespawnAfterHarvestDepletion;

	// Time in seconds
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "On Damage Settings", meta = (EditCondition = "bRespawnAfterHarvestDepletion"))
	float TimeToRespawnAfterDamageHarvest;

	// Time in seconds
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "On Damage Settings", meta = (EditCondition = "bCanDamageForHarvest"))
	float PresistanceAfterDepletion;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "On Damage Settings", meta = (EditCondition = "bCanDamageForHarvest"))
	bool bApplyImpulseOnDepletion;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "On Damage Settings", meta = (EditCondition = "bApplyImpulseOnDepletion"), meta = (ClampMin = 1))
	float ForceMultiplyer;



	// On Interact Settings

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "On Interact Settings", meta = (EditCondition = "bCanInteractForHarvest"))
	FText InteractionPrompt;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "On Interact Settings", meta = (EditCondition = "bCanInteractForHarvest"))
	bool bShouldOutline;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "On Interact Settings", meta = (EditCondition = "bCanInteractForHarvest"))
	TArray<FResourceRewardData> RewardsOnInteract;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "On Interact Settings", meta = (EditCondition = "bCanInteractForHarvest"))
	int32 MaxNumberOfInteractHarvests;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "On Interact Settings", meta = (EditCondition = "bCanInteractForHarvest"))
	bool bRespawnAfterInteractDepletion;

	// Time in seconds
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "On Interact Settings", meta = (EditCondition = "bRespawnAfterInteractDepletion"))
	float TimeToRespawnAfterInteractHarvest;


};
