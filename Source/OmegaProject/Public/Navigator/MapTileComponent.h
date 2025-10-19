// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Navigator/NavMarkerComponent.h"
#include "MapTileComponent.generated.h"
/*The map tiles are big map markers, only being rendered if they are within the maps bounds as well as other marker features.*/

UCLASS( ClassGroup=(Omega), Blueprintable, DisplayName = "Map Tile Marker", meta=(BlueprintSpawnableComponent) )
class OMEGAPROJECT_API UMapTileComponent : public UNavMarkerComponent
{
	GENERATED_BODY()

	UMapTileComponent();

	virtual bool CanInteract_Implementation(class UNavigationComponent* Selector, class APlayerController* SelectorOwner) const;

public:

	//The zoom level at which this map tile streams in. IE 5 = the map needs to be at 5x zoom before the tile streams in
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Marker Setup")
	float LODFadeInLevel;
	
};
