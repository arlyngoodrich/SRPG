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


UCLASS(ClassGroup = (Attributes), blueprintable, meta = (BlueprintSpawnableComponent))
class SRPG_API UMetabolismAttribute : public UBaseAttributeComponent
{
	GENERATED_BODY()

public:

	UMetabolismAttribute();

protected:

	virtual void BeginPlay() override;

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
	float StarvingLevel = 10,f ;

	UPROPERTY(BlueprintReadOnly, Replicated, Category = "Metabolism")
	EMetabolismStatus CurrentMetabolismStatus;

	void StartMetabolism();

	void Metabolize();

	FTimerHandle MetabolismTimerHandle;

	void LogFoodLevels();

private:

	void ChangeFood(float CarbsChange, float VitaminsChange, float ProteinChange, float WaterChange);

	void SetMetabolismStatus();
	
};
