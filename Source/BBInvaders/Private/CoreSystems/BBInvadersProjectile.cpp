// by Dmitry Kolontay


#include "CoreSystems/BBInvadersProjectile.h"
#include "CoreSystems/ProjectileDataAsset.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/StaticMeshComponent.h"
#include "BBInvadersUtils.h"

ABBInvadersProjectile::ABBInvadersProjectile() :
	movement{ CreateDefaultSubobject<UProjectileMovementComponent>("movementComp") },
	body{ CreateDefaultSubobject<UStaticMeshComponent>("body") }
{
	PrimaryActorTick.bCanEverTick = true;

	SetRootComponent(body);

	BBInvadersUtils::ConfigureDefaultCollision<true>
		(body, BBInvadersUtils::ECC_Projectile,
		BBInvadersUtils::ECC_Asteroid, ECC_Pawn, 
		BBInvadersUtils::ECC_Invader, ECC_WorldDynamic);

	body->SetSimulatePhysics(true);
	body->SetEnableGravity(false);
	body->BodyInstance.LinearDamping = 0.f;

	movement->UpdatedComponent = body;
	//movement->InitialSpeed = speed;
	//movement->MaxSpeed = speed;
	//movement->bRotationFollowsVelocity = true;
	movement->bShouldBounce = false;
	movement->ProjectileGravityScale = 0.f;
	movement->MaxSimulationIterations = simulationInteractions;
	InitialLifeSpan = lifespan;
}

void ABBInvadersProjectile::BeginPlay()
{
	Super::BeginPlay();
	body->OnComponentBeginOverlap.AddDynamic(
		this, &ABBInvadersProjectile::OnOverlapBegin);
	//movement
}

void ABBInvadersProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


}

void ABBInvadersProjectile::OnOverlapBegin(UPrimitiveComponent* component, AActor* otherActor, 
	UPrimitiveComponent* otherComp, int32 otherIndex, bool bFromSweep, const FHitResult& result)
{
	AActor* owner{ GetOwner() };

	if (owner && owner->StaticClass() != otherActor->StaticClass()) {
		Destroy();
	}
}

void ABBInvadersProjectile::SetProjectileData(const UProjectileDataAsset& data)
{
}


