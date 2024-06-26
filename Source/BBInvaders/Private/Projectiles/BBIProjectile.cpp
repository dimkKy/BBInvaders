// by Dmitry Kolontay


#include "Projectiles/BBIProjectile.h"
#include "CoreSystems/BBIAssetManager.h"
#include "Projectiles/ProjectileData.h"
//#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/StaticMeshComponent.h"
#include "BBInvadersUtils.h"

ABBIProjectile::ABBIProjectile() :
	//movement{ CreateDefaultSubobject<UProjectileMovementComponent>("movementComp") },
	body{ CreateDefaultSubobject<UStaticMeshComponent>("body") }, 
	projectileData{ nullptr }
{
	PrimaryActorTick.bCanEverTick = true;

	SetRootComponent(body);

	using namespace BBInvadersUtils;
	ConfigureBlockCollision(body, ECC_Projectile,
		ECC_Asteroid, ECC_Pawn, ECC_Invader, ECC_WorldDynamic);

	//body->SetSimulatePhysics(true);
	//body->SetEnableGravity(false);
	//body->BodyInstance.LinearDamping = 0.f;

	//movement->updatedComponent = body;
	//movement->InitialSpeed = speed;
	//movement->MaxSpeed = speed;
	//movement->bRotationFollowsVelocity = true;
	//movement->bShouldBounce = false;
	//movement->ProjectileGravityScale = 0.f;
	//movement->MaxSimulationIterations = simulationInteractions;
	InitialLifeSpan = lifespan;

	//body->GetSocketByName
}

void ABBIProjectile::BeginPlay()
{
	Super::BeginPlay();
	//body->OnComponentBeginOverlap.AddDynamic(
		//this, &ABBIProjectile::OnOverlapBegin);
	//movement
}

void ABBIProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


}

ABBIProjectile* ABBIProjectile::SpawnProjectile(UWorld& w, 
	const FTransform& transform, const UProjectileData& d, AActor* owner)
{
	ABBIProjectile* projectile{ w.SpawnActorDeferred<ABBIProjectile>(ABBIProjectile::StaticClass(),
		transform, owner, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn) };

	projectile->SetProjectileData(d);
	projectile->FinishSpawning(transform, true);

	return projectile;
}

void ABBIProjectile::NotifyHit(UPrimitiveComponent* myComp, AActor* other, 
	UPrimitiveComponent* otherComp, bool bSelfMoved, FVector hitLoc, 
	FVector hitNormal, FVector normalImpulse, const FHitResult& Hit)
{
	check(myComp == body);

	if (other->CanBeDamaged()) {
		//other->TakeDamage(projectileData->baseDamage, )
	}

	Destroy();
}

/*void ABBIProjectile::OnOverlapBegin(UPrimitiveComponent* component, AActor* otherActor, 
	UPrimitiveComponent* otherComp, int32 otherIndex, bool bFromSweep, const FHitResult& result)
{
	AActor* owner{ GetOwner() };

	if (owner && owner->StaticClass() != otherActor->StaticClass()) {
		Destroy();
	}
}*/

void ABBIProjectile::SetProjectileData(const UProjectileData& data)
{
	/*projectileData = &data;
	movement->InitialSpeed = data.initialSpeed;
	movement->MaxSpeed = data.maxSpeed;
	movement->bIsHomingProjectile = data.bIsHoming;
	/*TWeakObjectPtr<ThisClass> weakThis{ this };

	//UBBIAssetManager::Get().lo

	RequestAsyncLoad(SoftTexture,
		[WeakThis, SoftTexture, bMatchSize]() {
			if (UImage* StrongThis = WeakThis.Get())
			{
				ensureMsgf(SoftTexture.Get(), TEXT("Failed to load %s"), *SoftTexture.ToSoftObjectPath().ToString());
				StrongThis->SetBrushFromTexture(SoftTexture.Get(), bMatchSize);
			}
		}
	);*
	body->SetStaticMesh(data.bodyMesh.LoadSynchronous());*/
}

const UProjectileData* ABBIProjectile::GetProjectileData() const
{
	return projectileData;
}


