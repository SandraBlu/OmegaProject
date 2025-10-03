// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/OWeapon.h"

// Sets default values
AOWeapon::AOWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AOWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AOWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

