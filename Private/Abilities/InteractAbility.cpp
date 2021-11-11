// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/InteractAbility.h"

#include "DrawDebugHelpers.h"
#include "Camera/CameraComponent.h"
#include "MultiplayerProject/Public/Character/FPCharacterBase.h"
#include "AbilityManagerComponent.h"
#include "AttributeSet.h"
#include "AttributeSets/CharacterAttributeSet.h"
#include "Character/FPCharacterBase.h"
#include "Interactables/InteractInterface.h"
#include "Items/InventoryComponent.h"
#include "Items/ItemBase.h"
#include "Kismet/KismetMathLibrary.h"

UInteractAbility::UInteractAbility()
{
	AbilityTag = TAG("Ability.Interact");

	ActivationMode = EActivationMode::LocalOnly;
	
	Activators.Add(FActivationInfo(FName("Interact"), EBindingType::Action, true, false));
	
	BlockingTags.AddTag(TAG("State.Dead"));
	BlockingTags.AddTag(TAG("State.Stunned"));
	
}

void UInteractAbility::InitAbility(UAbilityManagerComponent* AbilityManager)
{
	Super::InitAbility(AbilityManager);
}


void UInteractAbility::ActivateAbility(const FActivationInfo& ActivationInfo)
{
	Super::ActivateAbility();
	
	FHitResult Hit;
	const FVector Start = FPCharacterBase->Camera->GetComponentLocation();
	const FVector End = Start + FPCharacterBase->Camera->GetForwardVector() * 500.f;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(FPCharacterBase);
	GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, Params);
	
	const bool bInteractable = Hit.GetActor() && Hit.GetActor()->GetClass()->ImplementsInterface(UInteractInterface::StaticClass());
	DrawDebugLine(GetWorld(), Start, Hit.IsValidBlockingHit() ? Hit.Location : End, bInteractable ? FColor::Green : FColor::Red, false, 5.f);
	
	if(bInteractable)
		Server_Interact(Hit.GetActor());
}

void UInteractAbility::Server_Interact_Implementation(UObject* Target)
{
	IInteractInterface::Execute_Interact(Target, FPCharacterBase);
}






