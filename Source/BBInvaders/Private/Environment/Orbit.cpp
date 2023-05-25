// by Dmitry Kolontay


#include "Environment/Orbit.h"
#include "GameFramework/RotatingMovementComponent.h"
//#include "Components/HierarchicalInstancedStaticMeshComponent.h"
#include "Components/SplineComponent.h"
//#include "BBInvadersUtils.h"
#include "Environment/Invader.h"
#include "CoreSystems/AssetProvider.h"

#include "BBInvadersUtils.h"

//const std::array<FVector2D, AOrbit::splineCount> 
	//AOrbit::orbitPointsRadiusVectors = AOrbit::CalculateRadiusVectors_Static();


AOrbit::AOrbit() :
	rotator{ CreateDefaultSubobject<URotatingMovementComponent>("rotator") },
	//invaders{ CreateDefaultSubobject<UHierarchicalInstancedStaticMeshComponent>("invaders") },
	//spline{ CreateDefaultSubobject<USplineComponent>("spline") },
	radius{ 100.f }
{
	PrimaryActorTick.bCanEverTick = false;

	rotator->bUpdateOnlyIfRendered = true;
	rotator->RotationRate = FRotator::ZeroRotator;


	//invaders->bMultiBodyOverlap = true;
	//invaders->SetCollisionObjectType(BBInvadersUtils::ECC_Invader);
	//invaders->SetCollisionObjectType
	//invaders->UpdateInstanceTransform

	//spline->SetClosedLoop(true, false);
	//spline->UpdateSpline();
}

void AOrbit::BeginPlay()
{
	Super::BeginPlay();
	check(invaders.Num());
}

TArray<FVector> AOrbit::CalcRadiusVectors(int32 size, float length/* = 1.f*/)
{
	check(size > 0);

	TArray<FVector> out;
	out.Reserve(size);

	const float angle{ PI / (2 * size) };
	for (SIZE_T i{ 0 }; i < size; ++i) {
		out[i] = { FMath::Sin(angle * i), FMath::Cos(angle * i), 0.f };
		out[i] *= length;
	}
	return out;
}

void AOrbit::SetRotationSpeed(bool bRandom, float speed)
{
	if (bRandom) {
		rotator->RotationRate = BBInvadersUtils::unitRotator *
			FMath::RandRange(maxRotationSpeed, maxRotationSpeed);
	}
	else {
		rotator->RotationRate = BBInvadersUtils::unitRotator * 
			FMath::Clamp(speed, -1.f * maxRotationSpeed, maxRotationSpeed);
	}
}

void AOrbit::Shrink(float distance)
{
	radius -= distance;
	for (auto& invader : invaders) {
		invader->AddActorLocalOffset(FVector::ForwardVector * distance);
	}
}

void AOrbit::InitWithInvaders(float newRadius, bool bAdjustRadius/* = true*/)
{
	auto* world{ GetWorld() };
	check(world && newRadius > 0);
	check(invaders.Num() == 0);

	UStaticMesh* invaderMesh{ world->GetSubsystem<UAssetProvider>()->invaderMesh };
	float invaderMeshRadius{ invaderMesh->GetBounds().GetSphere().W };

	radius = newRadius;
	if (bAdjustRadius) {
		radius += invaderMeshRadius;
	}

	int32 newInvaderCount{ FMath::RandRange(1, 
		CalcMaxInvadersNum(invaderMeshRadius, radius)) };

	auto radiusVectors{ CalcRadiusVectors(newInvaderCount, radius) };

	FActorSpawnParameters spawnParams;
	spawnParams.Owner = this;
	spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	FVector thisLocation{ GetActorLocation() };
	FRotator thisRotation{ GetActorRotation() };

	for (SIZE_T i{ 0 }; i < newInvaderCount; ++i) {
		radiusVectors[i] = thisRotation.RotateVector(radiusVectors[i]);

		AInvader* invader{ world->SpawnActor<AInvader>(
			thisLocation + radiusVectors[i],
			FRotationMatrix::MakeFromX(radiusVectors[i] * -1.f).Rotator(), 
			spawnParams) };

		/*world->SpawnActorDeferred<AInvader>(
			thisTransform.GetLocation() + radiusVectors[i],
			FRotationMatrix::MakeFromX(radiusVectors[i] * -1.f).Rotator(),
			spawnParams)*/

		invader->SetMesh(*invaderMesh);
		invader->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);

		invaders.Add(invader);
	}
}

UE_NODISCARD int AOrbit::CalcMaxInvadersNum(float invaderRadius, float orbitRadius)
{	
	check(!FMath::IsNearlyZero(invaderRadius) && invaderRadius > 0.f);
	check(!FMath::IsNearlyZero(orbitRadius) && orbitRadius > 0.f);

	if (invaderRadius >= orbitRadius) {
		return 1;
	}
	else {
		//https://en.wikipedia.org/wiki/Law_of_cosines

		float ratio{ invaderRadius / orbitRadius };
		if (ratio < invaderNumLimit.first) {
			return invaderNumLimit.second;
		}
		else {
			return 2 * PI / FMath::Acos(1.f - 2 * FMath::Square(ratio));
		}
	}
}

float AOrbit::GetOuterRadius() const
{
	if (invaders.Num()) {
		check(false);
		return radius;
	}
	else {
		//return radius + invaders[0]->GetStaticMesh()->GetBounds().GetSphere().W;
		return radius + GetWorld()->GetSubsystem<UAssetProvider>()
			->invaderMesh->GetBounds().GetSphere().W;
	}
	return float();
}

UE_NODISCARD int AOrbit::GetInvadersNum() const
{
	return invaders.Num();
}

void AOrbit::Shoot()
{
	invaders[FMath::RandRange(0, invaders.Num() - 1)]->Shoot();
}

