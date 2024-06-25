// by Dmitry Kolontay

#pragma once

#include "CoreMinimal.h"
#include "Projectiles/ProjectileData.h"
#include "KineticProjectileData.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class BBINVADERS_API UKineticProjectileData : public UProjectileData
{
	GENERATED_BODY()
public:
	virtual void OnShooting(const FVector& from, const FVector& to, AActor* owner, AActor* target) const override;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly)
		float initialSpeed{ 100.f };

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly)
		float deceleration{ 0.01f };

	UPROPERTY(EditInstanceOnly)
		TSoftObjectPtr<UStaticMesh> bodyMesh;
};
