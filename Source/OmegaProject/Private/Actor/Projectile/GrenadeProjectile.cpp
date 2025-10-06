// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Projectile/GrenadeProjectile.h"

#include "Kismet/GameplayStatics.h"


AGrenadeProjectile::AGrenadeProjectile()
{
 	
	PrimaryActorTick.bCanEverTick = false;

}

void AGrenadeProjectile::InitGrenade(float Delay, float InExplosionRadius, float InExplosionForce, USoundBase* InExplosionSound)
{
	FuseTime = Delay;
	ExplosionRadius = InExplosionRadius;
	ExplosionForce = InExplosionForce;
	ExplosionSound = InExplosionSound;

	GetWorld()->GetTimerManager().SetTimer(TimerHandle_Explode, this, &AGrenadeProjectile::Explode, FuseTime);
}

void AGrenadeProjectile::BeginPlay()
{
	Super::BeginPlay();
}

void AGrenadeProjectile::Explode()
{
	// Play explosion particle
	if (ExplosionFX)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionFX, GetActorLocation());
	}

	// Apply radial force and damage
	TArray<AActor*> IgnoredActors;
	IgnoredActors.Add(this);

	// Damage within radius (optional)
	UGameplayStatics::ApplyRadialDamage(GetWorld(), /*DamageAmount*/ 50.f, GetActorLocation(), ExplosionRadius,
		nullptr, IgnoredActors, this, GetInstigatorController(), true);

	// Apply physics force to nearby physics objects
	TArray<FHitResult> HitResults;
	FCollisionShape Sphere = FCollisionShape::MakeSphere(ExplosionRadius);
	bool bHit = GetWorld()->SweepMultiByChannel(HitResults, GetActorLocation(), GetActorLocation() + FVector(0.1f),
		FQuat::Identity, ECC_PhysicsBody, Sphere);

	if (bHit)
	{
		for (auto& Hit : HitResults)
		{
			UPrimitiveComponent* PrimComp = Hit.GetComponent();
			if (PrimComp && PrimComp->IsSimulatingPhysics())
			{
				PrimComp->AddRadialForce(GetActorLocation(), ExplosionRadius, ExplosionForce, ERadialImpulseFalloff::RIF_Linear, true);
			}
		}
	}

	// Play sound effect
	if (ExplosionSound)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), ExplosionSound, GetActorLocation());
	}
	DrawDebugSphere(GetWorld(), GetActorLocation(), ExplosionRadius, 32, FColor::Red, false, 2.0f);
	Destroy();
}

