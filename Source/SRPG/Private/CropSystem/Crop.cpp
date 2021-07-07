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
	SetDefaultGrowthData();
}





// Called when the game starts or when spawned
void ACrop::BeginPlay()
{
	Super::BeginPlay();

	if (bRandomizeGenesAtBeginPlay)
	{
		CreateRandomGeneDataSet(GeneticData);
	}

	SetGrowthStage(EGrowthState::EGS_Seedling);
}


void ACrop::GetLifetimeReplicatedProps(TArray<FLifetimeProperty >& OutLifetimeProps) const
{

	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ACrop, CurrentWater);
	DOREPLIFETIME(ACrop, CurrentFertilizer);
	DOREPLIFETIME(ACrop, CurrentTemp);
	DOREPLIFETIME(ACrop, CurrentHealth);
	DOREPLIFETIME(ACrop, SeedlingGrowthData);
	DOREPLIFETIME(ACrop, MiddlingGrowthData);
	DOREPLIFETIME(ACrop, MatureGrowthData);
	DOREPLIFETIME(ACrop, GeneticData);
	DOREPLIFETIME(ACrop, DaysToNextGrowthLevel);
	DOREPLIFETIME(ACrop, CurrentGrowthState);
	DOREPLIFETIME(ACrop, CurrentGrowthData);
	DOREPLIFETIME(ACrop, bRandomizeGenesAtBeginPlay);
}



void ACrop::SetDefaultGrowthData()
{
	//Seedling Growth Defaults---------//
	SeedlingGrowthData.DaysToNextGrowthLevel = 5;
	SeedlingGrowthData.MaxHealth = 20;

	//Fertilizer Defaults
	SeedlingGrowthData.FertilizerData.FertilizerCap = 20;
	SeedlingGrowthData.FertilizerData.TargetFertilizerZoneHigh = 15;
	SeedlingGrowthData.FertilizerData.TargetFertilzerZoneLow = 5;

	//Temp Defaults
	SeedlingGrowthData.TempData.MaxTemp = 25;
	SeedlingGrowthData.TempData.MinTemp = 0;
	SeedlingGrowthData.TempData.TargetTempZoneHigh = 20;
	SeedlingGrowthData.TempData.TargetTempZoneLow = 10;
	SeedlingGrowthData.TempData.TempDamage = 10;

	//Water Defaults
	SeedlingGrowthData.WaterConsumptionPerDay = 5;
	SeedlingGrowthData.WaterData.WaterCap = 25;
	SeedlingGrowthData.WaterData.MatxWater = 20;
	SeedlingGrowthData.WaterData.MinWater = 5;
	SeedlingGrowthData.WaterData.TargetWaterZoneHigh = 18;
	SeedlingGrowthData.WaterData.TargetWaterZoneLow = 6;
	SeedlingGrowthData.WaterData.WaterDamage = 2;

	//Middling Growth Defaults ---------//
	MiddlingGrowthData.DaysToNextGrowthLevel = 7;
	MiddlingGrowthData.MaxHealth = 50;

	//Fertilizer Defaults
	MiddlingGrowthData.FertilizerData.FertilizerCap = 50;
	MiddlingGrowthData.FertilizerData.TargetFertilizerZoneHigh = 35;
	MiddlingGrowthData.FertilizerData.TargetFertilzerZoneLow = 15;

	//Temp Defaults
	MiddlingGrowthData.TempData.MaxTemp = 35;
	MiddlingGrowthData.TempData.MinTemp = 0;
	MiddlingGrowthData.TempData.TargetTempZoneHigh = 25;
	MiddlingGrowthData.TempData.TargetTempZoneLow = 10;
	MiddlingGrowthData.TempData.TempDamage = 10;

	//Water Defaults
	MiddlingGrowthData.WaterConsumptionPerDay = 10;
	MiddlingGrowthData.WaterData.WaterCap = 50;
	MiddlingGrowthData.WaterData.MatxWater = 40;
	MiddlingGrowthData.WaterData.MinWater = 10;
	MiddlingGrowthData.WaterData.TargetWaterZoneHigh = 30;
	MiddlingGrowthData.WaterData.TargetWaterZoneLow = 15;
	MiddlingGrowthData.WaterData.WaterDamage = 5;

	//Mature Growth Defaults ---------//
	MatureGrowthData.DaysToNextGrowthLevel = 7;
	MatureGrowthData.MaxHealth = 100;

	//Fertilizer Defaults
	MatureGrowthData.FertilizerData.FertilizerCap = 100;
	MatureGrowthData.FertilizerData.TargetFertilizerZoneHigh = 75;
	MatureGrowthData.FertilizerData.TargetFertilzerZoneLow = 25;

	//Temp Defaults
	MatureGrowthData.TempData.MaxTemp = 35;
	MatureGrowthData.TempData.MinTemp = 0;
	MatureGrowthData.TempData.TargetTempZoneHigh = 25;
	MatureGrowthData.TempData.TargetTempZoneLow = 10;
	MatureGrowthData.TempData.TempDamage = 10;

	//Water Defaults
	MatureGrowthData.WaterConsumptionPerDay = 15;
	MatureGrowthData.WaterData.WaterCap = 100;
	MatureGrowthData.WaterData.MatxWater = 80;
	MatureGrowthData.WaterData.MinWater = 10;
	MatureGrowthData.WaterData.TargetWaterZoneHigh = 60;
	MatureGrowthData.WaterData.TargetWaterZoneLow = 20;
	MatureGrowthData.WaterData.WaterDamage = 5;

}

