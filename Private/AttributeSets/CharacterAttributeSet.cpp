// Fill out your copyright notice in the Description page of Project Settings.


#include "AttributeSets/CharacterAttributeSet.h"

#include "Character/FPCharacterBase.h"
#include "Net/UnrealNetwork.h"


UCharacterAttributeSet::UCharacterAttributeSet()
{
	PrimaryComponentTick.bCanEverTick = false;
	
}

void UCharacterAttributeSet::BeginPlay()
{
	Super::BeginPlay();

	Health.ChangeDelegate.AddLambda([this](const FName& Name, float Value, float OldValue)->void
	{
		PRINT(TEXT("%s: Health == %f"), *GETAUTHTOSTRING, Value);
		
		if(Value <= 0.f)
			if(auto* OwningCharacter = Cast<AFPCharacterBase>(GetOwner()))
				OwningCharacter->Die();
	});
}


void UCharacterAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UCharacterAttributeSet, Health, COND_None, REPNOTIFY_OnChanged);
	DOREPLIFETIME_CONDITION_NOTIFY(UCharacterAttributeSet, Overheal, COND_None, REPNOTIFY_OnChanged);
}

void UCharacterAttributeSet::PreAttributeChange(float& Value, const FName& Name, const FAttribute& Attribute) const
{
	Super::PreAttributeChange(Value, Name, Attribute);

	if(Name == FName("Health"))
	{
		Value = FMath::Clamp<float>(Value, 0.f, Health.BaseValue);
	}
}


