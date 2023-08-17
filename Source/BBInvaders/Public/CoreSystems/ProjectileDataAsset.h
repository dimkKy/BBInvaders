// by Dmitry Kolontay

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
//#include "CoreSystems/Shooter.h"
#include "ProjectileDataAsset.generated.h"

enum class EShooterType : uint8;
enum class EAsteroidSize : uint8;

/**
 * 
 */
UCLASS()
class BBINVADERS_API UProjectileDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:
	UProjectileDataAsset();

	virtual FPrimaryAssetId GetPrimaryAssetId() const override;

	static const FPrimaryAssetType assetType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		EAsteroidSize userType1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		EShooterType userType2;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FText visibleName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float initialSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float maxSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FText description;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		bool bIsHoming;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float baseCost;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		TSoftObjectPtr<UTexture2D> icon;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		TSoftObjectPtr<UStaticMesh> bodyMesh;

#if WITH_EDITOR
	virtual EDataValidationResult IsDataValid(TArray<FText>& ValidationErrors) override;
#endif
};
