// by Dmitry Kolontay


#include "CoreSystems/BBInvadersGameStateBase.h"
#include "BBInvadersUtils.h"


FPlayAreaInfo ABBInvadersGameStateBase::GetMapInfo() const
{
    return mapInfo;
}

FVector ABBInvadersGameStateBase::CalcRandOutOfBoundsPos(float objectRadius) const
{
    check(mapInfo);
    
    float angle{ BBInvadersUtils::RandomAngle() };
    return mapInfo.forward.RotateAngleAxis(angle, mapInfo.up).GetSafeNormal()
        * mapInfo.halfSize.Size2D() + objectRadius;
}

void ABBInvadersGameStateBase::SetMapInfo(const AActor& center, const FVector& halfSize)
{
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
