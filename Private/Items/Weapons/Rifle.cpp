// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Weapons/Rifle.h"

#include "AttributeSet.h"
#include "DrawDebugHelpers.h"
#include "Camera/CameraComponent.h"
#include "Character/FPCharacterBase.h"
#include "Items/RecoilObject.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Net/UnrealNetwork.h"
#include "Tests/AutomationTestSettings.h"

URifle::URifle()
{
	FGameplayTagContainer BlockingTags;
	BlockingTags.AddTag(FGameplayTag::RequestGameplayTag(FName("State.Dead")));
	BlockingTags.AddTag(FGameplayTag::RequestGameplayTag(FName("State.Stunned")));
	BlockingTags.AddTag(FGameplayTag::RequestGameplayTag(FName("State.Reloading")));
	
	Binds.Add(FToolBindingInfo(FName("PrimaryFire"),  IE_Pressed, FName("Fire"), FGameplayTagContainer::EmptyContainer, BlockingTags));
	Binds.Add(FToolBindingInfo(FName("PrimaryFire"), IE_Released, FName("StopFiring")));
	
	Binds.Add(FToolBindingInfo(FName("SecondaryFire"), IE_Pressed, FName("Aim"), FGameplayTagContainer::EmptyContainer, BlockingTags));
	Binds.Add(FToolBindingInfo(FName("SecondaryFire"), IE_Released, FName("StopAiming")));

	BlockingTags.AddTag(FGameplayTag::RequestGameplayTag(FName("State.SwappingItems")));
	
	Binds.Add(FToolBindingInfo(FName("Reload"), IE_Pressed, FName("Reload"), FGameplayTagContainer::EmptyContainer, BlockingTags));
}

void URifle::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

void URifle::Fire(const FBindingInfo& Bind)
{
	bFiring = true;
	
	StartFiring();
}

void URifle::StartFiring()
{
	if(bFiring && !bCoolDown && OwningCharacter)
	{
		bFiring = true;
		bCoolDown = true;
		Ammo--;

		FHitResult Hit;
		const FVector Start = OwningCharacter->Camera->GetComponentLocation();
		const FVector End = Start + OwningCharacter->Camera->GetForwardVector() * Range;
		
		FCollisionQueryParams Params;
		Params.AddIgnoredActor(OwningCharacter);
		GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, Params);
		
		const bool bDamageable = Hit.GetActor() && UAttributeSet::HasAttribute(Hit.GetActor(), FName("Health"));
		DrawDebugLine(GetWorld(), Start, Hit.IsValidBlockingHit() ? Hit.Location : End, bDamageable ? FColor::Green : FColor::Red, false, 5.f);

		if(Ammo > 0)
		{
			const auto& LoopFire = [=]()->void{ bCoolDown = false; StartFiring(); };
			GetWorld()->GetTimerManager().SetTimer(FiringHandle, LoopFire, FiringRate, false);
		}

		if(OwningCharacter->IsLocallyControlled())
		{
			const FItemMeshTableRow* MeshRow = OwningCharacter->GetItemDataTable()->FindRow<FItemMeshTableRow>(Mesh->GetFName(), TEXT("Context"));
			
			if(FiringSound) UGameplayStatics::SpawnSoundAttached(FiringSound, OwningCharacter->GetFPItemMesh(), MeshRow ? MeshRow->MuzzleName : NAME_None);
			if(FiringMontage) if(auto* AnimInstance = OwningCharacter->FP_Mesh->GetAnimInstance()) AnimInstance->Montage_Play(FiringMontage);
			if(RecoilObjectClass) URecoilObject::AddRecoilInstance(OwningCharacter, RecoilObjectClass);
		}

		if(Hit.IsValidBlockingHit())
			Server_Fire(Hit);
	}
}


void URifle::Server_Fire_Implementation(const FHitResult& Hit)
{
	if(OwningCharacter->HasAuthority() && Hit.GetActor() && EffectClass)
	{
		const FEffectContext Context(Hit.GetActor(), OwningCharacter, UAbilityManagerComponent::GetAbilityManager(Hit.GetActor()));
		
		UEffect::TryApplyEffect(EffectClass, Context);
	}

	Multi_PlayFiringEffect(Hit);
	Multi_PlayFiringEffect_Implementation(Hit);
}

void URifle::Multi_PlayFiringEffect_Implementation(const FHitResult& Hit)
{
	if(Hit.GetComponent() && Hit.GetComponent()->IsSimulatingPhysics(Hit.BoneName) && OwningCharacter)
	{
		const FVector Impulse = UKismetMathLibrary::GetDirectionUnitVector(OwningCharacter->GetActorLocation(), Hit.Location) * 30000.f;
		Hit.GetComponent()->AddImpulseAtLocation(Impulse, Hit.Location, Hit.BoneName);
	}
	
	if(OwningCharacter && !OwningCharacter->IsLocallyControlled())
	{
		const FItemMeshTableRow* MeshRow = OwningCharacter->GetItemDataTable()->FindRow<FItemMeshTableRow>(Mesh->GetFName(), TEXT("Context"));
			
		if(FiringSound) UGameplayStatics::SpawnSoundAttached(FiringSound, OwningCharacter->GetFPItemMesh(), MeshRow ? MeshRow->MuzzleName : NAME_None);
		if(FiringMontage) if(auto* AnimInstance = OwningCharacter->FP_Mesh->GetAnimInstance()) AnimInstance->Montage_Play(FiringMontage);
		if(RecoilObjectClass) URecoilObject::AddRecoilInstance(OwningCharacter, RecoilObjectClass);
	}
}



void URifle::StopFiring(const FBindingInfo& Bind)
{
	bFiring = false;
}

void URifle::Aim(const FBindingInfo& Bind)
{
	
}

void URifle::StopAiming(const FBindingInfo& Bind)
{
	
}

void URifle::Reload(const FBindingInfo& Bind)
{
	Ammo = MaxAmmo;
}



