// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/OEquipItem.h"

#include "Actor/OWeapon.h"
#include "Component/OEquipmentComponent.h"

#define LOCTEXT_NAMESPACE "EquipItem"

UOEquipItem::UOEquipItem()
{
	bStackable = false;
	Weight = 1.f;
	bCanActivate = true;
	bToggleActiveOnUse = true;
	UseActionText = LOCTEXT("ItemUseActionText", "Equip");
}

void UOEquipItem::HandleEquip_Implementation()
{

}

void UOEquipItem::HandleUnequip_Implementation()
{

}

void UOEquipItem::Activated_Implementation()
{
	if (GetOwningPawn())
	{
		UseActionText = LOCTEXT("UnequipText", "Unequip");

		if (UOEquipmentComponent* EquipmentComponent = Cast<UOEquipmentComponent>(GetOwningPawn()->GetComponentByClass(UOEquipmentComponent::StaticClass())))
		{
			EquipmentComponent->EquipItem(this);
		}
	}
}

void UOEquipItem::Deactivated_Implementation()
{
	if (GetOwningPawn())
	{
		UseActionText = LOCTEXT("EquipText", "Equip");

		if (UOEquipmentComponent* EquipmentComponent = Cast<UOEquipmentComponent>(GetOwningPawn()->GetComponentByClass(UOEquipmentComponent::StaticClass())))
		{
			EquipmentComponent->UnequipItem(this);
		}
	}
}

UOEquipItem_Clothing::UOEquipItem_Clothing()
{

}

void UOEquipItem_Clothing::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	if (PropertyChangedEvent.GetPropertyName() == GET_MEMBER_NAME_CHECKED(UOEquipItem_Clothing, ClothingMesh))
	{
		if (ClothingMesh)
		{
			ClothingMaterials.Empty();

			for (auto& ClothingMeshMat : ClothingMesh->GetMaterials())
			{
				ClothingMaterials.Add(ClothingMeshMat.MaterialInterface);
			}
		}
	}
}

void UOEquipItem_Clothing::HandleUnequip_Implementation()
{
	if (UOEquipmentComponent* EquipmentComponent = Cast<UOEquipmentComponent>(GetOwningPawn()->GetComponentByClass(UOEquipmentComponent::StaticClass())))
	{
		if (EquipmentComponent->EquippableComponents.Contains(Slot) && EquipmentComponent->DefaultClothing.Contains(Slot) && EquipmentComponent->DefaultClothingMaterials.Contains(Slot))
		{
			//Set the clothing back to its default mesh and materials
			if (USkeletalMeshComponent* SlotComponent = *EquipmentComponent->EquippableComponents.Find(Slot))
			{
				if (USkeletalMesh* DefaultClothingMesh = *EquipmentComponent->DefaultClothing.Find(Slot))
				{
					SlotComponent->SetSkeletalMesh(DefaultClothingMesh);

					if (FDefaultClothingMeshMaterials* DefaultMaterials = EquipmentComponent->DefaultClothingMaterials.Find(Slot))
					{
						int32 Idx = 0;

						for (auto& DefaultMat : DefaultMaterials->Materials)
						{
							SlotComponent->SetMaterial(Idx, DefaultMat);
							++Idx;
						}
					}
				}
				else
				{
					SlotComponent->SetSkeletalMesh(nullptr);
				}

				if (EquipmentComponent->LeaderPoseComponent)
				{
					SlotComponent->SetLeaderPoseComponent(EquipmentComponent->LeaderPoseComponent);
				}
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Narrative Equipment tried unequipping item but the Equipment Component doesn't have a skeletal mesh component added. Have you called initialize?"), *GetNameSafe(this));
		}

	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Narrative Equipment tried unequipping your item, but your pawn doesn't have an Equipment Component added. Please add one."));
	}
}

void UOEquipItem_Clothing::HandleEquip_Implementation()
{
	if (UOEquipmentComponent* EquipmentComponent = Cast<UOEquipmentComponent>(GetOwningPawn()->GetComponentByClass(UOEquipmentComponent::StaticClass())))
	{
		if (ClothingMesh)
		{
			if (EquipmentComponent->EquippableComponents.Contains(Slot))
			{
				//Set the clothing mesh to the new mesh
				if (USkeletalMeshComponent* SlotComponent = *EquipmentComponent->EquippableComponents.Find(Slot))
				{
					SlotComponent->SetSkeletalMesh(ClothingMesh);

					int32 Idx = 0;
					for (auto& Mat : ClothingMaterials)
					{
						SlotComponent->SetMaterial(Idx, Mat);
						++Idx;
					}

					if (EquipmentComponent->LeaderPoseComponent)
					{
						SlotComponent->SetLeaderPoseComponent(EquipmentComponent->LeaderPoseComponent);
					}
				}
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Narrative Equipment tried equipping %s but the Equipment Component doesn't have a skeletal mesh component added. Have you called Initialize?"), *GetNameSafe(this));
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Narrative Equipment tried putting an item on, but your pawn doesn't have an Equipment Component added. Please add one."));
	}
}

UOEquipItem_Weapon::UOEquipItem_Weapon()
{

}

void UOEquipItem_Weapon::HandleUnequip_Implementation()
{
	UOEquipmentComponent* EquipmentComponent = Cast<UOEquipmentComponent>(GetOwningPawn()->GetComponentByClass(UOEquipmentComponent::StaticClass()));
	if (EquipmentComponent->EquippedWeapon)
	{
		EquipmentComponent->EquippedWeapon->OnUnEquip();
		EquipmentComponent->EquippedWeapon->Destroy();
		EquipmentComponent->EquippedWeapon = nullptr;
		EquipmentComponent->OnWeaponEquipped();
	}
}

void UOEquipItem_Weapon::HandleEquip_Implementation()
{
	if (WeaponClass)
	{
		UOEquipmentComponent* EquipmentComponent = Cast<UOEquipmentComponent>(GetOwningPawn()->GetComponentByClass(UOEquipmentComponent::StaticClass()));
		if (EquipmentComponent->EquippedWeapon)
		{
			EquipmentComponent->EquippedWeapon->OnUnEquip();
			EquipmentComponent->EquippedWeapon->Destroy();
			EquipmentComponent->EquippedWeapon = nullptr;
			EquipmentComponent->OnWeaponEquipped();
		}
		//Spawn in Weapon
		FActorSpawnParameters SpawnParams;
		SpawnParams.bNoFail = true;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
		SpawnParams.Owner = SpawnParams.Instigator = GetOwningPawn();

		if (AOWeapon* Weapon = GetWorld()->SpawnActor<AOWeapon>(WeaponClass, SpawnParams))
		{
			EquipmentComponent->EquippedWeapon = Weapon;
			EquipmentComponent->OnWeaponEquipped();
			//Attach Weapon to Pawn
			Weapon->OnEquip();
		}
	}
}

#undef LOCTEXT_NAMESPACE