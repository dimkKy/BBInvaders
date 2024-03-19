// by Dmitry Kolontay

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Orbit.generated.h"

class USplineComponent;
class AInvader;

DECLARE_DELEGATE_OneParam(FNotifyOrbitCleared, AOrbit*);


// Rewrite as independent carrier
UCLASS()
class BBINVADERS_API AOrbit : public AActor
{
	GENERATED_BODY()
	
public:
	AOrbit();
	virtual void Tick(float DeltaTime) override;

	UE_NODISCARD static AOrbit* SpawnOrbit(UWorld& w, const FTransform& transform,
		float radius, int32 invaderNum = 0, bool bNoRotation = false,
		double offsetAngle = 0., double sectorAngle = 2. * UE_DOUBLE_PI);

	void ChangeRotationSpeed(bool bZero);

	double RequestShrink(double distance);
	void InitWithInvaders(int32 invaderNum, double newRadius, bool bAdjustRadius = true,
		double offsetAngle = 0., double sectorAngle = 2. * UE_DOUBLE_PI);

	UE_NODISCARD static int32 CalcMaxInvadersNum(double invaderRadius, double orbitRadius);

	UE_NODISCARD double GetOuterRadius(double sizeMultiplier = 2.) const;
	UE_NODISCARD double GetInnerRadius(double sizeMultiplier = 2.) const;

	UE_NODISCARD int32 GetInvadersNum() const;
	
	void Shoot();

	FNotifyOrbitCleared onClearedDelegate;
protected:
	virtual void BeginPlay() override;

	UFUNCTION()
		void OnInvaderDestroyed(AInvader* invader);

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
		USceneComponent* body;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly)
		TArray<AInvader*> invaders;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly)
		double radius;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly)
		double invaderRadius;

	static float shrinkingStartDelay;

	double minRadius;

	static TArray<FVector> CalcRadiusVectors(int32 size, double length = 1., 
		double offsetAngle = 0., double sectorAngle = 2.* UE_DOUBLE_PI);

	static constexpr float maxRotationSpeed{ 25.f };
	static constexpr std::pair<double, int> invaderNumLimit{ 0.1, 32 };
};
