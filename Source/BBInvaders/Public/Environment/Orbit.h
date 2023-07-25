// by Dmitry Kolontay

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Orbit.generated.h"

class USplineComponent;
class URotatingMovementComponent;
class AInvader;

// Rewrite as independent carrier
UCLASS()
class BBINVADERS_API AOrbit : public AActor
{
	GENERATED_BODY()
	
public:
	AOrbit();
	virtual void Tick(float DeltaTime) override;

	void SetRotationSpeed(bool bRandom, float speed = 0.f);
	static void SetShrinkingSpeed(float speed);

	void Shrink(float distance);
	void InitWithInvaders(float newRadius, bool bAdjustRadius = true);

	UE_NODISCARD static int CalcMaxInvadersNum(float invaderRadius, float orbitRadius);

	UE_NODISCARD float GetOuterRadius(float offsetMultiplier = 2.f) const;
	UE_NODISCARD int GetInvadersNum() const;
	
	void Shoot();
protected:
	virtual void BeginPlay() override;

	UFUNCTION()
		void OnInvaderDestroyed(AInvader* invader);

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
		USceneComponent* body;

	//do I need rotator?
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
		URotatingMovementComponent* rotator;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly)
		TArray<AInvader*> invaders;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly)
		float radius;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly)
		float invaderRadius;

	static float shrinkingSpeed;
	static float shrinkingStartDelay;

	float minRadius;

	static TArray<FVector> CalcRadiusVectors(int32 size, float length = 1.f, float offsetAngle = 0.f);

	static constexpr float maxRotationSpeed{ 25.f };
	static constexpr std::pair<float, int> invaderNumLimit{ 0.1f, 32 };
};
