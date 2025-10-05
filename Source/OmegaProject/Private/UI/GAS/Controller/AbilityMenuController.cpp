// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/GAS/Controller/AbilityMenuController.h"

#include "Framework/OPlayerState.h"
#include "GAS/OAbilitySystemComponent.h"
#include "GAS/DataAsset/OAbilityInfo.h"

void UAbilityMenuController::BroadcastInitialValues()
{
	BroadcastAbilityInfo();
	AbilityPointsChange.Broadcast(GetOPS()->GetAbilityPts());
}

void UAbilityMenuController::BindCallbacksToDependencies()
{
	GetOASC()->AbilityStatusChangeDelegate.AddLambda([this] (const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag, int32 NewLevel)
	{
		if (SelectedAbility.Ability.MatchesTagExact(AbilityTag))
		{
			SelectedAbility.Status = StatusTag;
			bool bEnableSpendPoints = false;
			bool bEnableEquip = false;
			ShouldEnableButtons(StatusTag, CurrentAbilityPoints, bEnableSpendPoints, bEnableEquip);
			FString Description;
			FString NextLevelDescription;
			GetOASC()->GetDescriptionByAbilityTag(AbilityTag, Description, NextLevelDescription);
			AbilitySelectedDelegate.Broadcast(bEnableSpendPoints, bEnableEquip, Description, NextLevelDescription);
		}
		
		if (AbilityInfo)
		{
			FAbilityInfo Info = AbilityInfo->FindAbilityInfoForTag(AbilityTag);
			Info.StatusTag = StatusTag;
			AbilityInfoDelegate.Broadcast(Info);
		}
	});

	GetOASC()->AbilityEquippedDelegate.AddUObject(this, &UAbilityMenuController::OnAbilityEquipped);
	GetOPS()->OnAbilityPtsChangeDelegate.AddLambda([this] (int32 AbilityPoints)
	{
		AbilityPointsChange.Broadcast(AbilityPoints);
		CurrentAbilityPoints = AbilityPoints;

		bool bEnableSpendPoints = false;
		bool bEnableEquip = false;
		ShouldEnableButtons(SelectedAbility.Status, CurrentAbilityPoints, bEnableSpendPoints, bEnableEquip);
		FString Description;
		FString NextLevelDescription;
		GetOASC()->GetDescriptionByAbilityTag(SelectedAbility.Ability, Description, NextLevelDescription);
		AbilitySelectedDelegate.Broadcast(bEnableSpendPoints, bEnableEquip, Description, NextLevelDescription);
	});
}

void UAbilityMenuController::AbilitySelected(const FGameplayTag& AbilityTag)
{
	if (bWaitingForEquipSelection)
	{
		FGameplayTag SelectedAbilityType = AbilityInfo->FindAbilityInfoForTag(AbilityTag).AbilityType;
		StopWaitEquipDelegate.Broadcast(SelectedAbilityType);
		bWaitingForEquipSelection = false;
	}
	
	const FOGameplayTags GameplayTags = FOGameplayTags::Get();
	const int32 AbilityPoints = GetOPS()->GetAbilityPts();
	FGameplayTag AbilityStatus;

	const bool bTagValid = AbilityTag.IsValid();
	const bool bTagNone = AbilityTag.MatchesTag(GameplayTags.ability_none);
	const FGameplayAbilitySpec* AbilitySpec = GetOASC()->GetSpecFromAbilityTag(AbilityTag);
	const bool bSpecValid = AbilitySpec != nullptr;
	if (!bTagValid || bTagNone || !bSpecValid)
	{
		AbilityStatus = GameplayTags.ability_status_locked;
	}
	else 
	{
		AbilityStatus = GetOASC()->GetStatusFromSpec(*AbilitySpec);
	}
	SelectedAbility.Ability = AbilityTag;
	SelectedAbility.Status = AbilityStatus;
	bool bEnableSpendPoints = false;
	bool bEnableEquip = false;
	ShouldEnableButtons(AbilityStatus, AbilityPoints, bEnableSpendPoints, bEnableEquip);
	FString Description;
	FString NextLevelDescription;
	GetOASC()->GetDescriptionByAbilityTag(AbilityTag, Description, NextLevelDescription);
	AbilitySelectedDelegate.Broadcast(bEnableSpendPoints, bEnableEquip, Description, NextLevelDescription);
}

