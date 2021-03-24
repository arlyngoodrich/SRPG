// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "BuildingData.generated.h"

class ABuildingPiece;


UENUM(BlueprintType)
enum class EBuildingPieceStatus : uint8 {

	BPSE_Spawning       UMETA(DisplayName = "Spawning"),
	BPSE_Set			UMETA(DisplayName = "Set"),
	BPSE_FullyBuilt     UMETA(DisplayName = "Fully Built"),
};


USTRUCT(BlueprintType)
struct FBuildingCost : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Building Piece Data")
	FName BuildingPieceName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Building Piece Data", meta = (ClampMin = 0))
	int32 Quantity;

};




USTRUCT(BlueprintType)
struct FBuildingPieceData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

		UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Building Piece Data")
		FName BuildingPieceName;

		UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Building Piece Data")
		TSubclassOf<ABuildingPiece> InWorldActorClass;

		UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Building Piece Data")
		TArray<FBuildingCost> BuildingCosts;

};