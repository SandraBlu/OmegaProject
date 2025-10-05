// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/GAS/Controller/AttributeMenuController.h"

#include "Framework/OPlayerState.h"
#include "GAS/OAbilitySystemComponent.h"
#include "GAS/OAttributeSet.h"
#include "GAS/DataAsset/OAttributeInfo.h"

void UAttributeMenuController::BindCallbacksToDependencies()
{
	
	check(AttributeInfo);
	for (auto& Pair : GetOAS()->TagsToAttributes)
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Pair.Value()).AddLambda([this, Pair](const FOnAttributeChangeData& Data)
		{
			BroadcastAttributeInfo(Pair.Key, Pair.Value());
		});	
	}
	GetOPS()->OnAttributePtsChangeDelegate.AddLambda([this] (int32 Points)
	{
		AttributePointsChangeDelegate.Broadcast(Points);
	});
}

void UAttributeMenuController::BroadcastInitialValues()
{
	check(AttributeInfo);
	for (auto& Pair : GetOAS()->TagsToAttributes)
	{
		BroadcastAttributeInfo(Pair.Key, Pair.Value());
	}
	AttributePointsChangeDelegate.Broadcast(GetOPS()->GetAttributePts());
}

void UAttributeMenuController::UpgradeAttribute(const FGameplayTag& AttributeTag)
{
	GetOASC()->UpgradeAttribute(AttributeTag);
}

void UAttributeMenuController::BroadcastAttributeInfo(const FGameplayTag& AttributeTag, const FGameplayAttribute& Attribute) const
{
	FAttributeData Info = AttributeInfo->FindAttributeInfoForTag(AttributeTag);
	Info.AttributeValue = Attribute.GetNumericValue(AttributeSet);
	AttributeInfoDelegate.Broadcast(Info);
}