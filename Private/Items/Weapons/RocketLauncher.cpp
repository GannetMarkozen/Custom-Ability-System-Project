// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Weapons/RocketLauncher.h"
#include "MultiplayerProjectIncludes.h"
#include "Camera/CameraComponent.h"
#include "Character/FPCharacterBase.h"
#include "Items/RecoilObject.h"
#include "Kismet/GameplayStatics.h"
#include "MultiplayerProject/Public/Items/Weapons/ProjectileBase.h"


URocketLauncher::URocketLauncher()
{
	//InputBinds->AddBind(FBindingInfo(FName("PrimaryFire"), IE_Pressed, FName("Fire")));
	Binds.Add(FToolBindingInfo(FName("PrimaryFire"), IE_Pressed, FName("Fire"), FGameplayTagContainer::EmptyContainer, TAG_CONTAINER("State.Dead")));
}

void URocketLauncher::Fire(const FBindingInfo& BindingInfo)
{
	if(OwningCharacter)
	{
		const FRotator Rotation = OwningCharacter->Camera->GetComponentRotation();
		const FVector Location = OwningCharacter->Camera->GetComponentLocation() + Rotation.Vector() * 100.f;
		Server_Fire(Location, Rotation);

		if(FiringSound) UGameplayStatics::SpawnSoundAttached(FiringSound, OwningCharacter->GetFPItemMesh());
		if(FiringMontage) if(UAnimInstance* AnimInstance = OwningCharacter->FP_Mesh->GetAnimInstance()) AnimInstance->Montage_Play(FiringMontage);
		if(RecoilClass) URecoilObject::AddRecoilInstance(OwningCharacter, RecoilClass);

		BP_OnFired(BindingInfo);
	}
	else PRINT(TEXT("No OwningCharacter"));
}

void URocketLauncher::Server_Fire_Implementation(const FVector& Location, const FRotator& Rotation)
{
	Multi_Fire(Location, Rotation);

	if(ProjectileClass)
	{
		FActorSpawnParameters Params;
		Params.Owner = OwningCharacter;
		GetWorld()->SpawnActor<AProjectileBase>(ProjectileClass, Location, Rotation, Params);
	}
}

void URocketLauncher::Multi_Fire_Implementation(const FVector& Location, const FRotator& Rotation)
{
	if(OwningCharacter && !OwningCharacter->IsLocallyControlled())
		UGameplayStatics::SpawnSoundAttached(FiringSound, OwningCharacter->GetFPItemMesh());
}


