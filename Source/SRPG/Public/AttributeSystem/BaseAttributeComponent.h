// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AttributeData.h"
#include "BaseAttributeComponent.generated.h"


UCLASS( ClassGroup=(Attributes))
class SRPG_API UBaseAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UBaseAttributeComponent();

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Attribute Effects")
	void CheckInEffect(class UAttributeEffect* AttributeEffect);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Attribute Effects")
	void CheckOutEffect(class UAttributeEffect* AttributeEffect);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	
	UFUNCTION(Client, Reliable)
	void Client_InternalAddEffectToUI(UTexture2D* IconTexutre, FColor IconColor, FColor BackgroundColor, FName EffectName,const FText& Discription);
	void Client_InternalAddEffectToUI_Implementation(UTexture2D* IconTexutre, FColor IconColor, FColor BackgroundColor, FName EffectName, const FText& Discription);

	UFUNCTION(Client, Reliable)
	void Client_InternalRemoveEffectFromUI(FName EffectName);
	void Client_InternalRemoveEffectFromUI_Implementation(FName EffectName);


	UFUNCTION(BlueprintImplementableEvent, Category = "Attribute Effects")
	void AddEffectToUI(UTexture2D* IconTexutre, FColor IconColor, FColor BackgroundColor, FName EffectName, const FText& Discription);

	UFUNCTION(BlueprintImplementableEvent, Category = "Attribute Effects")
	void RemoveEffectFromUI(FName EffectName);

	UPROPERTY()
	TArray<class UAttributeEffect*> ActiveEffects;

};
