// by Dmitry Kolontay

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "CoreSystems/Shooter.h"
//shorten to invaderType.h
#include "Environment/Invader.h"
#include "BBInvadersAssetManager.generated.h"

class UProjectileDataAsset;
class UAsteroidMeshSetAsset;
class UInvaderVisualsAsset;
class UStaticMesh;

/**
 * 
 */
UCLASS(Blueprintable)
class BBINVADERS_API UBBInvadersAssetManager : public UAssetManager
{
	GENERATED_BODY()

public:

	virtual void StartInitialLoading() override;
	virtual void FinishInitialLoading() override;
	
	static UBBInvadersAssetManager& Get();

	int32 GetProjectilesAvailableToUserType(EShooterType userType, 
		TArray<TSoftObjectPtr<UProjectileDataAsset>>& outArray) const;

	//redo
	TSoftObjectPtr<UProjectileDataAsset> GetRandomProjectilesAvailableToUserType(EShooterType userType) const;

	TSoftObjectPtr<UAsteroidMeshSetAsset> GetRandomAsteroidMeshSet() const;

	//redo
	UStaticMesh* GetRandomInvaderMesh(EInvaderType type) const;

protected:
	virtual void PostInitialAssetScan() override;

	void OnDataFailureDetected(bool bIsCritical, const FText& errorText);

	void LoadProcessUnloadData(TSharedPtr<FStreamableHandle>& handle, TFunction<bool(UObject*)>&& processFunc, bool bForceGC = false);
	
	TArray <TSoftObjectPtr<UProjectileDataAsset>> projectileDataAssets[static_cast<int32>(EShooterType::EST_MAX) + 1];
	//TArray <FSoftObjectPath> projectileDataAssets[static_cast<int32>(EShooterType::EST_MAX) + 1];
	TSharedPtr<FStreamableHandle> projectileAssetsLoadHandle;
	void OnProjectileDataAssetsLoaded();
	bool IsProjectilesDataSufficient();

	TArray<TSoftObjectPtr<UAsteroidMeshSetAsset>> asteroidMeshSets;
	TSharedPtr<FStreamableHandle> asteroidMeshSetsLoadHandle;
	void OnAsteroidMeshSetsLoaded();

	TArray <TSoftObjectPtr<UInvaderVisualsAsset>> invaderVisuals[static_cast<int32>(EInvaderType::EIT_MAX)];
	TSharedPtr<FStreamableHandle> invaderVisualsLoadHandle;
	void OnInvaderVisualsLoaded();
};
