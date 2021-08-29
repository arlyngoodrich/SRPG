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
	BTE_Stair			UMETA(DisplayName = "Stair"),
	BTE_RoofCornerOut	UMETA(DisplayName = "Roof Corner Out"),
	BTE_RoofCornerIn	UMETA(DisplayName = "Roof Corner In"),
	BTE_Fireplace		UMETA(DisplayName = "Fireplace"),
	BTE_Field			UMETA(DisplayName = "Field"),
	BTE_Crop			UMETA(DisplayName = "Crop"),
	BTE_Stockpile		UMETA(DisplayName = "Stockpile"),
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
	TSubclassOf<class AItemBase> ResourceItem;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Building Piece Data", meta = (ClampMin = 0))
	int32 Quantity;

};





USTRUCT(BlueprintType)
struct FBuildingPieceData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

		UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Building Piece Data")
		FName BuildingDisplayName;

		UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Building Piece Data")
		UTexture2D* BuildingIcon;

		UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Building Piece Data")
		FColor IconBackgroundColor;

		UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Building Piece Data")
		FText Description;

		UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Building Piece Data")
		bool bCanPullResourceFromStockPile;

		UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Building Piece Data")
		TArray<FBuildingCost> BuildingCosts;

};