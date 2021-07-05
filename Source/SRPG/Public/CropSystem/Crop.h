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

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* CropMesh;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Crop Data | Water Data")
	FWaterData WaterData;
	
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Crop Data | Water Data")
	float CurrentWater;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Crop Data | Water Data")
	float WaterConsumptionPerDay;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Crop Data | Fertilizer Data")
	FFertilizerData FertilizerData;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Crop Data | Fertilizer Data")
	float CurrentFertilizer;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Crop Data | Temp Data")
	FTempData TempData;

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

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Crop Data | Genetic Data")
	FCropGeneData GeneticData;

	float CalculateGeneEffect(EGeneType ActiveGene);

	EGeneType SetActiveGene(EGeneType Gene1, EGeneType Gene2);

	void ApplyGrowthStage(FCropGrowthData NewGrowthStageData, EGrowthState NewGrowthState);

	void SetGrowthStage(EGrowthState NewGrowthState);

	void ModifyYieldData(float YieldModifer, TArray<FCropYieldData> CropYieldData, TArray<FCropYieldData>& OutNewYieldData);

	void CalculateHighLowRanges(float GeneticEffect, float& OutHighModifer, float& OutLowModifer);

	void ApplyGeneticEffect(FCropGrowthData NewGrowthData, FCropGrowthData& OutModifiedGrowthData);
};
