// by Dmitry Kolontay

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "CoreSystems/Shooter.h"
#include "ProjectileDataAsset.generated.h"

DECLARE_DELEGATE(FStreamableDelegate);

//enum class EShooterType : uint8;

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

	void PreLoadAsync(bool bLoadMesh = false, FStreamableDelegate onIconLoaded = {});

	static const FPrimaryAssetType assetType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		EShooterType userType;

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
	virtual EDataValidationResult IsDataValid(FDataValidationContext& context) const override;
#endif
};
