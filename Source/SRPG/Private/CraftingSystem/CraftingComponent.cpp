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
void UCraftingComponent::SetAssociatedOutputInventories(TArray<class UInventoryContainer*> OutputInventories){AssociatedOutputInventories = OutputInventories;}
void UCraftingComponent::SetCraftingRecipes(TArray<FCraftingRecipe> CraftingRecipes) {CraftableRecipes = CraftingRecipes;}

void UCraftingComponent::CraftRecipe(FCraftingRecipe Recipe)
{
	Server_CraftRecipe(Recipe);
}

TArray<FCraftingRecipe> UCraftingComponent::GetCraftingRecipes() {return CraftableRecipes;}


void UCraftingComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty >& OutLifetimeProps) const
{

	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UCraftingComponent, AssociatedInputInventories);
	DOREPLIFETIME(UCraftingComponent, AssociatedOutputInventories);
	DOREPLIFETIME(UCraftingComponent, CraftableRecipes);
	DOREPLIFETIME(UCraftingComponent, bIsFuelAvailable);
	DOREPLIFETIME(UCraftingComponent, ActiveRecipe);
}

bool UCraftingComponent::GetIsFuelAvailable()
{
	IsFuelAvailable();
	return bIsFuelAvailable;
}

bool UCraftingComponent::Server_CraftRecipe_Validate(FCraftingRecipe Recipe)
{
	return true;
}

void UCraftingComponent::Server_CraftRecipe_Implementation(FCraftingRecipe Recipe)
{

	if (bIsCrafting) { UE_LOG(LogCraftingSystem, Log, TEXT("Crafting in progress, please wait")) return; }
	if (bRequiresFuelToCraft && bFuelIsBurning == false) {UE_LOG(LogCraftingSystem,Log,TEXT("Cannot craft without fuel burning")) }


	bool bItemsRemoved = false;
	bItemsRemoved = Crafting_RemoveItems(Recipe);

	if (bItemsRemoved == false) { UE_LOG(LogCraftingSystem, Log, TEXT("Failed to remove input items for %s"), *Recipe.DisplayName.ToString()) return; }

	ActiveRecipe = Recipe;
	bIsCrafting = true;

	OnCraftStart(ActiveRecipe);
	GetWorld()->GetTimerManager().SetTimer(CraftingTimer, this, &UCraftingComponent::FinalizeCraft, Recipe.DefaultCraftingTime, false);

}

void UCraftingComponent::FinalizeCraft()
{
	Crafting_AddOutputs(ActiveRecipe);
	bIsCrafting = false;
	OnCraftFinish();

	if (bAutoCraftsWhenFueled && bFuelIsBurning)
	{
		StartAutoCrafting();
	}
}

void UCraftingComponent::StartBuriningFuel()
{


	if (IsFuelAvailable())
	{
		FCraftingFuel TargetFuel;

		//Get target fuel
		for (int32 i = 0; i < FuelTypes.Num(); i++)
		{
			if (CheckIfIngredientIsAvailable(FuelTypes[i].FuelIngredient))
			{
				TargetFuel = FuelTypes[i];
				break;
			}
		}

		BurnFuel(TargetFuel);
	}
	else
	{
		StopBurningFuel();
	}


}

void UCraftingComponent::BurnFuel(FCraftingFuel TargetFuel)
{

	//burn fuel removes fuel and then waits for it to burn

	if (RemoveIngredientFromInventories(TargetFuel.FuelIngredient))
	{

		if (bFuelIsBurning == false)
		{
			bFuelIsBurning = true;
			OnFuelBurnStart();
		}

		UE_LOG(LogCraftingSystem, Log, TEXT("Fuel was burned"))
		
		if(bAutoCraftsWhenFueled)
		{ 
			StartAutoCrafting();
		}

		//calls start burning fuel again
		GetWorld()->GetTimerManager().SetTimer(FuelBurnTimer, this, &UCraftingComponent::StartBuriningFuel, TargetFuel.TimeToConsumeOne, false);

	}
	else
	{
		UE_LOG(LogCraftingSystem,Warning,TEXT("Burn Fuel was given invalid fuel"))
		StopBurningFuel();
	}


}

void UCraftingComponent::StopBurningFuel()
{
	bFuelIsBurning = false;
	OnFuelBurnStop();
}

bool UCraftingComponent::IsFuelAvailable()
{
	int32 TotalQtyFound = 0;
	int32 OutQtyFound = 0;
	TArray<bool> bFuelAvailable;

	for (int32 i = 0; i < FuelTypes.Num(); i++)
	{
		bFuelAvailable.Add(CheckIfIngredientIsAvailable(FuelTypes[i].FuelIngredient));
	}

	if (bFuelAvailable.Contains(true))
	{
		bIsFuelAvailable = true;
		return true;
	}
	else
	{
		bIsFuelAvailable = false;
		return false;
	}

}

