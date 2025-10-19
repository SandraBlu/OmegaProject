// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Navigator/NavMarkerComponent.h"
#include "POINavMarker.generated.h"

/**
 * 
 */
UCLASS()
class OMEGAPROJECT_API UPOINavMarker : public UNavMarkerComponent
{
	GENERATED_BODY()

	UPOINavMarker();

	virtual bool CanInteract_Implementation(class UNavigationComponent* Selector, class APlayerController* SelectorOwner) const override;

public:

	void SetDiscovered(const bool bDiscovered);
	
};
