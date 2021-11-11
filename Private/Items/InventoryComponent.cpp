// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/InventoryComponent.h"

#include "Engine/ActorChannel.h"
#include "Items/ItemBase.h"
#include "MultiplayerProjectIncludes.h"

#include "Net/UnrealNetwork.h"


UInventoryComponent::UInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	SetIsReplicatedByDefault(true);
}


void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	if(HasAuthority())
		AddItemsByClasses(DefaultItems);
}

void UInventoryComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(UInventoryComponent, Items, COND_OwnerOnly);
}

bool UInventoryComponent::ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags)
{
	bool bWroteSomething = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);

	for(UItemBase* Item : Items)
		bWroteSomething |= Channel->ReplicateSubobject(Item, *Bunch, *RepFlags);

	return bWroteSomething;
}

void UInventoryComponent::OnRep_Items()
{
	OnAddedItemDelegate.Broadcast(this, Items.Last(), Items.Num() - 1);
}

void UInventoryComponent::AddItemByClass(const TSubclassOf<UItemBase>& ItemClass)
{
	if(!IsValid(ItemClass)) return;
	if(!HasAuthority())
	{
		Server_AddItemByClass(ItemClass);
	}
	else if(UItemBase* NewItem = NewObject<UItemBase>(GetOwner(), ItemClass))
	{
		Items.Add(NewItem);
		OnRep_Items();
	}
}

UInventoryComponent* UInventoryComponent::GetInventory(const AActor* Target)
{
	if(Target)
	{
		TArray<UInventoryComponent*> Inventories;
		Target->GetComponents<UInventoryComponent>(Inventories);
		if(Inventories.IsValidIndex(0))
			return Inventories[0];
	}
	return nullptr;
}




