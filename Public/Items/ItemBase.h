// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "ReplicatedObject.h"
#include "InputBindingObject.h"
#include "Engine/DataTable.h"

#include "ItemBase.generated.h"

USTRUCT(BlueprintType)
struct FItemMeshTableRow : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FVector RelativeLocation;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FRotator RelativeRotation;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName MuzzleName = FName("Muzzle");

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName GripName = FName("Grip");

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName SightsName = FName("Sights");
};
/**
 * 
 */
// The base item class that stores mesh and thumbnail data with replication abilities
UCLASS(Abstract, BlueprintType, Blueprintable, EditInlineNew, DefaultToInstanced)
class MULTIPLAYERPROJECT_API UItemBase : public UReplicatedObject
{
	GENERATED_BODY()
public:
	UItemBase();
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(BlueprintCallable, Category = "Item")
	virtual void Equip(class AFPCharacterBase* Character);

	UFUNCTION(BlueprintCallable, Category = "Item")
	virtual void UnEquip(class AFPCharacterBase* Character);

	UFUNCTION(BlueprintCallable, Category = "Item")
	const FORCEINLINE FGameplayTag& GetItemTag() const { return ItemTag; }
	
protected:
	UFUNCTION(BlueprintPure, meta = (DisplayName = "GetWorld"), Category = "Items")
	class UWorld* BP_GetWorld() const { return GetWorld(); }
	
	UPROPERTY(BlueprintReadWrite, Category = "State")
	class AFPCharacterBase* OwningCharacter;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Items")
	TObjectPtr<class USkeletalMesh> Mesh;

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "Equip"), Category = "Items")
	void BP_Equip(class AFPCharacterBase* Character);

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "Unequip"), Category = "Items")
	void BP_Unequip(class AFPCharacterBase* Character);
	
	UPROPERTY(EditDefaultsOnly, Category = "Configs")
	FGameplayTag ItemTag;
};
