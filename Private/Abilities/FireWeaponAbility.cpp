// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/FireWeaponAbility.h"

#include "Character/FPCharacterBase.h"
#include "Items/RecoilObject.h"
#include "Kismet/GameplayStatics.h"

UFireWeaponAbility::UFireWeaponAbility()
{
	ActivationMode = EActivationMode::LocalOnly;

	Activators.Add(FActivationInfo(FName("PrimaryFire"), EBindingType::Action, true, true));

	BlockingTags.AddTag(TAG("State.Dead"));
	BlockingTags.AddTag(TAG("State.Stunned"));
	BlockingTags.AddTag(TAG("State.SwappingItems"));
}

void UFireWeaponAbility::InitAbility(UAbilityManagerComponent* AbilityManager)
{
	Super::InitAbility(AbilityManager);
	
}

bool UFireWeaponAbility::CanActivateAbility(const FActivationInfo& ActivationInfo)
{
	return Super::CanActivateAbility(ActivationInfo);
}

void UFireWeaponAbility::ActivateAbility(const FActivationInfo& ActivationInfo)
{
	Super::ActivateAbility(ActivationInfo);

	GetAbilityManager()->AddTags(TAG_CONTAINER("State.Firing"));
	Fire();
}

void UFireWeaponAbility::EndAbility(const FActivationInfo& ActivationInfo)
{
	Super::EndAbility(ActivationInfo);

	//bFiring = false;
	GetAbilityManager()->RemoveTags(TAG_CONTAINER("State.Firing"));
}

void UFireWeaponAbility::Fire()
{
	if(GetAbilityManager()->GetOwnedTags().HasTagExact(TAG("State.Firing")))
	{
		if(IsValid(RecoilObjectClass))
		{
			URecoilObject::AddRecoilInstance(GetCharacter(), RecoilObjectClass);
		}
		
		if(IsValid(FiringMontage))
		{
			if(UAnimInstance* FPAnimInstance = FPCharacterBase->FP_Mesh->GetAnimInstance())
			{
				FPAnimInstance->Montage_Play(FiringMontage);
			}
		}

		if(IsValid(FiringSound))
		{
			if(FPCharacterBase->IsLocallyControlled())
			{
				UGameplayStatics::SpawnSoundAttached(FiringSound, FPCharacterBase->GetFPItemMesh());
			}
			else
			{
				Multi_PlayFiringSound();
			}
		}
		
		GetWorld()->GetTimerManager().SetTimer(AutoFireHandle, this, &UFireWeaponAbility::Fire, FireRate, false);
	}
}

void UFireWeaponAbility::Multi_PlayFiringSound_Implementation()
{
	if(IsValid(FiringSound) && !FPCharacterBase->IsLocallyControlled())
		UGameplayStatics::SpawnSoundAttached(FiringSound, FPCharacterBase->GetItemMesh());
}





