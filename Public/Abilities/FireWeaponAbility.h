// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FPAbility.h"
#include "FireWeaponAbility.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYERPROJECT_API UFireWeaponAbility : public UFPAbility
{
	GENERATED_BODY()
public:
	UFireWeaponAbility();
protected:
	virtual void InitAbility(UAbilityManagerComponent* AbilityManager) override;
	virtual bool CanActivateAbility(const FActivationInfo& ActivationInfo) override;
	virtual void ActivateAbility(const FActivationInfo& ActivationInfo) override;
	virtual void EndAbility(const FActivationInfo& ActivationInfo) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Configs")
	TSubclassOf<class URecoilObject> RecoilObjectClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Configs")
	class UAnimMontage* FiringMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Cofigs")
	class USoundBase* FiringSound;
	
	UPROPERTY(EditDefaultsOnly, Category = "Configs")
	float FireRate = 0.05f;

	UFUNCTION(NetMulticast, Unreliable)
	void Multi_PlayFiringSound();
	
	FTimerHandle AutoFireHandle;
	
	bool bFiring = false;
	
	virtual void Fire();

	
};
