// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FoliageManager.generated.h"

USTRUCT(BlueprintType)
struct FFoliageInstanceData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Foliage Instance Data")
	TArray<UStaticMesh*> StaticMeshes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Foliage Instance Data")
	TArray<FTransform> Transforms;

};

UCLASS()
class SRPG_API AFoliageManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFoliageManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Foliage Manager")
	void UpdateClientInstances(FFoliageInstanceData InstanceData);

	UFUNCTION(BlueprintImplementableEvent, Category = "Foliage Manager")
	void OnUpdateClientInstances(FFoliageInstanceData InstanceData);

	UFUNCTION(Client, Reliable)
	void Client_InternaUpdateClientInstances(FFoliageInstanceData InstanceData);
	void Client_InternaUpdateClientInstances_Implementation(FFoliageInstanceData InstanceData);





public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
