// by Dmitry Kolontay


#include "CoreSystems/BBInvadersAssetManager.h"
#include "CoreSystems/ProjectileDataAsset.h"
#include "Environment/AsteroidMeshSetAsset.h"
#include "Environment/InvaderVisualsAsset.h"

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
	return static_cast<UBBInvadersAssetManager&>(Super::Get());
}

int32 UBBInvadersAssetManager::GetProjectilesAvailableToUserType(EShooterType userType, 
	TArray<TSoftObjectPtr<UProjectileDataAsset>>& outArray) const
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

TSoftObjectPtr<UProjectileDataAsset> UBBInvadersAssetManager::
	GetRandomProjectilesAvailableToUserType(EShooterType userType) const
{
	//TODO
	check(userType != EShooterType::EST_PlayerOnly);
	return TSoftObjectPtr<UProjectileDataAsset>();
}

TSoftObjectPtr<UAsteroidMeshSetAsset> UBBInvadersAssetManager::GetRandomAsteroidMeshSet() const
{
	return TSoftObjectPtr<UAsteroidMeshSetAsset>();
}

UStaticMesh* UBBInvadersAssetManager::GetRandomInvaderMesh(EInvaderType type) const
{
	
	const TArray<TSoftObjectPtr<UInvaderVisualsAsset>>& arrayRef{ invaderVisuals[static_cast<int32>(type)] };
	return arrayRef[FMath::RandHelper(arrayRef.Num())].LoadSynchronous()->GetStaticMesh();
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

	invaderVisualsLoadHandle = LoadPrimaryAssetsWithType(UInvaderVisualsAsset::assetType, {},
		FStreamableDelegate::CreateUObject(this, &UBBInvadersAssetManager::OnInvaderVisualsLoaded));
}

void UBBInvadersAssetManager::OnDataFailureDetected(bool bIsCritical, const FText& errorText)
{
	//TODO
}

void UBBInvadersAssetManager::LoadProcessUnloadData(TSharedPtr<FStreamableHandle>& handle, 
	TFunction<bool(UObject*)>&& processFunc, bool bForceGC)
{
	if (!handle.IsValid()) {
		return;
	}
	int32 loadedNum, requestedNum;
	handle->GetLoadedCount(loadedNum, requestedNum);
	if (loadedNum) {
		TArray<UObject*> loadedAssets;
		loadedAssets.Reserve(loadedNum);
		handle->GetLoadedAssets(loadedAssets);

		TArray<FPrimaryAssetId> assetsToUnload;

		for (auto* asset : loadedAssets) {
			//redo? other objects stay loaded
			if (processFunc(asset)) {
				assetsToUnload.Add(asset->GetPrimaryAssetId());
			}
		}
		//do i need to unload here?
		UnloadPrimaryAssets(assetsToUnload);		
	}
	handle->ReleaseHandle();
	if (bForceGC) {
		GEngine->ForceGarbageCollection(true);
	}
}

void UBBInvadersAssetManager::OnProjectileDataAssetsLoaded()
{
	auto processor = [this](UObject* object) {
		if (auto* projectileData{ ExactCast<UProjectileDataAsset>(object) }) {
			projectileDataAssets[static_cast<int32>(projectileData->userType)].Emplace(projectileData);
			return true;
		}
		else {
			return false;
		}
	};

	LoadProcessUnloadData(projectileAssetsLoadHandle, processor);
}

bool UBBInvadersAssetManager::IsProjectilesDataSufficient()
{
	return true;
}

void UBBInvadersAssetManager::OnAsteroidMeshSetsLoaded()
{
	auto processor = [this](UObject* object) {
		if (auto* projectileData{ ExactCast<UAsteroidMeshSetAsset>(object) }) {
			asteroidMeshSets.Emplace(projectileData);
			return true;
		}
		else {
			return false;
		} 
	};

	LoadProcessUnloadData(projectileAssetsLoadHandle, processor);
}

void UBBInvadersAssetManager::OnInvaderVisualsLoaded()
{
	auto processor = [this](UObject* object) {
		if (auto* invaderVisual{ ExactCast<UInvaderVisualsAsset>(object) }) {
			invaderVisuals[invaderVisual->GetInvaderTypeI()].Emplace(invaderVisual);
			return true;
		}
		else {
			return false;
		}

	};

	LoadProcessUnloadData(invaderVisualsLoadHandle, processor);
}
