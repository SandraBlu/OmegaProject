// Fill out your copyright notice in the Description page of Project Settings.


#include "Navigator/CustomWaypoint.h"

// Sets default values
ACustomWaypoint::ACustomWaypoint()
{
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	//WaypointMarker = CreateDefaultSubobject<UCustomWaypointMarker>(TEXT("WaypointMarker"));

#if WITH_EDITOR

	SetIsSpatiallyLoaded(false);

#endif 

}

