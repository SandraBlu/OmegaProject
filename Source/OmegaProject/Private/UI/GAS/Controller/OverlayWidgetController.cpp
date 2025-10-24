// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/GAS/Controller/OverlayWidgetController.h"

#include "AbilitySystemComponent.h"
#include "OGameplayTags.h"
#include "Framework/OPlayerState.h"
#include "GAS/OAbilitySystemComponent.h"
#include "GAS/OAttributeSet.h"
#include "GAS/DataAsset/OAbilityInfo.h"
#include "GAS/DataAsset/OLevelUpInfo.h"
#include "UI/OUserWidget.h"

void UOverlayWidgetController::BroadcastInitialValues()
{
	
	OnHealthChange.Broadcast(GetOAS()->GetHealth());
	OnMaxHealthChange.Broadcast(GetOAS()->GetMaxHealth());
	OnStaminaChange.Broadcast(GetOAS()->GetStamina());
	OnMaxStaminaChange.Broadcast(GetOAS()->GetMaxStamina());
	OnEnergyChange.Broadcast(GetOAS()->GetEnergy());
	OnMaxEnergyChange.Broadcast(GetOAS()->GetMaxEnergy());
}

void UOverlayWidgetController::BindCallbacksToDependencies()
{
	//XP Change
	GetOPS()->OnXPChangeDelegate.AddUObject(this, &UOverlayWidgetController::OnXPChange);
	GetOPS()->OnLevelChangeDelegate.AddLambda([this](int32 NewLevel)
	{
		OnLevelChangeDelegate.Broadcast(NewLevel);
	});
	//Attribute Change
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(GetOAS()->GetHealthAttribute()).AddLambda([this](const FOnAttributeChangeData& Data){OnHealthChange.Broadcast(Data.NewValue);});
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(GetOAS()->GetMaxHealthAttribute()).AddLambda([this](const FOnAttributeChangeData& Data){OnMaxHealthChange.Broadcast(Data.NewValue);});
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(GetOAS()->GetStaminaAttribute()).AddLambda([this](const FOnAttributeChangeData& Data){OnStaminaChange.Broadcast(Data.NewValue);});
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(GetOAS()->GetMaxStaminaAttribute()).AddLambda([this](const FOnAttributeChangeData& Data){OnMaxStaminaChange.Broadcast(Data.NewValue);});
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(GetOAS()->GetEnergyAttribute()).AddLambda([this](const FOnAttributeChangeData& Data){OnEnergyChange.Broadcast(Data.NewValue);});
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(GetOAS()->GetMaxEnergyAttribute()).AddLambda([this](const FOnAttributeChangeData& Data){OnMaxEnergyChange.Broadcast(Data.NewValue);});

	 //Ability Change
	if (GetOASC())
	{
		GetOASC()->AbilityEquippedDelegate.AddUObject(this, &UOverlayWidgetController::OnAbilityEquipped);
		if (GetOASC()->bGrantedAbilitiesGiven)
		{
			BroadcastAbilityInfo();
		}
		else
		{
			GetOASC()->AbilityGivenDelegate.AddUObject(this, &UOverlayWidgetController::BroadcastAbilityInfo);
		}

		//Effect Message
    	GetOASC()->EffectAssetTags.AddLambda(
    		[this](const FGameplayTagContainer& AssetTags)
    		{
    			for (const FGameplayTag& Tag : AssetTags)
    			{
    				FGameplayTag MessageTag = FGameplayTag::RequestGameplayTag(FName("message"));
    				if (Tag.MatchesTag(MessageTag))
    				{
    					const FUIMessageRow* Row = GetDataTableRowByTag<FUIMessageRow>(MessageWidgetDataTable, Tag);
    					MessageWidgetDelegate.Broadcast(*Row);
    				}
    			}
    		}
    	);	
	}
	
}




void UOverlayWidgetController::OnXPChange(int32 NewXP)
{
	const UOLevelUpInfo* LevelUpInfo = GetOPS()->LevelUpInfo;
	checkf(LevelUpInfo, TEXT("Unable to find XPinfo, Enter in BP_PlayerState"));

	const int32 CurrentLevel = LevelUpInfo->FindLevelForXP(NewXP);
	const int32 MaxLevel = LevelUpInfo->LevelUpInfo.Num();

	if (CurrentLevel <= MaxLevel && CurrentLevel >0)
	{
		const int32 LevelUpReq = LevelUpInfo->LevelUpInfo[CurrentLevel].LevelUpRequirement;
		const int32 PrevLevelUpReq = LevelUpInfo->LevelUpInfo[CurrentLevel -1].LevelUpRequirement;

		const int32 DeltaLevelReq = LevelUpReq - PrevLevelUpReq;
		const int32 XPForThisLevel = NewXP - PrevLevelUpReq;

		const float XPBarPercent = static_cast<float>(XPForThisLevel)/ static_cast<float>(DeltaLevelReq);

		OnXPPercentChangeDelegate.Broadcast(XPBarPercent);
	}
}

void UOverlayWidgetController::OnAbilityEquipped(const FGameplayTag& AbilityTag, const FGameplayTag& Status, const FGameplayTag& Slot, const FGameplayTag& PrevSlot) const
{
	//Clear Out Old Slot
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
}