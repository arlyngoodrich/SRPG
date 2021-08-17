// Fill out your copyright notice in the Description page of Project Settings.


#include "AttributeSystem/MetabolismAttribute.h"
#include "AttributeSystem/StaminaAttribute.h"
#include "LogFiles.h"

//UE4 Includes
#include "Net/UnrealNetwork.h"

UMetabolismAttribute::UMetabolismAttribute()
{
	

}

void UMetabolismAttribute::OnConsume(float CarbsChange, float VitaminsChange, float ProteinChange, float WaterChange)
{
	if (GetOwnerRole() != ROLE_Authority)
	{
		UE_LOG(LogAttributeSystem, Warning, TEXT("%s cannot change food level from client"), *GetOwner()->GetName())
			return;
	}

	ChangeFood(CarbsChange, VitaminsChange, ProteinChange, WaterChange);

}

void UMetabolismAttribute::BeginPlay()
{
	Super::BeginPlay();
	
	if (GetOwnerRole() == ROLE_Authority)
	{
		ChangeFood(MaxFood * .25, MaxFood * .5, MaxFood*.25, MaxWater);
		StartMetabolism();

		UStaminaAttribute* StaminaAttributeTest = GetOwner()->FindComponentByClass<UStaminaAttribute>();
		if (StaminaAttributeTest != nullptr)
		{
			StaminaAttribute = StaminaAttributeTest;
			StaminaAttribute->Stamina_OnStartReGen.AddDynamic(this, &UMetabolismAttribute::OnStartStaminaRegen);
			StaminaAttribute->Stamina_OnStopReGen.AddDynamic(this, &UMetabolismAttribute::OnStopStaminaRegen);
		}

	}
}

void UMetabolismAttribute::OnRep_MetabolicStatusChange()
{
	BP_OnMetabolicStatusUpdate(CurrentMetabolismStatus);

}

void UMetabolismAttribute::OnRep_MetabolicBalanceChange()
{
	BP_OnMetabolicBalanceUpdate(CurrentMetabolicBalance);
	Metabolism_OnBalanceChange.Broadcast(CurrentMetabolicBalance);
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
	DOREPLIFETIME(UMetabolismAttribute, CurrentMetabolicBalance);

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
	SetMetabolicBalance();

	LogFoodLevels();

}


void UMetabolismAttribute::StartMetabolism()
{
	GetWorld()->GetTimerManager().SetTimer(MetabolismTimerHandle, this, &UMetabolismAttribute::Metabolize, MetabolicFrequency, true);
}

void UMetabolismAttribute::Metabolize()
{
	ChangeFood(-CarbDecayAmount, -VitaminDecayAmount, -ProteinDecayAmount, -WaterDecayAmount);
}


void UMetabolismAttribute::SetMetabolismStatus()
{

	if (GetOwnerRole() != ROLE_Authority) 
	{
		UE_LOG(LogAttributeSystem, Warning, TEXT("%s cannot set metabolism status on client"), *GetOwner()->GetName()) 
		return; 
	}

	EMetabolismStatus MetabolismStatusCheck;
	MetabolismStatusCheck = CurrentMetabolismStatus;

	//If full
	if (CurrentFood >= FullLevel)
	{
		MetabolismStatusCheck = EMetabolismStatus::EMS_Full;
	}
	//If content - greater than hungry but lass than full
	else if (CurrentFood >= HungerLevel && CurrentFood < FullLevel)
	{
		MetabolismStatusCheck = EMetabolismStatus::EMS_Content;
	}
	// If hungry -  greater than starving level but less than hungry level
	else if (CurrentFood >= StarvingLevel && CurrentFood < HungerLevel )
	{
		MetabolismStatusCheck = EMetabolismStatus::EMS_Hungry;
	}
	// If starving - less than starving level
	else if (CurrentFood < StarvingLevel)
	{
		MetabolismStatusCheck = EMetabolismStatus::EMS_Starving;
	}

	if (MetabolismStatusCheck != CurrentMetabolismStatus)
	{
		CurrentMetabolismStatus = MetabolismStatusCheck;
		OnRep_MetabolicStatusChange();
		const FString ResourceString = StaticEnum<EMetabolismStatus>()->GetValueAsString(CurrentMetabolismStatus);
		UE_LOG(LogAttributeSystem, Log, TEXT("%s new metabolism status: %s"), *GetOwner()->GetName(), *ResourceString)
	}


}

void UMetabolismAttribute::SetMetabolicBalance()
{
	if (GetOwnerRole() != ROLE_Authority)
	{
		UE_LOG(LogAttributeSystem, Warning, TEXT("%s - cannot set metabolic balance from client"), *GetOwner()->GetName())
		return;
	}

	EMetabolicBalanceType MetabolicBalanceTest;
	MetabolicBalanceTest = CurrentMetabolicBalance;

	//If carb heavy
	if (CurrentCarbs / CurrentFood > .5)
	{
		MetabolicBalanceTest = EMetabolicBalanceType::EMBT_Carbs;
	}
	//If vitamin heavy
	else if (CurrentVitamins / CurrentFood > .5)
	{
		MetabolicBalanceTest = EMetabolicBalanceType::EMBT_Vitamins;
	}
	//If Protein heavy
	else if (CurrentProtein / CurrentFood > .5)
	{
		MetabolicBalanceTest = EMetabolicBalanceType::EMBT_Protein;
	}
	else
	{
		MetabolicBalanceTest = EMetabolicBalanceType::EMBT_None;
	}

	if (CurrentMetabolicBalance != MetabolicBalanceTest)
	{
		CurrentMetabolicBalance = MetabolicBalanceTest;
		OnRep_MetabolicBalanceChange();
		const FString ResourceString = StaticEnum<EMetabolicBalanceType>()->GetValueAsString(CurrentMetabolicBalance);
		UE_LOG(LogAttributeSystem,Log,TEXT("%s new metabolic balance: %s"), *GetOwner()->GetName(),*ResourceString)
	}

}

void UMetabolismAttribute::OnStartStaminaRegen()
{

	GetWorld()->GetTimerManager().ClearTimer(MetabolismTimerHandle);
	GetWorld()->GetTimerManager().SetTimer(MetabolismTimerHandle, this, &UMetabolismAttribute::Metabolize, MetabolicFrequency*StaminaRegnModifier, true);

}

void UMetabolismAttribute::OnStopStaminaRegen()
{
	GetWorld()->GetTimerManager().ClearTimer(MetabolismTimerHandle);
	StartMetabolism();
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
