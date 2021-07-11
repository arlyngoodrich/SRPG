// Fill out your copyright notice in the Description page of Project Settings.


#include "CropSystem/Crop.h"
#include "LogFiles.h"

//UE4 Includes
#include "Net/UnrealNetwork.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
ACrop::ACrop()
{
	CropMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CropMesh"));
	RootComponent = CropMesh;
	SetDefaultGrowthData();
	bReplicates = true;
	CurrentHealth = SeedlingGrowthData.MaxHealth;
	HealthState = EHealthState::EHS_Healthy;
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
	DOREPLIFETIME(ACrop, CurrentHealth);
	DOREPLIFETIME(ACrop, SeedlingGrowthData);
	DOREPLIFETIME(ACrop, MiddlingGrowthData);
	DOREPLIFETIME(ACrop, MatureGrowthData);
	DOREPLIFETIME(ACrop, GeneticData);
	DOREPLIFETIME(ACrop, DaysToNextGrowthLevel);
	DOREPLIFETIME(ACrop, CurrentGrowthState);
	DOREPLIFETIME(ACrop, CurrentGrowthData);
	DOREPLIFETIME(ACrop, bRandomizeGenesAtBeginPlay);
	DOREPLIFETIME(ACrop, WaterState);
	DOREPLIFETIME(ACrop, TempState);
	DOREPLIFETIME(ACrop, FertilizerState);
	DOREPLIFETIME(ACrop, HealthState);
	
}


