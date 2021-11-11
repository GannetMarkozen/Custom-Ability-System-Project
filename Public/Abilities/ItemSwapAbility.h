// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Ability.h"
#include "FPAbility.h"
#include "AbilityPlugin/Public/AttributeSet.h"
#include "ItemSwapAbility.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYERPROJECT_API UItemSwapAbility : public UFPAbility
{
	GENERATED_BODY()
public:
	UItemSwapAbility();
protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	virtual void InitAbility(class UAbilityManagerComponent* AbilityManager) override;
	virtual bool CanActivateAbility(const FActivationInfo& ActivationInfo) override;
	virtual void ActivateAbility(const FActivationInfo& ActivationInfo) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, ReplicatedUsing = "OnRep_ItemIndex", Category = "Init")
	int32 ItemIndex = 0;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "State")
	int32 LastItemIndex = 0;

	UFUNCTION(Server, Reliable)
	void Server_SetItemIndex(const int32 NewItemIndex);
	FORCEINLINE void Server_SetItemIndex_Implementation(const int32 NewItemIndex)
	{
		ItemIndex = NewItemIndex;
		OnRep_ItemIndex();
	}

	UFUNCTION()
	void OnRep_ItemIndex();

	UFUNCTION()
	virtual void InitFirstIndex(class UInventoryComponent* Inventory, class UItemBase* Item, const int32 Index);
};
