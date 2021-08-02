// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AttributeSystem/AttributeData.h"
#include "StaminaAttribute.generated.h"



UCLASS(ClassGroup = (Attributes), blueprintable, meta = (BlueprintSpawnableComponent))
class SRPG_API UStaminaAttribute : public UActorComponent
{
	GENERATED_BODY()

public:

	// Sets default values for this component's properties
	UStaminaAttribute();

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Stamina")
	void UseStamina(float StaminaAmount);

	UFUNCTION(BluePrintCallable, BlueprintAuthorityOnly, Category = "Stamina")
	void SetWeight(float NewWeight);

protected:

	// Called when the game starts
	virtual void BeginPlay() override;

	void Initalize();

	UFUNCTION()
	void StartStaminaSprintDecayTimer();

	UFUNCTION()
	void StartStaminaReGenTimer();

	UPROPERTY(BlueprintReadOnly, Category = "References")
	class UTP_CharacterMovement* CharacterMovementComponent;

	UPROPERTY(BlueprintReadOnly, Category = "References")
	class ATP_Character* PlayerCharacter;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Stamina Config")
	float MaxStamina;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Stamina")
	float CurrentStamina;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Stamina Config")
	float ExhaustedLevel;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Stamina Config")
	float StaminaRegenAmount;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Stamina Config")
	float SprintStaminaDecay;

	UPROPERTY(BlueprintReadOnly, Replicated, Category = "Stamina")
	bool bIsExhausted;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Stamina Config")
	float EncumberanceWeight;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Stamina Config")
	float EncumberanceSprintModifer;

	UPROPERTY(BlueprintReadOnly, Replicated, Category = "Stamina")
	bool bIsEncumbered;

	UPROPERTY(BlueprintReadOnly, Replicated, Category = "Stamina")
	float CurrentWeight;

	float DefaultSprintModifier;

	FTimerHandle StaminaReGentimer;

	FTimerHandle SprintStaminaDecayTimer;

	void ChangeStaminaAmount(float ChangeAmount);

	void SetExhausted();

	void SetStaminaOK();

	void SetEncumbered();

	void SetWeightOK();

	bool bCanStaminaRegen;

	void ReGenStamina();

	void UseStaminaWhileSprinting();

};
