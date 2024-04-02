// by Dmitry Kolontay

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "BBIGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class BBINVADERS_API UBBIGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:

#if WITH_EDITOR
	virtual EDataValidationResult IsDataValid(FDataValidationContext& context) const override;
#endif
};
