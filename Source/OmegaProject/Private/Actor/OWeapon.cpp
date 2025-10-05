// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/OWeapon.h"

#include "Character/OPlayer.h"

// Sets default values
AOWeapon::AOWeapon()
{
	SKMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	SKMesh->bReceivesDecals = false;
	SKMesh->SetCollisionObjectType(ECC_WorldDynamic);
	SKMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SKMesh->SetCollisionResponseToAllChannels(ECR_Ignore);
	RootComponent = SKMesh;
	InactiveWeaponSocket = FName("inactive_katana");
	ActiveWeaponSocket = FName("weapon_r");
}

void AOWeapon::OnEquip()
{
	AttachMeshToPawn(PawnOwner->GetMesh(), ActiveWeaponSocket);
	PlayEquipMontage("Draw");
}

void AOWeapon::OnUnEquip()
{
	AttachMeshToPawn(PawnOwner->GetMesh(), InactiveWeaponSocket);
}

USkeletalMeshComponent* AOWeapon::GetWeaponMesh() const
{
	return SKMesh;
}

void AOWeapon::BeginPlay()
{
	Super::BeginPlay();

	PawnOwner = Cast<AOPlayer>(GetOwner());
}

void AOWeapon::PlayEquipMontage(const FName SectionName)
{
	UAnimInstance* AnimInstance = PawnOwner->GetMesh()->GetAnimInstance();
	if (AnimInstance && EquipAnim)
	{
		AnimInstance->Montage_Play(EquipAnim);
		AnimInstance->Montage_JumpToSection(SectionName, EquipAnim);
	}
}

class AOPlayer* AOWeapon::GetPawnOwner() const
{
	return PawnOwner;
}

void AOWeapon::AttachMeshToPawn(USceneComponent* InParent, FName InSocketName)
{
	if (PawnOwner)
	{
		FAttachmentTransformRules TransformRules(EAttachmentRule::SnapToTarget, true);
		USkeletalMeshComponent* PawnMesh = PawnOwner->GetMesh();
		AttachToComponent(PawnOwner->GetMesh(), TransformRules, InSocketName);
	}
}

