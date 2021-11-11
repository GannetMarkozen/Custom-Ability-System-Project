// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilityManagerComponent.h"
#include "FPCharacterBase.generated.h"


UCLASS()
class MULTIPLAYERPROJECT_API AFPCharacterBase : public ACharacter, public IAbilityManagerInterface
{
	GENERATED_BODY()

public:
	AFPCharacterBase();

protected:
	virtual void BeginPlay() override;
	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
public:		
	virtual class UAbilityManagerComponent* GetAbilityManagerComponent_Implementation() const override { return AbilityManager; }

	// First person camera component
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	TObjectPtr<class UCameraComponent> Camera;

	// First person view arms mesh component
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	TObjectPtr<class USkeletalMeshComponent> FP_Mesh;

	// Manages and activates abilities through tags
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	TObjectPtr<class UAbilityManagerComponent> AbilityManager;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	TObjectPtr<class UCharacterAttributeSet> AttributeSet;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	TObjectPtr<class UInventoryComponent> Inventory;

	UFUNCTION(BlueprintCallable, Category = "FPCharacter")
	virtual void Die();

	UFUNCTION(BlueprintCallable, Category = "FPCharacter")
	void SetItemMesh(class USkeletalMesh* NewMesh);

	UFUNCTION(BlueprintCallable, Category = "FPCharacter")
	const FORCEINLINE class UDataTable* GetItemDataTable() const { return ItemMeshDataTable; }

	UFUNCTION(BlueprintNativeEvent, Category = "FPCharacter")
	class USkeletalMeshComponent* GetItemMesh() const;
	FORCEINLINE class USkeletalMeshComponent* GetItemMesh_Implementation() const { return nullptr; }

	UFUNCTION(BlueprintNativeEvent, Category = "FPCharacter")
	class USkeletalMeshComponent* GetFPItemMesh() const;
	FORCEINLINE class USkeletalMeshComponent* GetFPItemMesh_Implementation() const { return nullptr; }
	
protected:
	virtual void MoveForward(const float Value);
	virtual void MoveRight(const float Value);
	virtual void LookUp(const float Value);
	virtual void Turn(const float Value);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Configurations")
	class UDataTable* ItemMeshDataTable;

	UPROPERTY(EditDefaultsOnly, Category = "Configurations")
	float Sensitivity = 1.f;

	UFUNCTION(Server, Reliable)
	void Server_SetItemMesh(class USkeletalMesh* NewMesh);
	FORCEINLINE void Server_SetItemMesh_Implementation(class USkeletalMesh* NewMesh) { SetItemMesh(NewMesh); }

	UPROPERTY(ReplicatedUsing = "OnRep_NewItemMesh")
	class USkeletalMesh* NewItemMesh;

	UFUNCTION()
	void OnRep_NewItemMesh();
};
