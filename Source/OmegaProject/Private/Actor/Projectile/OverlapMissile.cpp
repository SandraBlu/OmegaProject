// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Projectile/OverlapMissile.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "OGameplayTags.h"
#include "Components/SphereComponent.h"
#include "GAS/OAbilitySystemLibrary.h"
#include "Kismet/GameplayStatics.h"

AOverlapMissile::AOverlapMissile()
{
	PrimaryActorTick.bCanEverTick = false;
	SphereComp->SetSphereRadius(20.0f);
}

void AOverlapMissile::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	SphereComp->OnComponentBeginOverlap.AddDynamic(this, &AOverlapMissile::OnActorOverlap);
}

void AOverlapMissile::OnHit()
{
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, ImpactVFX, GetActorLocation());
	UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, GetActorLocation());
	bHit = true;
}

void AOverlapMissile::Destroyed()
{
	if (!bHit && !HasAuthority()) OnHit();
	Super::Destroyed();
}

bool AOverlapMissile::IsValidOverlap(AActor* OtherActor)
{
	if (DamageEffectParams.SourceAbilitySystemComponent == nullptr) return false;
	AActor* SourceAvatarActor = DamageEffectParams.SourceAbilitySystemComponent->GetAvatarActor();
	if (SourceAvatarActor == OtherActor) return false;
	if (!UOAbilitySystemLibrary::IsNotFriend(SourceAvatarActor, OtherActor)) return false;
	return true;
}


void AOverlapMissile::OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                      UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!IsValidOverlap(OtherActor)) return;
	if (!bHit) OnHit();
	if (HasAuthority())
	{
		const FOGameplayTags& GameplayTags = FOGameplayTags::Get();
		APawn* HitPawn = Cast<APawn>(OtherActor);
		
		bool bIsValidBlock = false;
		const bool bIsPlayerBlocking = UOAbilitySystemLibrary::NativeDoesActorHaveTag(HitPawn,GameplayTags.status_blocking);
		
		if (bIsPlayerBlocking)
		{
			bIsValidBlock = UOAbilitySystemLibrary::IsValidBlock(this, HitPawn);
		}
		FGameplayEventData Data;
		Data.Instigator = this;
		Data.Target = HitPawn;
		if (bIsValidBlock)
		{
			UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(HitPawn, GameplayTags.Event_Blocking_Successful, Data);
			
		}
		else
		{
			if (UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor))
			{
				const FVector DeathImpulse = GetActorForwardVector() * DamageEffectParams.DeathImpulseMagnitude;
				DamageEffectParams.DeathImpulse = DeathImpulse;
				const bool bKnockback = FMath::RandRange(1, 100) <DamageEffectParams.KnockbackChance;
				if (bKnockback)
				{
					FRotator Rotation = GetActorRotation();
					Rotation.Pitch = 25.f;
					const FVector KnockbackDirection = Rotation.Vector();
					const FVector KnockbackForce = KnockbackDirection * DamageEffectParams.KnockbackMagnitude;
					DamageEffectParams.KnockbackForce = KnockbackForce;
				}
			
				DamageEffectParams.TargetAbilitySystemComponent = TargetASC;
				UOAbilitySystemLibrary::ApplyDamageEffect(DamageEffectParams);
			}
		}
		Destroy();
	}
	else bHit = true;
}