// by Dmitry Kolontay

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Engine/DamageEvents.h"
#include "CoreSystems/Shooter.h"
#include "ProjectileData.generated.h"

class ABBIPlayerState;

DECLARE_DELEGATE(FStreamableDelegate);

enum class EDamageType : uint8
{
	EDT_Explosive,
	/**/
	EDT_Kinetic,
	/**/
	EDT_Laser,
	/**/
	//EDT_MAX	UMETA(Hidden),
};

namespace EDT {
	inline constexpr EDamageType Explosive{ EDamageType::EDT_Explosive };
	inline constexpr EDamageType Kinetic{ EDamageType::EDT_Kinetic };
	inline constexpr EDamageType Laser{ EDamageType::EDT_Laser };
}

//USTRUCT()
struct FBBIDamageEvent : public FDamageEvent
{
	//GENERATED_BODY()

	EDamageType type{ EDT::Explosive };
	/** Direction the shot came from. Should be normalized. *
	UPROPERTY()
	FVector_NetQuantizeNormal ShotDirection;

	/** Describes the trace/location that caused this damage *
	UPROPERTY()
	FHitResult HitInfo;*/

	FBBIDamageEvent(EDamageType inType) : type{ inType } {};

	/** ID for this class. NOTE this must be unique for all damage events. */
	static const int32 ClassID{ 10 };

	virtual int32 GetTypeID() const override 
		{ return FBBIDamageEvent::ClassID; };
};

namespace BBIDamageEvents {
	inline const FBBIDamageEvent Explosive{ EDT::Explosive };
	inline const FBBIDamageEvent Kinetic{ EDT::Kinetic };
	inline const FBBIDamageEvent Laser{ EDT::Laser };
}

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

	virtual void OnShooting(const FVector& from, const FVector& to, AActor* owner, AActor* target) const
		PURE_VIRTUAL(UProjectileData::OnShooting, return; );

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly)
		FText visibleName{};

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly)
		FText description{};

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly)
		float baseCost{ 0.f };

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly)
		float baseDamage{ 0.f };

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
