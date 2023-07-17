// by Dmitry Kolontay

#include "Environment/Asteroid.h"
#include "Components/StaticMeshComponent.h"
#include "BBInvadersUtils.h"
#include "Player/PlayerPawn.h"
#include "CoreSystems/BBInvadersGameStateBase.h"
#include "CoreSystems/AssetProvider.h"

AAsteroid::AAsteroid() :
	body{CreateDefaultSubobject<UStaticMeshComponent>("body")},
	size{ EAsteroidSize::EAS_MAX }, velocity{0.f}, 
	rotationInfo{EForceInit::ForceInitToZero}
{
	PrimaryActorTick.bCanEverTick = true;
	SetRootComponent(body);

	BBInvadersUtils::ConfigureDefaultCollision<true>(body, BBInvadersUtils::ECC_Asteroid,
		ECC_WorldStatic, BBInvadersUtils::ECC_Projectile);

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

void AAsteroid::SetSizeAssignMesh(EAsteroidSize newSize, const UAssetProvider& provider)
{
	check(newSize != EAsteroidSize::EAS_MAX);
	if (size == newSize) {
		return;
	}
	size = newSize;
	check(body->SetStaticMesh(provider.GetAsteroidMesh(size)));
}

void AAsteroid::SetVelocity(const AActor& target)
{
	//FVector targetLocation{ playerPawn->GetActorLocation() };
	FVector toTarget{ target.GetActorLocation() - RootComponent->GetRelativeLocation()};

	
	//TODO
	velocity = toTarget.GetSafeNormal().RotateAngleAxis(
		FMath::RandRange(-aimAngleAmplitude, aimAngleAmplitude), 
		target.GetActorUpVector()) * 1.f;
}

void AAsteroid::SetVelocity(const FVector& newVel)
{
	velocity = newVel;
}

void AAsteroid::SetRotation()
{
	//TODO
	check(false);
}

void AAsteroid::SetRotation(const FVector& rotAxis, float rotSpeed)
{
	rotationInfo = FQuat::FQuat(rotAxis.GetSafeNormal(), rotSpeed);
}

/*void AAsteroid::SetVelocity(float newVelocity)
{
	currentVelocity = newVelocity;
}*/

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

void AAsteroid::Split()
{
	check(size != EAsteroidSize::EAS_MAX && size != EAsteroidSize::EAS_Small);

	UWorld* world{ GetWorld() };

	const UAssetProvider* provider{ world->GetSubsystem<UAssetProvider>() };
	check(provider);

	EAsteroidSize newSize{ static_cast<EAsteroidSize>(static_cast<int32>(size) - 1) };

	SetSizeAssignMesh(newSize, *provider);

	float splitHalfAngle{ BBInvadersUtils::RandAbsRange(splitAngleAmplitude) };

	FVector forward{ GetActorForwardVector() };
	//TODO take up from pawn

	auto* gameState{ CastChecked<ABBInvadersGameStateBase>(world->GetGameState()) };
	FVector up{ GetActorUpVector() };

	SetActorRotation(FRotationMatrix::MakeFromX(
		forward.RotateAngleAxis(splitHalfAngle, up)).Rotator());

	FTransform newAsteroidTransform{ FRotationMatrix::MakeFromX(
		forward.RotateAngleAxis(-1.f * splitHalfAngle, up)).Rotator(),
		GetActorLocation(), GetActorScale3D() };

	auto* newAsteroid{ world->SpawnActorDeferred<AAsteroid>(
		ThisClass::StaticClass(), newAsteroidTransform,
		nullptr, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn) };

	newAsteroid->SetSizeAssignMesh(newSize, *provider);
	//newAsteroid->SetVelocity(velocity)
	newAsteroid->FinishSpawning(newAsteroidTransform, true);

	//FRotator rotation {}


}

void AAsteroid::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	/*const FQuat CurRelRotQuat = 
		RootComponent->GetRelativeRotationCache().RotatorToQuat(RootComponent->GetRelativeRotation());*/
	
	/*const FQuat NewRelRotQuat{ 
		RootComponent->GetRelativeRotation().Quaternion() *
		FQuat{ rotationAxis, rotationSpeed * DeltaTime } };*/

	const FQuat NewRelRotQuat{
		RootComponent->GetRelativeRotation().Quaternion() *
		FQuat { rotationInfo.X, rotationInfo.Y, rotationInfo.Z, rotationInfo.W * DeltaTime } };
	
	RootComponent->SetWorldLocationAndRotation(
		RootComponent->GetRelativeLocation() + velocity * DeltaTime,
		NewRelRotQuat);
}
