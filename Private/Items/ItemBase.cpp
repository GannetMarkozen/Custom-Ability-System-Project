// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/ItemBase.h"

#include "Character/FPCharacterBase.h"

UItemBase::UItemBase()
{
	
}

void UItemBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}


void UItemBase::Equip(AFPCharacterBase* Character)
{
	OwningCharacter = Character;
	Character->SetItemMesh(Mesh);
	
	BP_Equip(Character);
}

void UItemBase::UnEquip(AFPCharacterBase* Character)
{
	OwningCharacter = nullptr;
	Character->SetItemMesh(nullptr);
	
	BP_Unequip(Character);
}






