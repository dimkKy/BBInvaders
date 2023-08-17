// by Dmitry Kolontay

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Shooter.generated.h"

UENUM(BlueprintType)
enum class EShooterType : uint8
{
	/***/
	EST_PlayerOnly,
	/**Player and menu pawn*/
	EST_Human,
	/***/
	EST_Invader,
	/*Both invaders**/
	EST_AdvancedInvader,
	EST_MAX,
};

UINTERFACE(MinimalAPI)
class UShooter : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class BBINVADERS_API IShooter
{
	GENERATED_BODY()
public:
	virtual void Shoot()
		PURE_VIRTUAL(IShooter::Shoot, return; );

	/*virtual EShooterType GetShooterType() const
		PURE_VIRTUAL(IShooter::GetShooterType, return EShooterType::EST_MAX; );*/
};
