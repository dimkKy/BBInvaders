// by Dmitry Kolontay

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "BBInvadersPlayerState.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnMoneyCountChanged, int32);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnApprovalRatingChanged, int32);

class UFactoryInfoBase;

struct FFactoryInstanceInfo 
{
public:
	FFactoryInstanceInfo(const UFactoryInfoBase& i);

	const UFactoryInfoBase& GetFactoryInfo() const;
	bool IsUnderConstruction() const;
	float GetBuildingTimeLeft() const;
	int32 GetTier() const;

	float StartUpgrading();
	float ProceedUpgrading(float deltaTime);

protected:
	void FinishConstruction();

	const UFactoryInfoBase& info;
	int32 tier;
	bool bIsUnderConstruction;
	float buildingTimeLeft;
};

/**
 * 
 */
UCLASS()
class BBINVADERS_API ABBIPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	ABBIPlayerState();
	
	int32 GetMoneyCount() const;
	float GetApprovalRating() const;

	FOnMoneyCountChanged moneyChangedEvent;
	FOnApprovalRatingChanged ratingChangedEvent;

protected:

	int32 money;
	float approvalRating;
	
	void ChangeMoneyCount(int32 term);
	void ChangeApprovalRating(float term);

	TArray<TArray<FFactoryInstanceInfo>> factories;

	friend class ABBIGameModeBase;


};
