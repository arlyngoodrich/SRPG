// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "InventorySystem/ItemData.h"
#include "CraftingData.generated.h"


USTRUCT(BlueprintType)
struct FCraftingPart 
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Recipe Component Info")
	TSubclassOf<class AItemBase> InWorldActorClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Recipe Component Info")
	int32 StackQuantity;

};

USTRUCT(BlueprintType)
struct FCraftingFuel
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Fuel Info")
	FCraftingPart FuelIngredient;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Fuel Info")
	float TimeToConsumeOne = 1.f;

};

UENUM(BlueprintType)
enum class ERecipeCategory : uint8 {

	ECC_None			UMETA(DisplayName = "None"),
	ECC_Tools			UMETA(DisplayName = "Tools"),
	ECC_Weapons			UMETA(DisplayName = "Weapons"),
	ECC_Equipment		UMETA(DisplayName = "Equipment"),
	ECC_Consumables		UMETA(DisplayName = "Consumables"),
};


UENUM(BlueprintType)
enum class ECraftingStationLevel : uint8 {

	ECSL_None			UMETA(DisplayName = "None"),
	ECSL_Primitive		UMETA(DisplayName = "Primitive"),
	ECSL_Basic			UMETA(DisplayName = "Basic"),
	ECSL_Advanced		UMETA(DisplayName = "Advanced"),
};

UENUM(BlueprintType)
enum class ECraftingStationType : uint8 {

	ECST_None			UMETA(DisplayName = "None"),
	ECST_Primitive		UMETA(DisplayName = "Primitive"),
	ECST_Wood			UMETA(DisplayName = "Basic"),
	ECST_Metal			UMETA(DisplayName = "Advanced"),
	ECST_Consumable		UMETA(DisplayName = "Consumable"),
	ECST_Equipment		UMETA(DisplayName = "Equipment"),
};


USTRUCT(BlueprintType)
struct FCraftingRecipe : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recipe Info")
	FName DisplayName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recipe Info")
	FText Discription;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recipe Info")
	UTexture2D* RecipeIcon = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recipe Info")
	ERecipeCategory RecipeCategory;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recipe Info")
	TArray<ECraftingStationType> EligibleCraftingStations;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recipe Info")
	TArray<ECraftingStationLevel> EligibleCraftingStationLevels;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recipe Info")
	TArray<FCraftingPart> RecipeInputs;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recipe Info")
	TArray<FCraftingPart> RecipeOutputs;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recipe Info")
	float DefaultCraftingTime = 1.f;

};