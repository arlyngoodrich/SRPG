// Fill out your copyright notice in the Description page of Project Settings.


#include "AttributeSystem/AttributeEffect.h"
#include "AttributeSystem/BaseAttributeComponent.h"
#include "LogFiles.h"

void UAttributeEffect::InitalizeEffect(class UBaseAttributeComponent* SetSpawningAttributeComponent, class UBaseAttributeComponent* SetTargetAttributeComponent)
{
	if (SetSpawningAttributeComponent == nullptr || SetTargetAttributeComponent == nullptr)
	{
		UE_LOG(LogAttributeSystem, Error, TEXT("Attribute component cannot be negative for attribute effect %s"), *this->GetName())
			return;
	}

	SpawningAttributeComponent = SetSpawningAttributeComponent;
	TargetAttributeComponent = SetTargetAttributeComponent;
	GetWorld();
	CheckInEffectToAttribute();
	OnEffectTriggered();
	Internal_OnEffectTriggered();
}



void UAttributeEffect::CheckInEffectToAttribute()
{
	if (TargetAttributeComponent == nullptr)
	{
		UE_LOG(LogAttributeSystem, Error, TEXT("Attribute Component is null for %s"), *this->GetName())
			return;
	}
	TargetAttributeComponent->CheckInEffect(this);

}

void UAttributeEffect::Internal_OnEffectTriggered()
{
}

void UAttributeEffect::CheckOutEffectFromAttribute()
{
	TargetAttributeComponent->CheckOutEffect(this);
}
