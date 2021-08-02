// Fill out your copyright notice in the Description page of Project Settings.


#include "AttributeSystem/AttributeEffect.h"
#include "AttributeSystem/BaseAttributeComponent.h"
#include "LogFiles.h"


void UAttributeEffect::InitalizeEffect(class UBaseAttributeComponent* SpawningComponent)
{
	OriginatingComponent = SpawningComponent;
}

void UAttributeEffect::OnEffectDeactivated()
{
	if (OriginatingComponent == nullptr) { UE_LOG(LogAttributeSystem, Error, TEXT("Attribute Effect Originating Component is Null")) return; }

	
}
