// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/OInteractingComponent.h"

#define LOCTEXT_NAMESPACE "InteractionComponent"

// Sets default values for this component's properties
UOInteractingComponent::UOInteractingComponent()
{
	InteractTime = 0.f;
	InteractDistance = 200.f;
	InteractableItemName = LOCTEXT("InteractNameText", "Interact Object");
	InteractableActionText = LOCTEXT("InteractActionText", "Interact");
	
	auto FocusedOverlayFinder = ConstructorHelpers::FObjectFinder<UMaterialInterface>(TEXT("/Game/Inventory/M_ItemPulse.M_ItemPulse"));
	if (FocusedOverlayFinder.Succeeded())
	{
		FocusedOverlayMaterial = FocusedOverlayFinder.Object;
	}

	SetAutoActivate(true);
	SetComponentTickEnabled(false);
}


void UOInteractingComponent::Deactivate()
{
	Super::Deactivate();
}

void UOInteractingComponent::SetupAttachment(class UStaticMeshComponent* PickupMesh)
{
}

void UOInteractingComponent::SetInteractableNameText(const FText& NewNameText)
{
	InteractableItemName = NewNameText;
}

void UOInteractingComponent::SetInteractableActionText(const FText& NewActionText)
{
	InteractableActionText = NewActionText;
}

void UOInteractingComponent::BeginFocus(class APawn* Interactor, class UOInteractComponent* InteractComp)
{
	if (!IsActive() || !GetOwner() || !Interactor)
	{
		return;
	}

	OnBeginFocus.Broadcast(Interactor, InteractComp);

	if (GetNetMode() != NM_DedicatedServer)
	{
		TInlineComponentArray<UMeshComponent*> Meshes;
		GetOwner()->GetComponents(Meshes);

		for (auto& Mesh : Meshes)
		{
			if (Mesh)
			{
				Mesh->SetOverlayMaterial(FocusedOverlayMaterial);
			}
		}
	}
}

void UOInteractingComponent::EndFocus(class APawn* Interactor, class UOInteractComponent* InteractComp)
{
	OnEndFocus.Broadcast(Interactor, InteractComp);

	if (GetNetMode() != NM_DedicatedServer)
	{
		TInlineComponentArray<UMeshComponent*> Meshes;
		GetOwner()->GetComponents(Meshes);

		for (auto& Mesh : Meshes)
		{
			if (Mesh)
			{
				Mesh->SetOverlayMaterial(nullptr);
			}
		}
	}
}

void UOInteractingComponent::BeginInteract(class APawn* Interactor, class UOInteractComponent* InteractComp)
{
	OnBeginInteract(Interactor, InteractComp);
	OnBeginInteracted.Broadcast(Interactor, InteractComp);
}

void UOInteractingComponent::EndInteract(class APawn* Interactor, class UOInteractComponent* InteractComp)
{
	OnEndInteract(Interactor, InteractComp);
	OnEndInteracted.Broadcast(Interactor, InteractComp);
}

bool UOInteractingComponent::Interact(class APawn* Interactor, class UOInteractComponent* InteractComp)
{
	FText ErrorMessage;

	const bool bCanInteract = CanInteract(Interactor, InteractComp, ErrorMessage);

	if (bCanInteract)
	{
		OnInteract(Interactor, InteractComp);
		OnInteracted.Broadcast(Interactor, InteractComp);
	}

	return bCanInteract;
}

void UOInteractingComponent::OnInteract_Implementation(class APawn* Interactor, class UOInteractComponent* InteractComp)
{
}

void UOInteractingComponent::OnBeginInteract_Implementation(class APawn* Interactor, class UOInteractComponent* InteractComp)
{
}

void UOInteractingComponent::OnEndInteract_Implementation(class APawn* Interactor, class UOInteractComponent* InteractComp)
{
}

bool UOInteractingComponent::CanInteract_Implementation(class APawn* Interactor, class UOInteractComponent* InteractComp,
	FText& OutErrorText)
{
	return true;
}

#undef LOCTEXT_NAMESPACE

