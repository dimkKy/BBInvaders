// by Dmitry Kolontay


#include "CoreSystems/BBInvadersGameStateBase.h"
#include "BBInvadersUtils.h"

FPlayAreaInfo::FPlayAreaInfo() :
    center{ 0.f }, forward{ 0.f },
    up{ 0.f }, halfSize{ 0.f }
{

}

FPlayAreaInfo::operator bool() const
{
    return !halfSize.IsZero();
}

//---------

ABBInvadersGameStateBase::ABBInvadersGameStateBase() :
    mapInfo{}, cachedCenter{ nullptr }, currentInflation{ 1.f }
{

}

FPlayAreaInfo ABBInvadersGameStateBase::GetMapInfo() const
{
    return mapInfo;
}

const AActor* ABBInvadersGameStateBase::GetCenter() const
{
    return cachedCenter.Get();
}

FVector ABBInvadersGameStateBase::CalcRandOutOfBoundsPos(float objectRadius) const
{
    check(mapInfo);
    
    float angle{ BBInvadersUtils::RandomAngle() };
    return mapInfo.forward.RotateAngleAxis(angle, mapInfo.up).GetSafeNormal()
        * (mapInfo.halfSize.Size2D() + objectRadius);
}

float ABBInvadersGameStateBase::GetCurrentInflation() const
{
    return currentInflation;
}

void ABBInvadersGameStateBase::SetMapInfo(const AActor& center, const FVector& halfSize)
{
    check(IsValid(&center));
    cachedCenter = MakeWeakObjectPtr(&center);

    return SetMapInfo(center.GetActorLocation(), 
        center.GetActorForwardVector(), center.GetActorUpVector(), halfSize);
}

void ABBInvadersGameStateBase::SetMapInfo(
    const FVector& center, const FVector& forward, const FVector& up, const FVector& halfSize)
{
    mapInfo.center = center;
    mapInfo.forward = forward;
    mapInfo.up = up;
    mapInfo.halfSize = halfSize;
}