void ACrop::SetDefaultGrowthData()
{
	//Seedling Growth Defaults---------//
	SeedlingGrowthData.DaysToNextGrowthLevel = 5;
	SeedlingGrowthData.MaxHealth = 20;

	//Fertilizer Defaults
	SeedlingGrowthData.FertilizerConsumptionPerDay = 3;
	SeedlingGrowthData.FertilizerData.FertilizerCap = 20;
	SeedlingGrowthData.FertilizerData.TargetFertilizerZoneHigh = 15;
	SeedlingGrowthData.FertilizerData.TargetFertilzerZoneLow = 5;

	//Temp Defaults
	SeedlingGrowthData.TempData.MaxTemp = 25;
	SeedlingGrowthData.TempData.MinTemp = 0;
	SeedlingGrowthData.TempData.TargetTempZoneHigh = 20;
	SeedlingGrowthData.TempData.TargetTempZoneLow = 10;
	SeedlingGrowthData.TempData.TempDamageGene = 10;

	//Water Defaults
	SeedlingGrowthData.WaterConsumptionPerDay = 5;
	SeedlingGrowthData.WaterData.WaterCap = 25;
	SeedlingGrowthData.WaterData.MatxWater = 20;
	SeedlingGrowthData.WaterData.MinWater = 5;
	SeedlingGrowthData.WaterData.TargetWaterZoneHigh = 18;
	SeedlingGrowthData.WaterData.TargetWaterZoneLow = 6;
	SeedlingGrowthData.WaterData.WaterDamageGene = 2;

	//Middling Growth Defaults ---------//
	MiddlingGrowthData.DaysToNextGrowthLevel = 7;
	MiddlingGrowthData.MaxHealth = 50;

	//Fertilizer Defaults
	MiddlingGrowthData.FertilizerConsumptionPerDay = 5;
	MiddlingGrowthData.FertilizerData.FertilizerCap = 50;
	MiddlingGrowthData.FertilizerData.TargetFertilizerZoneHigh = 35;
	MiddlingGrowthData.FertilizerData.TargetFertilzerZoneLow = 15;

	//Temp Defaults
	MiddlingGrowthData.TempData.MaxTemp = 35;
	MiddlingGrowthData.TempData.MinTemp = 0;
	MiddlingGrowthData.TempData.TargetTempZoneHigh = 25;
	MiddlingGrowthData.TempData.TargetTempZoneLow = 10;
	MiddlingGrowthData.TempData.TempDamageGene = 10;

	//Water Defaults
	MiddlingGrowthData.WaterConsumptionPerDay = 10;
	MiddlingGrowthData.WaterData.WaterCap = 50;
	MiddlingGrowthData.WaterData.MatxWater = 40;
	MiddlingGrowthData.WaterData.MinWater = 10;
	MiddlingGrowthData.WaterData.TargetWaterZoneHigh = 30;
	MiddlingGrowthData.WaterData.TargetWaterZoneLow = 15;
	MiddlingGrowthData.WaterData.WaterDamageGene = 5;

	//Mature Growth Defaults ---------//
	MatureGrowthData.DaysToNextGrowthLevel = 7;
	MatureGrowthData.MaxHealth = 100;

	//Fertilizer Defaults
	MatureGrowthData.FertilizerConsumptionPerDay = 10;
	MatureGrowthData.FertilizerData.FertilizerCap = 100;
	MatureGrowthData.FertilizerData.TargetFertilizerZoneHigh = 75;
	MatureGrowthData.FertilizerData.TargetFertilzerZoneLow = 25;

	//Temp Defaults
	MatureGrowthData.TempData.MaxTemp = 35;
	MatureGrowthData.TempData.MinTemp = 0;
	MatureGrowthData.TempData.TargetTempZoneHigh = 25;
	MatureGrowthData.TempData.TargetTempZoneLow = 10;
	MatureGrowthData.TempData.TempDamageGene = 10;

	//Water Defaults
	MatureGrowthData.WaterConsumptionPerDay = 15;
	MatureGrowthData.WaterData.WaterCap = 100;
	MatureGrowthData.WaterData.MatxWater = 80;
	MatureGrowthData.WaterData.MinWater = 10;
	MatureGrowthData.WaterData.TargetWaterZoneHigh = 60;
	MatureGrowthData.WaterData.TargetWaterZoneLow = 20;
	MatureGrowthData.WaterData.WaterDamageGene = 5;
		
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

void ACrop::BP_AddWater(float WaterAmount)
{
	AddWater(WaterAmount);
}

void ACrop::BP_AddFertilizer(float FertilizerAmount)
{
	AddFertilizer(FertilizerAmount);
}

FCropSaveData ACrop::GetCropSaveData()
{
	FCropSaveData SaveData;
	SaveData.SaveData_CurrentFertilizer = CurrentFertilizer;
	SaveData.SaveData_CurrentGrowthState = CurrentGrowthState;
	SaveData.SaveData_CurrentHealth = CurrentHealth;
	SaveData.SaveData_CurrentWater = CurrentWater;
	SaveData.SaveData_DaysToNextGrowthLevel = DaysToNextGrowthLevel;
	SaveData.SaveData_FertilizerState = FertilizerState;
	SaveData.SaveData_WaterState = WaterState;
	SaveData.SaveData_TempState = TempState;
	SaveData.SaveData_HealthState = HealthState;
	
	return SaveData;
}

void ACrop::LoadCropSaveData(FCropSaveData SaveData, FCropGeneData SavedGeneticData)
{
	CurrentFertilizer = SaveData.SaveData_CurrentFertilizer;
	CurrentHealth = SaveData.SaveData_CurrentHealth;
	CurrentWater = SaveData.SaveData_CurrentWater;
	DaysToNextGrowthLevel = SaveData.SaveData_DaysToNextGrowthLevel;
	GeneticData = SavedGeneticData;
	FertilizerState = SaveData.SaveData_FertilizerState;
	WaterState = SaveData.SaveData_WaterState;
	TempState = SaveData.SaveData_TempState;
	HealthState = SaveData.SaveData_HealthState;

	UE_LOG(LogCropSystem, Log, TEXT("Crop Data Loaded"))

	SetGrowthStage(SaveData.SaveData_CurrentGrowthState);
}

void ACrop::BP_AdvanceDay(float PreviousDayTempaverage)
{
	AdvanceDay(PreviousDayTempaverage);
}


FCropGrowthData ACrop::GetCropData()
{
	return CurrentGrowthData;
}

FCropGeneData ACrop::GetGeneData()
{
	return GeneticData;
}

FCropGeneData ACrop::BP_GetCrossBredGeneData(FCropGeneData PartnerGeneData)
{
	FCropGeneData OutCrossbredGenes;
	GetCrossbredSeedGeneticData(PartnerGeneData, OutCrossbredGenes);

	return OutCrossbredGenes;
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

	OutGeneticData.GrowthRateGene = GetNewGene(GeneticData.GrowthRateGene, PartnerGeneData.GrowthRateGene);
	OutGeneticData.HarvestYieldGene = GetNewGene(GeneticData.HarvestYieldGene, PartnerGeneData.HarvestYieldGene);
	OutGeneticData.SeedYieldGene = GetNewGene(GeneticData.SeedYieldGene, PartnerGeneData.SeedYieldGene);
	OutGeneticData.TargetFertilizerZoneGene = GetNewGene(GeneticData.TargetFertilizerZoneGene, PartnerGeneData.TargetFertilizerZoneGene);
	OutGeneticData.TargetTempZoneGene = GetNewGene(GeneticData.TargetTempZoneGene, PartnerGeneData.TargetTempZoneGene);
	OutGeneticData.TargetWaterZoneGene = GetNewGene(GeneticData.TargetWaterZoneGene, PartnerGeneData.TargetWaterZoneGene);
	OutGeneticData.TempDamageGene = GetNewGene(GeneticData.TempDamageGene, PartnerGeneData.TempDamageGene);
	OutGeneticData.WaterDamageGene = GetNewGene(GeneticData.WaterDamageGene, PartnerGeneData.WaterDamageGene);

	UE_LOG(LogCropSystem, Log, TEXT("Calculated crossbred seed gene data"))
}

void ACrop::CreateRandomGeneDataSet(FCropGeneData& OutRandomGeneDataSet)
{
	OutRandomGeneDataSet.GrowthRateGene = CreateRandomeGene();
	OutRandomGeneDataSet.HarvestYieldGene = CreateRandomeGene();
	OutRandomGeneDataSet.SeedYieldGene = CreateRandomeGene();
	OutRandomGeneDataSet.TargetFertilizerZoneGene = CreateRandomeGene();
	OutRandomGeneDataSet.TargetTempZoneGene = CreateRandomeGene();
	OutRandomGeneDataSet.TargetWaterZoneGene = CreateRandomeGene();
	OutRandomGeneDataSet.TempDamageGene = CreateRandomeGene();
	OutRandomGeneDataSet.WaterDamageGene = CreateRandomeGene();

	UE_LOG(LogCropSystem, Log, TEXT("Calculated random gene data set"))
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
	CurrentGrowthState = NewGrowthState;
	DaysToNextGrowthLevel = ModifiedGrowthData.DaysToNextGrowthLevel;

	UE_LOG(LogCropSystem, Log, TEXT("%s moved to new %s stage"), *GetName(), *StaticEnum<EGrowthState>()->GetValueAsString(NewGrowthState))

	OnCropDataUpdate();

}

void ACrop::SetGrowthStage(EGrowthState NewGrowthState)
{
	switch (NewGrowthState)
	{
	case EGrowthState::EGS_Seedling:
		ApplyGrowthStage(SeedlingGrowthData, EGrowthState::EGS_Seedling);
		CurrentHealth = SeedlingGrowthData.MaxHealth;
		break;
	case EGrowthState::EGS_Middling:
		ApplyGrowthStage(MiddlingGrowthData, EGrowthState::EGS_Middling);
		CurrentHealth = MiddlingGrowthData.MaxHealth;
		break;
	case EGrowthState::EGS_Mature:
		ApplyGrowthStage(MatureGrowthData, EGrowthState::EGS_Mature);
		CurrentHealth = MatureGrowthData.MaxHealth;
		break;
	default:
		break;
	}

}

void ACrop::AdvanceDay(float PreviousDayAverageTemp)
{
	//Remove daily Water and Fertilizer used
	float NewWater;
	float NewFertilizer;

	NewWater = FMath::Clamp(CurrentWater - CurrentGrowthData.WaterConsumptionPerDay, 0.f, CurrentWater);
	NewFertilizer = FMath::Clamp(CurrentFertilizer - CurrentGrowthData.FertilizerConsumptionPerDay, 0.f, CurrentFertilizer);

	CurrentWater = NewWater;
	CurrentFertilizer = NewFertilizer;

	//GetTempState
	TempState = CalculateTempState(PreviousDayAverageTemp);

	//GetWaterState
	WaterState = CalculateWaterState();

	//GetFertilizerState
	FertilizerState = CalculateFertilizerState();

	//GetHealthChange
	float ChangeInHealth;
	CalculateHealthChange(ChangeInHealth);

	//Change current health
	float NewHealth;
	NewHealth = FMath::Clamp(CurrentHealth + ChangeInHealth, 0.f, CurrentGrowthData.MaxHealth);
	CurrentHealth = NewHealth;

	UE_LOG(LogCropSystem, Log, TEXT("%s health changed %s. New health is: %s"), 
		*GetName(), *FString::SanitizeFloat(ChangeInHealth), *FString::SanitizeFloat(CurrentHealth))

	//TODO die if 0 health

	//GetOverallState
	HealthState = CalculateHealthState(ChangeInHealth);

	//Remove Days to next growth stage
	if (CurrentGrowthState != EGrowthState::EGS_Mature)
	{
		if (CurrentGrowthData.DaysToNextGrowthLevel - 1 == 0)
		{
			//Advance to next growth stage
			switch (CurrentGrowthState)
			{
			case EGrowthState::EGS_Seedling:
				SetGrowthStage(EGrowthState::EGS_Middling);
				break;
			case EGrowthState::EGS_Middling:
				SetGrowthStage(EGrowthState::EGS_Mature);
				break;
			case EGrowthState::EGS_Mature:
				break;
			default:
				break;
			}
		}
		else
		{
			CurrentGrowthData.DaysToNextGrowthLevel -= 1;
			OnCropDataUpdate();
		}
	}


}

void ACrop::AddWater(float WaterAmount)
{
	float NewWater;
	NewWater = FMath::Clamp(CurrentWater + WaterAmount, 0.f, CurrentGrowthData.WaterData.WaterCap);
	CurrentWater = NewWater;

	OnWaterUpdate();

	UE_LOG(LogCropSystem,Log,TEXT("%s water added to %s.  Current water is: %s"), 
		*FString::SanitizeFloat(WaterAmount),*GetName(),*FString::SanitizeFloat(CurrentWater))

}

void ACrop::AddFertilizer(float FertilizerAmount)
{
	float NewFertilizer;
	NewFertilizer = FMath::Clamp(CurrentFertilizer + FertilizerAmount, 0.f, CurrentGrowthData.FertilizerData.FertilizerCap);
	CurrentFertilizer = NewFertilizer;

	UE_LOG(LogCropSystem, Log, TEXT("%s fertililzer added to %s.  Current fertilizer is: %s"),
		*FString::SanitizeFloat(FertilizerAmount), *GetName(), *FString::SanitizeFloat(CurrentFertilizer))

	OnFertilizerUpdate();
}


void ACrop::ModifyYieldData(float YieldModifer, TArray<FCropYieldData> CropYieldData, TArray<FCropYieldData>& OutNewYieldData)
{

	OutNewYieldData.Empty();

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
	

	OutModifiedGrowthData.DaysToNextGrowthLevel = NewGrowthData.DaysToNextGrowthLevel * CalculateGeneEffect(GeneticData.GrowthRateGene.ActiveGene);
	OutModifiedGrowthData.Scale = NewGrowthData.Scale * CalculateGeneEffect(GeneticData.GrowthRateGene.ActiveGene);

	//Update Yeilds
	ModifyYieldData(CalculateGeneEffect(GeneticData.HarvestYieldGene.ActiveGene), NewGrowthData.FallYield, OutModifiedGrowthData.FallYield);
	ModifyYieldData(CalculateGeneEffect(GeneticData.HarvestYieldGene.ActiveGene), NewGrowthData.SpringYield, OutModifiedGrowthData.SpringYield);
	ModifyYieldData(CalculateGeneEffect(GeneticData.HarvestYieldGene.ActiveGene), NewGrowthData.WinterYield, OutModifiedGrowthData.WinterYield);
	ModifyYieldData(CalculateGeneEffect(GeneticData.HarvestYieldGene.ActiveGene), NewGrowthData.SummerYield, OutModifiedGrowthData.SummerYield);
	ModifyYieldData(CalculateGeneEffect(GeneticData.SeedYieldGene.ActiveGene), NewGrowthData.SeedYield, OutModifiedGrowthData.SeedYield);


	//Fertilizer
	float FertilizerModiferHigh;
	float FertilizerModiferLow;
	CalculateHighLowRanges(CalculateGeneEffect(GeneticData.TargetFertilizerZoneGene.ActiveGene), FertilizerModiferHigh, FertilizerModiferLow);
	
	OutModifiedGrowthData.FertilizerData.TargetFertilizerZoneHigh = NewGrowthData.FertilizerData.TargetFertilizerZoneHigh * FertilizerModiferHigh;
	OutModifiedGrowthData.FertilizerData.TargetFertilzerZoneLow = NewGrowthData.FertilizerData.TargetFertilzerZoneLow * FertilizerModiferLow;

	//Water
	float WaterModiferHigh;
	float WaterModiferLow;
	CalculateHighLowRanges(CalculateGeneEffect(GeneticData.TargetWaterZoneGene.ActiveGene), WaterModiferHigh, WaterModiferLow);

	OutModifiedGrowthData.WaterData.TargetWaterZoneHigh = NewGrowthData.WaterData.TargetWaterZoneHigh * WaterModiferHigh;
	OutModifiedGrowthData.WaterData.TargetWaterZoneHigh = NewGrowthData.WaterData.TargetWaterZoneLow * WaterModiferLow;
	OutModifiedGrowthData.WaterData.WaterDamageGene = NewGrowthData.WaterData.WaterDamageGene * CalculateGeneEffect(GeneticData.WaterDamageGene.ActiveGene);

	//Temp
	float TempModiferHigh;
	float TempModiferLow;
	CalculateHighLowRanges(CalculateGeneEffect(GeneticData.TargetTempZoneGene.ActiveGene), TempModiferHigh, TempModiferLow);

	OutModifiedGrowthData.TempData.MaxTemp = NewGrowthData.TempData.MaxTemp * TempModiferHigh;
	OutModifiedGrowthData.TempData.MinTemp = NewGrowthData.TempData.MinTemp * TempModiferLow;
	OutModifiedGrowthData.TempData.TempDamageGene = NewGrowthData.TempData.TempDamageGene * CalculateGeneEffect(GeneticData.TempDamageGene.ActiveGene);

}

ETempState ACrop::CalculateTempState(float AverageTemp)
{
	ETempState OutTempState;
	OutTempState = ETempState::ETS_None;

	//If hoter then max temp
	if (AverageTemp > CurrentGrowthData.TempData.MaxTemp)
	{
		OutTempState = ETempState::ETS_ToHot;
	}
	//If colder than min temp
	else if(AverageTemp < CurrentGrowthData.TempData.MinTemp)
	{
		OutTempState = ETempState::ETS_ToCold;
	}
	//If in target zone (greater than target zone low and less than target zone high)
	else if (AverageTemp >= CurrentGrowthData.TempData.TargetTempZoneLow && AverageTemp <= CurrentGrowthData.TempData.TargetTempZoneHigh)
	{
		OutTempState = ETempState::ETS_JustRight;
	}
	else
	{
		OutTempState = ETempState::ETS_None;
	}


	return OutTempState;
}

EFertilizerState ACrop::CalculateFertilizerState()
{
	EFertilizerState OutFertilizerState;

	// If no fertilizer
	if (CurrentFertilizer == 0)
	{
		OutFertilizerState = EFertilizerState::EFS_NoFertilized;
	}
	//If fertilzer is greater than target zone low and less than target zone high
	else if (CurrentFertilizer >= CurrentGrowthData.FertilizerData.TargetFertilzerZoneLow && CurrentFertilizer <= CurrentGrowthData.FertilizerData.TargetFertilizerZoneHigh)
	{
		OutFertilizerState = EFertilizerState::EFS_PerfectFertilizer;
	}
	//Otherwise, normal fertilizer
	else
	{
		OutFertilizerState = EFertilizerState::EFS_Fertilized;
	}


	return OutFertilizerState;
}

EWaterState ACrop::CalculateWaterState()
{
	EWaterState OutWaterState;
	
	//If too much water
	if (CurrentWater > CurrentGrowthData.WaterData.MatxWater)
	{
		OutWaterState = EWaterState::EWS_OverWater;
	}
	//If not enough water
	else if(CurrentWater <= CurrentGrowthData.WaterData.MinWater)
	{
		OutWaterState = EWaterState::EWS_UnderWater;
	}
	//If water in target zone
	else if (CurrentWater >= CurrentGrowthData.WaterData.MinWater && CurrentWater < CurrentGrowthData.WaterData.MatxWater)
	{
		OutWaterState = EWaterState::EWS_PerfectWater;
	}
	//Water is not in target zone but not greater than or less than max/min
	else
	{
		OutWaterState = EWaterState::EWS_None;
	}


	return OutWaterState;
}

EHealthState ACrop::CalculateHealthState(float PreviousDayHealthChange)
{
	EHealthState OutHealthState;
	OutHealthState = EHealthState::EHS_None;

	//If no change and greater than 70% health, then healthy
	if (PreviousDayHealthChange == 0 && CurrentHealth > CurrentGrowthData.MaxHealth * .7)
	{
		OutHealthState = EHealthState::EHS_Healthy;
	}
	//If positve change and greter than 70% health, then very healthy
	else if (PreviousDayHealthChange > 0 && CurrentHealth > CurrentGrowthData.MaxHealth * .7)
	{
		OutHealthState = EHealthState::EHS_VeryHealthy;
	}
	//If neagtaive change and less than 30% health, then dying
	else if (PreviousDayHealthChange < 0 && CurrentHealth < CurrentGrowthData.MaxHealth * .3)
	{
		OutHealthState = EHealthState::EHS_Dying;
	}
	//If negative change or less than 70% health, then not healthy
	else if (PreviousDayHealthChange <= 0 || CurrentHealth <= CurrentGrowthData.MaxHealth * .7)
	{
		OutHealthState = EHealthState::EHS_NotHealthy;
	}


	return OutHealthState;
}

void ACrop::CalculateHealthChange(float& OutHealthChange)
{

	//Caculate health effect for temp 
	if (TempState == ETempState::ETS_ToCold || TempState == ETempState::ETS_ToHot)
	{
		//Subtract temp damage
		OutHealthChange -= CurrentGrowthData.TempData.TempDamageGene;
	}
	else if (TempState == ETempState::ETS_JustRight)
	{
		//Add temp damage
		OutHealthChange += CurrentGrowthData.TempData.TempDamageGene;
	}

	//Calculate health effect for fertilizer
	if (FertilizerState == EFertilizerState::EFS_Fertilized)
	{
		OutHealthChange += CurrentGrowthData.FertilizerData.FertilizerHealthEffect;
	}
	else if (FertilizerState == EFertilizerState::EFS_PerfectFertilizer)
	{
		OutHealthChange += CurrentGrowthData.FertilizerData.PerfectFertilizerHealthEffect;
	}

	//Calculate health effect for water
	if (WaterState == EWaterState::EWS_OverWater || WaterState == EWaterState::EWS_UnderWater)
	{
		OutHealthChange -= CurrentGrowthData.WaterData.WaterDamageGene;
	}
	else if (WaterState == EWaterState::EWS_PerfectWater)
	{
		OutHealthChange += CurrentGrowthData.WaterData.WaterDamageGene;
	}

}







