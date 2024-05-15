// by Dmitry Kolontay


#include "Projectiles/KineticProjectileData.h"
#include "Projectiles/BBIProjectile.h"
#include "Projectiles/KineticMoveComponent.h"
#include "BBInvadersUtils.h"

void UKineticProjectileData::OnShooting(const FVector& from, const FVector& to, AActor* owner, AActor* target) const
{
	check(owner);
	UWorld* world{ owner->GetWorld() };
	ABBIProjectile* projectile{ world->SpawnActorDeferred<ABBIProjectile>(
		ABBIProjectile::StaticClass(), owner->GetActorTransform(), nullptr,
		nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn) };

	//setup projectile with visuals
	UActorComponent* movecomp{ projectile->AddComponentByClass(
		UKineticMoveComponent::StaticClass(), false, FTransform::Identity, true)};

	projectile->body->SetStaticMesh(projectileMesh.LoadSynchronous());

	CastChecked<UKineticMoveComponent>(movecomp)->SetParams(initialSpeed, deceleration);
	projectile->FinishAddComponent(movecomp, false, FTransform::Identity);

	//FMatrix::
	projectile->FinishSpawning(FTransform{ 
		FRotationMatrix::MakeFromX(owner->GetActorForwardVector()) });
}
