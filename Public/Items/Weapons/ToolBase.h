// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/ItemBase.h"
#include "ToolBase.generated.h"

USTRUCT(BlueprintType)
struct FToolBindingInfo : public FBindingInfo
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	FGameplayTagContainer RequiredTags;

	UPROPERTY(EditDefaultsOnly)
	FGameplayTagContainer BlockedTags;

	FToolBindingInfo(){}
	FToolBindingInfo(const FName& InputName, const TEnumAsByte<EInputEvent> InputEvent, const FName& FunctionName)
	{
		this->InputName = InputName;
		this->InputEvent = InputEvent;
		this->FunctionName = FunctionName;
	}
	FToolBindingInfo(const FName& InputName, const TEnumAsByte<EInputEvent> InputEvent, const FName& FunctionName,
		const FGameplayTagContainer& RequiredTags, const FGameplayTagContainer& BlockedTags)
	{
		this->InputName = InputName;
		this->InputEvent = InputEvent;
		this->FunctionName = FunctionName;
		this->RequiredTags = RequiredTags;
		this->BlockedTags = BlockedTags;
	}
};
/**
 * 
 */

// An item with interaction and functionality
UCLASS(Abstract)
class MULTIPLAYERPROJECT_API UToolBase : public UItemBase
{
	GENERATED_BODY()
public:
	UToolBase();
protected:
	virtual void Equip(class AFPCharacterBase* Character) override;
	virtual void UnEquip(class AFPCharacterBase* Character) override;

	UFUNCTION(Server, Reliable)
	void Server_Equip(class AFPCharacterBase* Character);
	virtual void Server_Equip_Implementation(class AFPCharacterBase* Character);

	UFUNCTION(NetMulticast, Reliable)
	void Multi_Equip(class AFPCharacterBase* Character);
	virtual void Multi_Equip_Implementation(class AFPCharacterBase* Character);

	// Bound inputs on equipped and unbound on unequipped
	UPROPERTY()
	class UInputBindingObject* InputBinds;

	UPROPERTY(EditDefaultsOnly, Category = "Configurations")
	TArray<FToolBindingInfo> Binds;

	UPROPERTY(EditDefaultsOnly, Category = "Configurations")
	class UAnimMontage* FP_EquipMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Configurations")
	class UAnimMontage* EquipMontage;

	FTimerHandle ItemSwappingTimerHandle;

	// Granted and removed on equipped and unequipped
	UPROPERTY(EditDefaultsOnly, Category = "Configurations")
	TArray<TSubclassOf<class UAbility>> Abilities;
	
	void AddBind(FToolBindingInfo& Bind);

	// Determine whether or not the function binding can run
	virtual bool CanProcessFunction(const FBindingInfo& Bind) const;

	// Determine whether or not the function binding can run
	UFUNCTION(BlueprintNativeEvent, meta = (DisplayName = "Can Process Function"), Category = "Tool")
	bool BP_CanProcessFunction(const FBindingInfo& Bind) const;
	FORCEINLINE bool BP_CanProcessFunction_Implementation(const FBindingInfo& Bind) const { return true; }
};
