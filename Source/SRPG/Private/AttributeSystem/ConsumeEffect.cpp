// Fill out your copyright notice in the Description page of Project Settings.


#include "AttributeSystem/ConsumeEffect.h"
#include "AttributeSystem/MetabolismAttribute.h"
#include "LogFiles.h"

void UConsumeEffect::Internal_OnEffectTriggered()
{
	Super::Internal_OnEffectTriggered();

	UMetabolismAttribute* MetabolismAttribute;

	if (Cast<UMetabolismAttribute>(TargetAttributeComponent))
	{
		MetabolismAttribute = Cast<UMetabolismAttribute>(TargetAttributeComponent);
		MetabolismAttribute->OnConsume(CarbsChange, VitaminsChange, ProteinChange, WaterChange);

	}
	else
	{
		UE_LOG(LogAttributeSystem,Error,TEXT("%s : Conusmption Effect requries target component to be metabolism"), *this->GetName())
	}

	CheckOutEffectFromAttribute();

}