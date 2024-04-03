// by Dmitry Kolontay


#include "CoreSystems/BBIAssetManager.h"
#include "Projectiles/ProjectileData.h"
#include "Environment/AsteroidMeshSet.h"
#include "Environment/InvaderVisuals.h"

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
	TArray<TSoftObjectPtr<UProjectileData>>& outArray) const
{
	check(outArray.Num() == 0);
	int32 userTypeI{ static_cast<int32>(userType) };

	verify(userTypeI > static_cast<int8>(EShooterType::EST_None) 
		&& userTypeI < static_cast<int8>(EShooterType::EST_MAX));

	outArray = projectileDataAssets[userTypeI - 1];
	return outArray.Num();
}

TSoftObjectPtr<UProjectileData> UBBIAssetManager::
	RandomProjectile(EShooterType userType) const
{
	//TODO
	int32 userTypeI{ static_cast<int32>(userType) };

	verify(userTypeI > static_cast<int8>(EShooterType::EST_None)
		&& userTypeI < static_cast<int8>(EShooterType::EST_MAX));
	return TSoftObjectPtr<UProjectileData>();
}

TSoftObjectPtr<UAsteroidMeshSet> UBBIAssetManager::RandomAsteroidMeshSet() const
{
	return asteroidMeshSets[FMath::RandHelper(asteroidMeshSets.Num())];
}

UStaticMesh* UBBIAssetManager::RandomInvaderVisuals(EInvaderType type) const
{
	
	const TArray<TSoftObjectPtr<UInvaderVisuals>>& arrayRef{ invaderVisuals[static_cast<int32>(type)] };
	return arrayRef[FMath::RandHelper(arrayRef.Num())].LoadSynchronous()->GetStaticMesh();
}

void UBBIAssetManager::PostInitialAssetScan()
{
	Super::PostInitialAssetScan();

	/*TArray<FSoftObjectPath> projectileAssetsPaths;
	if (GetPrimaryAssetPathList(UProjectileData::assetType, projectileAssetsPaths)) {
		for()
	}*/

	projectileAssetsLoadHandle = LoadPrimaryAssetsWithType(UProjectileData::assetType, {},
		FStreamableDelegate::CreateUObject(this, &UBBIAssetManager::OnProjectileDataAssetsLoaded));

	asteroidMeshSetsLoadHandle = LoadPrimaryAssetsWithType(UAsteroidMeshSet::assetType, {},
		FStreamableDelegate::CreateUObject(this, &UBBIAssetManager::OnAsteroidMeshSetsLoaded));

	invaderVisualsLoadHandle = LoadPrimaryAssetsWithType(UInvaderVisuals::assetType, {},
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
		if (auto* projectileData{ ExactCast<UProjectileData>(object) }) {
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
		if (auto* projectileData{ ExactCast<UAsteroidMeshSet>(object) }) {
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
		if (auto* invaderVisual{ ExactCast<UInvaderVisuals>(object) }) {
			invaderVisuals[invaderVisual->GetInvaderTypeI()].Emplace(invaderVisual);
			return true;
		}
		else {
			return false;
		}

	};

	LoadProcessUnloadData(invaderVisualsLoadHandle, processor);
}
