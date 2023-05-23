// by Dmitry Kolontay


#include "CoreSystems/BBInvadersProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/StaticMeshComponent.h"
#include "BBInvadersUtils.h"

ABBInvadersProjectile::ABBInvadersProjectile() :
	movement{ CreateDefaultSubobject<UProjectileMovementComponent>("movementComp") },
	body{ CreateDefaultSubobject<UStaticMeshComponent>("body") }
{
	PrimaryActorTick.bCanEverTick = true;

	SetRootComponent(body);

	//body->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	body->SetSimulatePhysics(true);
	body->SetEnableGravity(false);
	body->BodyInstance.LinearDamping = 0.f;
	body->BodyInstance.SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	body->SetGenerateOverlapEvents(true);
	body->SetCollisionObjectType(BBInvadersUtils::ECC_Projectile);
	body->SetCollisionResponseToAllChannels(ECR_Ignore);
	body->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	body->SetCollisionResponseToChannel(BBInvadersUtils::ECC_Asteroid, ECR_Overlap);
	body->SetCollisionResponseToChannel(BBInvadersUtils::ECC_Invader, ECR_Overlap);

	body->OnComponentBeginOverlap.AddDynamic(this, &ABBInvadersProjectile::OnOverlapBegin);

	movement->UpdatedComponent = body;
	movement->InitialSpeed = speed;
	movement->MaxSpeed = speed;
	//movement->bRotationFollowsVelocity = true;
	movement->bShouldBounce = false;
	movement->ProjectileGravityScale = 0.f;
	movement->MaxSimulationIterations = simulationInteractions;
	InitialLifeSpan = lifespan;
}

void ABBInvadersProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABBInvadersProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABBInvadersProjectile::OnOverlapBegin(UPrimitiveComponent* component, AActor* otherActor, 
	UPrimitiveComponent* otherComp, int32 otherIndex, bool bFromSweep, const FHitResult& result)
{
	if (!GetOwner()->IsA(otherActor->StaticClass())) {
		Destroy();
		body->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

