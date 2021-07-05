// Fill out your copyright notice in the Description page of Project Settings.


#include "CropSystem/Crop.h"

//UE4 Includes
#include "Net/UnrealNetwork.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
ACrop::ACrop()
{
	CropMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CropMesh"));
	RootComponent = CropMesh;

}

// Called when the game starts or when spawned
void ACrop::BeginPlay()
{
	Super::BeginPlay();
	
}


void ACrop::GetLifetimeReplicatedProps(TArray<FLifetimeProperty >& OutLifetimeProps) const
{

	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ACrop, WaterData);
	DOREPLIFETIME(ACrop, CurrentWater);
	DOREPLIFETIME(ACrop, FertilizerData);
	DOREPLIFETIME(ACrop, CurrentFertilizer);
	DOREPLIFETIME(ACrop, TempData);
	DOREPLIFETIME(ACrop, CurrentTemp);
	DOREPLIFETIME(ACrop, CurrentHealth);
	DOREPLIFETIME(ACrop, SeedlingGrowthData);
	DOREPLIFETIME(ACrop, MiddlingGrowthData);
	DOREPLIFETIME(ACrop, MatureGrowthData);
	DOREPLIFETIME(ACrop, GeneticData);
	DOREPLIFETIME(ACrop, DaysToNextGrowthLevel);
	DOREPLIFETIME(ACrop, WaterConsumptionPerDay);
	DOREPLIFETIME(ACrop, CurrentGrowthState);
	DOREPLIFETIME(ACrop, CurrentGrowthData);
}



float ACrop::CalculateGeneEffect(EGeneType ActiveGene)
{
	switch (ActiveGene)
	{
	case EGeneType::EGT_A:
		return 0.0f;
	case EGeneType::EGT_B:
		return .9f;
	case EGeneType::EGT_C:
		return 1.1f;
	case EGeneType::EGT_D:
		return .9f;
	case EGeneType::EGT_E:
		return 1.2f;
	default:
		break;
	}

	return 0.0f;
}

EGeneType ACrop::SetActiveGene(EGeneType Gene1, EGeneType Gene2)
{
	EGeneType ActiveGene;

	if (Gene1 >= Gene2)
	{
		ActiveGene = Gene1;
	}
	else
	{
		ActiveGene = Gene2;
	}

	return ActiveGene;
}

void ACrop::ApplyGrowthStage(FCropGrowthData NewGrowthStageData, EGrowthState NewGrowthState)
{
	CropMesh->SetStaticMesh(NewGrowthStageData.GrowthLevelMesh);
	CurrentGrowthData = NewGrowthStageData;
	DaysToNextGrowthLevel = NewGrowthStageData.DaysToNextGrowthLevel;
	//TODO add UI and client notify
}

void ACrop::SetGrowthStage(EGrowthState NewGrowthState)
{
	switch (NewGrowthState)
	{
	case EGrowthState::EGS_Seedling:
		ApplyGrowthStage(SeedlingGrowthData, EGrowthState::EGS_Seedling);
		break;
	case EGrowthState::EGS_Middling:
		ApplyGrowthStage(MiddlingGrowthData, EGrowthState::EGS_Middling);
		break;
	case EGrowthState::EGS_Mature:
		ApplyGrowthStage(MatureGrowthData, EGrowthState::EGS_Mature);
		break;
	default:
		break;
	}

}

void ACrop::ModifyYieldData(float YieldModifer, TArray<FCropYieldData> CropYieldData, TArray<FCropYieldData>& OutNewYieldData)
{
	for (int32 i = 0; i < CropYieldData.Num(); i++)
	{
		FCropYieldData NewYield;
		NewYield = CropYieldData[i];

		NewYield.MaxYield = CropYieldData[i].MaxYield * YieldModifer;
		NewYield.MinYield = CropYieldData[i].MinYield * YieldModifer;

		OutNewYieldData.Add(NewYield);
	}
}

void ACrop::CalculateHighLowRanges(float GeneticEffect, float& OutHighModifer, float& OutLowModifer)
{
	OutHighModifer = GeneticEffect;
	OutLowModifer = 2 - GeneticEffect;

}

void ACrop::ApplyGeneticEffect(FCropGrowthData NewGrowthData, FCropGrowthData& OutModifiedGrowthData)
{
	OutModifiedGrowthData = NewGrowthData;

	OutModifiedGrowthData.DaysToNextGrowthLevel = NewGrowthData.DaysToNextGrowthLevel * CalculateGeneEffect(GeneticData.GrowthRate.ActiveGene);

	//Update Yeilds
	ModifyYieldData(CalculateGeneEffect(GeneticData.HarvestYield.ActiveGene), NewGrowthData.FallYield, OutModifiedGrowthData.FallYield);
	ModifyYieldData(CalculateGeneEffect(GeneticData.HarvestYield.ActiveGene), NewGrowthData.SpringYield, OutModifiedGrowthData.SpringYield);
	ModifyYieldData(CalculateGeneEffect(GeneticData.HarvestYield.ActiveGene), NewGrowthData.WinterYield, OutModifiedGrowthData.WinterYield);
	ModifyYieldData(CalculateGeneEffect(GeneticData.HarvestYield.ActiveGene), NewGrowthData.SummerYield, OutModifiedGrowthData.SummerYield);


	//Fertilizer
	float FertilizerModiferHigh;
	float FertilizerModiferLow;
	CalculateHighLowRanges(CalculateGeneEffect(GeneticData.TargetFertilizerZone.ActiveGene), FertilizerModiferHigh, FertilizerModiferLow);
	
	OutModifiedGrowthData.FertilizerData.TargetFertilizerZoneHigh = NewGrowthData.FertilizerData.TargetFertilizerZoneHigh * FertilizerModiferHigh;
	OutModifiedGrowthData.FertilizerData.TargetFertilzerZoneLow = NewGrowthData.FertilizerData.TargetFertilzerZoneLow * FertilizerModiferLow;

	//Water
	float WaterModiferHigh;
	float WaterModiferLow;
	CalculateHighLowRanges(CalculateGeneEffect(GeneticData.TargetWaterZone.ActiveGene), WaterModiferHigh, WaterModiferLow);

	OutModifiedGrowthData.WaterData.TargetWaterZoneHigh = NewGrowthData.WaterData.TargetWaterZoneHigh * WaterModiferHigh;
	OutModifiedGrowthData.WaterData.TargetWaterZoneHigh = NewGrowthData.WaterData.TargetWaterZoneLow * WaterModiferLow;
	OutModifiedGrowthData.WaterData.WaterDamage = NewGrowthData.WaterData.WaterDamage * CalculateGeneEffect(GeneticData.WaterDamage.ActiveGene);

	//Temp
	float TempModiferHigh;
	float TempModiferLow;
	CalculateHighLowRanges(CalculateGeneEffect(GeneticData.TargetTempZone.ActiveGene), TempModiferHigh, TempModiferLow);

	OutModifiedGrowthData.TempData.MaxTemp = NewGrowthData.TempData.MaxTemp * TempModiferHigh;
	OutModifiedGrowthData.TempData.MinTemp = NewGrowthData.TempData.MinTemp * TempModiferLow;
	OutModifiedGrowthData.TempData.TempDamage = NewGrowthData.TempData.TempDamage * CalculateGeneEffect(GeneticData.TempDamage.ActiveGene);

	//TODO Seed Yeild?
}






