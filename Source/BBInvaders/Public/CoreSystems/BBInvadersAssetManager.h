// by Dmitry Kolontay

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "CoreSystems/Shooter.h"
#include "BBInvadersAssetManager.generated.h"

class UProjectileDataAsset;
class UAsteroidMeshSetAsset;

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

	//Redo to async?
	int32 GetProjectilesAvailableToPlayer(TArray<UProjectileDataAsset*>& outArray);

protected:
	virtual void PostInitialAssetScan() override;

	void OnDataFailureDetected(bool bIsCritical, const FText& errorText);
	
	TArray <TSoftObjectPtr<UProjectileDataAsset>> projectileDataAssets[static_cast<int32>(EShooterType::EST_MAX) + 1];
	TSharedPtr<FStreamableHandle> projectileAssetsLoadHandle;

	void OnProjectileDataAssetsLoaded();
	bool IsProjectilesDataSufficient();

	TArray<TSoftObjectPtr<UAsteroidMeshSetAsset>> asteroidMeshSets;
	TSharedPtr<FStreamableHandle> asteroidMeshSetsLoadHandle;

	void OnAsteroidMeshSetsLoaded();
};
