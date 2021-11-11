// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/ItemSwapAbility.h"

#include "AbilityManagerComponent.h"
#include "Abilities/FireWeaponAbility.h"
#include "AttributeSets/CharacterAttributeSet.h"
#include "Items/InventoryComponent.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/Character.h"
#include "Items/ItemBase.h"
#include "Character/FPCharacterBase.h"

UItemSwapAbility::UItemSwapAbility()
{
	ActivationMode = EActivationMode::LocalOnly;

	Activators.Add(FActivationInfo(FName(TEXT("ScrollUp")), EBindingType::Action, true, false));
	Activators.Add(FActivationInfo(FName(TEXT("ScrollDown")), EBindingType::Action, true, false));

	BlockingTags.AddTag(TAG("State.Dead"));
	BlockingTags.AddTag(TAG("State.Stunned"));
}

void UItemSwapAbility::InitAbility(UAbilityManagerComponent* AbilityManager)
{
	Super::InitAbility(AbilityManager);
	
	// Equip item on granted
	if(auto* Inventory = UInventoryComponent::GetInventory(FPCharacterBase)) Inventory->OnAddedItemDelegate.AddDynamic(this, &UItemSwapAbility::InitFirstIndex);
}

bool UItemSwapAbility::CanActivateAbility(const FActivationInfo& ActivationInfo)
{
	return Super::CanActivateAbility();
}

void UItemSwapAbility::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UItemSwapAbility, ItemIndex, COND_OwnerOnly, REPNOTIFY_OnChanged);
}

void UItemSwapAbility::InitFirstIndex(UInventoryComponent* Inventory, UItemBase* Item, const int32 Index)
{
	OnRep_ItemIndex();
	if(Inventory) Inventory->OnAddedItemDelegate.RemoveAll(this);
}

void UItemSwapAbility::ActivateAbility(const FActivationInfo& ActivationInfo)
{
	Super::ActivateAbility();
	
	if(const UInventoryComponent* Inventory = UInventoryComponent::GetInventory(GetCharacter()))
	{
		if(Inventory->GetItems().Num() <= 1) return;
		
		if(ActivationInfo.ActivationKey == FName("ScrollUp"))
		{
			Server_SetItemIndex(Inventory->GetItems().IsValidIndex(ItemIndex + 1) ? ItemIndex + 1 : 0);
		}
		else if(ActivationInfo.ActivationKey == FName("ScrollDown"))
		{
			Server_SetItemIndex(Inventory->GetItems().IsValidIndex(ItemIndex - 1) ? ItemIndex - 1 : Inventory->GetItems().Num() - 1);
		}
	}
}

void UItemSwapAbility::OnRep_ItemIndex()
{
	if(const UInventoryComponent* Inventory = UInventoryComponent::GetInventory(GetCharacter()))
	{
		const TArray<UItemBase*>& Items = Inventory->GetItems();
		if(Items.IsValidIndex(ItemIndex) && IsValid(Items[ItemIndex]) && Items.IsValidIndex(LastItemIndex) && IsValid(Items[LastItemIndex]))
		{
			PRINT(TEXT("Switching from index %i to %i"), LastItemIndex, ItemIndex);
			Items[LastItemIndex]->UnEquip(FPCharacterBase);
			Items[ItemIndex]->Equip(FPCharacterBase);
			
			LastItemIndex = ItemIndex;
		}
	}
}




