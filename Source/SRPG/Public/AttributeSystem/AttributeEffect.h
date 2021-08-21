// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "AttributeEffect.generated.h"

/**
 * 
 */
UCLASS(blueprintable, BlueprintType)
class SRPG_API UAttributeEffect : public UObject
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintCallable,BlueprintAuthorityOnly, Category = "Attribute Effect")
	void InitalizeEffect(class UBaseAttributeComponent* SetSpawningAttributeComponent, class UBaseAttributeComponent* SetTargetAttributeComponent);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attribute Effect")
	UTexture2D* EffectIcon;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attribute Effect")
	FColor IconColor;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attribute Effect")
	FColor BackgroundColor;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attribute Effect")
	FName EffectName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attribute Effect")
	FText EffectDescription;


protected:

	
	UPROPERTY(BlueprintReadOnly, Category = "Attribute Effect")
	class UBaseAttributeComponent* TargetAttributeComponent;

	UPROPERTY(BlueprintReadOnly, Category = "Attribute Effect")
	class UBaseAttributeComponent* SpawningAttributeComponent;

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Attribute Effect")
	void CheckInEffectToAttribute();

	//Attribute Component should be valid when this is triggered and should be ready for BP implementation of effect logic. 
	UFUNCTION(BlueprintImplementableEvent, Category = "Attribute Effect")
	void OnEffectTriggered();

	virtual void Internal_OnEffectTriggered();

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Attribute Effect")
	void CheckOutEffectFromAttribute();

};