void UCraftingComponent::StartAutoCrafting()
{
	FCraftingRecipe TargetRecipe;

	if (bIsCrafting == true) { return; }

	for (int32 i = 0; i < CraftableRecipes.Num(); i++)
	{
		if (CanRecipeBeCrafted(CraftableRecipes[i]))
		{
			TargetRecipe = CraftableRecipes[i];
			break;
		}
	}
	CraftRecipe(TargetRecipe);
}

bool UCraftingComponent::CanRecipeBeCrafted(FCraftingRecipe Recipe)
{

	TArray<FCraftingPart> Inputs = Recipe.RecipeInputs;
	TArray<bool> EnoughIngredients;

	//Go through each recipe input and check to see if there is enough ingredients;
	for (int32 Index = 0; Index != Inputs.Num(); Index++)
	{
		EnoughIngredients.Add(CheckIfIngredientIsAvailable(Inputs[Index]));
	}

	// If there is not enough of one ingredient or not enough spasce for the crafted recipe
	if (EnoughIngredients.Contains(false) || !EnoughSpaceForCraftedRecipe(Recipe))
	{
		return false;
	}
	else
	{
		return true;
	}

}


bool UCraftingComponent::CheckIfIngredientIsAvailable(FCraftingPart Ingredient)
{
	FCraftingPart ActiveIngredient = Ingredient;
	int32 QuantityNeeded = ActiveIngredient.StackQuantity;
	int32 QuantityFound = 0;

	GetTotalQuantityOfIngredient(ActiveIngredient, QuantityFound);

	FItemData ItemData;
	GetItemDataFromClass(ActiveIngredient.InWorldActorClass, ItemData);
	UE_LOG(LogCraftingSystem, Log, TEXT("%d of %s found"), QuantityFound, *ItemData.DisplayName.ToString())

		//Check if enough of the active ingredent was found,
		if (QuantityFound >= ActiveIngredient.StackQuantity)
		{
			return true;
		}
		else
		{
			return false;
		}

}

