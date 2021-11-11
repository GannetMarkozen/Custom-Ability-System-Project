// Fill out your copyright notice in the Description page of Project Settings.


#include "Door.h"

#include "Net/UnrealNetwork.h"


ADoor::ADoor()
{
	PrimaryActorTick.bCanEverTick = false;

	bReplicates = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;
}

void ADoor::BeginPlay()
{
	Super::BeginPlay();
	
}

void ADoor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ADoor, bDoorIsOpen);
}


void ADoor::Interact(AFPCharacter* Character)
{
	bDoorIsOpen = !bDoorIsOpen;
}

void ADoor::OnRep_ToggleDoor()
{
	const FString Message = bDoorIsOpen ? "Door is now open" : "Door is now closed";
	UE_LOG(LogTemp, Warning, TEXT("%s"), *Message);
}

