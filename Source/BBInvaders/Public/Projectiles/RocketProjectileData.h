// by Dmitry Kolontay

#pragma once

#include "CoreMinimal.h"
#include "Projectiles/ProjectileData.h"
#include "RocketProjectileData.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class BBINVADERS_API URocketProjectileData : public UProjectileData
{
	GENERATED_BODY()
public:

UPROPERTY(EditInstanceOnly, BlueprintReadOnly)
	float initialSpeed{ 0.f };

UPROPERTY(EditInstanceOnly, BlueprintReadOnly)
	float maxSpeed{ 100 };

UPROPERTY(EditInstanceOnly, BlueprintReadOnly)
	bool bIsHoming{ false };


UPROPERTY(EditInstanceOnly, BlueprintReadOnly)
	TSoftObjectPtr<UStaticMesh> bodyMesh;

#if WITH_EDITOR
	virtual EDataValidationResult IsDataValid(FDataValidationContext& context) const override;
#endif
};
