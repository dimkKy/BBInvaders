// by Dmitry Kolontay


#include "CoreSystems/BBInvadersAssetManager.h"
#include "CoreSystems/ProjectileDataAsset.h"
#include "Environment/AsteroidMeshSetAsset.h"
#include "Environment/InvaderVisualsAsset.h"

void UBBIAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();
	//ScanPathsForPrimaryAssets()
	//GetPrimaryAssetIdList
}

void UBBIAssetManager::FinishInitialLoading()
{
	Super::FinishInitialLoading();
}

UBBIAssetManager& UBBIAssetManager::Get()
{
	return static_cast<UBBIAssetManager&>(Super::Get());
}

int32 UBBIAssetManager::ProjectileAssets(EShooterType userType, 
	TArray<TSoftObjectPtr<UProjectileDataAsset>>& outArray) const
{
	check(outArray.Num() == 0);
	int32 userTypeI{ static_cast<int32>(userType) };

	verify(userTypeI > static_cast<int8>(EShooterType::EST_None) 
		&& userTypeI < static_cast<int8>(EShooterType::EST_MAX));

	outArray = projectileDataAssets[userTypeI - 1];
	return outArray.Num();
}

TSoftObjectPtr<UProjectileDataAsset> UBBIAssetManager::
	RandomProjectile(EShooterType userType) const
{
	//TODO
	int32 userTypeI{ static_cast<int32>(userType) };

	verify(userTypeI > static_cast<int8>(EShooterType::EST_None)
		&& userTypeI < static_cast<int8>(EShooterType::EST_MAX));
	return TSoftObjectPtr<UProjectileDataAsset>();
}

TSoftObjectPtr<UAsteroidMeshSetAsset> UBBIAssetManager::RandomAsteroidMeshSet() const
{
	return asteroidMeshSets[FMath::RandHelper(asteroidMeshSets.Num())];
}

UStaticMesh* UBBIAssetManager::RandomInvaderVisuals(EInvaderType type) const
{
	
	const TArray<TSoftObjectPtr<UInvaderVisualsAsset>>& arrayRef{ invaderVisuals[static_cast<int32>(type)] };
	return arrayRef[FMath::RandHelper(arrayRef.Num())].LoadSynchronous()->GetStaticMesh();
}

void UBBIAssetManager::PostInitialAssetScan()
{
	Super::PostInitialAssetScan();

	/*TArray<FSoftObjectPath> projectileAssetsPaths;
	if (GetPrimaryAssetPathList(UProjectileDataAsset::assetType, projectileAssetsPaths)) {
		for()
	}*/

	projectileAssetsLoadHandle = LoadPrimaryAssetsWithType(UProjectileDataAsset::assetType, {},
		FStreamableDelegate::CreateUObject(this, &UBBIAssetManager::OnProjectileDataAssetsLoaded));

	asteroidMeshSetsLoadHandle = LoadPrimaryAssetsWithType(UAsteroidMeshSetAsset::assetType, {},
		FStreamableDelegate::CreateUObject(this, &UBBIAssetManager::OnAsteroidMeshSetsLoaded));

	invaderVisualsLoadHandle = LoadPrimaryAssetsWithType(UInvaderVisualsAsset::assetType, {},
		FStreamableDelegate::CreateUObject(this, &UBBIAssetManager::OnInvaderVisualsLoaded));
}

void UBBIAssetManager::OnDataFailureDetected(bool bIsCritical, const FText& errorText)
{
	//TODO
}

void UBBIAssetManager::LoadProcessUnloadData(TSharedPtr<FStreamableHandle>& handle, 
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
			if (!processFunc(asset)) {
				//some error
			}

			auto id{ asset->GetPrimaryAssetId() };
			if (id.IsValid()) {
				assetsToUnload.Add(id);
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

void UBBIAssetManager::OnProjectileDataAssetsLoaded()
{
	auto processor = [this](UObject* object) {
		if (auto* projectileData{ ExactCast<UProjectileDataAsset>(object) }) {
			for (int32 i{ 1 }; i < static_cast<int32>(EShooterType::EST_MAX); ++i) {
				if (projectileData->IsToShooter(static_cast<EShooterType>(i))) {
					projectileDataAssets[i - 1].Emplace(projectileData);
				}
			}
			return true;
		}
		else {
			return false;
		}
	};

	LoadProcessUnloadData(projectileAssetsLoadHandle, processor);
}

bool UBBIAssetManager::IsProjectilesDataSufficient()
{
	return true;
}

void UBBIAssetManager::OnAsteroidMeshSetsLoaded()
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

void UBBIAssetManager::OnInvaderVisualsLoaded()
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
