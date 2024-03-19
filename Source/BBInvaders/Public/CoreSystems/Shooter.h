// by Dmitry Kolontay

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Shooter.generated.h"

//UHT does not see it for some reason
UENUM(BlueprintType, meta = (Bitflags, UseEnumValuesAsMaskValuesInEditor = "true"))
enum class EShooterType : uint8
{
	EST_None UMETA(Hidden),
	/***/
	EST_Player,
	/**Player and menu pawn*/
	EST_MenuPawn,
	/***/
	EST_Invader,
	/*Both invaders**/
	EST_AdvancedInvader,
	EST_MAX	UMETA(Hidden),
};
ENUM_CLASS_FLAGS(EShooterType);

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

	virtual EShooterType GetShooterType() const
		PURE_VIRTUAL(IShooter::GetShooterType, return EShooterType::EST_MAX; );
};
