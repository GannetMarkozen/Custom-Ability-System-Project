// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "ProjectileBase.generated.h"

UCLASS(Abstract)
class MULTIPLAYERPROJECT_API AProjectileBase : public AActor
{
	GENERATED_BODY()

public:
	AProjectileBase();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	class USphereComponent* OverlapSphere;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	class UStaticMeshComponent* Mesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	class UProjectileMovementComponent* MovementComponent;

	UPROPERTY(EditAnywhere, Category = "Configurations")
	float BaseDamage = 50.f;

	UPROPERTY(EditDefaultsOnly, Category = "Configurations")
	class USoundBase* ExplosionSound;

	UPROPERTY(EditDefaultsOnly, Category = "Configurations")
	class UParticleSystem* ExplosionParticleEffect;

	UPROPERTY(EditDefaultsOnly, Category = "Configurations")
	float ExplosionParticleScale = 1.f;

	UPROPERTY(EditDefaultsOnly, Category = "Configurations")
	float Knockback = 300.f;

	UPROPERTY(EditDefaultsOnly, Category = "Configurations")
	float CharacterIsFallingMultiplier = 1.5f;

	UPROPERTY(EditDefaultsOnly, Category = "Configurations")
	float Radius = 200.f;

	UFUNCTION()
	virtual void OnSphereBeginOverlap(class UPrimitiveComponent* OverlappedComponent, class AActor* OtherActor,
		class UPrimitiveComponent* OtherComp, const int32 OtherBodyIndex, const bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(NetMulticast, Reliable)
	void Multi_PlayExplosionEffect(const FVector& Location);

	UFUNCTION(NetMulticast, Reliable)
	void Multi_ApplyKnockbackAfterReplicatedDeath(const FVector& Location, class AActor* TargetCharacter);

	void ApplyKnockback(const FVector& Location, const TArray<FHitResult>& Hits) const;




	UPROPERTY(EditDefaultsOnly, Category = "Temp")
	TSubclassOf<class UEffect> EffectClass;
};
