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

	UE_NODISCARD static ThisClass* SpawnOrbit(UWorld& w, const FTransform& transform,
		float radius, int32 invaderNum = 0, bool bZeroRotationSpeed = false);

	void ChangeRotationSpeed(bool bZero);
	static void SetShrinkingSpeed(float speed);

	void Shrink(double distance);
	void InitWithInvaders(double newRadius, bool bAdjustRadius = true);

	UE_NODISCARD static int32 CalcMaxInvadersNum(double invaderRadius, double orbitRadius);

	UE_NODISCARD double GetOuterRadius(double offsetMultiplier = 2.) const;
	UE_NODISCARD int32 GetInvadersNum() const;
	
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
		double radius;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly)
		double invaderRadius;

	static double shrinkingSpeed;
	static float shrinkingStartDelay;

	double minRadius;

	static TArray<FVector> CalcRadiusVectors(int32 size, double length = 1., double offsetAngle = 0.);

	static constexpr float maxRotationSpeed{ 25.f };
	static constexpr std::pair<double, int> invaderNumLimit{ 0.1, 32 };
};
