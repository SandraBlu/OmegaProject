// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "OItemWidget.generated.h"

/**
 * 
 */
UCLASS()
class OMEGAPROJECT_API UOItemWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadOnly, Category = "Inventory Item Widget", meta = (ExposeOnSpawn = true))
	class UOItem* Item;
	
};
