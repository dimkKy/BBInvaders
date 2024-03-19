// by Dmitry Kolontay

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "BBInvadersGameStateBase.generated.h"

class UProjectileDataAsset;

struct FPlayAreaInfo {	
	explicit operator bool() const;

	const FVector& Center() const { return center; };
	const FVector& Forward() const { return forward; };
	const FVector& GetUp() const { return up; };
	double Radius() const { return radius; };
	const FVector& HalfSize() const { return halfSize; };
	const FTransform& DefaultTransform() const { return defaultTransform; };

	void Set(const FVector& cntr, const FVector& frwrd, 
		const FVector& _up, const FVector& hlfSz);

protected:
	FVector center{ 0. };
	FVector forward{ 0. };
	FVector up{ 0. };

	FTransform defaultTransform{};
	FVector halfSize{ 0. };
	double radius{ 0. };

	void SetHalfSize(const FVector& hS);
	void RecalcTransform();
};

/**
 * 
 */
UCLASS()
class BBINVADERS_API ABBIGameStateBase : public AGameStateBase
{
	GENERATED_BODY()

public:
	static ABBIGameStateBase* Get(const UWorld* world);

	FVector CalcRandOutOfBoundsPos(double objectRadius) const;

	const FPlayAreaInfo& GetMapInfo() const;
	//FVector GetUpVector() const;
	//FVector GetCenter() const;
	//double GetMapRadius() const;

	const AActor* GetCenterActor() const;

	UE_NODISCARD float GetCurrentInflation() const;;

#if WITH_EDITOR
	virtual EDataValidationResult IsDataValid(FDataValidationContext& context) const override;
#endif
protected:
	APawn* Refresh();

	void SetMapInfo(const AActor& center, const FVector& halfSize);
	void SetMapInfo(const FVector& center, const FVector& forward,
		const FVector& up, const FVector& halfSize);

	FPlayAreaInfo mapInfo{};
	TWeakObjectPtr<const AActor> cachedCenter{ nullptr };

	float currentInflation{ 1.f };

	friend class ABBIGameModeBase;
};
