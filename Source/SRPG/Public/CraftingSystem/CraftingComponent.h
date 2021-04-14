// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CraftingData.h"
#include "CraftingComponent.generated.h"


UCLASS( ClassGroup=(Custom), blueprintable, meta=(BlueprintSpawnableComponent) )
class SRPG_API UCraftingComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCraftingComponent();

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	void SetAssociatedInputInventories(TArray<class UInventoryContainer*> InputInventories);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	void SetCraftingRecipes(TArray<FCraftingRecipe> CraftingRecipes);

	UFUNCTION(BlueprintPure, Category = "Crafting Information")
	TArray<FCraftingRecipe> GetCraftingRecipes();

protected:

	bool CanRecipeBeCrafted(FCraftingRecipe Recipe);

	bool IsItemPartOfRecipe(FItemData Item, FCraftingRecipe Recipe);

	void GetQuantityOfIngredientFromInventory(FCraftingPart Ingredient, UInventoryContainer* TargetInventory, int32& OutQuantityFound);
	
	void GetTotalQuantityOfIngredient(FCraftingPart Ingredient, int32& OutQuantityFound);

	UPROPERTY(BlueprintReadOnly, Replicated, Category = "References")
	TArray<class UInventoryContainer*> AssociatedInputInventories;

	UPROPERTY(BlueprintReadOnly, Replicated, Category = "References")
	TArray<FCraftingRecipe> CraftableRecipes;

};
