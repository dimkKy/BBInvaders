// by Dmitry Kolontay

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "BBInvadersGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class BBINVADERS_API UBBInvadersGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:

#if WITH_EDITOR
	virtual EDataValidationResult IsDataValid(TArray<FText>& ValidationErrors) override;
#endif
};
