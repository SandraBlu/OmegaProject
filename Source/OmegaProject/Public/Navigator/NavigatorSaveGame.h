// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFramework/SaveGame.h"
#include "NavigatorSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class OMEGAPROJECT_API UNavigatorSaveGame : public USaveGame
{
	GENERATED_BODY()

public:

	UPROPERTY()
	TArray<FTransform> CustomMarkerTransforms; 

	UPROPERTY()
	FGameplayTagContainer DiscoveredPOIs;
	
};
