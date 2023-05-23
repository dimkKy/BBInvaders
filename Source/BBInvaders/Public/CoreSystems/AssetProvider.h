// by Dmitry Kolontay

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "Environment/Asteroid.h"
#include "AssetProvider.generated.h"


class ABBInvadersProjectile;

/**
 * 
 */
UCLASS(Blueprintable)
class BBINVADERS_API UAssetProvider : public UWorldSubsystem
{
	GENERATED_BODY()
public:
	//UAssetProvider();
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		TSubclassOf<ABBInvadersProjectile> projectileClass;
	//TSoftClassPtr<ABBInvadersProjectile> projectileClass;

	UPROPERTY(EditDefaultsOnly)
		UStaticMesh* asteroidMeshes[EAS_MAX];
	//TSoftObjectPtr<UStaticMesh> asteroidMeshes[EAS_MAX];

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		UStaticMesh* invaderMesh;

#if WITH_EDITOR
	virtual EDataValidationResult IsDataValid(TArray<FText>& ValidationErrors) override;
#endif
};
