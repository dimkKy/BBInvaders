// by Dmitry Kolontay

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "BBInvadersPlayerState.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnMoneyCountChanged, int32);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnApprovalRatingChanged, int32);

/**
 * 
 */
UCLASS()
class BBINVADERS_API ABBInvadersPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	ABBInvadersPlayerState();
	
	int32 GetMoneyCount() const;
	float GetApprovalRating() const;

	FOnMoneyCountChanged moneyChangetEvent;
	FOnApprovalRatingChanged ratingChangetEvent;

protected:

	int32 money;
	float approvalRating;
	
	void ChangeMoneyCount(int32 term);
	void ChangeApprovalRating(float term);

	friend class ABBInvadersGameModeBase;
};
