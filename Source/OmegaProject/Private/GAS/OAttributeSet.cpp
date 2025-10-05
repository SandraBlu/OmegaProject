// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/OAttributeSet.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "GameplayEffect.h"
#include "GameplayEffectExtension.h"
#include "OAbilityTypes.h"
#include "OGameplayTags.h"
#include "Framework/OPlayerController.h"
#include "GameFramework/Character.h"
#include "GameplayEffectComponents/TargetTagsGameplayEffectComponent.h"
#include "GAS/OAbilitySystemLibrary.h"
#include "Interface/OCombatInterface.h"
#include "Interface/OPlayerInterface.h"
#include "Net/UnrealNetwork.h"

UOAttributeSet::UOAttributeSet()
{
	const FOGameplayTags& GameplayTags = FOGameplayTags::Get();
	
	TagsToAttributes.Add(GameplayTags.Attributes_Primary_Agility, GetAgilityAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Primary_Intelligence, GetIntelligenceAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Primary_Perception, GetPerceptionAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Primary_Resilience, GetResilienceAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Primary_Strength, GetStrengthAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Primary_Vigor, GetVigorAttribute);

	//Secondary Attributes
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_Armor, GetArmorAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_ArmorPenetration, GetArmorPenetrationAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_BlockChance, GetBlockChanceAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_CriticalHitChance, GetCritHitChanceAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_CriticalHitDamage, GetCritHitDamageAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_CriticalHitResistance, GetCritHitResistanceAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_DodgeChance, GetDodgeChanceAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_Stealth, GetStealthAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_HealthRegen, GetHealthRegenAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_StaminaRegen, GetStaminaRegenAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_EnergyRegen, GetEnergyRegenAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_MaxHealth, GetMaxHealthAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_MaxStamina, GetMaxStaminaAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_MaxEnergy, GetMaxEnergyAttribute);

	TagsToAttributes.Add(GameplayTags.Attributes_Resistance_Dark, GetDarkResistanceAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Resistance_Electric, GetElectricResistanceAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Resistance_Fire, GetFireResistanceAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Resistance_Ice, GetIceResistanceAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Resistance_Physical, GetPhysicalResistanceAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Resistance_Poison, GetPoisonResistanceAttribute);
}

void UOAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UOAttributeSet, Health, COND_None, REPNOTIFY_Always)
	DOREPLIFETIME_CONDITION_NOTIFY(UOAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always)
	DOREPLIFETIME_CONDITION_NOTIFY(UOAttributeSet, Stamina, COND_None, REPNOTIFY_Always)
	DOREPLIFETIME_CONDITION_NOTIFY(UOAttributeSet, MaxStamina, COND_None, REPNOTIFY_Always)
	DOREPLIFETIME_CONDITION_NOTIFY(UOAttributeSet, Energy, COND_None, REPNOTIFY_Always)
	DOREPLIFETIME_CONDITION_NOTIFY(UOAttributeSet, MaxEnergy, COND_None, REPNOTIFY_Always)

	// Primary Attributes
	DOREPLIFETIME_CONDITION_NOTIFY(UOAttributeSet, Agility, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UOAttributeSet, Intelligence, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UOAttributeSet, Perception, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UOAttributeSet, Resilience, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UOAttributeSet, Strength, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UOAttributeSet, Vigor, COND_None, REPNOTIFY_Always);

	// Secondary Attributes

	DOREPLIFETIME_CONDITION_NOTIFY(UOAttributeSet, Armor, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UOAttributeSet, ArmorPenetration, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UOAttributeSet, BlockChance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UOAttributeSet, DodgeChance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UOAttributeSet, CritHitChance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UOAttributeSet, CritHitDamage, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UOAttributeSet, CritHitResistance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UOAttributeSet, HealthRegen, COND_None, REPNOTIFY_Always);	
	DOREPLIFETIME_CONDITION_NOTIFY(UOAttributeSet, StaminaRegen, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UOAttributeSet, EnergyRegen, COND_None, REPNOTIFY_Always);	
	DOREPLIFETIME_CONDITION_NOTIFY(UOAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);	
	DOREPLIFETIME_CONDITION_NOTIFY(UOAttributeSet, MaxStamina, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UOAttributeSet, MaxEnergy, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UOAttributeSet, Stealth, COND_None, REPNOTIFY_Always);

	// Resistance Attributes
	DOREPLIFETIME_CONDITION_NOTIFY(UOAttributeSet, PhysicalResistance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UOAttributeSet, PoisonResistance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UOAttributeSet, DarkResistance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UOAttributeSet, ElectricResistance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UOAttributeSet, FireResistance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UOAttributeSet, IceResistance, COND_None, REPNOTIFY_Always);
}

void UOAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);
	if (Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxHealth());
	}
	if (Attribute == GetStaminaAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxStamina());
	}
	if (Attribute == GetEnergyAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxEnergy());
	}
	
}

void UOAttributeSet::SetEffectProperties(const FGameplayEffectModCallbackData& Data, FEffectProperties& Props) const
{
	Props.EffectContextHandle = Data.EffectSpec.GetContext();
	Props.SourceASC = Props.EffectContextHandle.GetOriginalInstigatorAbilitySystemComponent();

	if (IsValid(Props.SourceASC) && Props.SourceASC->AbilityActorInfo.IsValid() && Props.SourceASC->AbilityActorInfo->AvatarActor.IsValid())
	{
		Props.SourceAvatarActor = Props.SourceASC->AbilityActorInfo->AvatarActor.Get();
		Props.SourceController = Props.SourceASC->AbilityActorInfo->PlayerController.Get();
		if (Props.SourceController == nullptr && Props.SourceAvatarActor != nullptr)
		{
			if (const APawn* Pawn = Cast<APawn>(Props.SourceAvatarActor))
			{
				Props.SourceController = Pawn->GetController();
			}
		}
		if (Props.SourceController)
		{
			Props.SourceCharacter = Cast<ACharacter>(Props.SourceController->GetPawn());
		}
	}
	if (Data.Target.AbilityActorInfo.IsValid() && Data.Target.AbilityActorInfo->AvatarActor.IsValid())
	{
		Props.TargetAvatarActor = Data.Target.AbilityActorInfo->AvatarActor.Get();
		Props.TargetController = Data.Target.AbilityActorInfo->PlayerController.Get();
		Props.TargetCharacter = Cast<ACharacter>(Props.TargetAvatarActor);
		Props.TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Props.TargetAvatarActor);
	}
}


void UOAttributeSet::ShowFloatingText(const FEffectProperties& Props, float DamageAmount, bool bBlockedHit, bool bDodgedHit, bool bCriticalHit) const
{
	if (Props.SourceCharacter != Props.TargetCharacter)
	{
		if(AOPlayerController* PC = Cast<AOPlayerController>(Props.SourceCharacter->Controller))
		{
			PC->ShowDamageNumber(DamageAmount, Props.TargetCharacter, bBlockedHit, bDodgedHit, bCriticalHit);
			return;
		}
		if(AOPlayerController* PC = Cast<AOPlayerController>(Props.TargetCharacter->Controller))
		{
			PC->ShowDamageNumber(DamageAmount, Props.TargetCharacter, bBlockedHit, bDodgedHit, bCriticalHit);
		}
	}
}

void UOAttributeSet::SendXPEvent(const FEffectProperties& Props)
{
	if (Props.TargetCharacter->Implements<UOCombatInterface>())
	{
		const int32 TargetLevel = IOCombatInterface::Execute_GetPlayerLevel(Props.TargetCharacter);
		const ECharacterClass TargetClass = IOCombatInterface::Execute_GetCharacterClass(Props.TargetCharacter);
		const int32 XPReward = UOAbilitySystemLibrary::GetXPRewardForEnemySlay(Props.TargetCharacter, TargetClass, TargetLevel);
		const FOGameplayTags& GameplayTags = FOGameplayTags::Get();
		FGameplayEventData Payload;
		Payload.EventTag = GameplayTags.Attributes_meta_xp;
		Payload.EventMagnitude = XPReward;
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(Props.SourceCharacter, GameplayTags.Attributes_meta_xp, Payload);
	}
}

void UOAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);
	FEffectProperties Props;
	SetEffectProperties(Data, Props);

	if (Props.TargetCharacter->Implements<UOCombatInterface>() && IOCombatInterface::Execute_IsDead(Props.TargetCharacter)) return;
	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		SetHealth(FMath::Clamp(GetHealth(), 0.f, GetMaxHealth()));
	}
	if (Data.EvaluatedData.Attribute == GetStaminaAttribute())
	{
		SetStamina(FMath::Clamp(GetStamina(), 0.f, GetMaxStamina()));
	}
	if (Data.EvaluatedData.Attribute == GetEnergyAttribute())
	{
		HandleEnergy(Props);
	}
	if (Data.EvaluatedData.Attribute == GetDamageAttribute())
	{
		HandleIncomingDamage(Props);
	}
	if (Data.EvaluatedData.Attribute == GetXPAttribute())
	{
		HandleIncomingXP(Props);
	}
}

