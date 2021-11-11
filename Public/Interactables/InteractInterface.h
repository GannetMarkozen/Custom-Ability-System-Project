#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InteractInterface.generated.h"



UINTERFACE(MinimalAPI, BlueprintType)
class UInteractInterface : public UInterface
{
	GENERATED_BODY()
};

class MULTIPLAYERPROJECT_API IInteractInterface
{
	GENERATED_BODY()
protected:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interact Interface")
	void Interact(class AFPCharacterBase* Character);
	virtual void Interact_Implementation(class AFPCharacterBase* Character) {}
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interact Interface")
	void Inspect(class AFPCharacterBase* Character);
	virtual void Inspect_Implementation(class AFPCharacterBase* Character) {}
};