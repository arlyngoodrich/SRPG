// Fill out your copyright notice in the Description page of Project Settings.


#include "AttributeSystem/MetabolismAttribute.h"
#include "LogFiles.h"

//UE4 Includes
#include "Net/UnrealNetwork.h"

UMetabolismAttribute::UMetabolismAttribute()
{


}

void UMetabolismAttribute::BeginPlay()
{
	Super::BeginPlay();
	
	if (GetOwnerRole() == ROLE_Authority)
	{
		ChangeFood(MaxFood / .5, MaxFood / .25, MaxFood / .25, MaxWater);
		StartMetabolism();
	}
}



void UMetabolismAttribute::GetLifetimeReplicatedProps(TArray<FLifetimeProperty >& OutLifetimeProps) const
{

	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UMetabolismAttribute, CurrentWater);
	DOREPLIFETIME(UMetabolismAttribute, CurrentFood);
	DOREPLIFETIME(UMetabolismAttribute, CurrentCarbs);
	DOREPLIFETIME(UMetabolismAttribute, CurrentVitamins);
	DOREPLIFETIME(UMetabolismAttribute, CurrentProtein);
	DOREPLIFETIME(UMetabolismAttribute, CurrentMetabolismStatus);
	
}


void UMetabolismAttribute::ChangeFood(float CarbsChange, float VitaminsChange, float ProteinChange, float WaterChange)
{
	if (GetOwnerRole() != ROLE_Authority)
	{
		UE_LOG(LogAttributeSystem, Warning, TEXT("%s cannot change food level from client"), *GetOwner()->GetName())
			return;
	}

	CurrentWater = FMath::Clamp(CurrentWater + WaterChange, 0.f, MaxWater);
	CurrentCarbs = FMath::Clamp(CurrentCarbs + CarbsChange, 0.f, MaxFood - CurrentProtein - CurrentVitamins);
	CurrentVitamins = FMath::Clamp(CurrentVitamins + VitaminsChange, 0.f, MaxFood - CurrentCarbs - CurrentProtein);
	CurrentProtein = FMath::Clamp(CurrentProtein + ProteinChange, 0.f, MaxFood - CurrentCarbs - CurrentVitamins);

	CurrentFood = CurrentCarbs + CurrentVitamins + CurrentProtein;
	SetMetabolismStatus();

	LogFoodLevels();

}


void UMetabolismAttribute::StartMetabolism()
{
	GetWorld()->GetTimerManager().SetTimer(MetabolismTimerHandle, this, &UMetabolismAttribute::Metabolize, MetabolicFrequency, true);
}

void UMetabolismAttribute::Metabolize()
{
	ChangeFood(CarbDecayAmount, VitaminDecayAmount, ProteinDecayAmount, WaterDecayAmount);
}


void UMetabolismAttribute::SetMetabolismStatus()
{

	if (GetOwnerRole() != ROLE_Authority) 
	{
		UE_LOG(LogAttributeSystem, Warning, TEXT("%s cannot set metabolism status on client"), *GetOwner()->GetName()) 
		return; 
	}

	//If full
	if (CurrentFood >= FullLevel)
	{
		CurrentMetabolismStatus = EMetabolismStatus::EMS_Full;
	}
	//If content - greater than hungry but lass than full
	else if (CurrentFood >= HungerLevel && CurrentFood < FullLevel)
	{
		CurrentMetabolismStatus = EMetabolismStatus::EMS_Content;
	}
	// If hungry -  greater than starving level but less than hungry level
	else if (CurrentFood >= StarvingLevel && CurrentFood < HungerLevel )
	{
		CurrentMetabolismStatus = EMetabolismStatus::EMS_Hungry;
	}
	// If starving - less than starving level
	else if (CurrentFood < StarvingLevel)
	{
		CurrentMetabolismStatus = EMetabolismStatus::EMS_Starving;
	}

	FString MetabolismStatusString = UEnum::GetValueAsString<EMetabolismStatus>(CurrentMetabolismStatus);
	UE_LOG(LogAttributeSystem,Log,TEXT("%s new metabolism status: %s"),*GetOwner()->GetName(),*MetabolismStatusString)
}


void UMetabolismAttribute::LogFoodLevels()
{
	UE_LOG
	(
		LogAttributeSystem, Log, TEXT("CurrentWater: %s, Total Food: %s, Carbs: %s, Vitamins: %s, Protein: %s"),
		*FString::SanitizeFloat(CurrentWater),
		*FString::SanitizeFloat(CurrentFood),
		*FString::SanitizeFloat(CurrentCarbs),
		*FString::SanitizeFloat(CurrentVitamins),
		*FString::SanitizeFloat(CurrentProtein)
	)

}
