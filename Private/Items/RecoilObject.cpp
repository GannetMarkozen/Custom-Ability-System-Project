// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/RecoilObject.h"
#include "MultiplayerProjectIncludes.h"

URecoilObject::URecoilObject()
{
	
}

bool URecoilObject::Tick(const float DeltaTime)
{
	Timeline.TickTimeline(DeltaTime);
	return true;
}

URecoilObject* URecoilObject::AddRecoilInstance(APawn* Target, const TSubclassOf<URecoilObject>& Class)
{
	if(!Target || !Class) return nullptr;
	if(URecoilObject* RecoilObject = NewObject<URecoilObject>(Target, Class))
	{
		RecoilObject->TickDelegate = FTickerDelegate::CreateUObject(RecoilObject, &URecoilObject::Tick);
		RecoilObject->TickDelegateHandle = FTicker::GetCoreTicker().AddTicker(RecoilObject->TickDelegate);
		
		RecoilObject->AddRecoil(Target);
		return RecoilObject;
	}
	return nullptr;
}


void URecoilObject::AddRecoil(APawn* NewWielder)
{
	Wielder = NewWielder;
	
	if(RecoilCurve && Wielder)
	{
		FOnTimelineFloatStatic TimelineProgress;
		TimelineProgress.BindUObject(this, &URecoilObject::TimelineProgress);
		Timeline.AddInterpFloat(RecoilCurve, TimelineProgress);
		Timeline.SetPlayRate(PlaySpeed);

		FOnTimelineEvent EndEvent;
		EndEvent.BindDynamic(this, &URecoilObject::TimelineEnd);
		Timeline.AddEvent(Timeline.GetTimelineLength(), EndEvent);
		
		RecoilVector = FVector2D(FMath::FRandRange(RecoilMaxAngleLeft, -RecoilMaxAngleRight) * RecoilMultiplier,RecoilMagnitude * RecoilMultiplier);
		Timeline.PlayFromStart();
	}
}

void URecoilObject::TimelineProgress(float Value)
{
	RecoilProgressDelegate.ExecuteIfBound(Value);
	
	Value *= -1.f;
	const float Difference = Value - LastValue;
	LastValue = Value;
	
	if(Wielder)
	{
		Wielder->AddControllerPitchInput(Difference * RecoilVector.Y);
		Wielder->AddControllerYawInput(Difference * RecoilVector.X);
	}
}

void URecoilObject::TimelineEnd()
{
	OnRecoilEndDelegate.ExecuteIfBound();
	TickDelegate.Unbind();
	TickDelegateHandle.Reset();
	
	ConditionalBeginDestroy();
}

