// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BuildingData.h"
#include "BuildingPiece.generated.h"

UCLASS(ClassGroup = (BuildingSystem))
class SRPG_API ABuildingPiece : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABuildingPiece();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Building Piece Data")
	FBuildingPieceData BuildingPieceData;

	UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadOnly, Category = "Building Piece Data")
	EBuildingPieceStatus BuildStatus;

public:
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Building System", DisplayName = "Update Piece Build Status")
	void BP_UpdateBuildStatus(EBuildingPieceStatus NewBuildStatus);

protected:
	UFUNCTION(BlueprintImplementableEvent, BlueprintAuthorityOnly, Category = "Building System", DisplayName = "Build Status Changed to Set")
	void BP_OnBuildStatusChangeToSet();

	UFUNCTION(BlueprintImplementableEvent, BlueprintAuthorityOnly, Category = "Building System", DisplayName = "Build Status Changed to Built")
	void BP_OnBuildStatusChangeToBuilt();

private:
	void UpdateBuildStatus(EBuildingPieceStatus NewBuildStatus);

};
