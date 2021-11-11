// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/Weapons/ToolBase.h"
#include "Rifle.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYERPROJECT_API URifle : public UToolBase
{
	GENERATED_BODY()
public:
	URifle();
protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	UPROPERTY(EditDefaultsOnly, Category = "Configurations")
	TSubclassOf<class URecoilObject> RecoilObjectClass;

	UPROPERTY(EditDefaultsOnly, Category = "Configurations")
	class USoundBase* FiringSound;

	UPROPERTY(EditDefaultsOnly, Category = "Configurations")
	class UAnimMontage* FiringMontage;

	UFUNCTION()
	virtual void Fire(const FBindingInfo& Bind);

	UFUNCTION()
	virtual void StopFiring(const FBindingInfo& Bind);

	UFUNCTION()
	virtual void Aim(const FBindingInfo& Bind);

	UFUNCTION()
	virtual void StopAiming(const FBindingInfo& Bind);

	UFUNCTION()
	virtual void Reload(const FBindingInfo& Bind);

	void StartFiring();

	UFUNCTION(BlueprintImplementableEvent, Category = "Rifle")
	void OnFire();

	UFUNCTION(BlueprintImplementableEvent, Category = "Rifle")
	void OnStopFiring();

	UFUNCTION(Server, Reliable)
	void Server_Fire(const FHitResult& Hit);

	UFUNCTION(NetMulticast, Reliable)
	void Multi_PlayFiringEffect(const FHitResult& Hit);

	UPROPERTY(EditDefaultsOnly, Category = "Configurations")
	TSubclassOf<class UEffect> EffectClass;

	UPROPERTY(EditDefaultsOnly, Category = "Configurations")
	float FiringRate = 0.15f;

	UPROPERTY(EditDefaultsOnly, Category = "Configurations")
	int32 Ammo = 30.f;

	UPROPERTY(EditDefaultsOnly, Category = "Configurations")
	int32 MaxAmmo = 30.f;

	UPROPERTY(EditDefaultsOnly, Category = "Configurations")
	float Range = 10000.f;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "State")
	bool bFiring = false;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "State")
	bool bCoolDown = false;

	FTimerHandle FiringHandle;
	
};
