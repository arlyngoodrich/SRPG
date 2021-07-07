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

	//Used by plant's are in the wild or part of the level design so they can have a starting gene set.  Can also be used for testing.  
	UFUNCTION(BlueprintCallable, Category = "Crop Data", meta = (DisplayName = "Get Random Gene Set"))
	FCropGeneData BP_GetRandomGeneSet();

	//Used to set the plant's genes.  Used when a plan is loaded or spawned from a seed.  
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Crop Data", meta = (DisplayName = "Set Genes"))
	void BP_SetGenes(FCropGeneData NewGenes);

	//Used to set the plant's growth stage.  Used when the plan is loaded.  
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Crop Data", meta = (DisplayName = "Set Growth Stage"))
	void BP_SetGrowhtStage(EGrowthState NewGrowthState);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Crop Data", meta = (DisplayName = "Add Water"))
	void BP_AddWater(float WaterAmount);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Crop Data", meta = (DisplayName = "Add Fertilizer"))
	void BP_AddFertilizer(float FertilizerAmount);

	//Helpter function to get Save Data struct 
	UFUNCTION(BlueprintCallable, Category = "Crop Data", meta = (DisplayName = "Get Crop Save Data"))
	FCropSaveData GetCropSaveData();

	//Helper function to set save data after load
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Crop Data", meta = (DisplayName = "Load Crop Save Data"))
	void LoadCropSaveData(FCropSaveData SaveData);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Crop Data", meta = (DisplayName = "AdvanceDay"))
	void BP_AdvanceDay(float PreviousDayTempaverage);

	//Get's the current growth data of the crop 
	UFUNCTION(BlueprintPure, Category = "Crop Data", meta = (DisplayName = "Get Growth Data"))
	FCropGrowthData GetCropData();

	//Get's the crop's gene data
	UFUNCTION(BlueprintPure, Category = "Crop Data", meta = (DisplayName = "Get Gene Data"))
	FCropGeneData GetGeneData();

	UFUNCTION(BlueprintImplementableEvent, Category = "Crop Data")
	void OnCropDataUpdate();

	UFUNCTION(BlueprintImplementableEvent, Category = "Crop Data")
	void OnWaterUpdate();

	UFUNCTION(BlueprintImplementableEvent, Category = "Crop Data")
	void OnFertilizerUpdate();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//The mesh for the crop 
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* CropMesh;
	
	//Amount of water the plant currently has stored
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Crop Data | Water Data")
	float CurrentWater;

	//Amount of fertilizer the plan current has stored
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Crop Data | Fertilizer Data")
	float CurrentFertilizer;

	//Current health of the plan.  Is caculated daily based on the previous day's Water/Temp values.  
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Crop Data | Health Data")
	float CurrentHealth;

	//The amount of days remaining for the plan to advance to the next growht stage.  Will no longer adavnce if the plant is mature.  
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Crop Data | Growth Data")
	int32 DaysToNextGrowthLevel;

	//The enumn for the plant's current growth state.  
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Crop Data | Growth Data")
	EGrowthState CurrentGrowthState;

	//Data struct for the plant's current growht state. 
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Crop Data | Crowth Data")
	FCropGrowthData CurrentGrowthData;

	//Data for the growth state when a plant is a seedling.  This is the default value and will be modified by the genes.  
	UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadOnly, Category = "Crop Data | Growth Data")
	FCropGrowthData SeedlingGrowthData;
	
	//Data for the growth state when a plant is a middling.  This is the default value and will be modified by the genes. 
	UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadOnly, Category = "Crop Data | Growth Data")
	FCropGrowthData MiddlingGrowthData;

	//Data for the growth state when a plant is mature.  This is the default value and will be modified by the genes. 
	UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadOnly, Category = "Crop Data | Growth Data")
	FCropGrowthData MatureGrowthData;

	//Configuration setting for if a plant should randomize their genes at start.  If false, the plan will expected to have it's genes set from a seed
	UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadOnly, Category = "Crop Data | Genetic Data")
	bool bRandomizeGenesAtBeginPlay = false;

	//The plant's genetic data
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Crop Data | Genetic Data")
	FCropGeneData GeneticData;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Crop Data | Plant State Data")
	EWaterState WaterState;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Crop Data | Plant State Data")
	ETempState TempState;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Crop Data | Plant State Data")
	EFertilizerState FertilizerState;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Crop Data | Plant State Data")
	EHealthState HealthState;

	//Helper function to set the default growth data in the plants constructor.  Set's defaul value for all plants.  
	void SetDefaultGrowthData();

	//Helper function to calculate the effect of a gene. Will effect all genes if edited.  
	float CalculateGeneEffect(EGeneType ActiveGene);

	//Used to calculate new genes for seeds when a plant is crossbred with another
	void GetCrossbredSeedGeneticData(FCropGeneData PartnerGeneData, FCropGeneData& OutGeneticData);

	//Native function for creating random gene set
	void CreateRandomGeneDataSet(FCropGeneData& OutRandomGeneDataSet);

	//Helper function to create a single random gene
	FGeneData CreateRandomeGene();

	//Helper function to convert an int (1-5) into a gene 
	EGeneType ConvertIntToGene(int32 GeneInt);

	//Crossbreeds two individual genes
	FGeneData GetNewGene(FGeneData GenePairA, FGeneData GenePairB);

	//Applies new growth stage to the plant's state.  Will change mesh and other variables.  
	void ApplyGrowthStage(FCropGrowthData NewGrowthStageData, EGrowthState NewGrowthState);

	//Changes Growth stage data by selecting a new growth state. Will use the plant's default growth state data.  
	void SetGrowthStage(EGrowthState NewGrowthState);

	//Called on every new day.  Will remove water and fertilzer.  
	void AdvanceDay(float PreviousDayAverageTemp);

	//Add water to the plant's current water storage.  
	void AddWater(float WaterAmount);

	//Add fertilizer to the plant's current fertilizer storage
	void AddFertilizer(float FertilizerAmount);

	//Apply's genetic effect to harvest and seed yields
	void ModifyYieldData(float YieldModifer, float SeedYieldModifer, TArray<FCropYieldData> CropYieldData, TArray<FCropYieldData>& OutNewYieldData);

	//Helper function to convert genetic effect to the high and low ranges for target zones
	void CalculateHighLowRanges(float GeneticEffect, float& OutHighModifer, float& OutLowModifer);

	//Applies the genetic effect to growth data 
	void ApplyGeneticEffect(FCropGrowthData NewGrowthData, FCropGrowthData& OutModifiedGrowthData);

	ETempState CalculateTempState(float AverageTemp);

	EFertilizerState CalculateFertilizerState();

	EWaterState CalculateWaterState();

	//Takes current plant states and caculates health effect
	void CalculateHealthChange(float& OutHealthChange);

	EHealthState CalculateHealthState(float PreviousDayHealthChange);


};
