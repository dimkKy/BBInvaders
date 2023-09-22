// by Dmitry Kolontay


#include "Player/BBInvadersPlayerController.h"
#include "CoreSystems/BBInvadersAssetManager.h"
#include "CoreSystems/BBInvadersGameModeBase.h"
#include "UI/BBInvadersHUD.h"
#include "UI/ProjectileSelector.h"


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
	return GetBBInvadersHUD()->GetProjectileSelector()->GetSelectedProjectile();
}

void ABBInvadersPlayerController::OnGameplayInitiated()
{
	auto&& assetManager{ UBBInvadersAssetManager::Get() };
	TArray<TSoftObjectPtr<UProjectileDataAsset>> projectiles;
	if (assetManager.GetProjectilesAvailableToUserType(EShooterType::EST_PlayerOnly, projectiles)) {
		//projectilesHanle = assetManager.LoadAssetList(projectiles, , FStreamableManager::AsyncLoadHighPriority);
	}
	
}

void ABBInvadersPlayerController::OnGameplayEnd()
{
	GetWorld()->GetAuthGameMode<ABBInvadersGameModeBase>()->GoToMainMenu();
}

void ABBInvadersPlayerController::ExitGame()
{
	ConsoleCommand("quit");
}

void ABBInvadersPlayerController::RequestPauseToggle()
{
	GetWorld()->GetAuthGameMode<ABBInvadersGameModeBase>()->TogglePause();
}

void ABBInvadersPlayerController::BeginPlay()
{
	GetBBInvadersHUD()->RequestBindings(*this);
}

void ABBInvadersPlayerController::AcknowledgePossession(APawn* P)
{
	Super::AcknowledgePossession(P);
	SetViewTarget(P);
	if (MyHUD) {
		GetBBInvadersHUD()->OnViewTargetChange(*P);
	}
}
