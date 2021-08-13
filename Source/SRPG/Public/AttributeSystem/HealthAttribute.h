// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSystem/BaseAttributeComponent.h"
#include "HealthAttribute.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDeath);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHealthChange, float, ChangeInHealth, float, NewHealth);


USTRUCT(BlueprintType)
struct FHealthChangeData
{
	GENERATED_USTRUCT_BODY()

	//Current ammount of the attribute
	UPROPERTY(BlueprintReadOnly)
	float PreviousHealth;

	UPROPERTY(BlueprintReadOnly)
	float NewHealth;

	UPROPERTY(BlueprintReadOnly)
	float HealthChangeAmount;
};


/**
 * 
 */
UCLASS(ClassGroup = (Attributes), blueprintable, meta = (BlueprintSpawnableComponent))
class SRPG_API UHealthAttribute : public UBaseAttributeComponent
{
	GENERATED_BODY()

public:

	UHealthAttribute();

	//On Actor take any damange signature
	UFUNCTION()
	virtual void OnOwnerTakeDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

	//Modify amount is added to 1 and muliplied to the base regen amount
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Attribute Effect")
	void ModifyHealthReGenAmount(float ModifyAmount);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Attribute Effect")
	void AddHealth(float HealthAmount);

	UPROPERTY(BlueprintAssignable)
	FOnDeath Death_OnOwnerDeath;

	UPROPERTY(BlueprintAssignable)
	FOnHealthChange Health_OnHealthChange;

protected:

	// Called when the game starts
	virtual void BeginPlay() override;

	//BP Event that runs on client and server when health reaches 0
	UFUNCTION(BlueprintImplementableEvent, Category = "Health", meta = (DisplayName = "On Death"))
	void BP_OnDeath();

	//BP Event that runs on client and server 
	UFUNCTION(BlueprintImplementableEvent, Category = "Health", meta = (DisplayName = "On Health Change"))
	void BP_OnHealthChange(float ChangeAmount, float NewHealth);

	//Overridable function triggered when owning actor dies.  Runs on client and server.
	UFUNCTION()
	virtual void OnRep_IsDead();

	//Overridable function triggered when health changes.  Runs on client and server. 
	UFUNCTION()
	virtual void OnRep_HealthChange();

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Health")
	float MaxHealth;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Health")
	float CurrentHealth;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Health")
	bool bShouldReGenHealth;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Health", meta = (EditCondition = "bShouldReGenHealth"))
	float BaseHealthRegenAmount = 1.f;

	UPROPERTY(BlueprintReadOnly, Category = "Health")
	float ReGenModifer = 1.f;

	UPROPERTY(ReplicatedUsing = OnRep_HealthChange, BlueprintReadOnly, Category = "Health");
	FHealthChangeData HealthChangeData;

	UPROPERTY(ReplicatedUsing = OnRep_IsDead, BlueprintReadOnly, Category = "Health")
	bool bIsDead;


private:

	void ChangeHealth(float ChangeAmount);

	void StartReGeneratingHealth();

	void ReGenHealth();

	FTimerHandle HealthRegenerationTimerHandle;

};
