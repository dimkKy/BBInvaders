// by Dmitry Kolontay

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Environment/Asteroid.h"
#include "AsteroidMeshSet.generated.h"

class UStaticMesh;
struct FStreamableHandle;

/**
 * 
 */
UCLASS()
class BBINVADERS_API UAsteroidMeshSet : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:
	virtual FPrimaryAssetId GetPrimaryAssetId() const override;

	static const FPrimaryAssetType assetType;

	UStaticMesh* GetStaticMesh(EAsteroidSize size) const;

#if WITH_EDITOR
	virtual EDataValidationResult IsDataValid(FDataValidationContext& context) const override;
#endif

protected:
#define EAS_MAX static_cast<uint8>(EAsteroidSize::EAS_MAX)
	UPROPERTY(EditInstanceOnly)
		TSoftObjectPtr<UStaticMesh> asteroidMeshes[EAS_MAX];
#undef EAS_MAX

	TSharedPtr<FStreamableHandle> meshesHanle;
};
