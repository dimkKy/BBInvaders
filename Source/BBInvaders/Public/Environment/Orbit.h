// by Dmitry Kolontay

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <array>
#include "Orbit.generated.h"

class USplineComponent;
class URotatingMovementComponent;
class UStaticMesh;
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

	void Init(UStaticMesh& invaderBody, int32 invaderCount, float newRadius);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
		URotatingMovementComponent* rotator;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		TArray<AInvader*> invaders;

	int32 activeInvaderCount;
	float radius;


	static constexpr int32 splineCount = 3;


	template<int size = splineCount>
	static std::array<FVector2D, size> CalculateRadiusVectors_Static();

	static TArray<FVector> CalcRadiusVectors(int32 size);

	static const std::array<FVector2D, splineCount> orbitPointsRadiusVectors;

	//static constexpr std::array<std::pair<float, float>, splineCount>
		//orbitPointsRadiusVectors = OrbitHelper::CalculateRadiusVectors_Static<splineCount>();
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite )
		USplineComponent* spline;
	
	//USplineComponent* splines[splineCount];
};

template<int size>
inline std::array<FVector2D, size> AOrbit::CalculateRadiusVectors_Static(){
	static_assert(size >= 3, "at least 3 points");
	std::array<FVector2D, size> out;
	//FMath::si
	const float angle = PI * 2 / size;
	for (SIZE_T i{ 0 }; i < size; ++i) {
		out[i] = { FMath::Sin(angle * i), FMath::Cos(angle * i) };
	}
	return out;
}