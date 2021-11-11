// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Ability.h"
#include "AbilityPlugin/Public/AttributeSet.h"
#include "JumpAbility.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYERPROJECT_API UJumpAbility : public UAbility
{
	GENERATED_BODY()
public:
	UJumpAbility();
protected:
	virtual void InitAbility(class UAbilityManagerComponent* AbilityManager) override;
	virtual bool CanActivateAbility(const FActivationInfo& ActivationInfo) override;
	virtual void ActivateAbility(const FActivationInfo& ActivationInfo) override;
};