bool UCraftingComponent::EnoughSpaceForCraftedRecipe(FCraftingRecipe Recipe)
{
	TArray<FItemData> InputItems;
	TArray<FItemData> OutputItems;
	TArray<bool> ItemChecks;

	GetItemDataFromRecipe(Recipe, InputItems, OutputItems);

	for (int32 i = 0; i < OutputItems.Num(); i++)
	{
		FItemData ActiveItem = OutputItems[i];
		TArray<bool> InventoryChecks;

		for (int32 i_INV = 0; i_INV < AssociatedOutputInventories.Num(); i_INV++)
		{
			if (AssociatedOutputInventories[i_INV]->BP_CheckIfItemCouldBeAdded(ActiveItem))
			{
				InventoryChecks.Add(true);
				break;
			}
			else
			{
				InventoryChecks.Add(false);
			}
		}

		if (InventoryChecks.Contains(true))
		{
			ItemChecks.Add(true);
		}
		else
		{
			ItemChecks.Add(false);
		}
	}

	if (ItemChecks.Contains(false))
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

void UCraftingComponent::GetItemDataFromRecipe(FCraftingRecipe Recipe, TArray<FItemData>& OutInputItems, TArray<FItemData>& OutOutputItems)
{
	TArray<FCraftingPart> InputParts;
	TArray<FCraftingPart> OutputParts;

	InputParts = Recipe.RecipeInputs;
	OutputParts = Recipe.RecipeOutputs;

	for (int32 i = 0; i < InputParts.Num(); i++)
	{
		FItemData ItemData;
		if (GetItemDataFromClass(InputParts[i].InWorldActorClass, ItemData))
		{
			ItemData.StackQuantity = InputParts[i].StackQuantity;
			OutInputItems.Add(ItemData);
		}
	}

	for (int32 i = 0; i < OutputParts.Num(); i++)
	{
		FItemData ItemData;
		if (GetItemDataFromClass(OutputParts[i].InWorldActorClass, ItemData))
		{
			ItemData.StackQuantity = OutputParts[i].StackQuantity;
			OutOutputItems.Add(ItemData);
		}
	}
}

bool UCraftingComponent::GetItemDataFromClass(UClass* Class, FItemData& OutItemData)
{

	TSubclassOf<AItemBase> ItemActor = Class;
	if (ItemActor != nullptr)
	{
		if (ItemActor.GetDefaultObject())
		{
			OutItemData = ItemActor.GetDefaultObject()->ItemData;
			return true;
		}
	}

	UE_LOG(LogCraftingSystem, Warning, TEXT("Could not find item data from class"))

	return false;
}

bool UCraftingComponent::Crafting_RemoveItems(FCraftingRecipe Recipe)
{
	if (AssociatedInputInventories.Num() == 0) { UE_LOG(LogCraftingSystem, Warning, TEXT("No input inventory for crafting")) }
	if (!CanRecipeBeCrafted(Recipe)) { UE_LOG(LogCraftingSystem, Log, TEXT("Recipe can not be crafted")) return false; }
	if (!EnoughSpaceForCraftedRecipe(Recipe)) { UE_LOG(LogCraftingSystem, Log, TEXT("Not enough space for output")) return false; }

	//Remove Input Items from Inventories;
	TArray<FCraftingPart> Inputs;
	TArray<bool> InputResults;
	Inputs = Recipe.RecipeInputs;

	for (int32 i = 0; i < Inputs.Num(); i++)
	{
		InputResults.Add(RemoveIngredientFromInventories(Inputs[i]));
	}

	if (InputResults.Contains(false))
	{
		UE_LOG(LogCraftingSystem, Warning, TEXT("Not all inputs were succesfully removed from inventorys for *s"), *Recipe.DisplayName.ToString())
			return false;
	}
	else
	{
		UE_LOG(LogCraftingSystem, Log, TEXT("Inputs succesfully used for %s receipe"), *Recipe.DisplayName.ToString())
		return true;
	}
	
	
	return false;
}

bool UCraftingComponent::RemoveIngredientFromInventories(FCraftingPart Ingredient)
{
	FCraftingPart ActiveInput = Ingredient;
	UClass* InputClass = ActiveInput.InWorldActorClass.Get();
	int32 InputAmount = ActiveInput.StackQuantity;
	FItemData InputItemData;

	GetItemDataFromClass(InputClass, InputItemData);


	//Cycle through inventories until input amount is removed
	for (int32 i_INV = 0; i_INV < AssociatedInputInventories.Num(); i_INV++)
	{
		UInventoryContainer* ActiveInventory = nullptr;
		ActiveInventory = AssociatedInputInventories[i_INV];
		if (ActiveInventory != nullptr)
		{
			int32 AmountRemoved;
			ActiveInventory->RemoveQuantityOfItem(InputItemData, InputAmount, AmountRemoved);
			InputAmount -= AmountRemoved;
			if (InputAmount == 0) { break; }
		}
	}

	if (InputAmount == 0)
	{
		return true;
	}
	else
	{
		UE_LOG(LogCraftingSystem, Warning, TEXT("%s input does not equal 0"), *InputItemData.DisplayName.ToString())
		return false;
		}


	return false;
}



void UCraftingComponent::Crafting_AddOutputs(FCraftingRecipe Recipe)
{

	//Add Output Items to Inventories;
	TArray<FCraftingPart> Outputs;
	TArray<bool> OutputResults;
	Outputs = Recipe.RecipeOutputs;

	for (int32 i = 0; i < Outputs.Num(); i++)
	{
		FCraftingPart ActiveOutput = Outputs[i];
		UClass* OutputClass = ActiveOutput.InWorldActorClass.Get();
		FItemData OutputItemData;
		GetItemDataFromClass(OutputClass, OutputItemData);
		OutputItemData.StackQuantity = ActiveOutput.StackQuantity;


		for (int32 i_INV = 0; i_INV < AssociatedOutputInventories.Num(); i_INV++)
		{
			UInventoryContainer* ActiveInventory = AssociatedOutputInventories[i_INV];
			if (ActiveInventory != nullptr)
			{
				bool bWasFullyAdded;
				FItemData LeftOverItemData;
				ActiveInventory->BP_AutoAddItem(OutputItemData, bWasFullyAdded, LeftOverItemData);
				if (bWasFullyAdded)
				{
					OutputItemData.StackQuantity = 0;
					UE_LOG(LogCraftingSystem, Log, TEXT("Output Item %s succesfully created"), *OutputItemData.DisplayName.ToString())
						break;
				}
				else
				{
					OutputItemData = LeftOverItemData;

				}
			}
		}

		if (OutputItemData.StackQuantity == 0)
		{
			OutputResults.Add(true);
		}
		else
		{
			OutputResults.Add(false);
			UE_LOG(LogCraftingSystem, Warning, TEXT("Could not fully create %s output"), *OutputItemData.DisplayName.ToString())
		}
	}

	if (OutputResults.Contains(false))
	{
		UE_LOG(LogCraftingSystem, Warning, TEXT("Not all outputs could be created"))
	}
	else
	{
		UE_LOG(LogCraftingSystem, Log, TEXT("Outputs succesfully created for %s receipe"), *Recipe.DisplayName.ToString())
	}

	return;
}



void UCraftingComponent::GetQuantityOfIngredientFromInventory(FCraftingPart Ingredient, UInventoryContainer* TargetInventory, int32& OutQuantityFound)
{
	OutQuantityFound = 0;

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

			FString ItemClassName = ItemClass->GetName();
			FString InputClassName = InputClass->GetName();
			UE_LOG(LogCraftingSystem,Verbose,TEXT("Item class = %s"),*ItemClassName)
			UE_LOG(LogCraftingSystem, Verbose, TEXT("Input class = %s"), *InputClassName)


			//If it is the ingredient we are looking for, add to quantity found
			if (ItemClass->IsChildOf(InputClass) || ItemClass == InputClass)
			{
				OutQuantityFound += TargetItem.StackQuantity;
				UE_LOG(LogCraftingSystem, Verbose,TEXT("%d found"), OutQuantityFound)
			}
			else
			{
				UE_LOG(LogCraftingSystem, Verbose,TEXT("Classes do not match"))
			}
		}
	}
	else
	{
		UE_LOG(LogCraftingSystem,Warning,TEXT("Cannot get quantity from invalid inventory reference"))
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



