// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InventorySystem/ItemData.h"
#include "ItemBase.generated.h"

UCLASS()
class SRPG_API AItemBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItemBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadOnly, Category = "Item Data")
	FItemData ItemData;

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Item Information")
	void UpdateItemInformation(FItemData NewItemData);

};