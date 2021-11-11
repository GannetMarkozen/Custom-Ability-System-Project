// Fill out your copyright notice in the Description page of Project Settings.


#include "MultiplayerProject/Public/Items/Weapons/ProjectileBase.h"

#include "AttributeSet.h"
#include "Character/FPCharacterBase.h"
#include "Components/SphereComponent.h"
#include "Effects/DamageEffect.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/ProjectileMovementComponent.h"


AProjectileBase::AProjectileBase()
{
	PrimaryActorTick.bCanEverTick = false;

	bReplicates = true;
	SetReplicateMovement(true);

	OverlapSphere = CreateDefaultSubobject<USphereComponent>(TEXT("OverlapSphere"));
	OverlapSphere->SetGenerateOverlapEvents(true);
	RootComponent = OverlapSphere;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetGenerateOverlapEvents(false);
	Mesh->SetCollisionObjectType(ECC_PhysicsBody);
	Mesh->SetupAttachment(OverlapSphere);

	MovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("MovementComponent"));
	MovementComponent->InitialSpeed = 5000.f;
	MovementComponent->MaxSpeed = 5000.f;
	MovementComponent->ProjectileGravityScale = 0.f;
	MovementComponent->bAutoActivate = true;
}

void AProjectileBase::BeginPlay()
{
	Super::BeginPlay();

	if(HasAuthority())
		OverlapSphere->OnComponentBeginOverlap.AddDynamic(this, &AProjectileBase::OnSphereBeginOverlap);
}

void AProjectileBase::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, const int32 OtherBodyIndex, const bool bFromSweep, const FHitResult& SweepResult)
{
	// Do not explode if overlapping owner
	if(OtherActor == GetOwner()) return;

	// Out hit results
	TArray<FHitResult> Hits;

	// Only trace pawns and physics bodies
	const std::initializer_list<TEnumAsByte<EObjectTypeQuery>>& TraceObjects = { UEngineTypes::ConvertToObjectType(ECC_Pawn), UEngineTypes::ConvertToObjectType(ECC_PhysicsBody) };

	// Sphere trace
	if(UKismetSystemLibrary::SphereTraceMultiForObjects(GetWorld(), GetActorLocation(), GetActorLocation(), Radius, TraceObjects, false, { this },
		EDrawDebugTrace::ForDuration,Hits, true, FLinearColor::Red, FLinearColor::Green, 10.f))
	{
		TArray<AActor*> TracedActors;
		for(const FHitResult& Hit : Hits)
		{
			if(!Hit.GetActor()) continue;

			if(!TracedActors.FindByKey(Hit.GetActor()))
			{
				TracedActors.Add(Hit.GetActor());

				bool bKillingBlow = false;
				
				// Check for killing blow. If true, apply ragdoll knockback after replicating death
				if(Hit.GetActor()->GetClass()->IsChildOf(AFPCharacterBase::StaticClass()))
				{
					if(const auto* Manager = UAbilityManagerComponent::GetAbilityManager(Hit.GetActor()))
					{
						// Check if not dead then apply damage and check if dead to determine killing blow
						if(!Manager->GetOwnedTags().HasTagExact(TAG("State.Dead")) && //UAttributeSet::ApplyDamage(Hit.GetActor(), BaseDamage) &&
							EffectClass ? UEffect::TryApplyEffect(EffectClass, FEffectContext(Hit.GetActor(), GetOwner())) : UAttributeSet::ApplyDamage(Hit.GetActor(), BaseDamage) &&
							Manager->GetOwnedTags().HasTagExact(TAG("State.Dead")))
						{
							bKillingBlow = true;

							Multi_ApplyKnockbackAfterReplicatedDeath(GetActorLocation(), Hit.GetActor());
						}
					}
				}
				
				// No need to launch character if dealt killing blow
				if(!bKillingBlow)
				{
					if(auto* Character = Cast<ACharacter>(Hit.GetActor()))
					{
						FVector Impulse = UKismetMathLibrary::GetDirectionUnitVector(GetActorLocation(), Character->GetActorLocation());
						Impulse *= (Radius / FVector::Distance(GetActorLocation(), Character->GetActorLocation())) * Knockback;
						if(Character->GetCharacterMovement()->IsFalling()) Impulse *= CharacterIsFallingMultiplier;
						Character->LaunchCharacter(Impulse, false, false);
					}
				}
			}
		}
	}
	Multi_PlayExplosionEffect(GetActorLocation());
	ApplyKnockback(GetActorLocation(), Hits);
	
	if(ExplosionSound) UGameplayStatics::SpawnSoundAtLocation(GetWorld(), ExplosionSound, GetActorLocation());
	if(ExplosionParticleEffect) UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionParticleEffect, GetActorLocation(), FRotator(), FVector(ExplosionParticleScale));
	
	Destroy();
}

