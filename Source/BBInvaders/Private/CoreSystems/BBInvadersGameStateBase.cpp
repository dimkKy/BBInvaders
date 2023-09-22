// by Dmitry Kolontay


#include "CoreSystems/BBInvadersGameStateBase.h"
#include "CoreSystems/OutOfAreaActorTracker.h"
#include "Player/PlayerPawn.h"
#include "BBInvadersUtils.h"

FPlayAreaInfo::FPlayAreaInfo() :
    center{ 0. }, forward{ 0. },
    up{ 0. }, halfSize{ 0. }
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

const AActor* ABBInvadersGameStateBase::GetCenterActor() const
{
    return cachedCenter.Get();
}

FVector ABBInvadersGameStateBase::GetUpVector() const
{
    check(mapInfo);
    return mapInfo.up;
}

FVector ABBInvadersGameStateBase::GetCenter() const
{
    return mapInfo.center;
}

FVector ABBInvadersGameStateBase::CalcRandOutOfBoundsPos(double objectRadius) const
{
    check(mapInfo);
    
    double angle{ BBInvadersUtils::RandomAngle_Double() };
    return mapInfo.forward.RotateAngleAxis(angle, mapInfo.up).GetSafeNormal()
        * (mapInfo.halfSize.Size2D() + objectRadius);
}

float ABBInvadersGameStateBase::GetCurrentInflation() const
{
    return currentInflation;
}

APawn* ABBInvadersGameStateBase::Refresh()
{
    UWorld* world{ GetWorld() };
    APlayerPawn* pawn{ BBInvadersUtils::GetFirstActor<APlayerPawn>(world) };
    check(world && pawn);

    SetMapInfo(*pawn, pawn->CalcMapHalfSize());

    AOutOfAreaActorTracker* tracker{
        BBInvadersUtils::GetFirstActor<AOutOfAreaActorTracker>(world) };

    if (tracker) {
        tracker->SetTrackArea(pawn->GetActorTransform(), mapInfo.halfSize);
    }
    else {
        FActorSpawnParameters params;
        params.SpawnCollisionHandlingOverride =
            ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

        tracker = world->SpawnActor<AOutOfAreaActorTracker>(
            AOutOfAreaActorTracker::StaticClass(), pawn->GetActorTransform(), params);
    }
    return pawn;
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

