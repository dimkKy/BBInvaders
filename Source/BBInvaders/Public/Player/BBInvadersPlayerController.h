// Fill out your copyright notice in the Description page of Project Settings.

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

	//void OnTogglePauseMenu();
	//virtual void SetupInputComponent() override;
	ABBInvadersHUD* GetBBInvadersHUD() const;
	virtual void SetViewTarget(AActor* newTarget, FViewTargetTransitionParams params = {}) override;

	UFUNCTION()
		void ExitGame();



protected:
	virtual void BeginPlay() override;
	//virtual void PostInitializeComponents() override;
	//virtual void SetPawn(APawn* InPawn) override;
	virtual void AcknowledgePossession(APawn* P) override;
	//virtual void OnUnPossess() override;
};
