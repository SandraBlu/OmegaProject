// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/OPlayer.h"

#include "EnhancedInputSubsystems.h"
#include "NiagaraComponent.h"
#include "OGameplayTags.h"
#include "Actor/OWeapon.h"
#include "Component/OEquipmentComponent.h"
#include "Component/OFootstepComponent.h"
#include "Components/CapsuleComponent.h"
#include "Framework/OPlayerController.h"
#include "Framework/OPlayerState.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GAS/OAbilitySystemComponent.h"
#include "GAS/DataAsset/OLevelUpInfo.h"
#include "Input/OInputComponent.h"
#include "Input/OInputConfig.h"
#include "UI/MasterHUD.h"


AOPlayer::AOPlayer()
{
	GetCharacterMovement()->NavAgentProps.bCanCrouch = true;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	
	Gear = CreateDefaultSubobject<UOEquipmentComponent>("GearComp");
	CharacterClass = ECharacterClass::Elementalist;

	LevelUpFX = CreateDefaultSubobject<UNiagaraComponent>(TEXT("VFXComp"));
	LevelUpFX->SetupAttachment(GetRootComponent());
	LevelUpFX->bAutoActivate = false;
	
	FootstepComponent = CreateDefaultSubobject<UOFootstepComponent>(TEXT("FootstepComp"));

	bUsingBlock = false;
}

void AOPlayer::PossessedBy(AController* NewController)
{
	//server
	Super::PossessedBy(NewController);
	InitAbilityActorInfo();
	GrantAbilities();
}

void AOPlayer::OnRep_PlayerState()
{
	//client
	Super::OnRep_PlayerState();
	InitAbilityActorInfo();
}

int32 AOPlayer::GetPlayerLevel_Implementation()
{
	AOPlayerState* OPS = GetPlayerState<AOPlayerState>();
	check(OPS);
	return OPS->GetPlayerLevel();
}

FVector AOPlayer::GetCombatSocketLocation_Implementation(const FGameplayTag& CombatSocketTag)
{
	const FOGameplayTags& GameplayTags = FOGameplayTags::Get();
	if (CombatSocketTag.MatchesTagExact(GameplayTags.combatSocket_weapon) && IsValid(Gear->EquippedWeapon))
	{
		return Gear->EquippedWeapon->GetWeaponMesh()->GetSocketLocation(Gear->EquippedWeapon->FiringSocket);
	}
	if (CombatSocketTag.MatchesTagExact(GameplayTags.combatSocket_handL))
	{
		return GetMesh()->GetSocketLocation(HandRSocket);
	}
	if (CombatSocketTag.MatchesTagExact(GameplayTags.combatSocket_handR))
	{
		return GetMesh()->GetSocketLocation(HandLSocket);
	}
	return FVector();
}

 AActor* AOPlayer::GetCurrentEquippedWeapon_Implementation(AOWeapon* InWeapon)
 {
 	return Gear->EquippedWeapon;
 }

void AOPlayer::Die(const FVector& DeathImpulse)
{
	if (!Gear->EquippedWeapon) return;
	Gear->EquippedWeapon->DetachFromActor(FDetachmentTransformRules(EDetachmentRule::KeepWorld, true));
	Gear->EquippedWeapon->GetWeaponMesh()->SetSimulatePhysics(true);
	Gear->EquippedWeapon->GetWeaponMesh()->SetEnableGravity(true);
	Gear->EquippedWeapon->GetWeaponMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	Gear->EquippedWeapon->GetWeaponMesh()->AddImpulse(DeathImpulse);

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
	bDead = true;
	OnDeath.Broadcast(this);
}

void AOPlayer::AddToXP_Implementation(int32 InXP)
{
	AOPlayerState* OPS = GetPlayerState<AOPlayerState>();
	check(OPS);
	OPS->AddToXP(InXP);
}

void AOPlayer::LevelUp_Implementation()
{
	MulticastLevelUpVFX();
}

void AOPlayer::MulticastLevelUpVFX_Implementation()
{
	if (IsValid(LevelUpFX))
	{
		LevelUpFX->Activate(true);
	}
}

int32 AOPlayer::GetXP_Implementation() const
{
	AOPlayerState* OPS = GetPlayerState<AOPlayerState>();
	check(OPS);
	return OPS->GetXP();
}

int32 AOPlayer::FindLevelForXP_Implementation(int32 InXP) const
{
	AOPlayerState* OPS = GetPlayerState<AOPlayerState>();
	check(OPS);
	return OPS->LevelUpInfo->FindLevelForXP(InXP);
}

int32 AOPlayer::GetAttributePtsReward_Implementation(int32 Level) const
{
	AOPlayerState* OPS = GetPlayerState<AOPlayerState>();
	check(OPS);
	return OPS->LevelUpInfo->LevelUpInfo[Level].AttributePointReward;
}

