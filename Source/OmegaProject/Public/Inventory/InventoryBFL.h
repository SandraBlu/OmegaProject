// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "InventoryBFL.generated.h"

/**
 * 
 */
UCLASS()
class OMEGAPROJECT_API UInventoryBFL : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Inventory", meta = (DefaultToSelf = "Target"))
	static class UOInventoryComponent* GetInventoryComponentFromTarget(AActor* Target);

	
	//Sort the array of inventory items from a-z
	UFUNCTION(BlueprintPure, Category = "Inventory")
	static TArray<class UOItem*> SortItemArrayAlphabetical(TArray<class UOItem*> InItems, const bool bReverse);


	//Sort the array of inventory items using stack weight
	UFUNCTION(BlueprintPure, Category = "Inventory")
	static TArray<class UOItem*> SortItemArrayWeight(TArray<class UOItem*> InItems, const bool bReverse);
	
};
