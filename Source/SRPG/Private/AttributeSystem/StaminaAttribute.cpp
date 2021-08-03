// Fill out your copyright notice in the Description page of Project Settings.


#include "AttributeSystem/StaminaAttribute.h"
#include "Character/TP_CharacterMovement.h"
#include "Character/TP_Character.h"
#include "LogFiles.h"

//UE4 Includes
#include "Net/UnrealNetwork.h"
#include "..\..\Public\AttributeSystem\StaminaAttribute.h"


UStaminaAttribute::UStaminaAttribute()
{

	StaminaRegenAmount = 1.f;
	SprintStaminaDecay = 5.f;
	MaxStamina = 100.f;
	ExhaustedLevel = 50.f;
	CurrentStamina = MaxStamina;

	EncumberanceWeight = 150.f;
	EncumberanceSprintModifer = 1.5f;

}

void UStaminaAttribute::UseStamina(float StaminaAmount)
{
	ChangeStaminaAmount(-StaminaAmount);
	UE_LOG(LogAttributeSystem,Log,TEXT("Stamina: %s"),*FString::SanitizeFloat(CurrentStamina))

	StartStaminaReGenTimer();
}

void UStaminaAttribute::SetWeight(float NewWeight)
{
	if (PlayerCharacter == nullptr) { return; }


	CurrentWeight = NewWeight;

	//Check if newly encumbered
	if (CurrentWeight > EncumberanceWeight)
	{
		SetEncumbered();
	}
	//Check if weight newly OK
	else if (CurrentWeight < EncumberanceWeight)
	{
		SetWeightOK();
	}

	UE_LOG(LogAttributeSystem,Log,TEXT("New weight: %s"),*FString::SanitizeFloat(CurrentWeight))
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

	DOREPLIFETIME(UStaminaAttribute, CurrentStamina);
	DOREPLIFETIME(UStaminaAttribute, bIsExhausted);
	DOREPLIFETIME(UStaminaAttribute, CurrentWeight);
	DOREPLIFETIME(UStaminaAttribute, bIsEncumbered);
	
	
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
		PlayerCharacter->WantsToSprint_OnSprintStart.AddDynamic(this, &UStaminaAttribute::StartStaminaSprintDecayTimer);
		PlayerCharacter->WantsToNotSprint_OnSptrintStop.AddDynamic(this, &UStaminaAttribute::StartStaminaReGenTimer);
		DefaultSprintModifier = PlayerCharacter->GetSprintSpeedModifier();

		UE_LOG(LogAttributeSystem,Log, TEXT("Stamina Attribute Component sucessfully bound to Character"))
	}
}

void UStaminaAttribute::StartStaminaSprintDecayTimer()
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

void UStaminaAttribute::StartStaminaReGenTimer()
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

void UStaminaAttribute::OnRep_EncumberanceUpdate()
{
	OnEncumberanceChange.Broadcast(bIsEncumbered);
}

void UStaminaAttribute::OnRep_ExhaustedUpdate()
{
	OnExhuastionChange.Broadcast(bIsExhausted);
}

void UStaminaAttribute::ChangeStaminaAmount(float ChangeAmount)
{
	CurrentStamina = FMath::Clamp(CurrentStamina + ChangeAmount, 0.f, MaxStamina);

	//Check if newly exhausted
	if (CurrentStamina < ExhaustedLevel && !bIsExhausted)
	{
		SetExhausted();
	}
	//Check if newly OK 
	else if(CurrentStamina > ExhaustedLevel && bIsExhausted)
	{
		SetStaminaOK();
	}
}

void UStaminaAttribute::SetExhausted()
{
	bIsExhausted = true;
	if (PlayerCharacter == nullptr) { UE_LOG(LogAttributeSystem, Error, TEXT("Set Exhausted - Invalid player refence in stamina component")) return; }

	PlayerCharacter->SetSprintSpeedModifer(1.f);
	PlayerCharacter->SetWantsToSprint();

	if (GetOwnerRole() == ROLE_Authority)
	{
		OnRep_ExhaustedUpdate();
	}



	UE_LOG(LogAttributeSystem,Log,TEXT("Character is exhausted"))
}

void UStaminaAttribute::SetStaminaOK()
{
	bIsExhausted = false;

	if (PlayerCharacter == nullptr) { UE_LOG(LogAttributeSystem, Error, TEXT("Set Stamina OK - Invalid player refence in stamina component")) return; }

	if(bIsEncumbered)
	{
		PlayerCharacter->SetSprintSpeedModifer(EncumberanceSprintModifer);
	}
	else
	{
		PlayerCharacter->SetSprintSpeedModifer(DefaultSprintModifier);
	}

	if (GetOwnerRole() == ROLE_Authority)
	{
		OnRep_ExhaustedUpdate();
	}


	UE_LOG(LogAttributeSystem,Log,TEXT("Stamina is OK"))
}

void UStaminaAttribute::SetEncumbered()
{
	bIsEncumbered = true;
	 
	if (PlayerCharacter == nullptr) { UE_LOG(LogAttributeSystem, Error, TEXT("Set Encumbered - Invalid player refence in stamina component")) return; }

	PlayerCharacter->SetSprintSpeedModifer(EncumberanceSprintModifer);
	UE_LOG(LogAttributeSystem,Log,TEXT("Player is encumbered"))

	if(GetOwnerRole() == ROLE_Authority)
	{
		OnRep_EncumberanceUpdate();
	}
	
}

void UStaminaAttribute::SetWeightOK()
{
	bIsEncumbered = false;

	if (PlayerCharacter == nullptr) { UE_LOG(LogAttributeSystem, Error, TEXT("Set WEight OK - Invalid player refence in stamina component")) return; }

	PlayerCharacter->SetSprintSpeedModifer(DefaultSprintModifier);
	UE_LOG(LogAttributeSystem, Log, TEXT("Player weight is Ok "))

	if (GetOwnerRole() == ROLE_Authority)
	{
		OnRep_EncumberanceUpdate();
	}
}

void UStaminaAttribute::ReGenStamina()
{

	if (bCanStaminaRegen == false) { return; }

	ChangeStaminaAmount(StaminaRegenAmount);

	UE_LOG(LogAttributeSystem,Log,TEXT("Stamina: %s"), *FString::SanitizeFloat(CurrentStamina))


	if (CurrentStamina == MaxStamina)
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

	if (bIsExhausted) { return; }

	ChangeStaminaAmount(-SprintStaminaDecay);

	UE_LOG(LogAttributeSystem, Log, TEXT("Stamina: %s"), *FString::SanitizeFloat(CurrentStamina))
}