void UAbilityMenuController::SpendPointButtonPressed()
{
	if (GetOASC())
	{
		GetOASC()->ServerSpendAbilityPoint(SelectedAbility.Ability);
	}
}

void UAbilityMenuController::AbilityDeselect()
{
	if (bWaitingForEquipSelection)
	{
		FGameplayTag SelectedAbilityType = AbilityInfo->FindAbilityInfoForTag(SelectedAbility.Ability).AbilityType;
		StopWaitEquipDelegate.Broadcast(SelectedAbilityType);
		bWaitingForEquipSelection = false;
	}
	
	SelectedAbility.Ability = FOGameplayTags::Get().ability_none;
	SelectedAbility.Status = FOGameplayTags::Get().ability_status_locked;
	AbilitySelectedDelegate.Broadcast(false, false, FString(), FString());
}

void UAbilityMenuController::EquipButtonPressed()
{
	const FGameplayTag AbilityType = AbilityInfo->FindAbilityInfoForTag(SelectedAbility.Ability).AbilityType;

	WaitEquipDelegate.Broadcast(AbilityType);
	bWaitingForEquipSelection =true;

	const FGameplayTag SelectedStatus = GetOASC()->GetStatusFromAbilityTag(SelectedAbility.Ability);
	if (SelectedStatus.MatchesTagExact(FOGameplayTags::Get().ability_status_equipped))
	{
		SelectedSlot = GetOASC()->GetInputTagFromAbilityTag(SelectedAbility.Ability);
	}
}

void UAbilityMenuController::EquippedRowInputPressed(const FGameplayTag& SlotTag, const FGameplayTag& AbilityType)
{
	if (!bWaitingForEquipSelection) return;
	//check selected ability against slot ability type
	const FGameplayTag& SelectedAbilityType = AbilityInfo->FindAbilityInfoForTag(SelectedAbility.Ability).AbilityType;
	if (!SelectedAbilityType.MatchesTagExact(AbilityType)) return;

	GetOASC()->ServerEquipAbility(SelectedAbility.Ability, SlotTag);
	
}

void UAbilityMenuController::OnAbilityEquipped(const FGameplayTag& AbilityTag, const FGameplayTag& Status,	const FGameplayTag& Slot, const FGameplayTag& PrevSlot)
{
	//Clear Out Old Slot
	bWaitingForEquipSelection = false;
	const FOGameplayTags Tag = FOGameplayTags::Get();
	FAbilityInfo PrevSlotInfo;
	PrevSlotInfo.StatusTag = Tag.ability_status_unlocked;
	PrevSlotInfo.InputTag = PrevSlot;
	PrevSlotInfo.AbilityTag =Tag.ability_none;
	AbilityInfoDelegate.Broadcast(PrevSlotInfo);
	//Fill new slot
	FAbilityInfo Info = AbilityInfo->FindAbilityInfoForTag(AbilityTag);
	Info.StatusTag = Status;
	Info.InputTag = Slot;
	AbilityInfoDelegate.Broadcast(Info);

	StopWaitEquipDelegate.Broadcast(AbilityInfo->FindAbilityInfoForTag(AbilityTag).AbilityType);
	AbilityReassignedDelegate.Broadcast(AbilityTag);
	AbilityDeselect();
}

void UAbilityMenuController::ShouldEnableButtons(const FGameplayTag& AbilityStatus, int32 AbilityPoints, bool& bEnableSpendPointButton, bool& bEnableEquipButton)
{
	const FOGameplayTags GameplayTags = FOGameplayTags::Get();

	bEnableSpendPointButton = false;
	bEnableEquipButton = false;
	if (AbilityStatus.MatchesTagExact(GameplayTags.ability_status_equipped))
	{
		bEnableEquipButton = true;
		if (AbilityPoints > 0)
		{
			bEnableSpendPointButton = true;
		}
	}
	else if (AbilityStatus.MatchesTagExact(GameplayTags.ability_status_available))
	{
		if (AbilityPoints > 0)
		{
			bEnableSpendPointButton = true;
		}
	}
	else if (AbilityStatus.MatchesTagExact(GameplayTags.ability_status_unlocked))
	{
		bEnableEquipButton = true;
		if (AbilityPoints > 0)
		{
			bEnableSpendPointButton = true;
		}
	}
}