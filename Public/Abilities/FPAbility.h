#pragma once

#include "CoreMinimal.h"
#include "Ability.h"
#include "FPAbility.generated.h"

UCLASS(Abstract)
class MULTIPLAYERPROJECT_API UFPAbility : public UAbility
{
	GENERATED_BODY()
protected:
	virtual void InitAbility(UAbilityManagerComponent* AbilityManager) override;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = "Ability")
	class AFPCharacterBase* FPCharacterBase;
};