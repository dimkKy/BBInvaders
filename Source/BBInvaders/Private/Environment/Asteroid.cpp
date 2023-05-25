// by Dmitry Kolontay

#include "Environment/Asteroid.h"
#include "Components/StaticMeshComponent.h"
#include "BBInvadersUtils.h"
#include "Player/PlayerPawn.h"
#include "CoreSystems/AssetProvider.h"

AAsteroid::AAsteroid() :
	body{CreateDefaultSubobject<UStaticMeshComponent>("body")},
	size{EAS_Big}, currentVelocity{0.f}
{
	PrimaryActorTick.bCanEverTick = true;
	SetRootComponent(body);

	BBInvadersUtils::ConfigureDefaultCollision<true>(body, BBInvadersUtils::ECC_Asteroid,
		ECC_WorldStatic, BBInvadersUtils::ECC_Projectile);

	//asteroids->bMultiBodyOverlap = true;
}

void AAsteroid::SetSizeAssignMesh(EAsteroidSize newSize)
{
	check(newSize != EAsteroidSize::EAS_MAX);
	if (size == newSize) {
		return;
	}
	size = newSize;
	body->SetStaticMesh(GetWorld()->GetSubsystem<UAssetProvider>()->asteroidMeshes[size]);
}

void AAsteroid::SetVelocity(float newVelocity)
{
	currentVelocity = newVelocity;
}

float AAsteroid::GetMeshRadius() const
{
	check(body->GetStaticMesh());
	return body->GetStaticMesh()->GetBounds().GetSphere().W;
}

float AAsteroid::GetOnPlanetCollisionDamage() const
{
	switch (size) {
	case EAS_Small:
		return 65.f;
	case EAS_Medium:
		return 85.f;
	case EAS_Big:
		return 99.f;
	default:
		return 0.f;
	}
}

void AAsteroid::BeginPlay()
{
	Super::BeginPlay();
	body->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnOverlapBegin);
}

void AAsteroid::OnOverlapBegin(UPrimitiveComponent* component, AActor* otherActor,
	UPrimitiveComponent* otherComp, int32 otherIndex, bool bFromSweep, const FHitResult& result)
{
	if (auto* owner{ otherActor->GetOwner() }; otherActor->IsA(APlayerPawn::StaticClass())
		|| (owner && owner->IsA(APlayerPawn::StaticClass()))) {
		Destroy();
		body->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	if (otherActor->GetOwner()->IsA(APlayerPawn::StaticClass())) {
		switch (size) {
		case EAS_MAX:
			check(false);
			[[fallthrough]];
		case EAS_Small:
			Destroy();
			body->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			break;
		default:
			SetSizeAssignMesh(static_cast<EAsteroidSize>(static_cast<int32>(size) - 1));
			
			float splitHalfAngle{ FMath::RandRange(-1.f * maxSplitDeviationHalfAngle, maxSplitDeviationHalfAngle) };

			FVector forward{ GetActorForwardVector() };
			//TODO take up from pawn
			FVector up{ GetActorUpVector() };

			SetActorRotation(FRotationMatrix::MakeFromX(
				forward.RotateAngleAxis(splitHalfAngle, up)).Rotator());

			FTransform newAsteroidTransform{ FRotationMatrix::MakeFromX(
				forward.RotateAngleAxis(-1.f * splitHalfAngle, up)).Rotator(),
				GetActorLocation(), GetActorScale3D() };

			auto* newAsteroid{ GetWorld()->SpawnActorDeferred<AAsteroid>(
				ThisClass::StaticClass(), newAsteroidTransform,
				nullptr, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn) };

			newAsteroid->SetSizeAssignMesh(size);
			//newAsteroid->SetVelocity(velocity)
			newAsteroid->FinishSpawning(newAsteroidTransform, true);

			//FRotator rotation {}


			break;
		}
	}
}

void AAsteroid::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	SetActorLocation(GetActorLocation() + GetActorForwardVector() * currentVelocity * DeltaTime);
	//FTransform transform;
	//asteroids->GetInstanceTransform(, transform, true);
	//ast.AddToTranslation();
	//asteroids->UpdateInstanceTransform(0, transform, , , );
}
