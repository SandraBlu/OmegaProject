// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/OFootstepComponent.h"

#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Character/OPlayer.h"
#include "Framework/OPhysicalMaterial.h"
#include "Framework/Countdown/OEnums.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UOFootstepComponent::UOFootstepComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UOFootstepComponent::HandleFootstep(EFoot Foot)
{
	if (AOPlayer* Character = Cast<AOPlayer>(GetOwner()))
	{
		if (USkeletalMeshComponent* Mesh = Character->GetMesh())
		{
			FHitResult HitResult;
			const FVector SocketLocation = Mesh->GetSocketLocation(Foot == EFoot::Left ? LeftFootSocket : RightFootSocket);
			const FVector Location = SocketLocation + FVector::UpVector * 20;

			FCollisionQueryParams QueryParams;
			QueryParams.bReturnPhysicalMaterial = true;
			QueryParams.AddIgnoredActor(Character);

			if (GetWorld()->LineTraceSingleByChannel(HitResult, Location, Location + FVector::UpVector * -50.f, ECollisionChannel::ECC_WorldStatic, QueryParams))
			{
				if (HitResult.bBlockingHit)
				{
					if (HitResult.PhysMaterial.Get())
					{
						UOPhysicalMaterial* PhysicsMat = Cast<UOPhysicalMaterial>(HitResult.PhysMaterial.Get());

						if (PhysicsMat)
						{
							UGameplayStatics::PlaySoundAtLocation(this, PhysicsMat->FootstepSFX, Location, 1.f);
							UNiagaraComponent* NiagaraComp = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), PhysicsMat->FootstepVFX, Location);
						}
					}
				}
			}
		}
	}
}

