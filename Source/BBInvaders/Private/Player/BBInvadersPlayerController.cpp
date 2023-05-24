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

void ABBInvadersPlayerController::ExitGame()
{
	ConsoleCommand("quit");
}

void ABBInvadersPlayerController::AcknowledgePossession(APawn* P)
{
	Super::AcknowledgePossession(P);
	SetViewTarget(P);
	GetBBInvadersHUD()->OnViewTargetChange(*P);
}
