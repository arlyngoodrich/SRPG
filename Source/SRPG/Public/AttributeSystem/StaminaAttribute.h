// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSystem/BaseAttributeComponent.h"
#include "AttributeSystem/AttributeData.h"
#include "StaminaAttribute.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEncumberanceChange, bool, bNewEncumberance);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnExhaustedChange, bool, bNewExhausted);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStaminaChange, float, NewStamina);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStaminaStartReGen);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStaminaStopReGen);

UCLASS(ClassGroup = (Attributes), blueprintable, meta = (BlueprintSpawnableComponent))
class SRPG_API UStaminaAttribute : public UBaseAttributeComponent
{
	GENERATED_BODY()

public:

	// Sets default values for this component's properties
	UStaminaAttribute();

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Stamina")
	void UseStamina(float StaminaAmount);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Stamina")
	void SetWeight(float NewWeight);

	//Values less than 1 will decrease stamina used while greater than 1 will increase stamina used	
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Attribute Effects")
	void ModifyStaminaUse(float ModifyAmount);

	//Values greater than 1 will increase stamina regeneration while less than 1 will decrease stamina regeneration. 
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Attribute Effects")
	void ModifyStaminaReGen(float ModifyAmount);

	//Client safe function to bind to encumberance update.  Designed for use with UI. 
	UPROPERTY(BlueprintAssignable)
	FOnEncumberanceChange OnEncumberanceChange;

	//Client safe function to bind to exhaustion update.  Designed for use with UI. 
	UPROPERTY(BlueprintAssignable)
	FOnExhaustedChange OnExhuastionChange;

	UPROPERTY(BlueprintAssignable)
	FOnStaminaStartReGen Stamina_OnStartReGen;

	UPROPERTY(BlueprintAssignable)
	FOnStaminaStopReGen Stamina_OnStopReGen;

	//Server only delegate for stamina updates.  Fairly frequent so not client safe.  
	FOnStaminaChange OnStaminaChange;

protected:

	// Called when the game starts
	virtual void BeginPlay() override;

	void Initalize();

	UFUNCTION()
	void StartStaminaSprintDecayTimer();

	UFUNCTION()
	void StartStaminaReGenTimer();

	UFUNCTION()
	void StopStaminaRegen();

	UFUNCTION()
	void OnRep_EncumberanceUpdate();

	UFUNCTION()
	void OnRep_ExhaustedUpdate();

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

	//Becomes true when stamina is lower than exhaustion level.  Changes sprint modifer to 1 so character can only walk.  
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_ExhaustedUpdate, Category = "Stamina")
	bool bIsExhausted;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Stamina Config")
	float EncumberanceWeight;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Stamina Config")
	float EncumberanceSprintModifer;

	//becomes true when weight is over encumberance weight.  Changes sprint modifer to encumberance sprint modifier which should  slow character's sprint speed. 
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_EncumberanceUpdate, Category = "Stamina")
	bool bIsEncumbered;

	UPROPERTY(BlueprintReadOnly, Replicated, Category = "Stamina")
	float CurrentWeight;

	//----------------- Effect Modifers ----------------------//
	//Values less than 1 will decrease stamina used while greater than 1 will increase stamina used	
	UPROPERTY(BlueprintReadOnly, Replicated, Category = "Attribute Effect Modifiers")
	float StaminaUseModifer = 1.f;

	//Values greater than 1 will increase stamina regeneration while less than 1 will decrease stamina regeneration. 
	UPROPERTY(BlueprintReadOnly, Replicated, Category = "Attribute Effect Modifiers")
	float StaminaRegenModifer = 1.f;

	//----------------- Internal Properties-----------------//

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
