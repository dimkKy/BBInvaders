// by Dmitry Kolontay

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Environment/Asteroid.h"
#include "AsteroidMeshSetAsset.generated.h"

class UStaticMesh;
/**
 * 
 */
UCLASS()
class BBINVADERS_API UAsteroidMeshSetAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:
	virtual FPrimaryAssetId GetPrimaryAssetId() const override;

	static const FPrimaryAssetType assetType;

	UStaticMesh* GetStaticMesh(EAsteroidSize size) const;

#if WITH_EDITOR
	virtual EDataValidationResult IsDataValid(TArray<FText>& ValidationErrors) override;
#endif

protected:
#define EAS_MAX static_cast<uint8>(EAsteroidSize::EAS_MAX)
	UPROPERTY(EditInstanceOnly)
		TSoftObjectPtr<UStaticMesh> asteroidMeshes[EAS_MAX];
#undef EAS_MAX
};
