// Fill out your copyright notice in the Description page of Project Settings.


#include "AttributeSystem/HealthAttribute.h"
#include "LogFiles.h"

//UE4 Includes
#include "Net/UnrealNetwork.h"
#include "..\..\Public\AttributeSystem\HealthAttribute.h"

UHealthAttribute::UHealthAttribute()
{
	MaxHealth = 100.f;
}


void UHealthAttribute::GetLifetimeReplicatedProps(TArray<FLifetimeProperty >& OutLifetimeProps) const
{

	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UHealthAttribute, CurrentHealth);
	DOREPLIFETIME(UHealthAttribute, bIsDead);
	DOREPLIFETIME(UHealthAttribute, HealthChangeData);
	
}


// Called when the game starts
void UHealthAttribute::BeginPlay()
{
	Super::BeginPlay();

	AActor* MyOwner = GetOwner();
	if (!MyOwner) { UE_LOG(LogAttributeSystem, Error, TEXT("Health component could not get valid owner")) return;  }

	MyOwner->OnTakeAnyDamage.AddDynamic(this, &UHealthAttribute::OnOwnerTakeDamage);

	CurrentHealth = MaxHealth;
}

void UHealthAttribute::OnOwnerTakeDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	UE_LOG(LogAttributeSystem, Log, TEXT("%s took %s damage"), *GetOwner()->GetName(), *FString::SanitizeFloat(Damage))

}


void UHealthAttribute::ChangeHealth(float ChangeAmount)
{
	if (GetOwnerRole() != ROLE_Authority) 
	{ 
		UE_LOG(LogAttributeSystem, Warning, TEXT("%s :Cannot change health on client"), *GetOwner()->GetName())  
		return;  
	}

	float PreviousHealth;
	PreviousHealth = CurrentHealth;

	CurrentHealth = FMath::Clamp(CurrentHealth + ChangeAmount, 0.f, MaxHealth);
	
	HealthChangeData.NewHealth = CurrentHealth;
	HealthChangeData.PreviousHealth = PreviousHealth;
	HealthChangeData.HealthChangeAmount = CurrentHealth - PreviousHealth;

	OnRep_HealthChange();

	if (CurrentHealth == 0.f)
	{
		bIsDead = true;
		OnRep_IsDead();
	}

}


void UHealthAttribute::OnRep_HealthChange()
{
	UE_LOG(LogAttributeSystem, Log, TEXT("%s new health is: %s"), *GetOwner()->GetName(), *FString::SanitizeFloat(CurrentHealth))
	Health_OnHealthChange.Broadcast(HealthChangeData.HealthChangeAmount,HealthChangeData.NewHealth);
	BP_OnHealthChange(HealthChangeData.HealthChangeAmount, HealthChangeData.NewHealth);
}


void UHealthAttribute::OnRep_IsDead()
{
	UE_LOG(LogAttributeSystem, Log, TEXT("%s has died"), *GetOwner()->GetName())
	BP_OnDeath();
}



