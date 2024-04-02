// by Dmitry Kolontay

#include "Environment/Asteroid.h"
#include "Components/StaticMeshComponent.h"
#include "BBInvadersUtils.h"
#include "Player/PlayerPawn.h"
#include "CoreSystems/BBIGameStateBase.h"

AAsteroid::AAsteroid() :
	body{CreateDefaultSubobject<UStaticMeshComponent>("body")},
	size{ EAsteroidSize::EAS_MAX }, velocity{ 0.f }
{
	PrimaryActorTick.bCanEverTick = true;
	SetRootComponent(body);

	BBInvadersUtils::ConfigureDefaultCollision<true>(body, BBInvadersUtils::ECC_Asteroid,
		ECC_WorldStatic, BBInvadersUtils::ECC_Projectile, ECC_WorldDynamic, ECC_Pawn);

	//asteroids->bMultiBodyOverlap = true;
}

EAsteroidSize AAsteroid::GetSmaller(EAsteroidSize _size)
{
	if (_size!= EAsteroidSize::EAS_Small
		/*ensureAlways(!BBInvadersUtils::IsIn(_size, EAsteroidSize::EAS_MAX, EAsteroidSize::EAS_Small))*/) {
		return static_cast<EAsteroidSize>(static_cast<int32>(_size) - 1);
	}
	else {
		return EAsteroidSize::EAS_Small;
	}
	
}

EAsteroidSize AAsteroid::RandomSize()
{
	return static_cast<EAsteroidSize>(
		FMath::RandRange(0, static_cast<int32>(EAsteroidSize::EAS_MAX) - 1));
}

void AAsteroid::SetSizeAssignMesh(EAsteroidSize newSize)
{
	check(false);
	check(newSize != EAsteroidSize::EAS_MAX);
	if (size == newSize) {
		return;
	}
	size = newSize;
	//check(body->SetStaticMesh(provider.GetAsteroidMesh(size)));
}

void AAsteroid::CalculateRandomVelocity(const FVector& targetLocation)
{
	FVector toTarget{ targetLocation - RootComponent->GetRelativeLocation() };

	velocity = toTarget.GetSafeNormal().RotateAngleAxis(
		FMath::RandRange(-aimAngleAmplitude, aimAngleAmplitude), 
		ABBIGameStateBase::Get(GetWorld())->GetMapInfo().GetUp()) *
		FMath::RandRange(velocityRange.first, velocityRange.second);
}

/*void AAsteroid::CalculateRandomVelocity(const FVector& newVel)
{
	velocity = newVel;
}*/

void AAsteroid::SetRotation()
{
	//TODO WITH MATERIAL
}

void AAsteroid::SetRotation(const FVector& rotAxis, float rotSpeed)
{
	//TODO WITH MATERIAL
}

float AAsteroid::GetMeshRadius() const
{
	check(body->GetStaticMesh());
	return body->GetStaticMesh()->GetBounds().GetSphere().W;
}

float AAsteroid::GetOnPlanetCollisionDamage() const
{
	switch (size) {
	case EAsteroidSize::EAS_Small:
		return 65.f;
	case EAsteroidSize::EAS_Medium:
		return 85.f;
	case EAsteroidSize::EAS_Big:
		return 99.f;
	default:
		return 0.f;
	}
}

void AAsteroid::BeginPlay()
{
	Super::BeginPlay();
	body->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnOverlapBegin);

	/*if (velocity.IsNearlyZero()) {
		SetVelocityAndRotation();
	}

	rotationAxis = BBInvadersUtils::RandomVector().GetSafeNormal();*/
}

void AAsteroid::OnOverlapBegin(UPrimitiveComponent* component, AActor* otherActor,
	UPrimitiveComponent* otherComp, int32 otherIndex, bool bFromSweep, const FHitResult& result)
{
	//redo
	//otherActor->GetClass()
	auto* otherOwner{ otherActor->GetOwner() };
	if (otherActor->IsA(APlayerPawn::StaticClass())
		|| (otherOwner && otherOwner->IsA(APlayerPawn::StaticClass()))) {
		Destroy();
	}

	if (otherOwner && otherOwner->IsA(APlayerPawn::StaticClass())) {
		switch (size) {
		case EAsteroidSize::EAS_MAX:
			check(false);
			[[fallthrough]];
		case EAsteroidSize::EAS_Small:
			Destroy();
			break;
		default:
			Split();
			break;
		}
	}
}

AAsteroid* AAsteroid::Split()
{
	check(GetWorld() && size != EAsteroidSize::EAS_MAX);
	if (size == EAsteroidSize::EAS_Small) {
		Destroy();
		return nullptr;
	}
	
	EAsteroidSize newSize{ static_cast<EAsteroidSize>(static_cast<int32>(size) - 1) };
	SetSizeAssignMesh(newSize);

	UWorld* world{ GetWorld() };
	AAsteroid* newAsteroid{ AAsteroid::SpawnAsteroid(*world, GetActorLocation(), newSize) };

	float splitHalfAngle{ BBInvadersUtils::RandAbsRange(splitAngleAmplitude) };
	FVector up{ ABBIGameStateBase::Get(GetWorld())->GetMapInfo().GetUp() };

	newAsteroid->velocity = velocity.RotateAngleAxis(splitHalfAngle, up);

	velocity = velocity.RotateAngleAxis(-splitHalfAngle, up);

	return newAsteroid;
}

void AAsteroid::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	RootComponent->SetWorldLocationAndRotation(
		RootComponent->GetRelativeLocation() + velocity * DeltaTime,
		RootComponent->GetRelativeRotation(), true);
}

AAsteroid* AAsteroid::SpawnAsteroid(UWorld& w, const FVector& location, EAsteroidSize size)
{
	AAsteroid* newAsteroid{ SpawnAsteroidDeferred(w, size) };
	newAsteroid->FinishSpawning(
		{ BBInvadersUtils::RandomRotator(), location }, false);
	return newAsteroid;
}

AAsteroid* AAsteroid::SpawnAsteroid(UWorld& w, const FVector& location, 
	const FVector& targetLocation, EAsteroidSize size)
{
	auto* newAsteroid{ SpawnAsteroid(w, location, size)};
	newAsteroid->CalculateRandomVelocity(targetLocation);
	return newAsteroid;
}

UE_NODISCARD AAsteroid* AAsteroid::SpawnAsteroidDeferred(UWorld& w, EAsteroidSize size)
{
	auto* newAsteroid{ w.SpawnActorDeferred<AAsteroid>(
		AAsteroid::StaticClass(), FTransform::Identity, nullptr,
		nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn) };

	//auto* provider{ world->GetSubsystem<UAssetProvider>() };
	//check(provider);
	//w.GetGameState()

	newAsteroid->SetSizeAssignMesh(size);
	newAsteroid->SetRotation();

	return newAsteroid;
}

void AAsteroid::FinishSpawningSetVelocity(const FVector& location, const FVector& targetLoc)
{
	FinishSpawning({ BBInvadersUtils::RandomRotator(), location}, false);
	CalculateRandomVelocity(targetLoc);
}
