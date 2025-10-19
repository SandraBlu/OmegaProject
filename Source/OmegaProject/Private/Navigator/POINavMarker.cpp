// Fill out your copyright notice in the Description page of Project Settings.


#include "Navigator/POINavMarker.h"

#include "Navigator/NavigationComponent.h"
#include "Navigator/POI.h"

UPOINavMarker::UPOINavMarker()
{
	DefaultMarkerActionText = NSLOCTEXT("POINavigationMarker", "FastTravelText", "Fast Travel");
}

void UPOINavMarker::SetDiscovered(const bool bDiscovered)
{
	FLinearColor IconColor = DefaultMarkerSettings.IconTint;

	IconColor.A = bDiscovered ? 1.f : 0.5f;

	SetIconTint(IconColor);
}

bool UPOINavMarker::CanInteract_Implementation(class UNavigationComponent* Selector, class APlayerController* SelectorOwner) const
{
	if (APOI* POIOwner = Cast<APOI>(GetOwner()))
	{
		if (Selector)
		{
			return Selector->HasDiscoveredPOI(POIOwner->POITags);
		}
	}

	return false; 
}