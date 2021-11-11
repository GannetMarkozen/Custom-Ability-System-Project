// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemBase.h"

#include "InventoryComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FAddedItemDelegate, class UInventoryComponent*, Inventory, class UItemBase*, Item, const int32, Index);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FRemovedItemDelegate, class UInventoryComponent*, Inventory, class UItemBase*, Item);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable, BlueprintType)
class MULTIPLAYERPROJECT_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UInventoryComponent();

protected:
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual bool ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags) override;

	UPROPERTY(EditDefaultsOnly, Category = "Inventory|Configs")
	TArray<TSubclassOf<class UItemBase>> DefaultItems;
	
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, ReplicatedUsing = "OnRep_Items", meta = (AllowPrivateAccess = "true"), Category = "Inventory")
	TArray<class UItemBase*> Items;

	UFUNCTION()
	void OnRep_Items();
	
	UFUNCTION(Server, Reliable)
	void Server_AddItemByClass(class UClass* ItemClass);
	FORCEINLINE void Server_AddItemByClass_Implementation(class UClass* ItemClass)
	{
		AddItemByClass(ItemClass);
	}

	FORCEINLINE void Internal_RemoveItem(class UItemBase* Item)
	{
		Items.Remove(Item);
		OnRemovedItemDelegate.Broadcast(this, Item);
	}

	UFUNCTION(Server, Reliable)
	void Server_RemoveItem(class UItemBase* Item);
	FORCEINLINE void Server_RemoveItem_Implementation(class UItemBase* Item) { Internal_RemoveItem(Item); }

	UFUNCTION(Client, Reliable)
	void Client_RemoveItem(class UItemBase* Item);
	FORCEINLINE void Client_RemoveItem_Implementation(class UItemBase* Item) { Internal_RemoveItem(Item); }

public:
	UPROPERTY(BlueprintAssignable, Category = "Inventory")
	FAddedItemDelegate OnAddedItemDelegate;

	UPROPERTY(BlueprintAssignable, Category = "Inventory")
	FRemovedItemDelegate OnRemovedItemDelegate;
	
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool HasAuthority() const { return GetOwner() && GetOwner()->HasAuthority(); }

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	static class UInventoryComponent* GetInventory(const class AActor* Target);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	const TArray<class UItemBase*>& GetItems() const { return Items; }

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	FORCEINLINE void AddItem(class UItemBase* Item)
	{
		Items.Add(Item);
	}
	
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void AddItemByClass(const TSubclassOf<class UItemBase>& ItemClass);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	FORCEINLINE void AddItemsByClasses(const TArray<TSubclassOf<class UItemBase>>& ItemClasses)
	{
		for(const TSubclassOf<class UItemBase>& ItemClass : ItemClasses)
			AddItemByClass(ItemClass);
	}

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	FORCEINLINE void RemoveItem(class UItemBase* Item) { Server_RemoveItem(Item); Client_RemoveItem(Item); }

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	FORCEINLINE void RemoveItems(const TArray<class UItemBase*>& RemoveItems) { for(class UItemBase* Item : RemoveItems) RemoveItem(Item); }

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	FORCEINLINE void RemoveItemByIndex(const int32 Index) { if(Items.IsValidIndex(Index)) Server_RemoveItem(Items[Index]); Client_RemoveItem(Items[Index]); }
};