FCropGeneData ACrop::BP_GetRandomGeneSet()
{
	FCropGeneData OutGeneData;
	CreateRandomGeneDataSet(OutGeneData);

	return OutGeneData;
}

void ACrop::BP_SetGenes(FCropGeneData NewGenes)
{
	GeneticData = NewGenes;
}

void ACrop::BP_SetGrowhtStage(EGrowthState NewGrowthState)
{
	SetGrowthStage(NewGrowthState);
}

FCropSaveData ACrop::GetCropSaveData()
{
	FCropSaveData SaveData;
	SaveData.SaveData_CurrentFertilizer = CurrentFertilizer;
	SaveData.SaveData_CurrentGrowthState = CurrentGrowthState;
	SaveData.SaveData_CurrentGrowthStateData = CurrentGrowthData;
	SaveData.SaveData_CurrentHealth = CurrentHealth;
	SaveData.SaveData_CurrentTemp = CurrentTemp;
	SaveData.SaveData_CurrentWater = CurrentWater;
	SaveData.SaveData_DaysToNextGrowthLevel = DaysToNextGrowthLevel;
	SaveData.SaveData_GeneticData = GeneticData;

	return SaveData;
}

void ACrop::LoadCropSaveData(FCropSaveData SaveData)
{
	CurrentFertilizer = SaveData.SaveData_CurrentFertilizer;
	CurrentHealth = SaveData.SaveData_CurrentHealth;
	CurrentTemp = SaveData.SaveData_CurrentTemp;
	CurrentWater = SaveData.SaveData_CurrentWater;
	DaysToNextGrowthLevel = SaveData.SaveData_DaysToNextGrowthLevel;
	GeneticData = SaveData.SaveData_GeneticData;

	ApplyGrowthStage(SaveData.SaveData_CurrentGrowthStateData, SaveData.SaveData_CurrentGrowthState);
}

FCropGrowthData ACrop::GetCropData()
{
	return CurrentGrowthData;
}

FCropGeneData ACrop::GetGeneData()
{
	return GeneticData;
}