void UOAttributeSet::HandleEnergy(const FEffectProperties& Props)
{
	const FOGameplayTags& Tag = FOGameplayTags::Get();
	SetEnergy(FMath::Clamp(GetEnergy(), 0.f, GetMaxEnergy()));
	if (GetEnergy() == GetMaxEnergy())
	{
		UOAbilitySystemLibrary::AddGameplayTagToActorIfNone(Props.TargetCharacter,Tag.status_Rage_Full);
	}
	else if (GetEnergy() == 0.f)
	{
		UOAbilitySystemLibrary::AddGameplayTagToActorIfNone(Props.TargetCharacter,Tag.status_Rage_None);
	}
	else
	{
		UOAbilitySystemLibrary::RemoveGameplayTagFromActorIfFound(Props.TargetCharacter,Tag.status_Rage_Full);
		UOAbilitySystemLibrary::RemoveGameplayTagFromActorIfFound(Props.TargetCharacter,Tag.status_Rage_None);
	}
}

void UOAttributeSet::HandleIncomingDamage(const FEffectProperties& Props)
{
	const float LocalDamage = GetDamage();
	SetDamage(0.f);
	if (LocalDamage > 0.f)
	{
		const float NewHealth = GetHealth() - LocalDamage;
		SetHealth(FMath::Clamp(NewHealth, 0.f, GetMaxHealth()));

		const bool bFatal = NewHealth <= 0.f;
		if (bFatal)
		{
			FGameplayTagContainer TagContainer;
			TagContainer.AddTag(FOGameplayTags::Get().ability_death);
			Props.TargetASC->TryActivateAbilitiesByTag(TagContainer);
			if (IOCombatInterface* CombatInterface = Cast<IOCombatInterface>(Props.TargetAvatarActor))
			{
				CombatInterface->Die(UOAbilitySystemLibrary::GetDeathImpulse(Props.EffectContextHandle));
				UOAbilitySystemLibrary::AddGameplayTagToActorIfNone(Props.TargetCharacter, FOGameplayTags::Get().Event_Death);
			}
			SendXPEvent(Props);
		}
		else
		{
			FGameplayTagContainer TagContainer;
			TagContainer.AddTag(FOGameplayTags::Get().ability_HitReact);
			Props.TargetASC->TryActivateAbilitiesByTag(TagContainer);
			const FVector& KnockbackForce = UOAbilitySystemLibrary::GetKnockbackForce(Props.EffectContextHandle);
			if (!KnockbackForce.IsNearlyZero(1.f))
			{
				Props.TargetCharacter->LaunchCharacter(KnockbackForce, true, true);
			}
		}
		const bool bBlock = UOAbilitySystemLibrary::IsBlockedHit(Props.EffectContextHandle);
		const bool bDodge = UOAbilitySystemLibrary::IsDodgedHit(Props.EffectContextHandle);
		const bool bCriticalHit = UOAbilitySystemLibrary::IsCriticalHit(Props.EffectContextHandle);
		ShowFloatingText(Props, LocalDamage,bBlock, bDodge, bCriticalHit);
		if (UOAbilitySystemLibrary::IsSuccessfulDebuff(Props.EffectContextHandle))
		{
			Debuff(Props);
		}
	}
}

