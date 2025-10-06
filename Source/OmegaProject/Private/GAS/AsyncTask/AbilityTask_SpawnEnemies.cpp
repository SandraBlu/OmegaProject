// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/AsyncTask/AbilityTask_SpawnEnemies.h"

#include "AbilitySystemComponent.h"
#include "NavigationSystem.h"
#include "AI/NPCBase.h"
#include "Engine/AssetManager.h"
#include "Engine/StreamableManager.h"

UAbilityTask_SpawnEnemies* UAbilityTask_SpawnEnemies::WaitSpawnEnemies(UGameplayAbility* OwningAbility,FGameplayTag EventTag, TSoftClassPtr<ANPCBase> SoftEnemyClassToSpawn, int32 NumToSpawn, const FVector& SpawnOrigin,float RandomSpawnRadius)
{
	UAbilityTask_SpawnEnemies* Node = NewAbilityTask<UAbilityTask_SpawnEnemies>(OwningAbility);
	Node->CachedEventTag = EventTag;
	Node->CachedSoftEnemyClassToSpawn = SoftEnemyClassToSpawn;
	Node->CachedNumToSpawn = NumToSpawn;
	Node->CachedSpawnOrigin = SpawnOrigin;
	Node->CachedRandomSpawnRadius = RandomSpawnRadius;

	return Node;
}

void UAbilityTask_SpawnEnemies::Activate()
{
	FGameplayEventMulticastDelegate& Delegate = AbilitySystemComponent->GenericGameplayEventCallbacks.FindOrAdd(CachedEventTag);

	DelegateHandle = Delegate.AddUObject(this,&ThisClass::OnGameplayEventReceived);
}

void UAbilityTask_SpawnEnemies::OnDestroy(bool bInOwnerFinished)
{
	FGameplayEventMulticastDelegate& Delegate = AbilitySystemComponent->GenericGameplayEventCallbacks.FindOrAdd(CachedEventTag);

	Delegate.Remove(DelegateHandle);

	Super::OnDestroy(bInOwnerFinished);
}

void UAbilityTask_SpawnEnemies::OnGameplayEventReceived(const FGameplayEventData* InPayload)
{
	if (ensure(!CachedSoftEnemyClassToSpawn.IsNull()))
	{
		UAssetManager::Get().GetStreamableManager().RequestAsyncLoad(
			CachedSoftEnemyClassToSpawn.ToSoftObjectPath(),
			FStreamableDelegate::CreateUObject(this, &ThisClass::OnEnemyClassLoaded)
		);
	}
	else
	{
		if (ShouldBroadcastAbilityTaskDelegates())
		{
			DidNotSpawn.Broadcast(TArray<ANPCBase*>());
		}
		EndTask();
	}
}

void UAbilityTask_SpawnEnemies::OnEnemyClassLoaded()
{
	UClass* LoadedClass = CachedSoftEnemyClassToSpawn.Get();
	UWorld* World = GetWorld();

	if (!LoadedClass || !World)
	{
		if (ShouldBroadcastAbilityTaskDelegates())
		{
			DidNotSpawn.Broadcast(TArray<ANPCBase*>());
		}
		EndTask();
		return;
	}

	TArray<ANPCBase*> SpawnedEnemies;
	FActorSpawnParameters SpawnParam;
	SpawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	for (int32 i = 0; i < CachedNumToSpawn; i++)
	{   
		FVector RandomLocation;
		UNavigationSystemV1::K2_GetRandomReachablePointInRadius(this,CachedSpawnOrigin,RandomLocation,CachedRandomSpawnRadius);

		RandomLocation += FVector(0.f,0.f,150.f);
		const FRotator SpawnFacingRotation = AbilitySystemComponent->GetAvatarActor()->GetActorForwardVector().ToOrientationRotator();
		ANPCBase* SpawnedEnemy = World->SpawnActor<ANPCBase>(LoadedClass,RandomLocation,SpawnFacingRotation,SpawnParam);

		if (SpawnedEnemy)
		{
			SpawnedEnemies.Add(SpawnedEnemy);
		}
	}

	if (ShouldBroadcastAbilityTaskDelegates())
	{
		if (!SpawnedEnemies.IsEmpty())
		{
			OnSpawnFinished.Broadcast(SpawnedEnemies);
		}
		else
		{
			DidNotSpawn.Broadcast(TArray<ANPCBase*>());
		}
	}
}