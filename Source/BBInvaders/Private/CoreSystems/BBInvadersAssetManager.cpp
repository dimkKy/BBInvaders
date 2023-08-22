// by Dmitry Kolontay


#include "CoreSystems/BBInvadersAssetManager.h"
#include "CoreSystems/ProjectileDataAsset.h"
#include "Environment/AsteroidMeshSetAsset.h"

void UBBInvadersAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();
	//ScanPathsForPrimaryAssets()
	//GetPrimaryAssetIdList
}

void UBBInvadersAssetManager::FinishInitialLoading()
{
	Super::FinishInitialLoading();
}

int32 UBBInvadersAssetManager::GetProjectilesAvailableToPlayer(TArray<UProjectileDataAsset*>& outArray)
{
	int32 addedAmount{ 0 };

	if (projectileDataAssets[static_cast<int32>(EShooterType::EST_PlayerOnly)].Num()) {
		for (auto asset : projectileDataAssets[static_cast<int32>(EShooterType::EST_PlayerOnly)]) {
			outArray.Add(asset.LoadSynchronous());
			addedAmount++;
		}
	}
	else {
		for (auto asset : projectileDataAssets[static_cast<int32>(EShooterType::EST_MAX)]) {
			outArray.Add(asset.LoadSynchronous());
			addedAmount++;
		}
	}
	return addedAmount;
}

void UBBInvadersAssetManager::PostInitialAssetScan()
{
	Super::PostInitialAssetScan();

	/*TArray<FSoftObjectPath> projectileAssetsPaths;
	if (GetPrimaryAssetPathList(UProjectileDataAsset::assetType, projectileAssetsPaths)) {
		for()
	}*/

	projectileAssetsLoadHandle = LoadPrimaryAssetsWithType(UProjectileDataAsset::assetType, {},
		FStreamableDelegate::CreateUObject(this, &UBBInvadersAssetManager::OnProjectileDataAssetsLoaded));

	asteroidMeshSetsLoadHandle = LoadPrimaryAssetsWithType(UAsteroidMeshSetAsset::assetType, {},
		FStreamableDelegate::CreateUObject(this, &UBBInvadersAssetManager::OnAsteroidMeshSetsLoaded));
}

void UBBInvadersAssetManager::OnDataFailureDetected(bool bIsCritical, const FText& errorText)
{
	//TODO
}

void UBBInvadersAssetManager::OnProjectileDataAssetsLoaded()
{

	if (projectileAssetsLoadHandle.IsValid()) {
		int32 loadedNum, requestedNum;
		projectileAssetsLoadHandle->GetLoadedCount(loadedNum, requestedNum);
		if (loadedNum) {
			TArray<UObject*> loadedAssets;
			loadedAssets.Reserve(loadedNum);
			projectileAssetsLoadHandle->GetLoadedAssets(loadedAssets);

			TArray<FPrimaryAssetId> assetsToUnload;

			for (auto* asset : loadedAssets) {
				if (auto* projectileData{ ExactCast<UProjectileDataAsset>(asset) }) {
					projectileDataAssets[static_cast<int32>(projectileData->userType)].Emplace(projectileData);
					assetsToUnload.Add(projectileData->GetPrimaryAssetId());
					//projectileData->MarkPendingKill();					
				}
			}
			projectileAssetsLoadHandle->ReleaseHandle();
			projectileAssetsLoadHandle.Reset();

			if (!IsProjectilesDataSufficient()) {
				//todo
				return;
			}

			UnloadPrimaryAssets(assetsToUnload);
			//?
			GEngine->ForceGarbageCollection(true);
			return;
		}
	}
}

bool UBBInvadersAssetManager::IsProjectilesDataSufficient()
{
	return true;
}

void UBBInvadersAssetManager::OnAsteroidMeshSetsLoaded()
{

}
