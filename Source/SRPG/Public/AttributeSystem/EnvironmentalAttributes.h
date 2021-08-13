// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSystem/BaseAttributeComponent.h"
#include "EnvironmentalAttributes.generated.h"

UENUM(BlueprintType)
enum class ETemperatureState : uint8 {

	ETS_Freezing			UMETA(DisplayName = "Freezing"),
	ETS_Cold				UMETA(DisplayName = "Cold"),
	ETS_Comfortable		    UMETA(DisplayName = "Comfortable"),
	ETS_Hot					UMETA(DisplayName = "Hot"),
	ETS_Scortching			UMETA(DisplayName = "Scortching"),
};

UENUM(BlueprintType)
enum class EWetnessState : uint8 {

	EWS_Dry			UMETA(DisplayName = "Freezing"),
	EWS_Damp		UMETA(DisplayName = "Cold"),
	EWS_Wet		    UMETA(DisplayName = "Comfortable"),
	EWS_Soaked		UMETA(DisplayName = "Hot"),

};




UCLASS()
class SRPG_API UEnvironmentalAttributes : public UBaseAttributeComponent
{
	GENERATED_BODY()

public:

	UEnvironmentalAttributes();

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Enviornmental Attribute")
	void AdjustHotResitance(float Adjustment);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Enviornmental Attribute")
	void AdjustColdResitance(float Adjustment);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Enviornmental Attribute")
	void AdjustWetnessResitance(float Adjustment);

protected:

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintNativeEvent, Category = "Enviornmental Attribute")
	void SampleEnviornment(float SampledTemperature, float SampledWetness);

	virtual void Internal_SampleEnviornment(float SampledTemperature, float SampledWetness);

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Enviornmental Attribute")
	float CurrentTemperature;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Enviornmental Attribute")
	float TargetTemperature;

	//Should be between 0 and 1.  How much resitance to the current temperature increasing to toward the target temperature. 
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Enviornmental Attribute")
	float HeatResistance;

	//Should be between 0 and 1.  How much resitance to the current temperature raising to toward the target temperature.  
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Enviornmental Attribute")
	float ColdResistance;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Enviornmental Attribute")
	float CurrentWetness;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Enviornmental Attribute")
	float TargetWetness; 

	//Should be between 0 and 1.  How much resitance to the current wetness moving toward the target wetness. 
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Enviornmental Attribute")
	float WetnessResistance;

	//Should be between 0 and 1
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enviornmental Attribute", meta = (ClampMin = "0.0", ClampMax = "1.0"))
	float BaseHeatResitance = .1;

	//Should be between 0 and 1
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enviornmental Attribute", meta = (ClampMin = "0.0", ClampMax = "1.0"))
	float BaseColdResitance = .1;

	//Should be between 0 and 1
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enviornmental Attribute", meta = (ClampMin = "0.0", ClampMax = "1.0"))
	float BaseWetnessResitance = .1;

	//All temps in C.  If temp is less than Cold temp..
	//UPROPERTY(EditDeftaulsOnly, BlueprintReadOnly, Catrogy = "Enviornmental Attribute | Temperature Config")
	//float BaseFreezingTemp;

	//All temps in C. If temp is between cold base and comfortable base.  Freezing if less than this number.  
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enviornmental Attribute | Temperature Config")
	float BaseColdTemp = 0.f;

	//All temps in C. If temp is between Comfortable base and hot base
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enviornmental Attribute | Temperature Config")
	float BaseComfortableTemp = 15.f;

	//All temps in C. If temp is between hot base and Scortching base
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enviornmental Attribute | Temperature Config")
	float BaseHotTemp = 25.f;

	//All temps in C. If temp is greater than scortching base
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enviornmental Attribute | Temperature Config")
	float BaseScortchingTemp = 35.f;

	//On a scale of 0 to 100
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enviornmental Attribute | Wetness Config")
	float BaseDryLevel = 0;

	//Damp if greater than this number
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enviornmental Attribute | Wetness Config")
	float BaseDampLevel = 1;

	//Wet if greater than this number
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enviornmental Attribute | Wetness Config")
	float BaseWetLevel = 20;

	//Soaked if greater than this number
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enviornmental Attribute | Wetness Config")
	float BaseSoakedLevel = 50;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Enviornmental Attribute")
		ETemperatureState CurrentTempState;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Enviornmental Attribute")
		EWetnessState CurrentWetnessState;

private:

	void AdjustTemperature(float SampledTemperature);

	void AdjustWetness(float SampledWetness);

	void SetTempState();

	void SetWetnessState();

};
