// Fill out your copyright notice in the Description page of Project Settings.


#include "AttributeSystem/StaminaAttribute.h"
#include "Character/TP_CharacterMovement.h"
#include "Character/TP_Character.h"
#include "LogFiles.h"

//UE4 Includes
#include "Net/UnrealNetwork.h"


UStaminaAttribute::UStaminaAttribute()
{

	StaminaRegenAmount = 1.f;
	SprintStaminaDecay = 2.f;
	MaxStamina = 100.f;

	Stamina.AttributeMaxAmount = MaxStamina;
	Stamina.AttributeCurrentAmount = MaxStamina;
	Stamina.AtributeTriggers = StaminaTriggers;

}

// Called when the game starts
void UStaminaAttribute::BeginPlay()
{
	Super::BeginPlay();
	Initalize();
}


void UStaminaAttribute::GetLifetimeReplicatedProps(TArray<FLifetimeProperty >& OutLifetimeProps) const
{

	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UStaminaAttribute, Stamina);

}

void UStaminaAttribute::Initalize()
{
	AActor* MyOwner= GetOwner();
	UTP_CharacterMovement* Local_MovementComp;
	Local_MovementComp = MyOwner->FindComponentByClass<UTP_CharacterMovement>();

	if (Local_MovementComp == nullptr)
	{
		UE_LOG(LogAttributeSystem, Error, TEXT("Stamina Attribute Component failed to initalize Movementc Component reference")) return; 
	}
	else
	{
		CharacterMovementComponent = Local_MovementComp;
	}

	ATP_Character* Local_PlayerCharacter;
	Local_PlayerCharacter = Cast<ATP_Character>(MyOwner);

	if (Local_PlayerCharacter == nullptr)
	{
		UE_LOG(LogAttributeSystem, Error, TEXT("Stamina Attribute Component failed to initalize TP_Character reference")) return;
	}
	else
	{
		PlayerCharacter = Local_PlayerCharacter;
		PlayerCharacter->WantsToSprint_OnSprintStart.AddDynamic(this, &UStaminaAttribute::OnWantsToSprintStart);
		PlayerCharacter->WantsToNotSprint_OnSptrintStop.AddDynamic(this, &UStaminaAttribute::OnWantsToSprintStop);

		UE_LOG(LogAttributeSystem,Log, TEXT("Stamina Attribute Component sucessfully bound to Character"))
	}
}

void UStaminaAttribute::OnWantsToSprintStart()
{
	UE_LOG(LogAttributeSystem, Log, TEXT("Stamina received start sprint"))

	bCanStaminaRegen = false;

	//Start timer to decrease stamina during sprinting
	if (SprintStaminaDecayTimer.IsValid())
	{
		GetWorld()->GetTimerManager().UnPauseTimer(SprintStaminaDecayTimer);
	}
	else
	{
		GetWorld()->GetTimerManager().SetTimer(SprintStaminaDecayTimer, this, &UStaminaAttribute::UseStaminaWhileSprinting, 1.f, true);
	}


	//Pause regen timer if valid
	if (StaminaReGentimer.IsValid())
	{
		GetWorld()->GetTimerManager().PauseTimer(StaminaReGentimer);
	}

}

void UStaminaAttribute::OnWantsToSprintStop()
{
	UE_LOG(LogAttributeSystem, Log, TEXT("Stamina received stop sprint"))

	//Set bCanRegn to true
	bCanStaminaRegen = true;


	if (StaminaReGentimer.IsValid())
	{
		GetWorld()->GetTimerManager().UnPauseTimer(StaminaReGentimer);
	}
	else
	{
		GetWorld()->GetTimerManager().SetTimer(StaminaReGentimer, this, &UStaminaAttribute::ReGenStamina, 1.f, true);
	}
	
	//Pause Sprint Decay timer if valid
	if (SprintStaminaDecayTimer.IsValid())
	{
		GetWorld()->GetTimerManager().PauseTimer(SprintStaminaDecayTimer);
	}


}

void UStaminaAttribute::ReGenStamina()
{

	if (bCanStaminaRegen == false) { return; }

	Stamina.AttributeCurrentAmount = FMath::Clamp(Stamina.AttributeCurrentAmount + StaminaRegenAmount, 0.f, Stamina.AttributeMaxAmount);

	UE_LOG(LogAttributeSystem,Log,TEXT("Stamina: %s"), *FString::SanitizeFloat(Stamina.AttributeCurrentAmount))


	if (Stamina.AttributeCurrentAmount == Stamina.AttributeMaxAmount)
	{
		//Pause regen timer if valid
		if (StaminaReGentimer.IsValid())
		{
			GetWorld()->GetTimerManager().PauseTimer(StaminaReGentimer);
			UE_LOG(LogAttributeSystem,Log,TEXT("Stamina reached max amount, regen stopped"))
		}
	}


}

void UStaminaAttribute::UseStaminaWhileSprinting()
{
	FAttributeData NewStamina;
	ChangeAttributeAmount(Stamina, -SprintStaminaDecay, NewStamina);
	Stamina = NewStamina;
	UE_LOG(LogAttributeSystem, Log, TEXT("Stamina: %s"), *FString::SanitizeFloat(Stamina.AttributeCurrentAmount))
}
