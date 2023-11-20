// by Dmitry Kolontay


#include "Player/BBInvadersPlayerController.h"
#include "CoreSystems/BBInvadersAssetManager.h"
#include "CoreSystems/BBInvadersGameModeBase.h"
#include "CoreSystems/BBInvadersGameStateBase.h"
#include "CoreSystems/ProjectileDataAsset.h"
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

	if (!assetManager.GetProjectilesAvailableToUserType(EShooterType::EST_PlayerOnly, projectiles)) {
		//some error
		return;	
	}

	auto* gameState{ GetWorld()->GetGameState<ABBInvadersGameStateBase>() };
	TArray<FSoftObjectPath> projectilesToLoad;
	projectilesToLoad.Reserve(gameState->GetProjectilesBasekitSize());
	for (auto&& projectile : projectiles) {
		if (gameState->IsProjectileBasekit(projectile)) {
			projectilesToLoad.Add(projectile.ToSoftObjectPath());
		}
	}
	check(projectilesToLoad.Num())
		projectilesHanle = assetManager.LoadAssetList(projectilesToLoad,
			FStreamableDelegate::CreateUObject(this, &ABBInvadersPlayerController::OnProjectilesLoaded),
			FStreamableManager::AsyncLoadHighPriority);
	
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

void ABBInvadersPlayerController::OnProjectilesLoaded() const
{
	//add checks
	if (!projectilesHanle.IsValid()) {
		return;
	}
	int32 loadedNum, requestedNum;
	projectilesHanle->GetLoadedCount(loadedNum, requestedNum);
	if (loadedNum) {
		TArray<UObject*> loadedAssets;
		loadedAssets.Reserve(loadedNum);
		projectilesHanle->GetLoadedAssets(loadedAssets);

		TArray<UProjectileDataAsset*> loadedProjectiles;
		loadedProjectiles.Reserve(loadedAssets.Num());

		for (auto&& asset : loadedAssets) {
			if (!asset) {
				continue;
			}
			check(asset->IsA(UProjectileDataAsset::StaticClass()));
			loadedProjectiles.Add(static_cast<UProjectileDataAsset*>(asset));
		}
		GetBBInvadersHUD()->GetProjectileSelector()->SetAvailableProjectiles(loadedProjectiles);
	}
	projectilesHanle->ReleaseHandle();
}
