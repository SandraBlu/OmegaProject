// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/InventoryBFL.h"

#include "Component/OInventoryComponent.h"
#include "GameFramework/PlayerState.h"
#include "Inventory/OItem.h"

class UOInventoryComponent* UInventoryBFL::GetInventoryComponentFromTarget(AActor* Target)
{
	if (!Target)
	{
		return nullptr;
	}

	if (UOInventoryComponent* InventoryComp = Target->FindComponentByClass<UOInventoryComponent>())
	{
		return InventoryComp;
	}

	//Try player state, then pawn, then controller 
	if (APawn* OwningPawn = Cast<APawn>(Target))
	{
		if (OwningPawn->GetPlayerState())
		{
			return OwningPawn->GetPlayerState()->FindComponentByClass<UOInventoryComponent>();
		}

		if (OwningPawn->GetController())
		{
			return OwningPawn->GetController()->FindComponentByClass<UOInventoryComponent>();
		}
	}

	//Same for controller based target 
	if (APlayerController* OwningController = Cast<APlayerController>(Target))
	{
		if (OwningController->GetPawn())
		{
			if (OwningController->GetPlayerState<APlayerState>())
			{
				return OwningController->GetPlayerState<APlayerState>()->FindComponentByClass<UOInventoryComponent>();
			}


			return OwningController->GetPawn()->FindComponentByClass<UOInventoryComponent>();
		}
	}

	return nullptr;
}

TArray<class UOItem*> UInventoryBFL::SortItemArrayAlphabetical(TArray<class UOItem*> InItems, const bool bReverse)
{
	TArray<class UOItem*> RetItems = InItems;

	//Sort the replies by their Y position in the graph
	if (bReverse)
	{
		RetItems.Sort([](const UOItem& ItemA, const UOItem& ItemB) {
			return ItemA.DisplayName.ToString() < ItemB.DisplayName.ToString();
			});
	}
	else
	{
		RetItems.Sort([](const UOItem& ItemA, const UOItem& ItemB) {
			return ItemA.DisplayName.ToString() > ItemB.DisplayName.ToString();
			});
	}

	return RetItems;
}

TArray<class UOItem*> UInventoryBFL::SortItemArrayWeight(TArray<class UOItem*> InItems, const bool bReverse)
{
	TArray<class UOItem*> RetItems = InItems;

	//Sort the replies by their Y position in the graph
	if (bReverse)
	{
		RetItems.Sort([](const UOItem& ItemA, const UOItem& ItemB) {
			return ItemA.GetStackWeight() > ItemB.GetStackWeight();
			});
	}
	else
	{
		RetItems.Sort([](const UOItem& ItemA, const UOItem& ItemB) {
			return ItemA.GetStackWeight() < ItemB.GetStackWeight();
			});
	}


	return RetItems;
}