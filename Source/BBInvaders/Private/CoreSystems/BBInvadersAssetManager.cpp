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

UBBInvadersAssetManager& UBBInvadersAssetManager::Get()
{
	return static_cast<ThisClass&>(Super::Get());
}

int32 UBBInvadersAssetManager::GetProjectilesAvailableToUserType(EShooterType userType, TArray<FSoftObjectPath>& outArray)
{
	check(outArray.Num() == 0);

	if (userType != EShooterType::EST_MAX && projectileDataAssets[static_cast<int32>(userType)].Num()) {
		outArray = projectileDataAssets[static_cast<int32>(userType)];
	}
	else {
		outArray = projectileDataAssets[static_cast<int32>(EShooterType::EST_MAX)];
		
	}
	return outArray.Num();
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

void UBBInvadersAssetManager::LoadProcessUnloadData(TSharedPtr<FStreamableHandle>& handle, 
	TFunction<bool(UObject*)>&& processFunc, bool bForceGC)
{
	if (handle.IsValid()) {
		int32 loadedNum, requestedNum;
		handle->GetLoadedCount(loadedNum, requestedNum);
		if (loadedNum) {
			TArray<UObject*> loadedAssets;
			loadedAssets.Reserve(loadedNum);
			handle->GetLoadedAssets(loadedAssets);

			TArray<FPrimaryAssetId> assetsToUnload;

			for (auto* asset : loadedAssets) {
				if (processFunc(asset)) {
					assetsToUnload.Add(asset->GetPrimaryAssetId());
				}
			}
			handle->ReleaseHandle();

			UnloadPrimaryAssets(assetsToUnload);

			if (bForceGC) {
				GEngine->ForceGarbageCollection(true);
			}
			return;
		}
	}
}

void UBBInvadersAssetManager::OnProjectileDataAssetsLoaded()
{
	LoadProcessUnloadData(projectileAssetsLoadHandle, [this](UObject* object) {
		if (auto* projectileData{ ExactCast<UProjectileDataAsset>(object) }) {
			projectileDataAssets[static_cast<int32>(projectileData->userType)].Emplace(projectileData);
			return true;				
		}
		else {
			return false;
		}
	});
}

bool UBBInvadersAssetManager::IsProjectilesDataSufficient()
{
	return true;
}

void UBBInvadersAssetManager::OnAsteroidMeshSetsLoaded()
{
	LoadProcessUnloadData(projectileAssetsLoadHandle, [this](UObject* object) {
		if (auto * projectileData{ ExactCast<UAsteroidMeshSetAsset>(object) }) {
			asteroidMeshSets.Emplace(projectileData);
			return true;
		}
		else {
			return false;
		}
	});
}
