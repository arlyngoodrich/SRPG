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
	EWS_OverWater		UMETA(DisplayName = "Over Watered"),
	EWS_PerfectWater	UMETA(DisplayName = "Perfectly Watered"),
	EWS_UnderWater		UMETA(DisplayName = "Under Watered"),
};

UENUM(BlueprintType)
enum class EFertilizerState : uint8 {

	EFS_NoFertilized		UMETA(DisplayName = "No Fertilizer"),
	EFS_Fertilized			UMETA(DisplayName = "Fertilized"),
	EFS_PerfectFertilizer	UMETA(DisplayName = "Perfectly Fertilized"),
};


UENUM(BlueprintType)
enum class ETempState : uint8 {

	ETS_None			UMETA(DisplayName = "None"),
	ETS_ToCold			UMETA(DisplayName = "To Cold"),
	ETS_ToHot			UMETA(DisplayName = "To Hot"),
	ETS_JustRight		UMETA(DisplayName = "Just Right"),
};

UENUM(BlueprintType)
enum class EHealthState : uint8 {

	EHS_None			UMETA(DisplayName = "None"),
	EHS_Healthy			UMETA(DisplayName = "Healthy"),
	EHS_VeryHealthy		UMETA(DisplayName = "VeryHealthy"),
	EHS_NotHealthy		UMETA(DisplayName = "NotHealthy"),
	EHS_Dying			UMETA(DisplayName = "Dying"),
};


USTRUCT(BlueprintType)
struct FWaterData
{
	GENERATED_USTRUCT_BODY()
	
	//Clamps the max amount of water a plant can hold
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = " Crop Data | Water Data", meta = (ClampMin = "0.0"))
	float WaterCap;
	
	//Max water tollerance for a plant. Will cause damage if water level is above this value.  Is clamped to not exceed max water
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = " Crop Data | Water Data", meta = (ClampMin = "0.0"))
	float MatxWater;

	//Min water tollerance for a plant.  Will cause damage if water level goes below this value.  
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = " Crop Data | Water Data", meta = (ClampMin = "0.0"))
	float MinWater;

	//High value of target zone for water.  A water level in the target zone will provide a buff to growth and yeild.  
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = " Crop Data | Water Data", meta = (ClampMin = "0.0"))
	float TargetWaterZoneHigh;

	//Low value of target zone for water.  A water level in the target zone will provide a buff to growth and yeild. 
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = " Crop Data | Water Data", meta = (ClampMin = "0.0"))
	float TargetWaterZoneLow;

	//Damage per day caused by water level being above Max Water.  
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = " Crop Data | Water Data", meta = (ClampMin = "0.0"))
	float HighWaterDamage;

	//Damage per day caused by water level being below Min Water.  
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = " Crop Data | Water Data", meta = (ClampMin = "0.0"))
	float LowWaterDamage;
};

USTRUCT(BlueprintType)
struct FFertilizerData
{
	GENERATED_USTRUCT_BODY()

	//Clamps the max amount of fertilizer a plant can hold
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = " Crop Data | Fertilizer Data", meta = (ClampMin = "0.0"))
	float FertilizerCap;

	//High value of target zone for fertilizer. A fertilizer level in the target zone will provide a buff to growth and yeild
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = " Crop Data | Fertilizer Data", meta = (ClampMin = "0.0"))
	float TargetFertilizerZoneHigh;

	//Low value of target zone for fertilizer. A fertilizer level in the target zone will provide a buff to growth and yeild
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = " Crop Data | Fertilizer Data", meta = (ClampMin = "0.0"))
	float TargetFertilzerZoneLow;

};


USTRUCT(BlueprintType)
struct FTempData
{
	GENERATED_USTRUCT_BODY()

	//Max temperture tollerance for a plant. Will cause damage if temp level is above this value.  
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = " Crop Data | Temp Data", meta = (ClampMin = "0.0"))
	float MaxTemp;

	//Min temperture tollerance for a plant. Will cause damage if temp level is below this value.  
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = " Crop Data | Temp Data", meta = (ClampMin = "0.0"))
	float MinTemp;

	//High value of target zone for temperture. A temp level in the target zone will provide a buff to growth and yeild 
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = " Crop Data | Temp Data", meta = (ClampMin = "0.0"))
	float TargetTempZoneHigh;

	//Low value of target zone for temperture. A temp level in the target zone will provide a buff to growth and yeild 
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = " Crop Data | Temp Data", meta = (ClampMin = "0.0"))
	float TargetTempZoneLow;

	//Damage per day caused by temperture being above Max Temp
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = " Crop Data | Temp Data", meta = (ClampMin = "0.0"))
	float HighTempDamage;

	//Damage per day caused by temperture being below Min Temp
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = " Crop Data | Temp Data", meta = (ClampMin = "0.0"))
	float LowTempDamage;
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
	float MinYield;

	//Min amount of item possible when harvested by player
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Crop Data | Yield Data")
	float MaxYield;

};


USTRUCT(BlueprintType)
struct FCropGrowthData
{
	GENERATED_USTRUCT_BODY()

		UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Crop Data | Growth Data")
		class UStaticMesh* GrowthLevelMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Crop Data | Growth Data")
		int32 DaysToNextGrowthLevel;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Crop Data | Growth Data")
		float WaterConsumptionPerDay;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Crop Data | Growth Data")
		float MaxHealth;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Crop Data | Growth Data")
		TArray<FCropYieldData> WinterYield;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Crop Data | Growth Data")
		TArray<FCropYieldData> SpringYield;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Crop Data | Growth Data")
		TArray<FCropYieldData> SummerYield;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Crop Data | Growth Data")
		TArray<FCropYieldData> FallYield;

};
