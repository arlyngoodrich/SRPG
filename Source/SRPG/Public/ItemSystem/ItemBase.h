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

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Item Data", meta = (ExposeOnSpawn = "true"))
	FItemData ItemData;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Item Information")
	void UpdateItemInformation(FItemData NewItemData);

};
