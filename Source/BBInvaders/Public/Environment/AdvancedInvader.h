// by Dmitry Kolontay

#pragma once

#include "CoreMinimal.h"
#include "Environment/Invader.h"
#include "AdvancedInvader.generated.h"

class USpringArmComponent;

/**
 * 
 */
UCLASS()
class BBINVADERS_API AAdvancedInvader : public AInvader
{
	GENERATED_BODY()

public:
	AAdvancedInvader();
	virtual void Tick(float DeltaTime) override;		

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		USpringArmComponent* orbit;

	float rotationSpeed = 0.f;
	float descensionSpeed = 0.f;

	static const FVector2D rotationSpeedRange;
	static const FVector2D descensionSpeedRange;
};
