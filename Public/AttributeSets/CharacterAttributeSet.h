// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "CharacterAttributeSet.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYERPROJECT_API UCharacterAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
public:
	UCharacterAttributeSet();
protected:
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
	virtual void PreAttributeChange(float& OutValue, const FName& Name, const FAttribute& Attribute) const override;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, ReplicatedUsing = "OnRep_Health", Category = "Attribute")
	FAttribute Health = FAttribute(100.f);
	
	//DECLARE_ATTRIBUTE_ONREPFUNC(Health)
	UFUNCTION()
	FORCEINLINE void OnRep_Health(const FAttribute& OldHealth) const { Health.ChangeDelegate.Broadcast(FName(TEXT("Health")), Health.CurrentValue, OldHealth.CurrentValue); }

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, ReplicatedUsing = "OnRep_Overheal", Category = "Attribute")
	FAttribute Overheal = FAttribute(0.f);
	
	//DECLARE_ATTRIBUTE_ONREPFUNC(Overheal)
	UFUNCTION()
	FORCEINLINE void OnRep_Overheal() const { Overheal.ChangeDelegate.Broadcast(FName(TEXT("Health")), Overheal.CurrentValue, Overheal.CurrentValue); }
};
