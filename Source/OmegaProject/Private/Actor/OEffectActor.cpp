// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/OEffectActor.h"

// Sets default values
AOEffectActor::AOEffectActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AOEffectActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AOEffectActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

