// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/OCharacterBase.h"

#include "AbilitySystemComponent.h"
#include "OGameplayTags.h"
#include "Component/DebuffNiagaraComponent.h"
#include "GAS/OAbilitySystemComponent.h"

// Sets default values
AOCharacterBase::AOCharacterBase()
{
	PrimaryActorTick.bCanEverTick = true;
	EffectDebuffComponent = CreateDefaultSubobject<UDebuffNiagaraComponent>("EffectDebuffComponent");
	EffectDebuffComponent->SetupAttachment(GetRootComponent());
	EffectDebuffComponent->DebuffTag = FOGameplayTags::Get().Debuff_Stun;

	/*EffectAttachComp = CreateDefaultSubobject<USceneComponent>("EffectAttachPoint");
	EffectAttachComp->SetupAttachment(GetRootComponent());
	PassiveCloak = CreateDefaultSubobject<UPassiveNiagaraComponent>("CloakComponent");
	PassiveCloak->SetupAttachment(EffectAttachComp);
	PassiveLifeSiphon = CreateDefaultSubobject<UPassiveNiagaraComponent>("LifeSiphonComponent");
	PassiveLifeSiphon->SetupAttachment(EffectAttachComp);
	PassiveStaminaSiphon = CreateDefaultSubobject<UPassiveNiagaraComponent>("StaminaSiphonComponent");
	PassiveStaminaSiphon->SetupAttachment(EffectAttachComp);*/

}

// Called when the game starts or when spawned
void AOCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void AOCharacterBase::InitAbilityActorInfo()
{
}

void AOCharacterBase::ApplyEffectToSelf(TSubclassOf<UGameplayEffect> GameplayEffectClass, float Level) const
{
	check(IsValid(GetAbilitySystemComponent()));
	check(GameplayEffectClass);
	FGameplayEffectContextHandle ContextHandle = GetAbilitySystemComponent()->MakeEffectContext();
	ContextHandle.AddSourceObject(this);
	
	const FGameplayEffectSpecHandle SpecHandle = GetAbilitySystemComponent()->MakeOutgoingSpec(GameplayEffectClass, Level, ContextHandle);
	GetAbilitySystemComponent()->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), GetAbilitySystemComponent());
}

void AOCharacterBase::InitializeAttributes() const
{
	ApplyEffectToSelf(PrimaryAttributes, 1.f);
	ApplyEffectToSelf(SecondaryAttributes, 1.f);
	ApplyEffectToSelf(ResistanceAttributes, 1.f);
	ApplyEffectToSelf(VitalAttributes, 1.f);
}

void AOCharacterBase::GrantAbilities()
{
	UOAbilitySystemComponent* RASC = CastChecked<UOAbilitySystemComponent>(AbilitySystemComponent);
	if (!HasAuthority()) return;
	RASC->AddGrantedAbilities(GrantedAbilities);
	RASC->AddPassiveAbilities(GrantedPassiveAbilities);
}

void AOCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//EffectAttachComp->SetWorldRotation((FRotator::ZeroRotator));

}

float AOCharacterBase::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,AActor* DamageCauser)
{
	float DamageTaken =  Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	OnDamageDelegate.Broadcast(DamageTaken);
	return DamageTaken;
}

UAbilitySystemComponent* AOCharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

UOCombatComponent* AOCharacterBase::GetCombatComponent() const
{
	return CombatComp;
}

UAnimMontage* AOCharacterBase
::GetHitReactMontage_Implementation()
{
	return HitReactMontage;
}

UAnimMontage* AOCharacterBase
::GetStunnedMontage_Implementation()
{
	return StunnedMontage;
}

bool AOCharacterBase
::IsDead_Implementation() const
{
	return bDead;
}

AActor* AOCharacterBase
::GetAvatar_Implementation()
{
	return this;
}

TArray<FTaggedMontage> AOCharacterBase
::GetAttackMontages_Implementation()
{
	return AttackMontages;
}

UNiagaraSystem* AOCharacterBase
::GetBloodEffect_Implementation()
{
	return BloodEffect;
}

ECharacterClass AOCharacterBase
::GetCharacterClass_Implementation()
{
	return CharacterClass;
}

FOnASCRegistered AOCharacterBase
::GetOnASCRegisteredDelegate()
{
	return OnASCRegistered;
}

FOnDeath AOCharacterBase
::GetOnDeathDelegate()
{
	return OnDeath;
}

void AOCharacterBase
::Die(const FVector& DeathImpulse)
{
	
}

FOnDamageSignature& AOCharacterBase
::GetOnDamageSignature()
{
	return OnDamageDelegate;
}
