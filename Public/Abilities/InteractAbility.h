// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FPAbility.h"
#include "AbilityManagerComponent.h"
#include "GameFramework/Character.h"
#include "InteractAbility.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYERPROJECT_API UInteractAbility : public UFPAbility
{
	GENERATED_BODY()
public:
	UInteractAbility();

protected:
	virtual void InitAbility(class UAbilityManagerComponent* AbilityManager) override;
	virtual void ActivateAbility(const FActivationInfo& ActivationInfo) override;

	UFUNCTION(Server, Reliable)
	void Server_Interact(class UObject* Target);
};