void AProjectileBase::Multi_ApplyKnockbackAfterReplicatedDeath_Implementation(const FVector& Location, AActor* TargetCharacter)
{
	if(!TargetCharacter) return;

	TArray<FHitResult> Hits;
	if(UKismetSystemLibrary::SphereTraceMultiForObjects(GetWorld(), GetActorLocation(), GetActorLocation(), Radius, { UEngineTypes::ConvertToObjectType(ECC_Pawn) },
		false, { this }, EDrawDebugTrace::ForDuration,Hits, true, FLinearColor::Red, FLinearColor::Green, 10.f))
	{
		TArray<FHitResult> TargetHits;
		for(const FHitResult& Hit : Hits)
		{
			if(Hit.GetActor() && Hit.GetActor() == TargetCharacter)
			{
				TargetHits.Add(Hit);
			}
		}

		if(UAbilityManagerComponent::GetAbilityManager(TargetCharacter)->GetOwnedTags().HasTagExact(TAG("State.Dead")))
		{
			ApplyKnockback(Location, TargetHits);
		}
		else
		{
			UAbilityManagerComponent::GetAbilityManager(TargetCharacter)->OnTagsAdded.AddLambda([=](const FGameplayTagContainer& AddedTags)
			{
				if(AddedTags.HasTagExact(TAG("State.Dead")))
				{
					ApplyKnockback(Location, TargetHits);
					if(TargetCharacter && this)
						UAbilityManagerComponent::GetAbilityManager(TargetCharacter)->OnTagsAdded.RemoveAll(this);
				}
			});
		}
	}
}


void AProjectileBase::Multi_PlayExplosionEffect_Implementation(const FVector& Location)
{
	if(ExplosionSound) UGameplayStatics::SpawnSoundAtLocation(GetWorld(), ExplosionSound, Location);
	if(ExplosionParticleEffect) UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionParticleEffect, Location, FRotator(), FVector(ExplosionParticleScale));

	if(!HasAuthority())
	{
		TArray<FHitResult> Hits;
		const std::initializer_list<TEnumAsByte<EObjectTypeQuery>>& TraceObjects = { UEngineTypes::ConvertToObjectType(ECC_Pawn), UEngineTypes::ConvertToObjectType(ECC_PhysicsBody) };
		
		if(UKismetSystemLibrary::SphereTraceMultiForObjects(GetWorld(), GetActorLocation(), GetActorLocation(), Radius, TraceObjects, false, { this },
		EDrawDebugTrace::ForDuration,Hits, true, FLinearColor::Red, FLinearColor::Green, 10.f))
		{
			ApplyKnockback(Location, Hits);
		}
	}
}

void AProjectileBase::ApplyKnockback(const FVector& Location, const TArray<FHitResult>& Hits) const
{
	for(const FHitResult& Hit : Hits)
	{
		// Only apply impulse if the component is movable and simulating physics
		if(IsValid(Hit.GetComponent()) && Hit.GetComponent()->IsSimulatingPhysics())
		{
			if(auto* SkelMesh = Cast<USkeletalMeshComponent>(Hit.GetComponent()))
			{
				FVector Impulse = UKismetMathLibrary::GetDirectionUnitVector(Location, SkelMesh->GetBoneLocation(Hit.BoneName));
				Impulse *= ((Radius / FVector::Distance(Location, SkelMesh->GetBoneLocation(Hit.BoneName))) * Knockback * 500.f) / SkelMesh->GetNumBones();
				SkelMesh->AddImpulse(Impulse, Hit.BoneName);
			}
			else if(IsValid(Hit.GetActor()) && Hit.GetComponent() == Hit.GetActor()->GetRootComponent())
			{
				FVector Impulse = UKismetMathLibrary::GetDirectionUnitVector(Location, Hit.GetComponent()->GetComponentLocation());
				Impulse *= (Radius / FVector::Distance(Location, Hit.GetComponent()->GetComponentLocation())) * Knockback * 500.f;
				Hit.GetComponent()->AddImpulse(Impulse);
			}
		}
	}
}




