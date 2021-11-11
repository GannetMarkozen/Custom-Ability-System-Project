// Fill out your copyright notice in the Description page of Project Settings.


#include "MultiplayerProject/Public/Abilities/FPAbility.h"
#include "Character/FPCharacterBase.h"

void UFPAbility::InitAbility(UAbilityManagerComponent* AbilityManager)
{
	Super::InitAbility(AbilityManager);

	FPCharacterBase = Cast<AFPCharacterBase>(GetCharacter());
}
