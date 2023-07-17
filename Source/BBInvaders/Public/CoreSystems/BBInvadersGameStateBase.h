// by Dmitry Kolontay

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "BBInvadersGameStateBase.generated.h"

struct FPlayAreaInfo {	

	FVector center;
	FVector forward;
	FVector up;
	FVector halfSize;

	FPlayAreaInfo() :
		center{ 0.f }, forward{ 0.f },
		up{ 0.f }, halfSize{ 0.f } {};

	explicit operator bool() const
	{
		return !halfSize.IsZero();
	}
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

	FVector CalcRandOutOfBoundsPos(float objectRadius) const;

	FPlayAreaInfo GetMapInfo() const;
	const AActor* GetCenter() const;

	float GetCurrentInflation() const;

protected:

	FPlayAreaInfo mapInfo;
	TWeakObjectPtr<const AActor> cachedCenter;

	void SetMapInfo(const AActor& center, const FVector& halfSize);
	void SetMapInfo(const FVector& center, const FVector& forward,
		const FVector& up, const FVector& halfSize);

	float currentInflation;


	friend class ABBInvadersGameModeBase;
};
