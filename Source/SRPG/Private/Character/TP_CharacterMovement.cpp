// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/TP_CharacterMovement.h"
#include "Character/TP_Character.h"


void UTP_CharacterMovement::BeginPlay()
{
	Super::BeginPlay();

	OwningCharacter = Cast<ATP_Character>(PawnOwner);

	DefaultMaxSpeed = OwningCharacter->GetDefaultWalkSpeed();

}

float UTP_CharacterMovement::GetMaxSpeed() const
{
	float MaxSpeed = Super::GetMaxSpeed();

	if (OwningCharacter)
	{
		if (OwningCharacter->GetWantsToSprint() && OwningCharacter->bIsCrouched == false)
		{
			MaxSpeed = DefaultMaxSpeed * OwningCharacter->GetSprintSpeedModifier();
		}
		else
		{
			MaxSpeed = DefaultMaxSpeed;
		}
	}

	return MaxSpeed;
}

