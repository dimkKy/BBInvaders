// by Dmitry Kolontay

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "CoreSystems/Shooter.h"
#include "ProjectileData.generated.h"

class ABBIPlayerState;

DECLARE_DELEGATE(FStreamableDelegate);

//enum class EShooterType : uint8;

/**
 * 
 */
UCLASS(Abstract, NotBlueprintable)
class BBINVADERS_API UProjectileData : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:
	UProjectileData();

	virtual FPrimaryAssetId GetPrimaryAssetId() const override;

	void PreLoadAsync(bool bLoadMesh = false, FStreamableDelegate onIconLoaded = {});

	static inline const FPrimaryAssetType assetType{ "ProjectileData" };

	virtual void OnShooting()
		PURE_VIRTUAL(UProjectileData::OnShooting, return; );

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly)
		FText visibleName{};

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly)
		FText description{};

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly)
		float baseCost{ 0.f };

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly)
		TSoftObjectPtr<UTexture2D> icon;

	bool IsToShooter(EShooterType type) const;

	//ufunction? BlueprintNativeEvent
	virtual bool IsAvailableToPlayer(const ABBIPlayerState* state = nullptr) const;

#if WITH_EDITOR
	virtual EDataValidationResult IsDataValid(FDataValidationContext& context) const override;
#endif
protected:
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, meta = (Bitmask, BitmaskEnum = EShooterType))
		uint8 shooterTypes{ 0 };
};
