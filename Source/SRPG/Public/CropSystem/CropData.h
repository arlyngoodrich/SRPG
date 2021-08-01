// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "CropData.generated.h"

UENUM(BlueprintType)
enum class EGrowthState : uint8 {

	EGS_Seedling	UMETA(DisplayName = "Seedling"),
	EGS_Middling	UMETA(DisplayName = "Middling"),
	EGS_Mature		UMETA(DisplayName = "Mature"),
};


UENUM(BlueprintType)
enum class EWaterState : uint8 {

	EWS_None			UMETA(DisplayName = "None"),
	EWS_PerfectWater	UMETA(DisplayName = "Perfectly Watered"),
	EWS_OverWater		UMETA(DisplayName = "Over Watered"),
	EWS_UnderWater		UMETA(DisplayName = "Under Watered"),
};

UENUM(BlueprintType)
enum class EFertilizerState : uint8 {

	EFS_NoFertilized		UMETA(DisplayName = "No Fertilizer"),
	EFS_PerfectFertilizer	UMETA(DisplayName = "Perfectly Fertilized"),
	EFS_Fertilized			UMETA(DisplayName = "Fertilized"),
};


UENUM(BlueprintType)
enum class ETempState : uint8 {

	ETS_None			UMETA(DisplayName = "None"),
	ETS_JustRight		UMETA(DisplayName = "Just Right"),
	ETS_ToCold			UMETA(DisplayName = "To Cold"),
	ETS_ToHot			UMETA(DisplayName = "To Hot"),
};

UENUM(BlueprintType)
enum class EHealthState : uint8 {

	EHS_None			UMETA(DisplayName = "None"),
	EHS_Healthy			UMETA(DisplayName = "Healthy"),
	EHS_VeryHealthy		UMETA(DisplayName = "VeryHealthy"),
	EHS_NotHealthy		UMETA(DisplayName = "NotHealthy"),
	EHS_Dying			UMETA(DisplayName = "Dying"),
};


UENUM(BlueprintType)
enum class EGeneType : uint8 {

	EGT_A		UMETA(DisplayName = "A"),
	EGT_B		UMETA(DisplayName = "B"),
	EGT_C		UMETA(DisplayName = "C"),
	EGT_D		UMETA(DisplayName = "D"),
	EGT_E		UMETA(DisplayName = "E"),
};


USTRUCT(BlueprintType)
struct FWaterData
{
	GENERATED_USTRUCT_BODY()
	
	//Clamps the max amount of water a plant can hold
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = " Crop Data | Water Data", meta = (ClampMin = "0.0"))
	float WaterCap = 100;
	
	//Max water tollerance for a plant. Will cause damage if water level is above this value.  Is clamped to not exceed max water
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = " Crop Data | Water Data", meta = (ClampMin = "0.0"))
	float MatxWater = 90;

	//Min water tollerance for a plant.  Will cause damage if water level goes below this value.  
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = " Crop Data | Water Data", meta = (ClampMin = "0.0"))
	float MinWater = 20;

	//High value of target zone for water.  A water level in the target zone will provide a buff to growth and yeild.  
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = " Crop Data | Water Data", meta = (ClampMin = "0.0"))
	float TargetWaterZoneHigh= 80;

	//Low value of target zone for water.  A water level in the target zone will provide a buff to growth and yeild. 
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = " Crop Data | Water Data", meta = (ClampMin = "0.0"))
	float TargetWaterZoneLow = 40;

	//Damage per day caused by water level being above Max Water or below Min Water. 
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = " Crop Data | Water Data", meta = (ClampMin = "0.0"))
	float WaterDamageGene = 20;

};

USTRUCT(BlueprintType)
struct FFertilizerData
{
	GENERATED_USTRUCT_BODY()

	//Clamps the max amount of fertilizer a plant can hold
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = " Crop Data | Fertilizer Data", meta = (ClampMin = "0.0"))
	float FertilizerCap = 100;

	//High value of target zone for fertilizer. A fertilizer level in the target zone will provide a buff to growth and yeild
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = " Crop Data | Fertilizer Data", meta = (ClampMin = "0.0"))
	float TargetFertilizerZoneHigh = 90;

	//Low value of target zone for fertilizer. A fertilizer level in the target zone will provide a buff to growth and yeild
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = " Crop Data | Fertilizer Data", meta = (ClampMin = "0.0"))
	float TargetFertilzerZoneLow = 50;

	//Health effect for when a plant is fertilized
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = " Crop Data | Fertilizer Data", meta = (ClampMin = "0.0"))
	float FertilizerHealthEffect = 1.f;

	//Health effect for when a plant is fertilized
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = " Crop Data | Fertilizer Data", meta = (ClampMin = "0.0"))
	float PerfectFertilizerHealthEffect = 2.f;

};


USTRUCT(BlueprintType)
struct FTempData
{
	GENERATED_USTRUCT_BODY()

