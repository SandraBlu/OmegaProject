// Fill out your copyright notice in the Description page of Project Settings.


#include "Navigator/NavMarkerComponent.h"

#include "Navigator/NavigationComponent.h"

#define LOCTEXT_NAMESPACE "NavigationMarker"

UNavMarkerComponent::UNavMarkerComponent()
{

	DefaultMarkerSettings.bOverride_LocationDisplayName = true;
	DefaultMarkerSettings.bOverride_bShowActorRotation = true;
	DefaultMarkerSettings.bOverride_LocationIcon = true;
	DefaultMarkerSettings.bOverride_IconTint = true;
	DefaultMarkerSettings.bOverride_IconSize = true;
	DefaultMarkerSettings.bOverride_IconOffset = true;

	//The default UI that ships with navigator really benefits from a couple of overrides! Compass icons and screen space markers should be a little bigger
	CompassOverrideSettings.bOverride_IconSize = true;
	CompassOverrideSettings.IconSize = FVector2D(40.f, 40.f);
	ScreenspaceOverrideSettings.bOverride_IconSize = true;
	ScreenspaceOverrideSettings.IconSize = FVector2D(50.f, 50.f);

	DefaultMarkerSettings.LocationDisplayName = LOCTEXT("NavigatorLocationDisplayName", "Location Marker");
	DefaultMarkerSettings.LocationIcon = nullptr;
	DefaultMarkerSettings.IconTint = FLinearColor(1.f, 1.f, 1.f);
	DefaultMarkerSettings.IconSize = FVector2D(20.f, 20.f);

	MarkerStartFadeOutDistance = 13500.f;
	MarkerStartFadeInDistance = 15000.f;
	bPinToMapEdge = false;

	auto LocationIconFinder = ConstructorHelpers::FObjectFinder<UTexture2D>(TEXT("/Script/Engine.Texture2D'/OmegaProject/Assets/Icons/T_Marker_Default.T_Marker_Default'"));
	if (LocationIconFinder.Succeeded())
	{
		DefaultMarkerSettings.LocationIcon = LocationIconFinder.Object;
	}

	VisibilityFlags |= StaticCast<int32>(ENavigatorType::Compass);
	VisibilityFlags |= StaticCast<int32>(ENavigatorType::Map);
	VisibilityFlags |= StaticCast<int32>(ENavigatorType::Minimap);

	SetAutoActivate(true);
}

void UNavMarkerComponent::BeginPlay()
{
	Super::BeginPlay();

	RegisterMarker();
}

void UNavMarkerComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	RemoveMarker();
}

void UNavMarkerComponent::RegisterMarker()
{
	//Because navigation is a local thing, we can safely use the local player controller
	for (FConstPlayerControllerIterator Iter = GetWorld()->GetPlayerControllerIterator(); Iter; ++Iter)
	{
		if (APlayerController* PC = Iter->Get())
		{
			if (PC->IsLocalController())
			{
				if (UNavigationComponent* NavComp = Cast<UNavigationComponent>(PC->GetComponentByClass(UNavigationComponent::StaticClass())))
				{
					NavComp->AddMarker(this);
				}
			}
		}
	}
}

void UNavMarkerComponent::RemoveMarker()
{
	//Because navigation is a local thing, we can safely use the local player controller
	for (FConstPlayerControllerIterator Iter = GetWorld()->GetPlayerControllerIterator(); Iter; ++Iter)
	{
		if (APlayerController* PC = Iter->Get())
		{
			if (PC->IsLocalController())
			{
				if (UNavigationComponent* NavComp = Cast<UNavigationComponent>(PC->GetComponentByClass(UNavigationComponent::StaticClass())))
				{
					NavComp->RemoveMarker(this);
				}
			}
		}
	}
}

FNavigationMarkerSettings UNavMarkerComponent::GetMarkerSettings(ENavigatorType NavigatorType)
{
	FNavigationMarkerSettings Settings = DefaultMarkerSettings;
	FNavigationMarkerSettings Overrides = CompassOverrideSettings;

	switch (NavigatorType)
	{
		case ENavigatorType::Compass:
		{
			Overrides = CompassOverrideSettings;
		}
		break;
		case ENavigatorType::Map:
		{
			Overrides = WorldMapOverrideSettings;
		}
		break;
		case ENavigatorType::Minimap:
		{
			Overrides = MinimapOverrideSettings;
		}
		break;
		case ENavigatorType::Screenspace:
		{
			Overrides = ScreenspaceOverrideSettings;
		}
		break;
	}

	if (Overrides.bOverride_LocationDisplayName)
	{
		Settings.LocationDisplayName = Overrides.LocationDisplayName;
	}

	if (Overrides.bOverride_LocationIcon)
	{
		Settings.LocationIcon = Overrides.LocationIcon;
	}

	if (Overrides.bOverride_IconTint)
	{
		Settings.IconTint = Overrides.IconTint;
	}

	if (Overrides.bOverride_IconSize)
	{
		Settings.IconSize = Overrides.IconSize;
	}

	if (Overrides.bOverride_IconOffset)
	{
		Settings.IconOffset = Overrides.IconOffset;
	}

	if (Overrides.bOverride_bShowActorRotation)
	{
		Settings.bShowActorRotation = Overrides.bShowActorRotation;
	}

	return Settings;
}

void UNavMarkerComponent::RefreshMarker()
{
	OnRefreshRequired.Broadcast();
}

void UNavMarkerComponent::SetLocationIcon(class UTexture2D* NewLocationIcon)
{
	DefaultMarkerSettings.LocationIcon = NewLocationIcon;
	OnRefreshRequired.Broadcast();
}

void UNavMarkerComponent::SetIconTint(const FLinearColor NewIconTint)
{
	DefaultMarkerSettings.IconTint = NewIconTint;
	OnRefreshRequired.Broadcast();
}

void UNavMarkerComponent::SetLocationDisplayName(FText NewLocationDisplayName)
{
	DefaultMarkerSettings.LocationDisplayName = NewLocationDisplayName;
	OnRefreshRequired.Broadcast();
}

FText UNavMarkerComponent::GetMarkerActionText_Implementation(class UNavigationComponent* Selector, class APlayerController* SelectorOwner) const
{
	return DefaultMarkerActionText;
}

bool UNavMarkerComponent::CanInteract_Implementation(class UNavigationComponent* Selector, class APlayerController* SelectorOwner) const
{
	return true;
}

void UNavMarkerComponent::OnSelect_Implementation(class UNavigationComponent* Selector, class APlayerController* SelectorOwner)
{
	
}

bool UNavMarkerComponent::ShouldShowOn(int32 Categories)
{
	return (Categories & VisibilityFlags) == Categories;
}

void UNavMarkerComponent::SetVisibility(int32 Categories)
{
	VisibilityFlags = Categories;
	RemoveMarker();
	RegisterMarker();
}

#undef LOCTEXT_NAMESPACE 

