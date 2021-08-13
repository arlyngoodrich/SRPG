// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSystem/BaseAttributeComponent.h"
#include "MetabolismAttribute.generated.h"

/**
 * 
 */

UENUM(BlueprintType)
enum class EMetabolismStatus : uint8 {

	EMS_Starving		UMETA(DisplayName = "Starving"),
	EMS_Hungry			UMETA(DisplayName = "Hungry"),
	EMS_Content		    UMETA(DisplayName = "Content"),
	EMS_Full		    UMETA(DisplayName = "Full"),
};

UENUM(BlueprintType)
enum class EMetabolicBalanceType : uint8 {

	EMBT_None				UMETA(DisplayName = "Balanced"),
	EMBT_Carbs				UMETA(DisplayName = "Carb Heavy"),
	EMBT_Protein		    UMETA(DisplayName = "Protein Heavy"),
	EMBT_Vitamins		    UMETA(DisplayName = "Vitamin Heavy"),
};


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBalanceChange, EMetabolicBalanceType, NewMetabolisimBalanceType);

UCLASS(ClassGroup = (Attributes), blueprintable, meta = (BlueprintSpawnableComponent))
class SRPG_API UMetabolismAttribute : public UBaseAttributeComponent
{
	GENERATED_BODY()

public:

	UMetabolismAttribute();

	UPROPERTY(BlueprintAssignable)
	FOnBalanceChange Metabolism_OnBalanceChange;

protected:

	virtual void BeginPlay() override;

	//Triggered by change in Metabolic status.  Runs on both client and server.  
	UFUNCTION(BlueprintImplementableEvent, Category = "Metabolism", meta = (DisplayName = "On Metabolic Status Update"))
	void BP_OnMetabolicStatusUpdate(EMetabolismStatus NewMetabolicStatus);

	//Triggered by change in Metabolic balance.  Runs on both client and server.  
	UFUNCTION(BlueprintImplementableEvent, Category = "Metabolism", meta = (DisplayName = "On Metabolic Balance Update"))
	void BP_OnMetabolicBalanceUpdate(EMetabolicBalanceType NewBalance);

	UFUNCTION()
	void OnRep_MetabolicStatusChange();

	UFUNCTION()
	void OnRep_MetabolicBalanceChange();


	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Metabolism")
	float MaxWater = 100.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Metabolism")
	float MaxFood = 100.f;

	UPROPERTY(BlueprintReadOnly, Replicated, Category = "Metabolism")
	float CurrentWater;	

	UPROPERTY(BlueprintReadOnly, Replicated, Category = "Metabolism")
	float CurrentFood;

	UPROPERTY(BlueprintReadOnly, Replicated, Category = "Metabolism")
	float CurrentCarbs;

	UPROPERTY(BlueprintReadOnly, Replicated, Category = "Metabolism")
	float CurrentVitamins;

	UPROPERTY(BlueprintReadOnly, Replicated, Category = "Metabolism")
	float CurrentProtein;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Metabolism")
	float MetabolicFrequency = 1.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Metabolism")
	float StaminaRegnModifier = .5f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Metabolism")
	float WaterDecayAmount = 0.1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Metabolism")
	float CarbDecayAmount = 0.1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Metabolism")
	float VitaminDecayAmount = 0.1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Metabolism")
	float ProteinDecayAmount = 0.1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Metabolism")
	float FullLevel = 75.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Metabolism")
	float HungerLevel = 25.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Metabolism")
	float StarvingLevel = 10.f;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MetabolicStatusChange, Category = "Metabolism")
	EMetabolismStatus CurrentMetabolismStatus;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MetabolicBalanceChange, Category = "Metabolism")
	EMetabolicBalanceType CurrentMetabolicBalance;

	void StartMetabolism();

	void Metabolize();

	FTimerHandle MetabolismTimerHandle;

	void LogFoodLevels();

	class UStaminaAttribute* StaminaAttribute;

private:

	void ChangeFood(float CarbsChange, float VitaminsChange, float ProteinChange, float WaterChange);

	void SetMetabolismStatus();

	void SetMetabolicBalance();

	UFUNCTION()
	void OnStartStaminaRegen();

	UFUNCTION()
	void OnStopStaminaRegen();
	
};
