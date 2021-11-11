// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractInterface.h"
#include "GameFramework/Actor.h"
#include "Door.generated.h"

UCLASS()
class MULTIPLAYERPROJECT_API ADoor : public AActor, public IInteractInterface
{
	GENERATED_BODY()

public:
	ADoor();
	virtual void Interact(AFPCharacter* Character) override;
	virtual FString Inspect(AFPCharacter* Character) override { return bDoorIsOpen ? "Press [E] to close door" : "Press [E] to open door"; }

protected:
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* Mesh;

	UPROPERTY(ReplicatedUsing = OnRep_ToggleDoor)
	bool bDoorIsOpen = false;

	UFUNCTION()
	void OnRep_ToggleDoor();
};
