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

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Crafting Setup")
	void SetAssociatedInputInventories(TArray<class UInventoryContainer*> InputInventories);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Crafting Setup")
	void SetAssociatedOutputInventories(TArray<class UInventoryContainer*> OutputInventories);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Crafting Setup")
	void SetCraftingRecipes(TArray<FCraftingRecipe> CraftingRecipes);

	UFUNCTION(BlueprintCallable, Category = "Crafting")
	void CraftRecipe(FCraftingRecipe Recipe);

	UFUNCTION(BlueprintCallable, Category = "Crafting")
	bool CanRecipeBeCrafted(FCraftingRecipe Recipe);

	UFUNCTION(BlueprintPure, Category = "Crafting Information")
	TArray<FCraftingRecipe> GetCraftingRecipes();

	UFUNCTION(BlueprintPure, Category = "Crafting Information")
	bool GetIsFuelAvailable();

	UFUNCTION(BlueprintImplementableEvent, Category = "Crafting")
	void OnFuelBurnStart();

	UFUNCTION(BlueprintImplementableEvent, Category = "Crafting")
	void OnFuelBurnStop();

	UFUNCTION(BlueprintImplementableEvent, Category = "Crafting")
	void OnCraftStart(FCraftingRecipe RecipeBeingCrafted);

	UFUNCTION(BlueprintImplementableEvent, Category = "Crafting")
	void OnCraftFinish();


	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Configuration")
	bool bRequiresFuelToCraft;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Configuration")
	bool bAutoCraftsWhenFueled;
			

protected:

	UPROPERTY(BlueprintReadOnly, Replicated, Category = "Crafting Information")
	bool bIsFuelAvailable;

	UPROPERTY(EditDefaultsOnly, Category = "Configuration")
	TArray<FCraftingFuel> FuelTypes;

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_CraftRecipe(FCraftingRecipe Recipe);
	bool Server_CraftRecipe_Validate(FCraftingRecipe Recipe);
	void Server_CraftRecipe_Implementation(FCraftingRecipe Recipe);
	
	bool EnoughSpaceForCraftedRecipe(FCraftingRecipe Recipe);

	bool IsItemPartOfRecipe(FItemData Item, FCraftingRecipe Recipe);

	bool CheckIfIngredientIsAvailable(FCraftingPart Ingredient);

	void GetQuantityOfIngredientFromInventory(FCraftingPart Ingredient, UInventoryContainer* TargetInventory, int32& OutQuantityFound);
	
	void GetTotalQuantityOfIngredient(FCraftingPart Ingredient, int32& OutQuantityFound);

	void GetItemDataFromRecipe(FCraftingRecipe Recipe, TArray<FItemData>& OutInputItems, TArray<FItemData>& OutOutputItems);

	bool GetItemDataFromClass(UClass* Class, FItemData& OutItemData);

	bool Crafting_RemoveItems(FCraftingRecipe Recipe);

	bool RemoveIngredientFromInventories(FCraftingPart Ingredient);

	void Crafting_AddOutputs(FCraftingRecipe Recipe);

	void FinalizeCraft();

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Crafting Setup")
	void StartBuriningFuel();

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Crafting Setup")
	void StopBurningFuel();

	void BurnFuel(FCraftingFuel TargetFuel);

	bool IsFuelAvailable();

	bool bFuelIsBurning;

	void StartAutoCrafting();
	
	UPROPERTY(BlueprintReadOnly, Replicated, Category = "References")
	FCraftingRecipe ActiveRecipe;

	UPROPERTY(BlueprintReadOnly, Replicated, Category = "References")
	TArray<class UInventoryContainer*> AssociatedInputInventories;

	UPROPERTY(BlueprintReadOnly, Replicated, Category = "References")
	TArray<class UInventoryContainer*> AssociatedOutputInventories;

	UPROPERTY(BlueprintReadOnly, Replicated, Category = "References")
	TArray<FCraftingRecipe> CraftableRecipes;

	FTimerHandle CraftingTimer;

	FTimerHandle FuelBurnTimer;

	bool bIsCrafting;

};
