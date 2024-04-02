// by Dmitry Kolontay


#include "Player/BBIPlayerState.h"
#include "Environment/FactoryInfoBase.h"

FFactoryInstanceInfo::FFactoryInstanceInfo(const UFactoryInfoBase& i) :
	info{ i }, tier{ 0 }, bIsUnderConstruction{ true },
	buildingTimeLeft{ i.GetConstructionTime() }
{

}

const UFactoryInfoBase& FFactoryInstanceInfo::GetFactoryInfo() const
{
	return info;
}

bool FFactoryInstanceInfo::IsUnderConstruction() const
{
	return bIsUnderConstruction;
}

float FFactoryInstanceInfo::GetBuildingTimeLeft() const
{
	return buildingTimeLeft;
}

int32 FFactoryInstanceInfo::GetTier() const
{
	return tier;
}

float FFactoryInstanceInfo::StartUpgrading()
{
	bIsUnderConstruction = true;
	buildingTimeLeft = info.GetConstructionTime(tier);
	return buildingTimeLeft;
}

float FFactoryInstanceInfo::ProceedUpgrading(float deltaTime)
{
	if (bIsUnderConstruction) {
		buildingTimeLeft -= deltaTime;
		if (buildingTimeLeft <= 0.f) {
			FinishConstruction();
		}
	}
	return buildingTimeLeft;
}

void FFactoryInstanceInfo::FinishConstruction()
{
	tier++;
	bIsUnderConstruction = false;
	buildingTimeLeft = 0.f;
}

///////////////////////////////////////////////////////////////////////////////////

ABBIPlayerState::ABBIPlayerState() :
	money{100}, approvalRating{100.f}
{
}

int32 ABBIPlayerState::GetMoneyCount() const
{
	return money;
}

void ABBIPlayerState::ChangeMoneyCount(int32 term)
{
}

float ABBIPlayerState::GetApprovalRating() const
{
	return approvalRating;
}

void ABBIPlayerState::ChangeApprovalRating(float term)
{
}


