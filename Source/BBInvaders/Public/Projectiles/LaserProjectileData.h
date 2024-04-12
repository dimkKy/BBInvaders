// by Dmitry Kolontay

#pragma once

#include "CoreMinimal.h"
#include "Projectiles/ProjectileData.h"
#include "LaserProjectileData.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class BBINVADERS_API ULaserProjectileData : public UProjectileData
{
	GENERATED_BODY()
public:
	virtual void OnShooting(const FVector& from, const FVector& to, AActor* owner, AActor* target) const override;
};
