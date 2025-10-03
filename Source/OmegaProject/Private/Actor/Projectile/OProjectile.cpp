// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Projectile/OProjectile.h"

// Sets default values
AOProjectile::AOProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AOProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AOProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

