// by Dmitry Kolontay


#include "Environment/Orbit.h"
#include "GameFramework/RotatingMovementComponent.h"
//#include "Components/HierarchicalInstancedStaticMeshComponent.h"
#include "Components/SplineComponent.h"
#include "BBInvadersUtils.h"
#include "Environment/Invader.h"

double AOrbit::shrinkingSpeed = 100.;
float AOrbit::shrinkingStartDelay = 3.f;

AOrbit::AOrbit() :
	body{ CreateDefaultSubobject<USceneComponent>("body") },
	rotator{ CreateDefaultSubobject<URotatingMovementComponent>("rotator") },
	//invaders{ CreateDefaultSubobject<UHierarchicalInstancedStaticMeshComponent>("invaders") },
	radius{ 100. }, invaderRadius{ 0. }, minRadius { radius }
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;

	SetRootComponent(body);

	rotator->bUpdateOnlyIfRendered = true;
	rotator->RotationRate = FRotator::ZeroRotator;
	rotator->SetUpdatedComponent(body);
	
	//invaders->bMultiBodyOverlap = true;
	//invaders->SetCollisionObjectType(BBInvadersUtils::ECC_Invader);
	//invaders->SetCollisionObjectType
	//invaders->UpdateInstanceTransform
}

void AOrbit::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	Shrink(DeltaTime * shrinkingSpeed);
}

AOrbit* AOrbit::SpawnOrbit(UWorld& w, const FTransform& transform, float radius, int32 invaderNum, bool bZeroRotationSpeed)
{
	auto* newOrbit{ w.SpawnActorDeferred<ThisClass>(
		ThisClass::StaticClass(), transform, nullptr,
		nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn) };

	newOrbit->InitWithInvaders(radius);
	newOrbit->ChangeRotationSpeed(bZeroRotationSpeed);

	newOrbit->FinishSpawning(transform, true);
	return newOrbit;
}

void AOrbit::BeginPlay()
{
	Super::BeginPlay();
	FTimerHandle handle;
	GetWorld()->GetTimerManager().SetTimer(handle, FTimerDelegate{ 
		FTimerDelegate::CreateUObject(this, &AOrbit::SetActorTickEnabled, true) },
		shrinkingStartDelay, false);

	check(invaders.Num());
}

void AOrbit::OnInvaderDestroyed(AInvader* invader)
{
	check(invaders.Remove(invader) == 1);

	if (invaders.Num() < 1) {
		Destroy();
		SetActorTickEnabled(false);
	}
}

TArray<FVector> AOrbit::CalcRadiusVectors(int32 size, double length, double offsetAngle)
{
	check(size > 0);

	TArray<FVector> out;
	out.Reserve(size);
	const double angle{ UE_DOUBLE_PI / static_cast<double>(size) * 2.};
	for (int32 i{ 0 }; i < size; ++i) {
		out.Emplace(FMath::Sin(angle * i + offsetAngle), 
			FMath::Cos(angle * i + offsetAngle), 0.);
		out.Last() *= length;
	}
	return out;
}

void AOrbit::ChangeRotationSpeed(bool bZero)
{
	if (bZero) {
		rotator->RotationRate = FRotator::ZeroRotator;
	}
	else {
		rotator->RotationRate = BBInvadersUtils::unitRotation *
			FMath::RandRange(0.f, maxRotationSpeed);
	}
}

void AOrbit::SetShrinkingSpeed(float speed)
{
	shrinkingSpeed = speed;
}

void AOrbit::Shrink(double distance)
{
	double oldRadius{ radius };

	radius -= distance;
	if (radius < minRadius) {
		radius = minRadius;
	}

	for (auto* inv : invaders) {
		inv->AddActorLocalOffset(FVector::ForwardVector * (oldRadius - radius), true);
	}
}

void AOrbit::InitWithInvaders(double newRadius, bool bAdjustRadius/* = true*/)
{
	auto* world{ GetWorld() };
	check(world && newRadius > 0.f);
	check(invaders.Num() == 0);

	check(false);
	UStaticMesh* invaderMesh{ /*world->GetSubsystem<UAssetProvider>()->GetInvaderMesh()*/};

	invaderRadius = invaderMesh->GetBounds().GetSphere().W;

	radius = bAdjustRadius ? newRadius + invaderRadius : newRadius;

	minRadius = invaderRadius * 1.1;

	int32 newInvaderCount{ FMath::RandRange(1, 
		CalcMaxInvadersNum(invaderRadius, radius)) };
	
	auto radiusVectors{ CalcRadiusVectors(
		newInvaderCount, radius, FMath::RandRange(0., UE_DOUBLE_PI)) };

	FActorSpawnParameters spawnParams;
	spawnParams.Owner = this;
	spawnParams.SpawnCollisionHandlingOverride = 
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	FVector thisLocation{ GetActorLocation() };
	FRotator thisRotation{ GetActorRotation() };

	for (int32 i{ 0 }; i < newInvaderCount; ++i) {
		radiusVectors[i] = thisRotation.RotateVector(radiusVectors[i]);

		AInvader* invader{ world->SpawnActor<AInvader>(
			thisLocation + radiusVectors[i],
			FRotationMatrix::MakeFromX(radiusVectors[i] * -1.).Rotator(), 
			spawnParams) };

		/*world->SpawnActorDeferred<AInvader>(
			thisTransform.GetLocation() + radiusVectors[i],
			FRotationMatrix::MakeFromX(radiusVectors[i] * -1.).Rotator(),
			spawnParams)*/

		invader->SetMesh(*invaderMesh);
		invader->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);
		invader->onDestroyedDelegate.BindUObject(this, &AOrbit::OnInvaderDestroyed);

		invaders.Add(invader);
	}
}

int32 AOrbit::CalcMaxInvadersNum(double invaderRadius, double orbitRadius)
{	
	check(!FMath::IsNearlyZero(invaderRadius) && invaderRadius > 0.);
	check(!FMath::IsNearlyZero(orbitRadius) && orbitRadius > 0.);

	if (invaderRadius >= orbitRadius) {
		return 1;
	}
	else {
		//https://en.wikipedia.org/wiki/Law_of_cosines

		double ratio{ invaderRadius / orbitRadius };
		if (ratio < invaderNumLimit.first) {
			return invaderNumLimit.second;
		}
		else {
			return 2. / FMath::Acos(1. - 2. * FMath::Square(ratio)) * UE_DOUBLE_PI;
		}
	}
}

double AOrbit::GetOuterRadius(double offsetMultiplier/* = 2.f*/) const
{
	return radius + invaderRadius * offsetMultiplier;
}

int32 AOrbit::GetInvadersNum() const
{
	return invaders.Num();
}

void AOrbit::Shoot()
{
	invaders[FMath::RandRange(0, invaders.Num() - 1)]->Shoot();
}

