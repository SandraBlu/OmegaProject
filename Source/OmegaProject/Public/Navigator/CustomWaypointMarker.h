// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Navigator/NavMarkerComponent.h"
#include "CustomWaypointMarker.generated.h"

/**
 * 
 */
UCLASS()
class OMEGAPROJECT_API UCustomWaypointMarker : public UNavMarkerComponent
{
	GENERATED_BODY()

	UCustomWaypointMarker();

	virtual FText GetMarkerActionText_Implementation(class UNavigationComponent* Selector, class APlayerController* SelectorOwner) const override;
	virtual void OnSelect_Implementation(class UNavigationComponent* Selector, class APlayerController* SelectorOwner) override;
	
};
