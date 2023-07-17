// by Dmitry Kolontay

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "Environment/Asteroid.h"
#include "AssetProvider.generated.h"


class ABBInvadersProjectile;

/**
 * TO ASSET MANAGER!
 */
UCLASS(Blueprintable)
class BBINVADERS_API UAssetProvider : public UWorldSubsystem
{
	GENERATED_BODY()
public:
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;

	UStaticMesh* GetAsteroidMesh(EAsteroidSize size) const;
	UStaticMesh* GetInvaderMesh() const;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		TSubclassOf<ABBInvadersProjectile> projectileClass;
	//TSoftClassPtr<ABBInvadersProjectile> projectileClass;


	

#if WITH_EDITOR
	virtual EDataValidationResult IsDataValid(TArray<FText>& ValidationErrors) override;
#endif

protected:
	UPROPERTY(EditDefaultsOnly)
		UStaticMesh* asteroidMeshes[static_cast<int32>(EAsteroidSize::EAS_MAX)];

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		TSoftObjectPtr<UStaticMesh> invaderMesh;
};
