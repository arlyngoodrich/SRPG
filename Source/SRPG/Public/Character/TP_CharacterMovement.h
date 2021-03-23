// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "TP_CharacterMovement.generated.h"

/**
 * 
 */

class ATP_Character;

UCLASS()
class SRPG_API UTP_CharacterMovement : public UCharacterMovementComponent
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;

	virtual float GetMaxSpeed() const override;

protected:

	ATP_Character* OwningCharacter;

	float DefaultMaxSpeed;

};