void UOAttributeSet::Debuff(const FEffectProperties& Props)
{
	//SetEffectProperties----------------------------------------------------------------------
	const FOGameplayTags& Tag = FOGameplayTags::Get();
	FGameplayEffectContextHandle EffectContext = Props.SourceASC->MakeEffectContext();
	EffectContext.AddSourceObject(Props.SourceAvatarActor);
	
	const FGameplayTag DamageType = UOAbilitySystemLibrary::GetDamageType(Props.EffectContextHandle);
	const float DebuffDamage = UOAbilitySystemLibrary::GetDebuffDamage(Props.EffectContextHandle);
	const float DebuffDuration = UOAbilitySystemLibrary::GetDebuffDuration(Props.EffectContextHandle);
	const float DebuffFrequency = UOAbilitySystemLibrary::GetDebuffFrequency(Props.EffectContextHandle);
	
	FString DebuffName = FString::Printf(TEXT("DynamicDebuff_%s"), *DamageType.ToString());
	UGameplayEffect* Effect = NewObject<UGameplayEffect>(GetTransientPackage(), FName(DebuffName));

	Effect->DurationPolicy = EGameplayEffectDurationType::HasDuration;
	Effect->Period = DebuffFrequency;
	Effect->DurationMagnitude = FScalableFloat(DebuffDuration);

	UTargetTagsGameplayEffectComponent& AssetTagsComponent = Effect->FindOrAddComponent<UTargetTagsGameplayEffectComponent>();
	FInheritedTagContainer InheritedTagContainer;
	InheritedTagContainer.Added.AddTag(Tag.DamageTypeToDebuff[DamageType]);
	AssetTagsComponent.SetAndApplyTargetTagChanges(InheritedTagContainer);
	Effect->StackingType = EGameplayEffectStackingType::AggregateBySource;
	Effect->StackLimitCount = 1;

	const int32 Index = Effect->Modifiers.Num();
	Effect->Modifiers.Add(FGameplayModifierInfo());
	FGameplayModifierInfo& ModInfo = Effect->Modifiers[Index];

	ModInfo.ModifierMagnitude = FScalableFloat(DebuffDamage);
	ModInfo.ModifierOp = EGameplayModOp::Additive;
	ModInfo.Attribute = UOAttributeSet::GetDamageAttribute();
	
	if  (FGameplayEffectSpec* MutableSpec = new FGameplayEffectSpec(Effect, EffectContext, 1.f))
	{
		FOGameplayEffectContext* OContext = static_cast<FOGameplayEffectContext*>(MutableSpec->GetContext().Get());
		TSharedPtr<FGameplayTag> DebuffDamageType = MakeShareable(new FGameplayTag(DamageType));
		OContext->SetDamageType(DebuffDamageType);
		Props.TargetASC->ApplyGameplayEffectSpecToSelf(*MutableSpec);
	}
}

void UOAttributeSet::HandleIncomingXP(const FEffectProperties& Props)
{
	const float LocalXP = GetXP();
	SetXP(0.f);
	//Add to XP
	const int32 CurrentLevel = IOCombatInterface::Execute_GetPlayerLevel(Props.SourceCharacter);
	const int32 CurrentXP = IOPlayerInterface::Execute_GetXP(Props.SourceCharacter);
	const int32 NewLevel = IOPlayerInterface::Execute_FindLevelForXP(Props.SourceCharacter, CurrentXP + LocalXP);
	const int32 NumLevelUps = NewLevel - CurrentLevel;
	if (NumLevelUps > 0)
	{
		IOPlayerInterface::Execute_AddToPlayerLevel(Props.SourceCharacter, NumLevelUps);
		
		int32 AttributePtsReward = 0;
		int32 AbilityPtsReward = 0;

		for (int32 i=0; i < NumLevelUps; ++i)
		{
			AttributePtsReward += IOPlayerInterface::Execute_GetAttributePtsReward(Props.SourceCharacter, CurrentLevel + i);
			AbilityPtsReward += IOPlayerInterface::Execute_GetAbilityPtsReward(Props.SourceCharacter, CurrentLevel + i);	
		}
		
		IOPlayerInterface::Execute_AddToAttributePts(Props.SourceCharacter, AttributePtsReward);
		IOPlayerInterface::Execute_AddToAbilityPts(Props.SourceCharacter, AbilityPtsReward);

		bRestoreFullHealth = true;
		bRestoreFullStamina = true;
		bRestoreFullEnergy = true;
			
		IOPlayerInterface::Execute_LevelUp(Props.SourceCharacter);
	}
	IOPlayerInterface::Execute_AddToXP(Props.SourceCharacter, LocalXP);
}

void UOAttributeSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
	Super::PostAttributeChange(Attribute, OldValue, NewValue);

	if (Attribute == GetMaxHealthAttribute() && bRestoreFullHealth)
	{
		SetHealth(GetMaxHealth());
		bRestoreFullHealth = false;
	}
	if (Attribute ==GetMaxStaminaAttribute() && bRestoreFullStamina)
	{
		SetStamina(GetMaxStamina());
		bRestoreFullStamina = false;
	}
	if (Attribute ==GetMaxEnergyAttribute() && bRestoreFullEnergy)
	{
		SetEnergy(GetMaxEnergy());
		bRestoreFullEnergy = false;
	}
}

void UOAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UOAttributeSet, Health, OldHealth);
}

void UOAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UOAttributeSet, MaxHealth, OldMaxHealth);
}

void UOAttributeSet::OnRep_Stamina(const FGameplayAttributeData& OldStamina) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UOAttributeSet, Stamina, OldStamina);
}