int32 AOPlayer::GetAbilityPtsReward_Implementation(int32 Level) const
{
	AOPlayerState* OPS = GetPlayerState<AOPlayerState>();
	check(OPS);
	return OPS->LevelUpInfo->LevelUpInfo[Level].AbilityPointReward;
}

void AOPlayer::AddToPlayerLevel_Implementation(int32 InPlayerLevel)
{
	AOPlayerState* OPS = GetPlayerState<AOPlayerState>();
	check(OPS);
	OPS->AddToLevel(InPlayerLevel);
	if (UOAbilitySystemComponent* OASC = Cast<UOAbilitySystemComponent>(GetAbilitySystemComponent()))
	{
		OASC->UpdateAbilityStatus(OPS->GetPlayerLevel());
	}
}

void AOPlayer::AddToAttributePts_Implementation(int32 InAttributePoints)
{
	AOPlayerState* OPS = GetPlayerState<AOPlayerState>();
	check(OPS);
	OPS->AddToAttributePts(InAttributePoints);
}


void AOPlayer::AddToAbilityPts_Implementation(int32 InAbilityPoints)
{
	AOPlayerState* OPS = GetPlayerState<AOPlayerState>();
	check(OPS);
	OPS->AddToAbilityPts(InAbilityPoints);
}

int32 AOPlayer::GetAttributePoints_Implementation() const
{
	AOPlayerState* OPS = GetPlayerState<AOPlayerState>();
	check(OPS);
	return OPS->GetAttributePts();
}

int32 AOPlayer::GetAbilityPoints_Implementation() const
{
	AOPlayerState* OPS = GetPlayerState<AOPlayerState>();
	check(OPS);
	return OPS->GetAbilityPts();
}

AOWeapon* AOPlayer::GetCurrentWeapon_Implementation()
{
	if (Gear->EquippedWeapon)
	{
		return Gear->EquippedWeapon;
	}
	return nullptr;
}

UOFootstepComponent* AOPlayer::GetFootstepsComp() const
{
	return FootstepComponent;
}

bool AOPlayer::GetIsUsingBlock()
{
	return bUsingBlock;
}

bool AOPlayer::SetIsUsingBlock(bool block)
{
	bUsingBlock = block;
	return bUsingBlock;
}

void AOPlayer::BeginPlay()
{
	Super::BeginPlay();
	
}

void AOPlayer::InitAbilityActorInfo()
{
	AOPlayerState* OPS = GetPlayerState<AOPlayerState>();
	check(OPS);
	OPS->GetAbilitySystemComponent()->InitAbilityActorInfo(OPS, this);
	Cast<UOAbilitySystemComponent>(OPS->GetAbilitySystemComponent())->AbilityActorInfoSet();
	AbilitySystemComponent = OPS->GetAbilitySystemComponent();
	AttributeSet = OPS->GetAttributeSet();
	OnASCRegistered.Broadcast(AbilitySystemComponent);
	
	if (AOPlayerController* RPC = Cast<AOPlayerController>(GetController()))
	{
		if (AMasterHUD* OHUD = Cast<AMasterHUD>(RPC->GetHUD()))
		{
			OHUD->InitOverlay(RPC, OPS, AbilitySystemComponent, AttributeSet);
		}
	}
	InitializeAttributes();
}


void AOPlayer::AbilityInputTagPressed(FGameplayTag InputTag)
{
	if (Gear->EquippedWeapon == nullptr ) return;
	if (GetASC() == nullptr) return;
	if (GetASC()) GetASC()->AbilityInputTagPressed(InputTag);
}

void AOPlayer::AbilityInputTagReleased(FGameplayTag InputTag)
{
	if (GetASC() == nullptr) return;
	GetASC()->AbilityInputTagReleased(InputTag);
}

UOAbilitySystemComponent* AOPlayer::GetASC()
{
	if (OAbilitySystemComponent == nullptr)
	{
		OAbilitySystemComponent = CastChecked<UOAbilitySystemComponent>(AbilitySystemComponent);
	}
	return OAbilitySystemComponent;
}

void AOPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	ULocalPlayer* LocalPlayer = GetController<APlayerController>()->GetLocalPlayer();
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer);

	check(Subsystem);
	Subsystem->AddMappingContext(InputConfig->DefaultMappingContext, 0);
	if (UOInputComponent* InputComp = CastChecked<UOInputComponent>(InputComponent))
	{
	
		//Ability
		InputComp->BindAbilityActions(InputConfig, this, &ThisClass::AbilityInputTagPressed, &ThisClass::AbilityInputTagReleased);
	}
}