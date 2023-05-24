﻿// by Dmitry Kolontay


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
	spline{ CreateDefaultSubobject<USplineComponent>("spline") },
	radius{ 1.f }
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

void AOrbit::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

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

void AOrbit::Init(int32 invaderCount, float newRadius)
{
	
	auto* world{ GetWorld() };
	check(world && invaderCount > 0 && invaders.Num() == 0);
	radius = newRadius;

	auto radiusVectors{ CalcRadiusVectors(invaderCount, radius) };
	FVector thisLocation{ GetActorLocation() };
	for (SIZE_T i{ 0 }; i < invaderCount; ++i) {
		FTransform transform{ GetActorTransform() };

		radiusVectors[i] = transform.GetRotation().RotateVector(radiusVectors[i]);

		FActorSpawnParameters spawnParams;
		spawnParams.Owner = this;
		spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		AInvader* invader{ world->SpawnActor<AInvader>(
			transform.GetLocation() + radiusVectors[i], 
			FRotationMatrix::MakeFromX(radiusVectors[i] * -1.f).Rotator(), 
			spawnParams) };

		/*world->SpawnActorDeferred<AInvader>(
			transform.GetLocation() + radiusVectors[i],
			FRotationMatrix::MakeFromX(radiusVectors[i] * -1.f).Rotator(),
			spawnParams)*/

		auto provider{ GetWorld()->GetSubsystem<UAssetProvider>() };
		invader->SetMesh(*provider->invaderMesh);

		invader->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);
		invaders.Add(invader);
	}
}

void AOrbit::Shoot()
{
	invaders[FMath::RandRange(0, invaders.Num() - 1)]->Shoot();
}

