// by Dmitry Kolontay

#include "Environment/Asteroid.h"
#include "Components/StaticMeshComponent.h"
#include "BBInvadersUtils.h"
#include "Player/PlayerPawn.h"

AAsteroid::AAsteroid() :
	body{CreateDefaultSubobject<UStaticMeshComponent>("body")},
	size{EAS_Big}, currentVelocity{0.f}
{
	PrimaryActorTick.bCanEverTick = true;
	SetRootComponent(body);

	body->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	//asteroids->bMultiBodyOverlap = true;
	body->SetGenerateOverlapEvents(true);
	body->SetCollisionObjectType(BBInvadersUtils::ECC_Asteroid);
	body->SetCollisionResponseToAllChannels(ECR_Ignore);
	body->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Overlap);
	body->SetCollisionResponseToChannel(BBInvadersUtils::ECC_Projectile, ECR_Overlap);
	body->OnComponentBeginOverlap.AddDynamic(this, &AAsteroid::OnOverlapBegin);
}

void AAsteroid::SpawnNewAsteroid(const FVector2D& location)
{
	check(location.X >= 1.f || location.Y >= 1.f);
	FVector2D newUnitVelocity{ (location * -1.f).GetRotated(
		FMath::RandRange(-1.f * maxRotationDefiationHalfAngle, maxRotationDefiationHalfAngle)) };
	//asteroids->AddInstanceWorldSpace({});

}

void AAsteroid::OnOverlapBegin(UPrimitiveComponent* component, AActor* otherActor, UPrimitiveComponent* otherComp, int32 otherIndex, bool bFromSweep, const FHitResult& result)
{
	if (otherActor->IsA(APlayerPawn::StaticClass())) {
		Destroy();
		body->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		return;
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
			size = static_cast<EAsteroidSize>(static_cast<int32>(size) - 1);
			break;
		}
	}
}

void AAsteroid::BeginPlay()
{
	Super::BeginPlay();
}

void AAsteroid::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	SetActorLocation(GetActorLocation() + currentVelocity * DeltaTime);
	//FTransform transform;
	//asteroids->GetInstanceTransform(, transform, true);
	//ast.AddToTranslation();
	//asteroids->UpdateInstanceTransform(0, transform, , , );
}