void UOAttributeSet::OnRep_Agility(const FGameplayAttributeData& OldAgility) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UOAttributeSet, Agility, OldAgility);
}

void UOAttributeSet::OnRep_Intelligence(const FGameplayAttributeData& OldIntelligence) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UOAttributeSet, Intelligence, OldIntelligence);
}

void UOAttributeSet::OnRep_Perception(const FGameplayAttributeData& OldPerception) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UOAttributeSet, Perception, OldPerception);
}

void UOAttributeSet::OnRep_Resilience(const FGameplayAttributeData& OldResilience) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UOAttributeSet, Resilience, OldResilience);
}

void UOAttributeSet::OnRep_Strength(const FGameplayAttributeData& OldStrength) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UOAttributeSet, Strength, OldStrength);
}

void UOAttributeSet::OnRep_Vigor(const FGameplayAttributeData& OldVigor) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UOAttributeSet, Vigor, OldVigor);
}

void UOAttributeSet::OnRep_MaxStamina(const FGameplayAttributeData& OldMaxStamina) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UOAttributeSet, MaxStamina, OldMaxStamina);
}

void UOAttributeSet::OnRep_Energy(const FGameplayAttributeData& OldEnergy) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UOAttributeSet, Energy, OldEnergy);
}

void UOAttributeSet::OnRep_MaxEnergy(const FGameplayAttributeData& OldMaxEnergy) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UOAttributeSet, MaxEnergy, OldMaxEnergy);
}

void UOAttributeSet::OnRep_Armor(const FGameplayAttributeData& OldArmor) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UOAttributeSet, Armor, OldArmor);
}

void UOAttributeSet::OnRep_ArmorPenetration(const FGameplayAttributeData& OldArmorPenetration) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UOAttributeSet, ArmorPenetration, OldArmorPenetration);
}

void UOAttributeSet::OnRep_BlockChance(const FGameplayAttributeData& OldBlockChance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UOAttributeSet, BlockChance, OldBlockChance);
}

void UOAttributeSet::OnRep_DodgeChance(const FGameplayAttributeData& OldDodgeChance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UOAttributeSet, DodgeChance, OldDodgeChance);
}

void UOAttributeSet::OnRep_CritHitChance(const FGameplayAttributeData& OldCritHitChance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UOAttributeSet, CritHitChance, OldCritHitChance);
}

void UOAttributeSet::OnRep_CritHitDamage(const FGameplayAttributeData& OldCritHitDamage) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UOAttributeSet, CritHitDamage, OldCritHitDamage);
}

void UOAttributeSet::OnRep_CritHitResistance(const FGameplayAttributeData& OldCritHitResistance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UOAttributeSet, CritHitResistance, OldCritHitResistance);
}

void UOAttributeSet::OnRep_HealthRegen(const FGameplayAttributeData& OldHealthRegen) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UOAttributeSet, HealthRegen, OldHealthRegen);
}

void UOAttributeSet::OnRep_StaminaRegen(const FGameplayAttributeData& OldStaminaRegen) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UOAttributeSet, StaminaRegen, OldStaminaRegen);
}

void UOAttributeSet::OnRep_EnergyRegen(const FGameplayAttributeData& OldEnergyRegen) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UOAttributeSet, EnergyRegen, OldEnergyRegen);
}

void UOAttributeSet::OnRep_Stealth(const FGameplayAttributeData& OldStealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UOAttributeSet, Stealth, OldStealth);
}

void UOAttributeSet::OnRep_PhysicalResistance(const FGameplayAttributeData& OldPhysicalResistance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UOAttributeSet, PhysicalResistance, OldPhysicalResistance);
}

void UOAttributeSet::OnRep_PoisonResistance(const FGameplayAttributeData& OldPoisonResistance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UOAttributeSet, PoisonResistance, OldPoisonResistance);
}

void UOAttributeSet::OnRep_DarkResistance(const FGameplayAttributeData& OldDarkResistance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UOAttributeSet, DarkResistance, OldDarkResistance);
}

void UOAttributeSet::OnRep_ElectricResistance(const FGameplayAttributeData& OldElectricResistance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UOAttributeSet, ElectricResistance, OldElectricResistance);
}

void UOAttributeSet::OnRep_FireResistance(const FGameplayAttributeData& OldFireResistance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UOAttributeSet, FireResistance, OldFireResistance);
}

void UOAttributeSet::OnRep_IceResistance(const FGameplayAttributeData& OldIceResistance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UOAttributeSet, IceResistance, OldIceResistance);
}