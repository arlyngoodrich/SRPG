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



// Called when the game starts
void UBaseAttributeComponent::BeginPlay()
{
	Super::BeginPlay();
	
}

void UBaseAttributeComponent::Client_InternalAddEffectToUI_Implementation(UTexture2D* IconTexutre, FColor IconColor, FColor BackgroundColor, FName EffectName, const FText& Discription)
{
	AddEffectToUI(IconTexutre, IconColor, BackgroundColor, EffectName, Discription);
}

void UBaseAttributeComponent::Client_InternalRemoveEffectFromUI_Implementation(FName EffectName)
{
	RemoveEffectFromUI(EffectName);
}



void UBaseAttributeComponent::CheckInEffect(UAttributeEffect* AttributeEffect)
{

	if (AttributeEffect == nullptr)
	{
		UE_LOG(LogAttributeSystem, Warning, TEXT("Attempted to check in null effect on %s"), *GetOwner()->GetName())
		return;
	}

	if (GetOwnerRole() != ROLE_Authority)
	{
		UE_LOG(LogAttributeSystem, Warning, TEXT("Cannot check in attribute on client.  Owner: %s | Effect: %s"), *GetOwner()->GetName(), *AttributeEffect->GetName())
		return;
	}

	ActiveEffects.Add(AttributeEffect);
	Client_InternalAddEffectToUI(AttributeEffect->EffectIcon, AttributeEffect->IconColor, AttributeEffect->BackgroundColor, AttributeEffect->EffectName, AttributeEffect->EffectDescription);
	UE_LOG(LogAttributeSystem,Log,TEXT("%s effect checked into %s"),*AttributeEffect->GetName(),*GetOwner()->GetName())
}

void UBaseAttributeComponent::CheckOutEffect(UAttributeEffect* AttributeEffect)
{

	if (AttributeEffect == nullptr)
	{
		UE_LOG(LogAttributeSystem, Warning, TEXT("Attempted to check out null effect on %s"), *GetOwner()->GetName())
			return;
	}

	if (GetOwnerRole() != ROLE_Authority)
	{
		UE_LOG(LogAttributeSystem, Warning, TEXT("Cannot check out attribute on client.  Owner: %s | Effect: %s"), *GetOwner()->GetName(), *AttributeEffect->GetName())
			return;
	}


	if (ActiveEffects.Contains(AttributeEffect))
	{
		UE_LOG(LogAttributeSystem, Log, TEXT("%s effect checked out from %s"), *AttributeEffect->GetName(), *GetOwner()->GetName())
		Client_InternalRemoveEffectFromUI(AttributeEffect->EffectName);
		ActiveEffects.Remove(AttributeEffect);
	}
	else
	{
		UE_LOG(LogAttributeSystem,Warning,TEXT("%s is not found as valid effect for %s.  Cannot remove"),*AttributeEffect->GetName(),*GetOwner()->GetName())
	}
}
