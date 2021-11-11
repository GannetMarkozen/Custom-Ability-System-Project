// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/TimelineComponent.h"
#include "UObject/Object.h"
#include "RecoilObject.generated.h"

DECLARE_DELEGATE_OneParam(FRecoilProgress, float);
DECLARE_DELEGATE(FOnRecoilEnd);
/**
 * 
 */
UCLASS(Blueprintable, BlueprintType, DefaultToInstanced, EditInlineNew)
class MULTIPLAYERPROJECT_API URecoilObject : public UObject
{
	GENERATED_BODY()

public:
	URecoilObject();
	
	UFUNCTION(BlueprintCallable, Category = "Recoil")
	virtual void AddRecoil(class APawn* NewWielder);

	UFUNCTION(BlueprintCallable, Category = "Recoil")
	static class URecoilObject* AddRecoilInstance(class APawn* Target, const TSubclassOf<class URecoilObject>& Class);

	FTickerDelegate TickDelegate;
	FDelegateHandle TickDelegateHandle;

	FRecoilProgress RecoilProgressDelegate;
	FOnRecoilEnd OnRecoilEndDelegate;
	
protected:
	virtual bool Tick(const float DeltaTime);/*
	virtual FORCEINLINE bool IsTickable() const override { return !IsTemplate(RF_ClassDefaultObject) && GetWorld() && GetWorld()->HasBegunPlay() && !GIsEditor; }
	virtual FORCEINLINE TStatId GetStatId() const override { return UObject::GetStatID(); }
	virtual FORCEINLINE bool IsTickableInEditor() const override { return true; }*/
	
	UPROPERTY(BlueprintReadOnly, Category = "Recoil")
	class APawn* Wielder;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Recoil")
	class UCurveFloat* RecoilCurve;
	
	FTimeline Timeline;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Recoil")
	float RecoilMagnitude = 1.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Recoil")
	float RecoilMultiplier = 1.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Recoil")
	float RecoilMaxAngleLeft = 1.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Recoil")
	float RecoilMaxAngleRight = 1.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Recoil")
	float PlaySpeed = 1.f;

	UFUNCTION()
	virtual void TimelineProgress(float Value);

	UFUNCTION()
	virtual void TimelineEnd();
	
private:
	float LastValue = 0.f;

	FVector2D RecoilVector;
};
