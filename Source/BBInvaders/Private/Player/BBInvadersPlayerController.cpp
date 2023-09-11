// by Dmitry Kolontay


#include "Player/BBInvadersPlayerController.h"
#include "CoreSystems/BBInvadersAssetManager.h"
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

void ABBInvadersPlayerController::OnGameplayInitiated()
{
	auto&& assetManager{ UBBInvadersAssetManager::Get() };
	TArray<FSoftObjectPath> projectiles;
	if (assetManager.GetProjectilesAvailableToUserType(EShooterType::EST_PlayerOnly, projectiles)) {
		//projectilesHanle = assetManager.LoadAssetList(projectiles, , FStreamableManager::AsyncLoadHighPriority);
	}
	
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
