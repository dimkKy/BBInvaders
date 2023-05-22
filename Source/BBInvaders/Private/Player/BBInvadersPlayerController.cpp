// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/BBInvadersPlayerController.h"
#include "UI/BBInvadersHUD.h"


ABBInvadersPlayerController::ABBInvadersPlayerController()
{
	bAutoManageActiveCameraTarget = false;
	
	bShowMouseCursor = true;
}

ABBInvadersHUD* ABBInvadersPlayerController::GetBBInvadersHUD() const
{
	return CastChecked<ABBInvadersHUD>(MyHUD);
}

void ABBInvadersPlayerController::SetViewTarget(AActor* NewViewTarget, FViewTargetTransitionParams TransitionParams)
{
	Super::SetViewTarget(NewViewTarget, TransitionParams);
	GetBBInvadersHUD()->OnViewTargetChange(*NewViewTarget);
}

void ABBInvadersPlayerController::ExitGame()
{
	ConsoleCommand("quit");
}

void ABBInvadersPlayerController::BeginPlay()
{
	//SetViewTarget(BBInvadersUtils::GetFirstActor<AMainMenuCameraActor>(GetWorld()));
	Super::BeginPlay();
}

void ABBInvadersPlayerController::AcknowledgePossession(APawn* P)
{
	Super::AcknowledgePossession(P);
}
