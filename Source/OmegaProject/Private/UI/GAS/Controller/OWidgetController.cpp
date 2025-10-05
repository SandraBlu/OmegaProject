// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/GAS/Controller/OWidgetController.h"

#include "Framework/OPlayerController.h"
#include "Framework/OPlayerState.h"
#include "GAS/OAbilitySystemComponent.h"
#include "GAS/OAttributeSet.h"
#include "GAS/DataAsset/OAbilityInfo.h"

void UOWidgetController::SetWidgetControllerParams(const FWidgetControllerParams& WCParams)
{
	PlayerController = WCParams.PlayerController;
	PlayerState = WCParams.PlayerState;
	AbilitySystemComponent = WCParams.AbilitySystemComponent;
	AttributeSet = WCParams.AttributeSet;
}

void UOWidgetController::BroadcastInitialValues()
{
}

void UOWidgetController::BindCallbacksToDependencies()
{
}

void UOWidgetController::BroadcastAbilityInfo()
{
	if (!GetOASC()->bGrantedAbilitiesGiven) return;
	
	FForEachAbility BroadcastDelegate;
	BroadcastDelegate.BindLambda([this](const FGameplayAbilitySpec& AbilitySpec)
	{
		FAbilityInfo Info = AbilityInfo->FindAbilityInfoForTag(OASC->GetAbilityTagFromSpec(AbilitySpec));
		Info.InputTag = OASC->GetInputTagFromSpec(AbilitySpec);
		Info.StatusTag = OASC->GetStatusFromSpec(AbilitySpec);
		AbilityInfoDelegate.Broadcast(Info);
	});
	GetOASC()->ForEachAbility(BroadcastDelegate);
}

AOPlayerController* UOWidgetController::GetOPC()
{
	if (OPC == nullptr)
	{
		OPC = Cast<AOPlayerController>(PlayerController);
	}
	return OPC;
}

AOPlayerState* UOWidgetController::GetOPS()
{
	if (OPS == nullptr)
	{
		OPS = Cast<AOPlayerState>(PlayerState);
	}
	return OPS;
}

UOAbilitySystemComponent* UOWidgetController::GetOASC()
{
	if (OASC == nullptr)
	{
		OASC = Cast<UOAbilitySystemComponent>(AbilitySystemComponent);
	}
	return OASC;
}

UOAttributeSet* UOWidgetController::GetOAS()
{
	if (OAS == nullptr)
	{
		OAS = Cast<UOAttributeSet>(AttributeSet);
	}
	return OAS;
}