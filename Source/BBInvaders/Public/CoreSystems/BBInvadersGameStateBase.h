// by Dmitry Kolontay

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "BBInvadersGameStateBase.generated.h"

class UProjectileDataAsset;

struct FPlayAreaInfo {	
	FVector center;
	FVector forward;
	FVector up;
	FVector halfSize;

	FPlayAreaInfo();
	explicit operator bool() const;
};

/**
 * 
 */
UCLASS()
class BBINVADERS_API ABBInvadersGameStateBase : public AGameStateBase
{
	GENERATED_BODY()

public:
	ABBInvadersGameStateBase();

	FVector CalcRandOutOfBoundsPos(double objectRadius) const;

	FPlayAreaInfo GetMapInfo() const;
	FVector GetUpVector() const;
	FVector GetCenter() const;

	const AActor* GetCenterActor() const;

	UE_NODISCARD float GetCurrentInflation() const;

	UE_NODISCARD bool IsProjectileBasekit(const UProjectileDataAsset& projectile) const;
	UE_NODISCARD bool IsProjectileBasekit(const TSoftObjectPtr<UProjectileDataAsset>& projectile) const;
	UE_NODISCARD int32 GetProjectilesBasekitSize() const;

#if WITH_EDITOR
	virtual EDataValidationResult IsDataValid(FDataValidationContext& context) const override;
#endif
protected:
	APawn* Refresh();

	void SetMapInfo(const AActor& center, const FVector& halfSize);
	void SetMapInfo(const FVector& center, const FVector& forward,
		const FVector& up, const FVector& halfSize);

	UPROPERTY(EditDefaultsOnly)
		TSet<TSoftObjectPtr<UProjectileDataAsset>> basicKitProjectiles;

	FPlayAreaInfo mapInfo;
	TWeakObjectPtr<const AActor> cachedCenter;

	float currentInflation;

	friend class ABBInvadersGameModeBase;
};
