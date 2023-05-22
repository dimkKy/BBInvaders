// by Dmitry Kolontay

#include "Environment/Asteroid.h"
#include "Components/StaticMeshComponent.h"
#include "BBInvadersUtils.h"

AAsteroid::AAsteroid() :
	body{CreateDefaultSubobject<UStaticMeshComponent>("body")}
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

}

void AAsteroid::SpawnNewAsteroid(const FVector2D& location)
{
	check(location.X >= 1.f || location.Y >= 1.f);
	FVector2D newUnitVelocity{ (location * -1.f).GetRotated(
		FMath::RandRange(-1.f * maxRotationDefiationHalfAngle, maxRotationDefiationHalfAngle)) };
	//asteroids->AddInstanceWorldSpace({});

}

void AAsteroid::BeginPlay()
{
	Super::BeginPlay();
}

void AAsteroid::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	//FTransform transform;
	//asteroids->GetInstanceTransform(, transform, true);
	//ast.AddToTranslation();
	//asteroids->UpdateInstanceTransform(0, transform, , , );
}
