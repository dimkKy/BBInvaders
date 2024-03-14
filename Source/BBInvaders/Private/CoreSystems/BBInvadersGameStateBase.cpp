// by Dmitry Kolontay


#include "CoreSystems/BBInvadersGameStateBase.h"
#include "CoreSystems/OutOfAreaActorTracker.h"
#include "Player/PlayerPawn.h"
#include "CoreSystems/ProjectileDataAsset.h"
#include "BBInvadersUtils.h"
#if WITH_EDITOR
#include "Misc/DataValidation.h"
#endif


void FPlayAreaInfo::SetHalfSize(const FVector& hS)
{
    halfSize = hS;
    radius = halfSize.Size();
}

FPlayAreaInfo::operator bool() const
{
    return !FMath::IsNearlyZero(radius);
}

void FPlayAreaInfo::Set(const FVector& cntr, const FVector& frwrd, const FVector& _up, const FVector& hlfSz)
{
    center = center;
    forward = forward;
    up = _up;
    SetHalfSize(hlfSz);
    RecalcTransform();
}

void FPlayAreaInfo::RecalcTransform()
{
    defaultTransform = 
        { FRotationMatrix::MakeFromXZ(forward, up).ToQuat(), center };
}

const FPlayAreaInfo& ABBInvadersGameStateBase::GetMapInfo() const
{
    return mapInfo;
}

const AActor* ABBInvadersGameStateBase::GetCenterActor() const
{
    return cachedCenter.Get();
}


ABBInvadersGameStateBase* ABBInvadersGameStateBase::Get(const UWorld* world)
{
    check(world);
    return CastChecked<ABBInvadersGameStateBase>(world->GetGameState());
}

FVector ABBInvadersGameStateBase::CalcRandOutOfBoundsPos(double objectRadius) const
{
    check(mapInfo);
    
    double angle{ BBInvadersUtils::RandomAngle_Double() };
    return mapInfo.Forward().RotateAngleAxis(angle, mapInfo.GetUp()).GetSafeNormal()
        * (mapInfo.Radius() + objectRadius);
}

float ABBInvadersGameStateBase::GetCurrentInflation() const
{
    return currentInflation;
}

bool ABBInvadersGameStateBase::IsProjectileBasekit(const UProjectileDataAsset& projectile) const
{
    return IsProjectileBasekit(TSoftObjectPtr<UProjectileDataAsset>{ &projectile });
}

bool ABBInvadersGameStateBase::IsProjectileBasekit(const TSoftObjectPtr<UProjectileDataAsset>& projectile) const
{
    return basicKitProjectiles.Contains(projectile);
}

int32 ABBInvadersGameStateBase::GetProjectilesBasekitSize() const
{
    return basicKitProjectiles.Num();
}

#if WITH_EDITOR
EDataValidationResult ABBInvadersGameStateBase::IsDataValid(FDataValidationContext& context) const
{
    Super::IsDataValid(context);

    if (!basicKitProjectiles.Num()) {
        context.AddError(FText::FromString("empty basic kit"));
    }

    return context.GetNumErrors() > 0 ?
        EDataValidationResult::Invalid : EDataValidationResult::Valid;
}
#endif

APawn* ABBInvadersGameStateBase::Refresh()
{
    UWorld* world{ GetWorld() };
    APlayerPawn* pawn{ BBInvadersUtils::GetFirstActor<APlayerPawn>(world) };
    check(world && pawn);

    SetMapInfo(*pawn, pawn->CalcMapHalfSize());

    AOutOfAreaActorTracker* tracker{
        BBInvadersUtils::GetFirstActor<AOutOfAreaActorTracker>(world) };

    if (tracker) {
        tracker->SetTrackArea(pawn->GetActorTransform(), mapInfo.HalfSize());
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
    mapInfo.Set(center, forward, up, halfSize);
}

