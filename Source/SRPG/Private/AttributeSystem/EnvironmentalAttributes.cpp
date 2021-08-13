// Fill out your copyright notice in the Description page of Project Settings.


#include "AttributeSystem/EnvironmentalAttributes.h"
#include "LogFiles.h"

//UE4 Includes
#include "Net/UnrealNetwork.h"

UEnvironmentalAttributes::UEnvironmentalAttributes()
{

}

void UEnvironmentalAttributes::AdjustHotResitance(float Adjustment)
{
}

void UEnvironmentalAttributes::AdjustColdResitance(float Adjustment)
{
}

void UEnvironmentalAttributes::AdjustWetnessResitance(float Adjustment)
{
}

void UEnvironmentalAttributes::BeginPlay()
{
	Super::BeginPlay();
}


void UEnvironmentalAttributes::GetLifetimeReplicatedProps(TArray<FLifetimeProperty >& OutLifetimeProps) const
{

	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UEnvironmentalAttributes, CurrentTemperature);
	DOREPLIFETIME(UEnvironmentalAttributes, CurrentWetness);
	DOREPLIFETIME(UEnvironmentalAttributes, TargetTemperature);
	DOREPLIFETIME(UEnvironmentalAttributes, HeatResistance);
	DOREPLIFETIME(UEnvironmentalAttributes, ColdResistance);
	DOREPLIFETIME(UEnvironmentalAttributes, TargetWetness);
	DOREPLIFETIME(UEnvironmentalAttributes, WetnessResistance);
	DOREPLIFETIME(UEnvironmentalAttributes, CurrentTempState);
	DOREPLIFETIME(UEnvironmentalAttributes, CurrentWetnessState);

}


void UEnvironmentalAttributes::SampleEnviornment_Implementation(float SampledTemperature, float SampledWetness)
{
	if (GetOwnerRole() != ROLE_Authority)
	{
		UE_LOG(LogAttributeSystem, Error, TEXT("%s: Cannot sample enviornment from client"), *GetOwner()->GetName())
			return;
	}


	Internal_SampleEnviornment(SampledTemperature, SampledWetness);
}

void UEnvironmentalAttributes::Internal_SampleEnviornment(float SampledTemperature, float SampledWetness)
{
	if (GetOwnerRole() != ROLE_Authority)
	{
		UE_LOG(LogAttributeSystem, Error, TEXT("%s: Cannot sample enviornment from client"), *GetOwner()->GetName())
			return;
	}

	UE_LOG(LogAttributeSystem,Log,TEXT("%s sampled enviornment"), *GetOwner()->GetName())
}

void UEnvironmentalAttributes::AdjustTemperature(float SampledTemperature)
{
	if (GetOwnerRole() != ROLE_Authority)
	{
		UE_LOG(LogAttributeSystem, Error, TEXT("%s: Cannot adjsut temperature from client"), *GetOwner()->GetName())
			return;
	}

	TargetTemperature = SampledTemperature;
	
	//If current temp and target temp are the same, no need to do anything
	if (CurrentTemperature == TargetTemperature)
	{
		return;
	}

	//If target temp and current temp are less than 1 degree apart, auto set current to target. 
	if (FMath::Abs(CurrentTemperature - TargetTemperature) <= 1.f)
	{
		CurrentTemperature = TargetTemperature;
		UE_LOG(LogAttributeSystem,Log,TEXT("Temp auto set. Current Temp = %s"),*FString::SanitizeFloat(CurrentTemperature))
		return;
	}

	float TempDiff;
	TempDiff = TargetTemperature - CurrentTemperature;

	float ActiveResistance;
	if (TempDiff > 0)
	{
		ActiveResistance = HeatResistance;
	}
	else
	{
		ActiveResistance = ColdResistance;
	}

	float TempToAdjust;
	TempToAdjust = TempDiff - (TempDiff * ActiveResistance);
	
	float OldTemp;
	OldTemp = CurrentTemperature;

	CurrentTemperature += TempToAdjust;

	UE_LOG(LogAttributeSystem,Log,TEXT("%s Temp Adjustment. Adjustment Temp = %s | Current Temp = %s"),
		*FString::SanitizeFloat(TempToAdjust),
		*FString::SanitizeFloat(CurrentTemperature)
	)

}

void UEnvironmentalAttributes::AdjustWetness(float SampledWetness)
{
	if (GetOwnerRole() != ROLE_Authority)
	{
		UE_LOG(LogAttributeSystem, Error, TEXT("%s: Cannot adjsut wetness from client"), *GetOwner()->GetName())
			return;
	}

	TargetWetness = SampledWetness;

	//If the same, then return
	if (TargetWetness == CurrentWetness)
	{
		return;
	}

	if (FMath::Abs(TargetWetness - CurrentWetness) <= 1)
	{
		CurrentWetness = TargetWetness;
		UE_LOG(LogAttributeSystem,Log,TEXT("Wetness auto set. Current wetness = %s"),*FString::SanitizeFloat(CurrentWetness))
		return;
	}
	
	float WetnessDiff;
	WetnessDiff = TargetWetness - CurrentWetness;

	float WetnessAdjust;
	WetnessAdjust = WetnessDiff - (WetnessDiff * WetnessResistance);

	CurrentWetness += WetnessAdjust;

	UE_LOG(LogAttributeSystem, Log, TEXT("%s Wetness Adjustment. Wetness Adjustment = %s | Current Wetness = %s"),
		*FString::SanitizeFloat(WetnessAdjust),
		*FString::SanitizeFloat(CurrentWetness)
	)

}

void UEnvironmentalAttributes::SetTempState()
{
	if (GetOwnerRole() != ROLE_Authority)
	{
		UE_LOG(LogAttributeSystem, Error, TEXT("%s: Cannot adjsut temp state from client"), *GetOwner()->GetName())
			return;
	}

	ETemperatureState TestTempState;

	//If freezing
	if (CurrentTemperature < BaseColdTemp)
	{
		TestTempState = ETemperatureState::ETS_Freezing;
	}
	//If cold
	else if (CurrentTemperature >= BaseColdTemp && CurrentTemperature < BaseComfortableTemp)
	{
		TestTempState = ETemperatureState::ETS_Cold;
	}
	//If comfortable
	else if (CurrentTemperature >= BaseComfortableTemp && CurrentTemperature < BaseHotTemp)
	{
		TestTempState = ETemperatureState::ETS_Comfortable;
	}
	//If Hot
	else if (CurrentTemperature >= BaseHotTemp && CurrentTemperature < BaseScortchingTemp)
	{
		TestTempState = ETemperatureState::ETS_Hot;
	}
	//If Scortching
	else if (CurrentTemperature >= BaseScortchingTemp)
	{
		TestTempState = ETemperatureState::ETS_Scortching;
	}

	//See if new temp state
	if (CurrentTempState != TestTempState)
	{
		CurrentTempState = TestTempState;
		FString NewTempStateString = UEnum::GetValueAsString<ETemperatureState>(CurrentTempState);
		UE_LOG(LogAttributeSystem, Log, TEXT("%s: new temp state: "), *GetOwner()->GetName(), *NewTempStateString);
	}


}

void UEnvironmentalAttributes::SetWetnessState()
{
	if (GetOwnerRole() != ROLE_Authority)
	{
		UE_LOG(LogAttributeSystem, Error, TEXT("%s: Cannot adjsut wetness state from client"), *GetOwner()->GetName())
			return;
	}
}

