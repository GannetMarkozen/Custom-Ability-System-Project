// Fill out your copyright notice in the Description page of Project Settings.


#include "MultiplayerProject/Public/Character/FPCharacterBase.h"

#include "AttributeSets/CharacterAttributeSet.h"
#include "Camera/CameraComponent.h"
#include "AbilityPlugin/Public/AbilityManagerComponent.h"
#include "AbilityPlugin/Public/MultiplayerProjectIncludes.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Items/InventoryComponent.h"
#include "Items/ItemBase.h"

#include "Net/UnrealNetwork.h"

AFPCharacterBase::AFPCharacterBase()
{
	PrimaryActorTick.bCanEverTick = false;

	GetMesh()->SetGenerateOverlapEvents(true);
	GetMesh()->SetIsReplicated(true);
	
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->bUsePawnControlRotation = true;
	Camera->SetupAttachment(RootComponent);

	FP_Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FP_Mesh"));
	FP_Mesh->SetCollisionObjectType(ECC_Pawn);
	FP_Mesh->SetupAttachment(Camera);

	AbilityManager = CreateDefaultSubobject<UAbilityManagerComponent>(TEXT("AbilityManager"));
	AbilityManager->SetIsReplicated(true);

	AttributeSet = CreateDefaultSubobject<UCharacterAttributeSet>(TEXT("AttributeSet"));
	AttributeSet->SetIsReplicated(true);

	Inventory = CreateDefaultSubobject<UInventoryComponent>(TEXT("Inventory"));
	Inventory->SetIsReplicated(true);
}

void AFPCharacterBase::BeginPlay()
{
	Super::BeginPlay();

	
}

void AFPCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &AFPCharacterBase::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AFPCharacterBase::MoveRight);
	PlayerInputComponent->BindAxis("LookUp", this, &AFPCharacterBase::LookUp);
	PlayerInputComponent->BindAxis("Turn", this, &AFPCharacterBase::Turn);
}

void AFPCharacterBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(AFPCharacterBase, NewItemMesh, COND_None);
}

void AFPCharacterBase::Die()
{
	PRINT(TEXT("Died"));
	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	GetItemMesh()->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
	GetItemMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	GetItemMesh()->SetSimulatePhysics(true);
	
	AbilityManager->AddTags(TAG_CONTAINER("State.Dead"));
}

void AFPCharacterBase::SetItemMesh(USkeletalMesh* NewMesh)
{
	if(!HasAuthority())
	{
		Server_SetItemMesh(NewMesh);
	}
	else
	{
		NewItemMesh = NewMesh;
		OnRep_NewItemMesh();
	}
}

void AFPCharacterBase::OnRep_NewItemMesh()
{
	FItemMeshTableRow* TableRow = nullptr;
	if(ItemMeshDataTable && NewItemMesh)
		TableRow = ItemMeshDataTable->FindRow<FItemMeshTableRow>(NewItemMesh->GetFName(), "Context");
	
	const FTransform RelativeTransform(TableRow ? FTransform(TableRow->RelativeRotation, TableRow->RelativeLocation) : FTransform::Identity);
	
	GetItemMesh()->SetSkeletalMesh(NewItemMesh);
	GetItemMesh()->SetRelativeTransform(RelativeTransform);

	if(IsLocallyControlled())
	{
		GetFPItemMesh()->SetSkeletalMesh(NewItemMesh);
		GetFPItemMesh()->SetRelativeTransform(RelativeTransform);
	}

	/*
	ItemMesh->SetSkeletalMesh(NewItemMeshTransformPair.Mesh);
	ItemMesh->SetRelativeTransform(NewItemMeshTransformPair.RelativeTransform);

	if(IsLocallyControlled())
	{
		FP_ItemMesh->SetSkeletalMesh(NewItemMeshTransformPair.Mesh);
		FP_ItemMesh->SetRelativeTransform(NewItemMeshTransformPair.RelativeTransform);
	}*/
}


void AFPCharacterBase::MoveForward(const float Value) 
{
	if(Value != 0.f)
		AddMovementInput(GetActorForwardVector(), Value);
}

void AFPCharacterBase::MoveRight(const float Value)
{
	if(Value != 0.f)
		AddMovementInput(GetActorRightVector(), Value);
}

void AFPCharacterBase::LookUp(const float Value)
{
	if(Value != 0.f)
		AddControllerPitchInput(Value * Sensitivity);
}

void AFPCharacterBase::Turn(const float Value)
{
	if(Value != 0.f)
		AddControllerYawInput(Value * Sensitivity);
}


