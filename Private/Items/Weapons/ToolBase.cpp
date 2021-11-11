// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Weapons/ToolBase.h"
#include "AbilityManagerComponent.h"
#include "Character/FPCharacterBase.h"

UToolBase::UToolBase()
{
	InputBinds = CreateDefaultSubobject<UInputBindingObject>(TEXT("InputBindingObject"));
}

void UToolBase::Equip(AFPCharacterBase* Character)
{
	Super::Equip(Character);

	if(!Character) return;

	// Runs on first equip
	if(InputBinds->GetBinds().IsEmpty() && !Binds.IsEmpty())
		for(FToolBindingInfo& Bind : Binds)
			AddBind(Bind);
	
	if(auto* AbilityManager = UAbilityManagerComponent::GetAbilityManager(Character))
		AbilityManager->GrantAbilities(Abilities);

	// Play equip montages and add loose tag SwappingItems during duration
	if(FP_EquipMontage || EquipMontage)
	{
		if(auto* AnimInstance = Character->FP_Mesh->GetAnimInstance())
		{
			if(FP_EquipMontage)
			{
				Character->AbilityManager->AddLooseTags(TAG_CONTAINER("State.SwappingItems"));
				const std::function<void()>& RemoveLooseTag = [Character]()->void{ if(Character) Character->AbilityManager->RemoveLooseTags(TAG_CONTAINER("State.SwappingItems")); };
				GetWorld()->GetTimerManager().SetTimer(ItemSwappingTimerHandle, RemoveLooseTag, AnimInstance->Montage_Play(FP_EquipMontage), false);
			}
			Server_Equip(Character);
		}
	}

	if(InputBinds)
		InputBinds->Bind(this, Character);
}

void UToolBase::UnEquip(AFPCharacterBase* Character)
{
	Super::Equip(Character);

	if(!Character) return;
	
	if(auto* AbilityManager = UAbilityManagerComponent::GetAbilityManager(Character))
		AbilityManager->RemoveAbilitiesByClasses(Abilities);

	if(InputBinds)
		InputBinds->UnbindAll();
}

void UToolBase::Server_Equip_Implementation(AFPCharacterBase* Character)
{
	if(!Character) return;
	Multi_Equip(Character);
	Multi_Equip_Implementation(Character);
}

void UToolBase::Multi_Equip_Implementation(AFPCharacterBase* Character)
{
	if(!Character) return;
	if(EquipMontage)
	{
		if(auto* AnimInstance = Character->GetMesh()->GetAnimInstance())
		{
			AnimInstance->Montage_Play(EquipMontage);
		}
	}
}

void UToolBase::AddBind(FToolBindingInfo& Bind)
{
	UE_LOG(LogTemp, Warning, TEXT("Adding bind %s"), *Bind.InputName.ToString());
	Bind.CanProcessEventDelegate.BindUObject(this, &UToolBase::CanProcessFunction);
	InputBinds->AddBind(Bind);
}

bool UToolBase::CanProcessFunction(const FBindingInfo& Bind) const
{
	if(OwningCharacter)
		for(const FToolBindingInfo& ToolBind : Binds)
			if(ToolBind.InputName == Bind.InputName)
				return BP_CanProcessFunction(Bind) &&
					OwningCharacter->AbilityManager->GetOwnedTags().HasAll(ToolBind.RequiredTags) &&
						!OwningCharacter->AbilityManager->GetOwnedTags().HasAny(ToolBind.BlockedTags);

	return false;
}

