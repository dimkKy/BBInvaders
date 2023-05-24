// by Dmitry Kolontay

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "BBInvadersPlayerController.generated.h"

class ABBInvadersHUD;
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

	UFUNCTION()
		void ExitGame();

protected:
	virtual void AcknowledgePossession(APawn* P) override;
};
