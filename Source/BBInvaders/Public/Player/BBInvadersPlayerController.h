// by Dmitry Kolontay

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "BBInvadersPlayerController.generated.h"

class ABBInvadersHUD;
class UProjectileDataAsset;
/**
 * 
 */
UCLASS()
class BBINVADERS_API ABBInvadersPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ABBInvadersPlayerController();
	ABBInvadersHUD* GetBBInvadersHUD() const;

	UProjectileDataAsset* GetSelectedProjectile() const;

	UFUNCTION()
		void ExitGame();

protected:
	virtual void AcknowledgePossession(APawn* P) override;
};
