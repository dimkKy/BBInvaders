// by Dmitry Kolontay

#pragma once

#include "CoreMinimal.h"
#include "Environment/Invader.h"
#include "AdvancedInvader.generated.h"

class USpringArmComponent;

struct FTargetInfo 
{
	bool bAimed;
	FVector location;
	FVector up;

	FTargetInfo();
	void SetTarget(const AActor& actor);
	explicit operator bool() const;
};

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

	virtual float GetOnPlanetCollisionDamage() const override;

	void SetTarget(const AActor& actor);

	virtual int32 GetOnKillBounty() const override;

protected:
	virtual void BeginPlay() override;

	void RotateMoveToTarget(float deltatime = 0.f);
	/*UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		USpringArmComponent* orbit;*/

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly)
		float rotationSpeed;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly)
		float descensionSpeed;

	FTargetInfo target;

	static constexpr std::pair<float, float> rotationSpeedRange = { 25.f, 40.f };
	static constexpr std::pair<float, float> descensionSpeedRange = { 100.f, 125.f };
};
