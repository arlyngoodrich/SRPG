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
	EBuildingTypes BuildingType;

	UPROPERTY(SaveGame, Replicated, EditDefaultsOnly, BlueprintReadOnly, Category = "Building Piece Data")
	EBuildingPieceStatus BuildStatus;

	UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadOnly, Category = "Building Piece Data")
	EBuildingConnectionLevel ConnectionLevel;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Building Piece Data")
	bool bRequiresTerrain;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Building Piece Data", meta = (EditCondition = "!bRequiresTerrain"))
	bool bRequiresBuildingPiece;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Building Piece Data", meta = (EditCondition = "bRequiresBuildingPiece"))
	bool bRequiresBuiltBuildingPiece;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Building Piece Data", meta = (EditCondition = "bRequiresBuildingPiece"))
	bool bRequiresSnap;

public:
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Building System", DisplayName = "Update Piece Build Status")
	void BP_UpdateBuildStatus(EBuildingPieceStatus NewBuildStatus);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Building System", DisplayName = "Set Connection Level")
	void BP_SetConnectionLevel(EBuildingConnectionLevel NewConnectionLevel);

	UFUNCTION(BlueprintCallable, Category = "Building System", DisplayName = "Incrament Connection Level")
	void BP_IncreaseConnectionLevel(EBuildingConnectionLevel InConnectionLevel, EBuildingConnectionLevel& OutConnecitonLevel);
	

protected:
	UFUNCTION(BlueprintImplementableEvent, BlueprintAuthorityOnly, Category = "Building System", DisplayName = "Build Status Changed to Set")
	void BP_OnBuildStatusChangeToSet();

	UFUNCTION(BlueprintImplementableEvent, BlueprintAuthorityOnly, Category = "Building System", DisplayName = "Build Status Changed to Built")
	void BP_OnBuildStatusChangeToBuilt();

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Building System")
	void SetBuildStatusFromLoad(EBuildingPieceStatus LoadedBuildStatus);

private:
	void UpdateBuildStatus(EBuildingPieceStatus NewBuildStatus);

	void SetConnectionLevel(EBuildingConnectionLevel NewConnectionLevel);

};
