// by Dmitry Kolontay

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
//#include <array>
#include "Orbit.generated.h"

class USplineComponent;
class URotatingMovementComponent;
class AInvader;

/*namespace OrbitHelper {
	
	template<int pointsCount>
	constexpr std::array<std::pair<float, float>, pointsCount> CalculateRadiusVectors_Static() {
		std::array<std::pair<float, float>, pointsCount> out;
		for (auto i{ 0 }; i < pointsCount; ++i) {
			out[i] = { FMath::Sin(1.f), FMath::Cos(1.f) };
		}
		return out;
	}
}*/

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

	UE_NODISCARD float GetOuterRadius() const;
	UE_NODISCARD int GetInvadersNum() const;
	
	void Shoot();
protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
		URotatingMovementComponent* rotator;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly)
		TArray<AInvader*> invaders;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly)
		float radius;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly)
		float invaderRadius;

	static float shrinkingSpeed;

	float minRadius;
	static constexpr int32 splineCount = 3;

	//template<int size = splineCount>
	//static std::array<FVector2D, size> CalculateRadiusVectors_Static();

	static TArray<FVector> CalcRadiusVectors(int32 size, float length = 1.f);

	//static const std::array<FVector2D, splineCount> orbitPointsRadiusVectors;

	//static constexpr std::array<std::pair<float, float>, splineCount>
		//orbitPointsRadiusVectors = OrbitHelper::CalculateRadiusVectors_Static<splineCount>();
	/*UPROPERTY(EditInstanceOnly, BlueprintReadWrite )
		USplineComponent* spline;*/
	
	//static constexpr float maxShrinkingSpeed = 65.f;

	static constexpr float maxRotationSpeed = 65.f;
	static constexpr std::pair<float, int> invaderNumLimit = {0.05f, 62};
	//USplineComponent* splines[splineCount];
};

/*template<int size>
inline std::array<FVector2D, size> AOrbit::CalculateRadiusVectors_Static(){
	static_assert(size >= 3, "at least 3 points");
	std::array<FVector2D, size> out;
	//FMath::si
	const float angle = PI * 2 / size;
	for (SIZE_T i{ 0 }; i < size; ++i) {
		out[i] = { FMath::Sin(angle * i), FMath::Cos(angle * i) };
	}
	return out;
}*/