float ACrop::CalculateGeneEffect(EGeneType ActiveGene)
{
	switch (ActiveGene)
	{
	case EGeneType::EGT_A:
		return .8f;
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



void ACrop::GetCrossbredSeedGeneticData(FCropGeneData PartnerGeneData, FCropGeneData& OutGeneticData)
{
	OutGeneticData = GeneticData;

	OutGeneticData.GrowthRate = GetNewGene(GeneticData.GrowthRate, PartnerGeneData.GrowthRate);
	OutGeneticData.HarvestYield = GetNewGene(GeneticData.HarvestYield, PartnerGeneData.HarvestYield);
	OutGeneticData.SeedYield = GetNewGene(GeneticData.SeedYield, PartnerGeneData.SeedYield);
	OutGeneticData.TargetFertilizerZone = GetNewGene(GeneticData.TargetFertilizerZone, PartnerGeneData.TargetFertilizerZone);
	OutGeneticData.TargetTempZone = GetNewGene(GeneticData.TargetTempZone, PartnerGeneData.TargetTempZone);
	OutGeneticData.TargetWaterZone = GetNewGene(GeneticData.TargetWaterZone, PartnerGeneData.TargetWaterZone);
	OutGeneticData.TempDamage = GetNewGene(GeneticData.TempDamage, PartnerGeneData.TempDamage);
	OutGeneticData.WaterDamage = GetNewGene(GeneticData.WaterDamage, PartnerGeneData.WaterDamage);
}

void ACrop::CreateRandomGeneDataSet(FCropGeneData& OutRandomGeneDataSet)
{
	OutRandomGeneDataSet.GrowthRate = CreateRandomeGene();
	OutRandomGeneDataSet.HarvestYield = CreateRandomeGene();
	OutRandomGeneDataSet.SeedYield = CreateRandomeGene();
	OutRandomGeneDataSet.TargetFertilizerZone = CreateRandomeGene();
	OutRandomGeneDataSet.TargetTempZone = CreateRandomeGene();
	OutRandomGeneDataSet.TargetWaterZone = CreateRandomeGene();
	OutRandomGeneDataSet.TempDamage = CreateRandomeGene();
	OutRandomGeneDataSet.WaterDamage = CreateRandomeGene();
}

FGeneData ACrop::CreateRandomeGene()
{

	int32 RandomGeneType1;
	int32 RandomeGeneType2;

	FGeneData OutRandomeGene;

	RandomGeneType1 = FMath::RandRange(1, 5);
	RandomeGeneType2 = FMath::RandRange(1, 5);

	if (RandomGeneType1 >= RandomeGeneType2)
	{
		OutRandomeGene.ActiveGene = ConvertIntToGene(RandomeGeneType2);
		OutRandomeGene.RecessiveGene = ConvertIntToGene(RandomGeneType1);
	}
	else
	{
		OutRandomeGene.ActiveGene = ConvertIntToGene(RandomGeneType1);
		OutRandomeGene.RecessiveGene = ConvertIntToGene(RandomeGeneType2);
	}

	return OutRandomeGene;
}

EGeneType ACrop::ConvertIntToGene(int32 GeneInt)
{
	EGeneType GeneType;
	GeneType = EGeneType::EGT_A;

	switch (GeneInt)
	{
		case(1):
			GeneType = EGeneType::EGT_A;
			break;
		case(2):
			GeneType = EGeneType::EGT_B;
			break;
		case(3):
			GeneType = EGeneType::EGT_C;
			break;
		case(4):
			GeneType = EGeneType::EGT_D;
			break;
		case(5):
			GeneType = EGeneType::EGT_E;
			break;
	default:
		break;
	}

	return GeneType;
}

FGeneData ACrop::GetNewGene(FGeneData GenePairA, FGeneData GenePairB)
{

	EGeneType GenePairA_Dominate;
	EGeneType GenePairA_Recessive;

	EGeneType GenePairB_Dominate;
	EGeneType GenePairB_Recessive;

	FGeneData NewGene;
	NewGene = GenePairA;

	GenePairA_Dominate = GenePairA.ActiveGene;
	GenePairA_Recessive = GenePairA.RecessiveGene;

	GenePairB_Dominate = GenePairB.ActiveGene;
	GenePairB_Recessive = GenePairB.RecessiveGene;

	int32 i = FMath::RandRange(1,4);

	switch (i)
	{
	case(1):

		if (GenePairA_Dominate >= GenePairB_Dominate)
		{
			NewGene.ActiveGene = GenePairB_Dominate;
			NewGene.RecessiveGene = GenePairA_Dominate;
		}
		else
		{
			NewGene.ActiveGene = GenePairA_Dominate;
			NewGene.RecessiveGene = GenePairB_Dominate;
		}

		break;
	case(2):

		if (GenePairA_Dominate >= GenePairB_Recessive)
		{
			NewGene.ActiveGene = GenePairB_Recessive;
			NewGene.RecessiveGene = GenePairA_Dominate;
		}
		else
		{
			NewGene.ActiveGene = GenePairA_Dominate;
			NewGene.RecessiveGene = GenePairB_Recessive;
		}

		break;
	case(3):

		if (GenePairA_Recessive >= GenePairB_Dominate)
		{
			NewGene.ActiveGene = GenePairB_Dominate;
			NewGene.RecessiveGene = GenePairA_Recessive;
		}
		else
		{
			NewGene.ActiveGene = GenePairA_Recessive;
			NewGene.RecessiveGene = GenePairB_Dominate;
		}


		break;
	case(4):

		if (GenePairA_Recessive >= GenePairB_Recessive)
		{
			NewGene.ActiveGene = GenePairB_Recessive;
			NewGene.RecessiveGene = GenePairA_Recessive;
		}
		else
		{
			NewGene.ActiveGene = GenePairA_Recessive;
			NewGene.RecessiveGene = GenePairB_Recessive;
		}

		break;
	default:
		break;
	}
	

	return NewGene;
}



void ACrop::ApplyGrowthStage(FCropGrowthData NewGrowthStageData, EGrowthState NewGrowthState)
{

	FCropGrowthData ModifiedGrowthData;
	ModifiedGrowthData = NewGrowthStageData;

	ApplyGeneticEffect(NewGrowthStageData, ModifiedGrowthData);

	CropMesh->SetStaticMesh(ModifiedGrowthData.GrowthLevelMesh);
	CropMesh->SetWorldScale3D(ModifiedGrowthData.Scale);
	CurrentGrowthData = ModifiedGrowthData;
	DaysToNextGrowthLevel = ModifiedGrowthData.DaysToNextGrowthLevel;
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

void ACrop::ModifyYieldData(float YieldModifer, float SeedYieldModifer, TArray<FCropYieldData> CropYieldData, TArray<FCropYieldData>& OutNewYieldData)
{
	for (int32 i = 0; i < CropYieldData.Num(); i++)
	{
		FCropYieldData NewYield;
		NewYield = CropYieldData[i];

		NewYield.MaxYield = CropYieldData[i].MaxYield * YieldModifer;
		NewYield.MinYield = CropYieldData[i].MinYield * YieldModifer;
		NewYield.SeedYield = CropYieldData[i].SeedYield * SeedYieldModifer;

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
	OutModifiedGrowthData.Scale = NewGrowthData.Scale * CalculateGeneEffect(GeneticData.GrowthRate.ActiveGene);

	//Update Yeilds
	ModifyYieldData(CalculateGeneEffect(GeneticData.HarvestYield.ActiveGene), CalculateGeneEffect(GeneticData.SeedYield.ActiveGene), NewGrowthData.FallYield, OutModifiedGrowthData.FallYield);
	ModifyYieldData(CalculateGeneEffect(GeneticData.HarvestYield.ActiveGene), CalculateGeneEffect(GeneticData.SeedYield.ActiveGene), NewGrowthData.SpringYield, OutModifiedGrowthData.SpringYield);
	ModifyYieldData(CalculateGeneEffect(GeneticData.HarvestYield.ActiveGene), CalculateGeneEffect(GeneticData.SeedYield.ActiveGene), NewGrowthData.WinterYield, OutModifiedGrowthData.WinterYield);
	ModifyYieldData(CalculateGeneEffect(GeneticData.HarvestYield.ActiveGene), CalculateGeneEffect(GeneticData.SeedYield.ActiveGene), NewGrowthData.SummerYield, OutModifiedGrowthData.SummerYield);


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

}






