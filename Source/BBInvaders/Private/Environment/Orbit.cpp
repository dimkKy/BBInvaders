// by Dmitry Kolontay


#include "Environment/Orbit.h"
#include "GameFramework/RotatingMovementComponent.h"
#include "Components/HierarchicalInstancedStaticMeshComponent.h"
#include "Components/SplineComponent.h"
#include "BBInvadersUtils.h"

const std::array<FVector2D, AOrbit::splineCount> 
	AOrbit::orbitPointsRadiusVectors = AOrbit::CalculateRadiusVectors_Static();

AOrbit::AOrbit() :
	rotator{ CreateDefaultSubobject<URotatingMovementComponent>("rotator") },
	//invaders{ CreateDefaultSubobject<UHierarchicalInstancedStaticMeshComponent>("invaders") },
	spline{ CreateDefaultSubobject<USplineComponent>("spline") },
	activeInvaderCount{ 0 }, radius{ 1.f }
{
	PrimaryActorTick.bCanEverTick = false;

	rotator->bUpdateOnlyIfRendered = true;

	//invaders->bMultiBodyOverlap = true;
	//invaders->SetCollisionObjectType(BBInvadersUtils::ECC_Invader);
	//invaders->SetCollisionObjectType

	//invaders->UpdateInstanceTransform

	spline->SetClosedLoop(true, false);

	/*for (SIZE_T i{ 0 }; i < splineCount; ++i) {
		spline.cl
		spline->AddSplinePointAtIndex()
		splines[i] = CreateDefaultSubobject<USplineComponent>("spline" + i);
		splines[i]->AddPoint
	}*/

	spline->UpdateSpline();

	//GetWorld()->SpawnActor()
}

void AOrbit::BeginPlay()
{
	Super::BeginPlay();
	
}

TArray<FVector> AOrbit::CalcRadiusVectors(int32 size)
{
	check(size > 0);

	TArray<FVector> out;
	out.Reserve(size);

	const float angle{ PI / (2 * size) };
	for (SIZE_T i{ 0 }; i < size; ++i) {
		out[i] = { FMath::Sin(angle * i), FMath::Cos(angle * i), 0.f };
	}
	return out;
}

void AOrbit::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AOrbit::Init(UStaticMesh& invaderBody, int32 invaderCount, float newRadius)
{
	/*if (invaders->GetInstanceCount() != invaderCount) {
		invaders->ClearInstances();
	}
	
	activeInvaderCount = invaderCount;
	radius = newRadius;

	auto radiusVectors{ CalcRadiusVectors(invaderCount) };

	for (SIZE_T i{ 0 }; i < invaderCount; ++i) {
		invaders->AddInstance(
			FTransform{ FVector{radius * radiusVectors[i]} } );

	}*/

}

