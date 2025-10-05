// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "OItemTooltip.generated.h"

/**
 * 
 */
UCLASS()
class OMEGAPROJECT_API UOItemTooltip : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadOnly, Category = "Tooltip", meta = (ExposeOnSpawn = true))
	class UOItem* Item;
	
};
