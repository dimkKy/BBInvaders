// by Dmitry Kolontay


#include "Environment/FactoryInfoBase.h"

UFactoryInfoBase::UFactoryInfoBase()
{

}

FText UFactoryInfoBase::GetDisplayName() const
{
    return displayName;
}

FText UFactoryInfoBase::GetDescription() const
{
    return description;
}

int32 UFactoryInfoBase::GetConstructionCost(int32 tier) const
{
    return 0;
}

float UFactoryInfoBase::GetConstructionTime(int32 tier) const
{
    return 0;
}

int32 UFactoryInfoBase::GetIncome(int32 tier) const
{
    return int32();
}
