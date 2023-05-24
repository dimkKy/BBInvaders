// by Dmitry Kolontay

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PlanetaryThreatable.generated.h"

UINTERFACE(MinimalAPI)
class UPlanetaryThreatable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class BBINVADERS_API IPlanetaryThreatable
{
	GENERATED_BODY()

public:
	virtual float GetOnPlanetCollisionDamage() const 
		PURE_VIRTUAL(IPlanetaryThreatable::GetOnPlanetCollisionDamage, return 0.f; );
};
