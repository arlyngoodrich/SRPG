// Fill out your copyright notice in the Description page of Project Settings.


#include "CraftingSystem/CraftingComponent.h"
#include "LogFiles.h"
#include "InventorySystem/InventoryContainer.h"
#include "ItemSystem/ItemBase.h"


#include "Net/UnrealNetwork.h"

// Sets default values for this component's properties
UCraftingComponent::UCraftingComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


void UCraftingComponent::SetAssociatedInputInventories(TArray<UInventoryContainer*> InputInventories) {AssociatedInputInventories = InputInventories;}
void UCraftingComponent::SetCraftingRecipes(TArray<FCraftingRecipe> CraftingRecipes) {CraftableRecipes = CraftingRecipes;}

TArray<FCraftingRecipe> UCraftingComponent::GetCraftingRecipes() {return CraftableRecipes;}


void UCraftingComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty >& OutLifetimeProps) const
{

	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UCraftingComponent, AssociatedInputInventories);
	DOREPLIFETIME(UCraftingComponent, CraftableRecipes);

}

bool UCraftingComponent::CanRecipeBeCrafted(FCraftingRecipe Recipe)
{

	TArray<FCraftingPart> Inputs = Recipe.RecipeInputs;
	TArray<bool> EnoughIngredients;

	//Go through each recipe input and check to see if there is enough ingredients;
	for (int32 Index = 0; Index != Inputs.Num(); Index++)
	{
		FCraftingPart ActiveIngredient = Inputs[Index];
		int32 QuantityNeeded = ActiveIngredient.StackQuantity;
		int32 QuantityFound = 0;

		GetTotalQuantityOfIngredient(ActiveIngredient, QuantityFound);

		//Check if enough of the active ingredent was found,
		if (QuantityFound >= ActiveIngredient.StackQuantity)
		{
			EnoughIngredients.Add(true);
		}
		else
		{
			EnoughIngredients.Add(false);
		}
	}

	// If there is not enough of one ingredient, return false
	if (EnoughIngredients.Contains(false))
	{
		return false;
	}
	else
	{
		return true;
	}

}


void UCraftingComponent::GetTotalQuantityOfIngredient(FCraftingPart Ingredient, int32& OutQuantityFound)
{
	int32 QuantityFound = 0;
	for (int32 Index = 0; Index != AssociatedInputInventories.Num(); Index++)
	{
		UInventoryContainer* ActiveInventory = nullptr;
		ActiveInventory = AssociatedInputInventories[Index];
		int32 InventoryQuantity;

		GetQuantityOfIngredientFromInventory(Ingredient, ActiveInventory, InventoryQuantity);

		QuantityFound += InventoryQuantity;
	}

	OutQuantityFound = QuantityFound;
}

void UCraftingComponent::GetQuantityOfIngredientFromInventory(FCraftingPart Ingredient, UInventoryContainer* TargetInventory, int32& OutQuantityFound)
{

	//Make sure inventory isn't null
	if (TargetInventory != nullptr)
	{
		TArray<FItemData> InventoryItems;
		TArray<int32> PosX;
		TArray<int32> PosY;

		TargetInventory->GetInventoryData(InventoryItems, PosX, PosY);

		//Find matching items in inventory to needed ingredient
		for (int32 ITEM_Index = 0; ITEM_Index != InventoryItems.Num(); ITEM_Index++)
		{
			FItemData TargetItem;
			TargetItem = InventoryItems[ITEM_Index];

			UClass* ItemClass = nullptr;
			UClass* InputClass = nullptr;

			ItemClass = TargetItem.InWorldActorClass.Get();
			InputClass = Ingredient.InWorldActorClass.Get();

			//If it is the ingredient we are looking for, add to quantity found
			if (ItemClass == InputClass)
			{
				OutQuantityFound += TargetItem.StackQuantity;

			}
		}
	}
}

bool UCraftingComponent::IsItemPartOfRecipe(FItemData Item, FCraftingRecipe Recipe)
{

	TArray<FCraftingPart> Inputs = Recipe.RecipeInputs;

	for (int32 Index = 0; Index != Inputs.Num(); Index++)
	{
		UClass* ItemClass = nullptr;
		UClass* InputClass = nullptr;
		ItemClass = Item.InWorldActorClass.Get();
		InputClass = Inputs[Index].InWorldActorClass.Get();

		if (ItemClass == InputClass)
		{
			return true;
		}
	}

	return false;
}

