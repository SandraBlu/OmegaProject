// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GrenadeProjectile.generated.h"

UCLASS()
class OMEGAPROJECT_API AGrenadeProjectile : public AActor
{
	GENERATED_BODY()
	
public:	

	AGrenadeProjectile();

	void InitGrenade(float Delay, float ExplosionRadius, float ExplosionForce, USoundBase* ExplosionSound);

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category="Effects")
	UParticleSystem* ExplosionFX;

	UPROPERTY(EditDefaultsOnly, Category="Gameplay")
	float FuseTime;

	// New properties
	UPROPERTY(EditDefaultsOnly, Category="Explosion")
	float ExplosionRadius;

	UPROPERTY(EditDefaultsOnly, Category="Explosion")
	float ExplosionForce;

	UPROPERTY(EditDefaultsOnly, Category="Effects")
	USoundBase* ExplosionSound;

	UFUNCTION()
	void Explode();

private:
	FTimerHandle TimerHandle_Explode;

};
