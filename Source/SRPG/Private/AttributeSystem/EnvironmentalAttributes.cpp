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
	if (GetOwnerRole() != ROLE_Authority)
	{
		UE_LOG(LogAttributeSystem, Error, TEXT("%s: Cannot asjust resistance from client"), *GetOwner()->GetName())
			return;
	}

	HeatResistance = FMath::Clamp(HeatResistance + Adjustment, 0.f, 0.9f);
	UE_LOG(LogAttributeSystem,Log,TEXT("%s: Heat Resistance Adjusted to %s"), *GetOwner()->GetName(),*FString::SanitizeFloat(HeatResistance))
}

void UEnvironmentalAttributes::AdjustColdResitance(float Adjustment)
{
	if (GetOwnerRole() != ROLE_Authority)
	{
		UE_LOG(LogAttributeSystem, Error, TEXT("%s: Cannot asjust resistance from client"), *GetOwner()->GetName())
			return;
	}

	ColdResistance = FMath::Clamp(ColdResistance + Adjustment, 0.f, 0.9f);
	UE_LOG(LogAttributeSystem, Log, TEXT("%s: Cold Resistance Adjusted to %s"), *GetOwner()->GetName(), *FString::SanitizeFloat(ColdResistance))
		
}

void UEnvironmentalAttributes::AdjustWetnessResitance(float Adjustment)
{
	if (GetOwnerRole() != ROLE_Authority)
	{
		UE_LOG(LogAttributeSystem, Error, TEXT("%s: Cannot asjust resistance from client"), *GetOwner()->GetName())
			return;
	}

	WetnessResistance = FMath::Clamp(ColdResistance + Adjustment, 0.f, 0.9f);
	UE_LOG(LogAttributeSystem, Log, TEXT("%s: Wetness Resistance Adjusted to %s"), *GetOwner()->GetName(), *FString::SanitizeFloat(WetnessResistance))


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


void UEnvironmentalAttributes::SampleEnviornment(float SampledTemperature, float SampledWetness)
{
	if (GetOwnerRole() != ROLE_Authority)
	{
		UE_LOG(LogAttributeSystem, Error, TEXT("%s: Cannot sample enviornment from client"), *GetOwner()->GetName())
			return;
	}
	UE_LOG(LogAttributeSystem, Log, TEXT("%s sampled enviornment"), *GetOwner()->GetName())
	
	AdjustTemperature(SampledTemperature);
	AdjustWetness(SampledWetness);

}

void UEnvironmentalAttributes::OnRep_TempStateUpdate()
{
	BP_OnTempStateUpdate(CurrentTempState);
	Temp_OnStateChange.Broadcast(CurrentTempState);

}

void UEnvironmentalAttributes::OnRep_WetnessStateUpdate()
{
	BP_OnWetnessStateUpdate(CurrentWetnessState);
	Wetness_OnStateChange.Broadcast(CurrentWetnessState);
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
	else
	{
		float TempDiff;
		TempDiff = TargetTemperature - CurrentTemperature;

		float ActiveResistance;
		if (TempDiff > 0)
		{
			ActiveResistance = FMath::Clamp(HeatResistance + GetWetnessEffectOnWarming(),1.f,0.f);
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

		UE_LOG(LogAttributeSystem, Log, TEXT("%s Temp Adjustment. Adjustment Temp = %s | Current Temp = %s"),
			*FString::SanitizeFloat(TempToAdjust),
			*FString::SanitizeFloat(CurrentTemperature)
		)
	}
	
	SetTempState();
}

void UEnvironmentalAttributes::AdjustWetness(float SampledWetness)
{
	if (GetOwnerRole() != ROLE_Authority)
	{
		UE_LOG(LogAttributeSystem, Error, TEXT("%s: Cannot adjsut wetness from client"), *GetOwner()->GetName())
			return;
	}

	TargetWetness = FMath::Clamp(SampledWetness, 0.f, 100.f);

	//If the same, then return
	if (TargetWetness == CurrentWetness)
	{
		return;
	}

	if (FMath::Abs(TargetWetness - CurrentWetness) <= 1)
	{
		CurrentWetness = TargetWetness;
		UE_LOG(LogAttributeSystem,Log,TEXT("Wetness auto set. Current wetness = %s"),*FString::SanitizeFloat(CurrentWetness))
	}
	else
	{
		
		float WetnessDiff;
		WetnessDiff = TargetWetness - CurrentWetness;

		float UsedWetnessResitance;

		//If drying
		if (TargetWetness > CurrentWetness)
		{
			//This should make drying quicker if wet unless 0 wetness resistance
			UsedWetnessResitance = WetnessResistance - (WetnessResistance * GetHeatEffectOnDrying());
		}
		//If getting wetter
		else
		{
			UsedWetnessResitance = WetnessResistance;
		}

		float WetnessAdjust;
		WetnessAdjust = WetnessDiff - (WetnessDiff * UsedWetnessResitance);

		CurrentWetness += WetnessAdjust;

		UE_LOG(LogAttributeSystem, Log, TEXT("%s Wetness Adjustment. Wetness Adjustment = %s | Current Wetness = %s"),
			*FString::SanitizeFloat(WetnessAdjust),
			*FString::SanitizeFloat(CurrentWetness)
		)
	}

	SetWetnessState();
}

void UEnvironmentalAttributes::SetTempState()
{
	if (GetOwnerRole() != ROLE_Authority)
	{
		UE_LOG(LogAttributeSystem, Error, TEXT("%s: Cannot adjsut temp state from client"), *GetOwner()->GetName())
			return;
	}

	ETemperatureState TestTempState = CurrentTempState;

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
		const FString ResourceString = StaticEnum<ETemperatureState>()->GetValueAsString(CurrentTempState);
		OnRep_TempStateUpdate();
		UE_LOG(LogAttributeSystem, Log, TEXT("%s: new temp state: "), *GetOwner()->GetName(), *ResourceString);
	}


}

void UEnvironmentalAttributes::SetWetnessState()
{
	if (GetOwnerRole() != ROLE_Authority)
	{
		UE_LOG(LogAttributeSystem, Error, TEXT("%s: Cannot adjsut wetness state from client"), *GetOwner()->GetName())
			return;
	}

	EWetnessState TestWetState = CurrentWetnessState;

	//If dry 
	if (CurrentWetness >= BaseDryLevel && CurrentWetness < BaseDampLevel)
	{
		TestWetState = EWetnessState::EWS_Dry;
	}
	//If Damp
	else if (CurrentWetness >= BaseDampLevel && CurrentWetness < BaseWetLevel)
	{
		TestWetState = EWetnessState::EWS_Damp;
	}
	//If wet
	else if (CurrentWetness >= BaseWetLevel && CurrentWetness < BaseSoakedLevel)
	{
		TestWetState = EWetnessState::EWS_Wet;
	}
	else if (CurrentWetness >= BaseSoakedLevel)
	{
		TestWetState = EWetnessState::EWS_Soaked;
	}

	if (CurrentWetnessState != TestWetState)
	{
		CurrentWetnessState = TestWetState;
		const FString ResourceString = StaticEnum<EWetnessState>()->GetValueAsString(CurrentWetnessState);
		OnRep_WetnessStateUpdate();
		UE_LOG(LogAttributeSystem, Log, TEXT("%s: new temp state: "), *GetOwner()->GetName(), *ResourceString);

	}

}

float UEnvironmentalAttributes::GetHeatEffectOnDrying()
{
	float OutEffect = 0;

	switch (CurrentTempState)
	{
	case ETemperatureState::ETS_Freezing:
		OutEffect = 0;
		break;
	case ETemperatureState::ETS_Cold:
		OutEffect = 0;
		break;
	case ETemperatureState::ETS_Comfortable:
		OutEffect = 0;
		break;
	case ETemperatureState::ETS_Hot:
		OutEffect = HotEffectOnDrying;
		break;
	case ETemperatureState::ETS_Scortching:
		OutEffect = ScorchingEffectOnDrying;
		break;
	default:
		break;
	}

	return OutEffect;
}

float UEnvironmentalAttributes::GetWetnessEffectOnWarming()
{

	float OutEffect = 0;

	switch (CurrentWetnessState)
	{
	case EWetnessState::EWS_Dry:
		OutEffect = 0.f;
		break;
	case EWetnessState::EWS_Damp:
		OutEffect = DampEffectOnWarniming;
		break;
	case EWetnessState::EWS_Wet:
		OutEffect = WetEffectOnWarniming;
		break;
	case EWetnessState::EWS_Soaked:
		OutEffect = SoakedEffectOnWarming;
		break;
	default:
		break;
	}


	return OutEffect;
}