	//Max temperture tollerance for a plant. Will cause damage if temp level is above this value.  
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = " Crop Data | Temp Data", meta = (ClampMin = "0.0"))
	float MaxTemp = 35;

	//Min temperture tollerance for a plant. Will cause damage if temp level is below this value.  
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = " Crop Data | Temp Data", meta = (ClampMin = "0.0"))
	float MinTemp = 0;

	//High value of target zone for temperture. A temp level in the target zone will provide a buff to growth and yeild 
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = " Crop Data | Temp Data", meta = (ClampMin = "0.0"))
	float TargetTempZoneHigh = 30;

	//Low value of target zone for temperture. A temp level in the target zone will provide a buff to growth and yeild 
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = " Crop Data | Temp Data", meta = (ClampMin = "0.0"))
	float TargetTempZoneLow = 20;

	//Damage per day caused by temperture being above Max Temp or below Min Temp.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = " Crop Data | Temp Data", meta = (ClampMin = "0.0"))
	float TempDamageGene = 20;

};



USTRUCT(BlueprintType)
struct FCropYieldData
{
	GENERATED_USTRUCT_BODY()

	//Item that will be created when harvested by player
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Crop Data | Yield Data")
	TSubclassOf <class AItemBase> HarvestedItem;

	//Max amount of item possible when harvested by player
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Crop Data | Yield Data")
	int32 MinYield;

	//Min amount of item possible when harvested by player
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Crop Data | Yield Data")
	int32 MaxYield;

};


USTRUCT(BlueprintType)
struct FCropGrowthData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Crop Data | Growth Data")
	class UStaticMesh* GrowthLevelMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Crop Data | Growth Data")
	FVector Scale = FVector(1,1,1);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Crop Data | Growth Data")
	int32 DaysToNextGrowthLevel = 7;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Crop Data | Growth Data")
	float WaterConsumptionPerDay = 5;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Crop Data | Growth Data")
	float FertilizerConsumptionPerDay = 5;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Crop Data | Growth Data")
	float MaxHealth = 100;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Crop Data | Growth Data")
	FWaterData WaterData;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Crop Data | Growth Data")
	FFertilizerData FertilizerData;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Crop Data | Growth Data")
	FTempData TempData;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Crop Data | Growth Data")
	TArray<FCropYieldData> WinterYield;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Crop Data | Growth Data")
	TArray<FCropYieldData> SpringYield;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Crop Data | Growth Data")
	TArray<FCropYieldData> SummerYield;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Crop Data | Growth Data")
	TArray<FCropYieldData> FallYield;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Crop Data | Growth Data")
	TArray<FCropYieldData> SeedYield;


};


USTRUCT(BlueprintType)
struct FGeneData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Crop Data | Gene Data")
	EGeneType ActiveGene;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Crop Data | Gene Data")
	EGeneType RecessiveGene;

};



USTRUCT(BlueprintType)
struct FCropGeneData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Crop Data | Gene Data")
	FGeneData TargetWaterZoneGene;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Crop Data | Gene Data")
	FGeneData WaterDamageGene;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Crop Data | Gene Data")
	FGeneData TargetFertilizerZoneGene;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Crop Data | Gene Data")
	FGeneData TargetTempZoneGene;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Crop Data | Gene Data")
	FGeneData TempDamageGene;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Crop Data | Gene Data")
	FGeneData HarvestYieldGene;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Crop Data | Gene Data")
	FGeneData SeedYieldGene;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Crop Data | Gene Data")
	FGeneData GrowthRateGene;

};



USTRUCT(BlueprintType)
struct FCropSaveData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(BlueprintReadOnly, Category = "Crop Data | Save Data")
	float SaveData_CurrentWater;

	UPROPERTY(BlueprintReadOnly, Category = "Crop Data | Save Data")
	float SaveData_CurrentFertilizer;

	UPROPERTY(BlueprintReadOnly, Category = "Crop Data | Save Data")
	float SaveData_CurrentHealth;

	UPROPERTY(BlueprintReadOnly, Category = "Crop Data | Save Data")
	int32 SaveData_DaysToNextGrowthLevel;

	UPROPERTY(BlueprintReadOnly, Category = "Crop Data | Save Data")
	EGrowthState SaveData_CurrentGrowthState;

	UPROPERTY(BlueprintReadOnly, Category = "Crop Data | Save Data")
	EWaterState SaveData_WaterState;

	UPROPERTY(BlueprintReadOnly, Category = "Crop Data | Save Data")
	ETempState SaveData_TempState;

	UPROPERTY(BlueprintReadOnly, Category = "Crop Data | Save Data")
	EFertilizerState SaveData_FertilizerState;

	UPROPERTY(BlueprintReadOnly, Category = "Crop Data | Save Data")
	EHealthState SaveData_HealthState;

};

