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

	float GetCurrentInflation() const;

protected:
	APawn* Refresh();

	FPlayAreaInfo mapInfo;
	TWeakObjectPtr<const AActor> cachedCenter;

	void SetMapInfo(const AActor& center, const FVector& halfSize);
	void SetMapInfo(const FVector& center, const FVector& forward,
		const FVector& up, const FVector& halfSize);

	float currentInflation;

	friend class ABBInvadersGameModeBase;
};
