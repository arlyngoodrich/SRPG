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


UENUM(BlueprintType)
enum class EBuildingTypes : uint8 {

	BTE_None			UMETA(DisplayName = "None"),
	BTE_Foundation		UMETA(DisplayName = "Foundation"),
	BTE_Wall			UMETA(DisplayName = "Wall"),
	BTE_Pilar			UMETA(DisplayName = "Pilar"),
	BTE_Beam			UMETA(DisplayName = "Beam"),
	BTE_Floor			UMETA(DisplayName = "Floor"),
	BTE_Roof			UMETA(DisplayName = "Roof"),
};



UENUM(BlueprintType)
enum class EBuildingConnectionLevel : uint8 {

	BCLE_Foundation		UMETA(DisplayName = "Foundation"),
	BCLE_First			UMETA(DisplayName = "First Level"),
	BCLE_Second			UMETA(DisplayName = "Second Level"),
	BCLE_Third			UMETA(DisplayName = "Third Level"),
	BCLE_Fourth			UMETA(DisplayName = "Fourth Level"),
	BCLE_TooFar			UMETA(DisplayName = "Too Far"),
	BCLE_None			UMETA(DisplayName = "None")

};


USTRUCT(BlueprintType)
struct FBuildingCost : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Building Piece Data")
	FName ResourceName;

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