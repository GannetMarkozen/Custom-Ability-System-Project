// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "FPCharacter.generated.h"

UCLASS()
class MULTIPLAYERPROJECT_API AFPCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AFPCharacter();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(const float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Bindings
	void MoveForward(const float Value);
	void MoveRight(const float Value);
	void LookUp(const float Value);
	void LookRight(const float Value);

	void LeftMousePressed();
	void LeftMouseReleased();
	void MouseScrollUp();
	void MouseScrollDown();

	void Interact();
	UFUNCTION(Server, Reliable)
	void Server_Interact(class UObject* SourceObject);

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = "Components")
	class UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = "Components")
	class USkeletalMeshComponent* FPMesh;

	// The mouse sensitivity
	UPROPERTY(EditDefaultsOnly, Category = "Configurations")
	float Sensitivity = 0.5f;
};