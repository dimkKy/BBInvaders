// by Dmitry Kolontay

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "BBInvadersGameInstance.generated.h"

class UAssetProvider;

/**
 * 
 */
UCLASS()
class BBINVADERS_API UBBInvadersGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		TSubclassOf<UAssetProvider> assetProviderClass;
#if WITH_EDITOR
	virtual EDataValidationResult IsDataValid(TArray<FText>& ValidationErrors) override;
#endif
};
