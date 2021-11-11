// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/JumpAbility.h"

#include "AttributeSets/CharacterAttributeSet.h"
#include "Character/FPCharacterBase.h"
#include "GameFramework/CharacterMovementComponent.h"


UJumpAbility::UJumpAbility()
{
	AbilityTag = TAG("Ability.Jump");
	Activators.Add(FActivationInfo(FName(TEXT("Jump")), EBindingType::Action, true, false));
	ActivationMode = EActivationMode::LocalOnly;

	BlockingTags.AddTag(TAG("State.Dead"));
	BlockingTags.AddTag(TAG("State.Stunned"));
}

void UJumpAbility::InitAbility(UAbilityManagerComponent* AbilityManager)
{
	Super::InitAbility(AbilityManager);
}


bool UJumpAbility::CanActivateAbility(const FActivationInfo& ActivationInfo)
{
	// Only activate ability if not falling
	return Super::CanActivateAbility() && GetCharacter()->GetCharacterMovement() && !GetCharacter()->GetCharacterMovement()->IsFalling();
}


void UJumpAbility::ActivateAbility(const FActivationInfo& ActivationInfo)
{
	Super::ActivateAbility();

	if(IsValid(GetCharacter()))
		GetCharacter()->Jump();
	
	EndAbility();
}

