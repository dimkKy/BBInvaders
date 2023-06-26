// by Dmitry Kolontay


#include "Player/BBInvadersPlayerState.h"

ABBInvadersPlayerState::ABBInvadersPlayerState() :
	money{100}, approvalRating{100.f}
{
}

int32 ABBInvadersPlayerState::GetMoneyCount() const
{
	return money;
}

void ABBInvadersPlayerState::ChangeMoneyCount(int32 term)
{
}

float ABBInvadersPlayerState::GetApprovalRating() const
{
	return approvalRating;
}

void ABBInvadersPlayerState::ChangeApprovalRating(float term)
{
}
