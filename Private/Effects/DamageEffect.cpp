// Fill out your copyright notice in the Description page of Project Settings.


#include "Effects/DamageEffect.h"


UDamageEffect::UDamageEffect()
{
	Modifiers.Add(FModifier(FName("Health"), -20.f));

	EffectPeriod = EEffectPeriod::Instant;
	
	BlockedTags.AddTag(FGameplayTag::RequestGameplayTag(FName("State.Dead")));
	BlockedTags.AddTag(FGameplayTag::RequestGameplayTag(FName("State.Invincible")));
}