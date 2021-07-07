// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InventorySystem/ItemData.h"
#include "CropData.h"
#include "Crop.generated.h"


UCLASS()
class SRPG_API ACrop : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACrop();

	//Blueprint Accessor Functions
	UFUNCTION(BlueprintCallable, Category = "Crop Data", meta = (DisplayName = "Get Random Gene Set"))
	FCropGeneData BP_GetRandomGeneSet();

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Crop Data", meta = (DisplayName = "Set Genes"))
	void BP_SetGenes(FCropGeneData NewGenes);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Crop Data", meta = (DisplayName = "Set Growth Stage"))
	void BP_SetGrowhtStage(EGrowthState NewGrowthState);

	UFUNCTION(BlueprintCallable, Category = "Crop Data", meta = (DisplayName = "Get Crop Save Data"))
	FCropSaveData GetCropSaveData();

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Crop Data", meta = (DisplayName = "Load Crop Save Data"))
	void LoadCropSaveData(FCropSaveData SaveData);

	//GetCropData
	UFUNCTION(BlueprintPure, Category = "Crop Data", meta = (DisplayName = "Get Growth Data"))
	FCropGrowthData GetCropData();

	//GetGeneData
	UFUNCTION(BlueprintPure, Category = "Crop Data", meta = (DisplayName = "Get Gene Data"))
	FCropGeneData GetGeneData();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* CropMesh;
	
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Crop Data | Water Data")
	float CurrentWater;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Crop Data | Fertilizer Data")
	float CurrentFertilizer;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Crop Data | Temp Data")
	float CurrentTemp;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Crop Data | Health Data")
	float CurrentHealth;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Crop Data | Growth Data")
	int32 DaysToNextGrowthLevel;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Crop Data | Growth Data")
	EGrowthState CurrentGrowthState;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Crop Data | Crowth Data")
	FCropGrowthData CurrentGrowthData;

	UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadOnly, Category = "Crop Data | Growth Data")
	FCropGrowthData SeedlingGrowthData;
	
	UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadOnly, Category = "Crop Data | Growth Data")
	FCropGrowthData MiddlingGrowthData;

	UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadOnly, Category = "Crop Data | Growth Data")
	FCropGrowthData MatureGrowthData;

	UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadOnly, Category = "Crop Data | Genetic Data")
	bool bRandomizeGenesAtBeginPlay = false;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Crop Data | Genetic Data")
	FCropGeneData GeneticData;

	void SetDefaultGrowthData();

	float CalculateGeneEffect(EGeneType ActiveGene);

	void GetCrossbredSeedGeneticData(FCropGeneData PartnerGeneData, FCropGeneData& OutGeneticData);

	void CreateRandomGeneDataSet(FCropGeneData& OutRandomGeneDataSet);

	FGeneData CreateRandomeGene();

	EGeneType ConvertIntToGene(int32 GeneInt);

	FGeneData GetNewGene(FGeneData GenePairA, FGeneData GenePairB);

	void ApplyGrowthStage(FCropGrowthData NewGrowthStageData, EGrowthState NewGrowthState);

	void SetGrowthStage(EGrowthState NewGrowthState);

	void ModifyYieldData(float YieldModifer, float SeedYieldModifer, TArray<FCropYieldData> CropYieldData, TArray<FCropYieldData>& OutNewYieldData);

	void CalculateHighLowRanges(float GeneticEffect, float& OutHighModifer, float& OutLowModifer);

	void ApplyGeneticEffect(FCropGrowthData NewGrowthData, FCropGrowthData& OutModifiedGrowthData);
};
