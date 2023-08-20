// by Dmitry Kolontay

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "BBInvadersAssetManager.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class BBINVADERS_API UBBInvadersAssetManager : public UAssetManager
{
	GENERATED_BODY()

public:

	virtual void StartInitialLoading() override;
	//UPROP
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float initialSpeed;
};
