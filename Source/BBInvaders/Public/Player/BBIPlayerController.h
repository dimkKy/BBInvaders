// by Dmitry Kolontay

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "BBIPlayerController.generated.h"

class ABBIHUD;
class UProjectileData;
struct FStreamableHandle;

/**
 * 
 */
UCLASS()
class BBINVADERS_API ABBIPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ABBIPlayerController();
	ABBIHUD* GetBBInvadersHUD() const;

	UProjectileData* GetSelectedProjectile() const;

	UFUNCTION()
		void OnGameplayInitiated();
	UFUNCTION()
		void OnGameplayEnd();
	UFUNCTION()
		void ExitGame();
	UFUNCTION()
		void RequestPauseToggle();
protected:
	virtual void BeginPlay() override;
	virtual void AcknowledgePossession(APawn* P) override;

	void OnProjectilesLoaded() const;

	UPROPERTY()
		TArray<UProjectileData*> availableProjectiles;

	TSharedPtr<FStreamableHandle> projectilesHanle;
};
