// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/OInteractComponent.h"

#include "Component/OInteractingComponent.h"

// Sets default values for this component's properties
UOInteractComponent::UOInteractComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = true;
	PrimaryComponentTick.TickGroup = TG_PostPhysics;
	PrimaryComponentTick.bAllowTickOnDedicatedServer = true;

	CurrentInteractable = nullptr;
	LastInteractCheckTime = 0.f;
	InteractCheckDistance = 1000.f;
	RemainingInteractTime = -999.f;

	bInteractHeld = false;
	SetAutoActivate(true);
	SetIsReplicatedByDefault(true);
}

// Called when the game starts
void UOInteractComponent::BeginPlay()
{
	Super::BeginPlay();

	OwningController = Cast<APlayerController>(GetOwner());

	if (OwningController)
	{
		OwningPawn = OwningController->GetPawn();
	}
}


// Called every frame
void UOInteractComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//Server wasn't able to get this
	if (!OwningPawn && OwningController)
	{
		OwningPawn = OwningController->GetPawn();
	}

	if (IsActive())
	{
		if (GetWorld()->TimeSince(LastInteractCheckTime) > InteractCheckFrequency)
		{
			PerformInteractCheck(DeltaTime);
		}

		if (bInteractHeld && CurrentInteractable)
		{
			if (RemainingInteractTime > 0.f)
			{
				RemainingInteractTime -= DeltaTime;
			}

			if (RemainingInteractTime <= 0.f && RemainingInteractTime > -10.f)
			{
				const bool bInteracted = CurrentInteractable->Interact(OwningPawn, this);

				if (bInteracted)
				{
					OnInteracted.Broadcast(this, CurrentInteractable);
				}

				RemainingInteractTime = -999.f;
			}
		}
	}
}

void UOInteractComponent::Deactivate()
{
	Super::Deactivate();

	NoInteractableFound();
}

void UOInteractComponent::PerformInteractCheck(float DeltaTime)
{
	if (OwningController && OwningPawn)
	{
		FVector EyesLoc;
		FRotator EyesRot;

		OwningController->GetPlayerViewPoint(EyesLoc, EyesRot);

		//Add camera dist from pawn as long camera arms shouldn't effect how far you can interact 
		FVector TraceStart = EyesLoc;
		FVector TraceEnd = (EyesRot.Vector() * (InteractCheckDistance + FVector::Dist(OwningPawn->GetActorLocation(), EyesLoc))) + TraceStart;
		FHitResult TraceHit;

		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(OwningPawn);

		if (FMath::IsNearlyZero(InteractCheckSphereRadius))
		{
			GetWorld()->LineTraceSingleByChannel(TraceHit, TraceStart, TraceEnd, ECC_Visibility, QueryParams);
		}
		else
		{
			const FCollisionShape Sphere = FCollisionShape::MakeSphere(InteractCheckSphereRadius);

			GetWorld()->SweepSingleByChannel(TraceHit, TraceStart, TraceEnd, FQuat::Identity, ECC_Visibility, Sphere, QueryParams);

		}

		//Check if we hit an interactable object
		if (TraceHit.GetActor())
		{
			// Get all components of class UNarrativeInteractableComponent from the actor
			TArray<UOInteractingComponent*> InteractableComponents;
			TraceHit.GetActor()->GetComponents<UOInteractingComponent>(InteractableComponents);
			DrawDebugSphere(GetWorld(), TraceHit.ImpactPoint, 5.f, 16, FColor::Red, false, 0.0f);
			for (UActorComponent* Component : InteractableComponents)
			{
				if (UOInteractingComponent* InteractableComponent = Cast<UOInteractingComponent>(Component))
				{
					if (InteractableComponent->IsActive())
					{
						const float Distance = (OwningPawn->GetActorLocation() - TraceHit.ImpactPoint).Size();

						if (Distance <= InteractableComponent->InteractDistance)
						{
							FoundInteractable(InteractableComponent);
							return;
						}
					}
				}
			}
		}
	}

	NoInteractableFound();
}

void UOInteractComponent::NoInteractableFound()
{
	//Tell the interactable we've stopped focusing on it, and clear the current interactable
	if (CurrentInteractable)
	{
		if (CurrentInteractable)
		{
			CurrentInteractable->EndFocus(OwningPawn, this);
			OnLostInteractable.Broadcast(CurrentInteractable);
		}

		EndInteract();
	}

	CurrentInteractable = nullptr;
}

void UOInteractComponent::FoundInteractable(UOInteractingComponent* Reactor)
{
	if (Reactor != CurrentInteractable)
	{
		EndInteract();

		if (CurrentInteractable)
		{
			CurrentInteractable->EndFocus(OwningPawn, this);
		}

		CurrentInteractable = Reactor;
		CurrentInteractable->BeginFocus(OwningPawn, this);
		OnFoundInteractable.Broadcast(Reactor);
	}
}

void UOInteractComponent::ServerBeginInteract_Implementation()
{
	BeginInteract();
}

void UOInteractComponent::ServerEndInteract_Implementation()
{
	EndInteract();
}

void UOInteractComponent::BeginInteract()
{
	if (GetOwnerRole() < ROLE_Authority)
	{
		ServerBeginInteract();
	}
	
	bInteractHeld = true;

	OnInteractPressed.Broadcast(this);
	
	FText ErrorMessage;
	if (CurrentInteractable && CurrentInteractable->CanInteract(OwningPawn, this, ErrorMessage))
	{
		CurrentInteractable->BeginInteract(OwningPawn, this);
		RemainingInteractTime = CurrentInteractable->InteractTime;
	}
}

void UOInteractComponent::EndInteract()
{
	if (GetOwnerRole() < ROLE_Authority)
	{
		ServerEndInteract();
	}

	if (CurrentInteractable)
	{
		CurrentInteractable->EndInteract(OwningPawn, this);
		RemainingInteractTime = -999.f;
	}

	bInteractHeld = false;
	OnInteractReleased.Broadcast(this);
}

FTransform UOInteractComponent::GetInteractionStartTransform_Implementation()
{
	FVector EyesLoc;
	FRotator EyesRot;

	OwningController->GetPlayerViewPoint(EyesLoc, EyesRot);

	const FTransform InteractionTransform(EyesRot, EyesLoc);

	return InteractionTransform;
}

#undef LOCTEXT_NAMESPACE

