// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSystem/BaseAttributeComponent.h"
#include "StaminaAttribute.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = (Attributes), blueprintable, meta = (BlueprintSpawnableComponent))
class SRPG_API UStaminaAttribute : public UBaseAttributeComponent
{
	GENERATED_BODY()

public:

	// Sets default values for this component's properties
	UStaminaAttribute();

protected:

	// Called when the game starts
	virtual void BeginPlay() override;

	void Initalize();

	UFUNCTION()
	void OnWantsToSprintStart();

	UFUNCTION()
	void OnWantsToSprintStop();

	UPROPERTY(BlueprintReadOnly, Category = "References")
	class UTP_CharacterMovement* CharacterMovementComponent;

	UPROPERTY(BlueprintReadOnly, Category = "References")
	class ATP_Character* PlayerCharacter;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Stamina Config")
	FAttributeData Stamina;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Stamina Config")
	float MaxStamina;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Stamina Config")
	TArray<FAttributeEffectTrigger> StaminaTriggers;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Stamina Config")
	float StaminaRegenAmount;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Stamina Config")
	float SprintStaminaDecay;

	FTimerHandle StaminaReGentimer;

	FTimerHandle SprintStaminaDecayTimer;

	bool bCanStaminaRegen;

	void ReGenStamina();

	void UseStaminaWhileSprinting();

};
