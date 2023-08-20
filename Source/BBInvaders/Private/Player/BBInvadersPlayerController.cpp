// by Dmitry Kolontay


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

UProjectileDataAsset* ABBInvadersPlayerController::GetSelectedProjectile() const
{
	return nullptr;
}

void ABBInvadersPlayerController::ExitGame()
{
	ConsoleCommand("quit");
}

void ABBInvadersPlayerController::AcknowledgePossession(APawn* P)
{
	Super::AcknowledgePossession(P);
	SetViewTarget(P);
	if (MyHUD) {
		GetBBInvadersHUD()->OnViewTargetChange(*P);
	}
}
