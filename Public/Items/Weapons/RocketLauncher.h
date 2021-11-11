// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ToolBase.h"
#include "RocketLauncher.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class MULTIPLAYERPROJECT_API URocketLauncher : public UToolBase
{
	GENERATED_BODY()
public:
	URocketLauncher();
protected:
	UFUNCTION(BlueprintCallable, Category = "Rocket Launcher")
	void Fire(const FBindingInfo& BindingInfo);

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "On Fired"), Category = "Rocket Launcher")
	void BP_OnFired(const FBindingInfo& BindingInfo);

	UFUNCTION(Server, Reliable)
	void Server_Fire(const FVector& Location, const FRotator& Rotation);

	UFUNCTION(NetMulticast, Reliable)
	void Multi_Fire(const FVector& Location, const FRotator& Rotation);

	UPROPERTY(EditDefaultsOnly, Category = "Configurations")
	class UAnimMontage* FiringMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Configurations")
	class USoundBase* FiringSound;

	UPROPERTY(EditDefaultsOnly, Category = "Configurations")
	TSubclassOf<class URecoilObject> RecoilClass;

	UPROPERTY(EditDefaultsOnly, Category = "Configurations")
	TSubclassOf<class AProjectileBase> ProjectileClass;
};
