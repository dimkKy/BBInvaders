// by Dmitry Kolontay


#include "Player/BBIPlayerController.h"
#include "CoreSystems/BBIAssetManager.h"
#include "CoreSystems/BBIGameModeBase.h"
#include "CoreSystems/BBIGameStateBase.h"
#include "Projectiles/ProjectileData.h"
#include "UI/BBIHUD.h"
#include "UI/ProjectileSelector.h"


ABBIPlayerController::ABBIPlayerController()
{
	bAutoManageActiveCameraTarget = false;
	bShowMouseCursor = true;
}

ABBIHUD* ABBIPlayerController::GetBBInvadersHUD() const
{
	return CastChecked<ABBIHUD>(MyHUD);
}

UProjectileData* ABBIPlayerController::GetSelectedProjectile() const
{
	return GetBBInvadersHUD()->GetProjectileSelector()->GetSelectedProjectile();
}

void ABBIPlayerController::OnGameplayInitiated()
{
	UBBIAssetManager& assetManager{ UBBIAssetManager::Get() };
	TArray<TSoftObjectPtr<UProjectileData>> projectiles;

	if (!assetManager.ProjectileAssets(EShooterType::EST_Player, projectiles)) {
		//some error
		return;	
	}
	//redo
	auto* gameState{ GetWorld()->GetGameState<ABBIGameStateBase>() };
	TArray<FSoftObjectPath> projectilesToLoad;
	for (auto&& projectile : projectiles) {
		projectilesToLoad.Add(projectile.ToSoftObjectPath());
	}
	check(projectilesToLoad.Num())
		projectilesHanle = assetManager.LoadAssetList(projectilesToLoad,
			FStreamableDelegate::CreateUObject(this, &ABBIPlayerController::OnProjectilesLoaded),
			FStreamableManager::AsyncLoadHighPriority);
	
}

void ABBIPlayerController::OnGameplayEnd()
{
	GetWorld()->GetAuthGameMode<ABBIGameModeBase>()->GoToMainMenu();
}

void ABBIPlayerController::ExitGame()
{
	ConsoleCommand("quit");
}

void ABBIPlayerController::RequestPauseToggle()
{
	GetWorld()->GetAuthGameMode<ABBIGameModeBase>()->TogglePause();
}

void ABBIPlayerController::BeginPlay()
{
	GetBBInvadersHUD()->RequestBindings(*this);
}

void ABBIPlayerController::AcknowledgePossession(APawn* P)
{
	Super::AcknowledgePossession(P);
	SetViewTarget(P);
	if (MyHUD) {
		GetBBInvadersHUD()->OnViewTargetChange(*P);
	}
}

void ABBIPlayerController::OnProjectilesLoaded() const
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

		TArray<UProjectileData*> loadedProjectiles;
		loadedProjectiles.Reserve(loadedAssets.Num());

		for (auto&& asset : loadedAssets) {
			if (!asset) {
				continue;
			}
			check(asset->IsA(UProjectileData::StaticClass()));
			loadedProjectiles.Add(static_cast<UProjectileData*>(asset));
		}
		GetBBInvadersHUD()->GetProjectileSelector()->SetAvailableProjectiles(loadedProjectiles);
	}
	projectilesHanle->ReleaseHandle();
}
