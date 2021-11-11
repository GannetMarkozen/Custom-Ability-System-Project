// Fill out your copyright notice in the Description page of Project Settings.


#include "FPCharacter.h"

#include "DrawDebugHelpers.h"
#include "Camera/CameraComponent.h"
#include "MultiplayerProject/Interactables/InteractInterface.h"


AFPCharacter::AFPCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->bUsePawnControlRotation = true;
	Camera->SetupAttachment(RootComponent);

	FPMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FPMesh"));
	FPMesh->SetupAttachment(Camera);
}

void AFPCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void AFPCharacter::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AFPCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	PlayerInputComponent->BindAxis(FName("MoveForward"), this, &AFPCharacter::MoveForward);
	PlayerInputComponent->BindAxis(FName("MoveRight"), this, &AFPCharacter::MoveRight);
	PlayerInputComponent->BindAxis(FName("LookUp"), this, &AFPCharacter::LookUp);
	PlayerInputComponent->BindAxis(FName("LookRight"), this, &AFPCharacter::LookRight);

	PlayerInputComponent->BindAction(FName("Jump"), IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction(FName("LeftMouse"), IE_Pressed, this, &AFPCharacter::LeftMousePressed);
	PlayerInputComponent->BindAction(FName("LeftMouse"), IE_Released, this, &AFPCharacter::LeftMouseReleased);
	PlayerInputComponent->BindAction(FName("ScrollUp"), IE_Pressed, this, &AFPCharacter::MouseScrollUp);
	PlayerInputComponent->BindAction(FName("ScrollDown"), IE_Pressed, this, &AFPCharacter::MouseScrollDown);

	PlayerInputComponent->BindAction(FName("Interact"), IE_Pressed, this, &AFPCharacter::Interact);

}


void AFPCharacter::MoveForward(const float Value)
{
	if(Value == 0.f) return;
	AddMovementInput(GetActorForwardVector() * Value);
}

void AFPCharacter::MoveRight(const float Value)
{
	if(Value == 0.f) return;
	AddMovementInput(GetActorRightVector() * Value);
}

void AFPCharacter::LookUp(const float Value)
{
	if(Value == 0.f) return;
	AddControllerPitchInput(-Value * Sensitivity);
}

void AFPCharacter::LookRight(const float Value)
{
	if(Value == 0.f) return;
	AddControllerYawInput(Value * Sensitivity);
}


void AFPCharacter::LeftMousePressed()
{
}

void AFPCharacter::LeftMouseReleased()
{
}

void AFPCharacter::MouseScrollUp()
{
}

void AFPCharacter::MouseScrollDown()
{
}

void AFPCharacter::Interact()
{
	FHitResult HitResult;
	const FVector Start = Camera->GetComponentLocation();
	const FVector End = Start + Camera->GetComponentRotation().Vector() * 200.f;
	const bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility);
	DrawDebugLine(GetWorld(), Start, bHit ? HitResult.Location : End, bHit ? FColor::Green : FColor::Red, false, 5.f);
	if(bHit)
	{
		if(auto* InteractInterface = Cast<IInteractInterface>(HitResult.GetActor()))
			InteractInterface->Interact(this);
	}
}

void AFPCharacter::Server_Interact_Implementation(UObject* SourceObject)
{
	if(auto* InteractInterface = Cast<IInteractInterface>(SourceObject))
		InteractInterface->Interact(this);
}



