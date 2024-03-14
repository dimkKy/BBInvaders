// by Dmitry Kolontay


#include "Environment/Orbit.h"
#include "GameFramework/RotatingMovementComponent.h"
//#include "Components/HierarchicalInstancedStaticMeshComponent.h"
#include "Components/SplineComponent.h"
#include "BBInvadersUtils.h"
#include "Environment/Invader.h"
#include "CoreSystems/BBInvadersAssetManager.h"

float AOrbit::shrinkingStartDelay = 3.f;

AOrbit::AOrbit() :
	body{ CreateDefaultSubobject<USceneComponent>("body") },
	//invaders{ CreateDefaultSubobject<UHierarchicalInstancedStaticMeshComponent>("invaders") },
	radius{ 100. }, invaderRadius{ 0. }, minRadius { radius }
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;

	SetRootComponent(body);
	
	//invaders->bMultiBodyOverlap = true;
	//invaders->SetCollisionObjectType(BBInvadersUtils::ECC_Invader);
	//invaders->SetCollisionObjectType
	//invaders->UpdateInstanceTransform
}

void AOrbit::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//rotation only here
	// 
	//RequestShrink(DeltaTime * shrinkingSpeed);
	using namespace BBInvadersUtils;
	RootComponent->AddRelativeRotation(unitRotation, true);
}

AOrbit* AOrbit::SpawnOrbit(UWorld& w, const FTransform& tr, float radius, int32 invaderNum, bool bNoRotation)
{
	AOrbit* newOrbit{ w.SpawnActorDeferred<AOrbit>(
		AOrbit::StaticClass(), tr, nullptr,
		nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn) };

	newOrbit->InitWithInvaders(invaderNum, radius);
	newOrbit->ChangeRotationSpeed(bNoRotation);

	newOrbit->FinishSpawning(tr, true);
	return newOrbit;
}

void AOrbit::BeginPlay()
{
	Super::BeginPlay();
	/*FTimerHandle handle;
	GetWorld()->GetTimerManager().SetTimer(handle, FTimerDelegate{ 
		FTimerDelegate::CreateUObject(this, &AOrbit::SetActorTickEnabled, true) },
		shrinkingStartDelay, false);*/

	check(invaders.Num());
}

void AOrbit::OnInvaderDestroyed(AInvader* invader)
{
#if DO_CHECK
	check(invaders.Remove(invader) == 1);
#else
	invaders.Remove(invader);
#endif

	if (invaders.Num() < 1) {
		Destroy();
		onClearedDelegate.ExecuteIfBound(this);
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
		//rotator->RotationRate = FRotator::ZeroRotator;
	}
	else {
		//rotator->RotationRate = BBInvadersUtils::unitRotation *
			//FMath::RandRange(0.f, maxRotationSpeed);
	}
}

double AOrbit::RequestShrink(double distance)
{
	double oldRadius{ radius };

	radius -= distance;
	if (radius < minRadius) {
		radius = minRadius;
	}
	distance = oldRadius - radius;
	for (auto* inv : invaders) {
		inv->AddActorLocalOffset(FVector::ForwardVector * distance, true);
	}
	return distance;
}

void AOrbit::InitWithInvaders(int32 invaderNum, double newRadius, bool bAdjustRadius/* = true*/)
{
	auto* world{ GetWorld() };
	check(world && newRadius > 0.f);
	check(invaders.Num() == 0);

	check(false);
	UStaticMesh* invaderMesh{ UBBInvadersAssetManager::Get().
		GetRandomInvaderMesh(EInvaderType::EIT_Default) };

	invaderRadius = invaderMesh->GetBounds().GetSphere().W;

	radius = bAdjustRadius ? newRadius + invaderRadius : newRadius;

	minRadius = invaderRadius * 1.1;

	int32 maxInvaderCount{ CalcMaxInvadersNum(invaderRadius, radius) };
	int32 newInvaderCount{ invaderNum ? 
		std::max(maxInvaderCount, invaderNum) : 
		FMath::RandRange(1, maxInvaderCount) };
	invaders.Reserve(newInvaderCount);
	
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

double AOrbit::GetOuterRadius(double sizeMultiplier/* = 2.f*/) const
{
	return radius + invaderRadius * sizeMultiplier;
}

UE_NODISCARD double AOrbit::GetInnerRadius(double sizeMultiplier) const
{
	return radius - invaderRadius * sizeMultiplier;
}

int32 AOrbit::GetInvadersNum() const
{
	return invaders.Num();
}

void AOrbit::Shoot()
{
	invaders[FMath::RandRange(0, invaders.Num() - 1)]->Shoot();
}

