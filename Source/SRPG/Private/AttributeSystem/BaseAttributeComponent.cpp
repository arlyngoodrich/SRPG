// Fill out your copyright notice in the Description page of Project Settings.


#include "AttributeSystem/BaseAttributeComponent.h"
#include "LogFiles.h"
#include "AttributeSystem/AttributeEffect.h"

//UE4 Includes
#include "Net/UnrealNetwork.h"

// Sets default values for this component's properties
UBaseAttributeComponent::UBaseAttributeComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


void UBaseAttributeComponent::CheckOutEffect(UAttributeEffect* Effect)
{
	if (AppliedEffects.Contains(Effect))
	{
		AppliedEffects.Remove(Effect);
		UE_LOG(LogAttributeSystem,Log,TEXT("%s attribute removed"),*Effect->GetName())
	}
	else
	{
		UE_LOG(LogAttributeSystem,Warning,TEXT("Failed to remove %s from %s"),*Effect->GetName(),*this->GetName())
	}
}

// Called when the game starts
void UBaseAttributeComponent::BeginPlay()
{
	Super::BeginPlay();

	bIsAuthority = (GetOwnerRole() == ROLE_Authority);
	
}


void UBaseAttributeComponent::ChangeAttributeAmount(FAttributeData InAttribute, float ChangeAmount, FAttributeData& OutAttribute)
{
	if (!bIsAuthority) { UE_LOG(LogAttributeSystem, Warning, TEXT("Cannot change attribute from client")) return;  }

	OutAttribute = InAttribute;

	//set attribute to new level
	float NewAttributeAmount = 0.f;
	float OldAttributeAmount = InAttribute.AttributeCurrentAmount;

	NewAttributeAmount = FMath::Clamp(OldAttributeAmount + ChangeAmount, 0.f, InAttribute.AttributeMaxAmount);
	OutAttribute.AttributeCurrentAmount = NewAttributeAmount;

	//cycle throgh attribute effect triggers
	TArray<FAttributeEffectTrigger> EffectTriggers;
	EffectTriggers = InAttribute.AtributeTriggers;

	for (int32 i = 0; i < EffectTriggers.Num(); i++)
	{
		//create effect if valid
		FAttributeEffectTrigger ActiveEffectTrigger;
		ActiveEffectTrigger = EffectTriggers[i];

		if (ActiveEffectTrigger.bApplyWhenGreater && ActiveEffectTrigger.GreaterThanAmount < NewAttributeAmount)
		{
			//Make sure trigger not already active
			if (!CheckIfEffectIsAlreadyActive(ActiveEffectTrigger.AttributeEffect))
			{
				TSubclassOf<UAttributeEffect> OBjectClass = ActiveEffectTrigger.AttributeEffect->StaticClass();
				
				//Ensure valid attribute effect
				if (!OBjectClass) { UE_LOG(LogAttributeSystem, Error, TEXT("Attempting to create invalid attribute effect")) return; }

				UAttributeEffect* NewEffect = NewObject<UAttributeEffect>(this, OBjectClass);
				AppliedEffects.Add(NewEffect);
			}

		}
		if (ActiveEffectTrigger.bApplyWhenLessThan && ActiveEffectTrigger.LessThanAmount > NewAttributeAmount)
		{
			//Make sure trigger not already active
			if (!CheckIfEffectIsAlreadyActive(ActiveEffectTrigger.AttributeEffect))
			{
				TSubclassOf<UAttributeEffect> OBjectClass = ActiveEffectTrigger.AttributeEffect->StaticClass();

				//Ensure valid attribute effect
				if (!OBjectClass) { UE_LOG(LogAttributeSystem, Error, TEXT("Attempting to create invalid attribute effect")) return; }

				UAttributeEffect* NewEffect = NewObject<UAttributeEffect>(this, OBjectClass);
				AppliedEffects.Add(NewEffect);
			}
		}

	}


}

bool UBaseAttributeComponent::CheckIfEffectIsAlreadyActive(UClass* EffectClass)
{
	for (int32 i = 0; i < AppliedEffects.Num(); i++)
	{
		if (EffectClass == AppliedEffects[i]->GetClass())
		{
			return true;
		}
	}

	return false;
}
