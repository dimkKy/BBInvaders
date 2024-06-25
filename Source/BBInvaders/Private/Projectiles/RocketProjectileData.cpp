// by Dmitry Kolontay


#include "Projectiles/RocketProjectileData.h"
#include "Projectiles/BBIProjectile.h"
#include "Projectiles/KineticMoveComponent.h"
#include "Projectiles/HomingMoveComponent.h"
#if WITH_EDITOR
#include "Misc/DataValidation.h"
#endif

void URocketProjectileData::OnShooting(const FVector& from, const FVector& to, AActor* owner, AActor* target) const
{
	check(owner);
	UWorld* world{ owner->GetWorld() };

	ABBIProjectile* projectile{ world->SpawnActorDeferred<ABBIProjectile>(
			ABBIProjectile::StaticClass(), owner->GetActorTransform(), nullptr,
			nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn) };

	UActorComponent* movecomp{ nullptr };

	if (bIsHoming) {
		movecomp = projectile->AddComponentByClass(
			UHomingMoveComponent::StaticClass(), false, FTransform::Identity, true);
		//todo
	}
	else {
		movecomp = projectile->AddComponentByClass(
			UKineticMoveComponent::StaticClass(), false, FTransform::Identity, true);
		CastChecked<UKineticMoveComponent>(movecomp)->SetParams(initialSpeed, accelerationSpeed);
		
	}
	projectile->FinishAddComponent(movecomp, false, FTransform::Identity);

	projectile->body->SetStaticMesh(bodyMesh.LoadSynchronous());

	projectile->FinishSpawning(FTransform{
			FRotationMatrix::MakeFromX(owner->GetActorForwardVector()) });
}

EDataValidationResult URocketProjectileData::IsDataValid(FDataValidationContext& context) const
{
	Super::IsDataValid(context);

	if (maxSpeed <= 0.f)
		context.AddError(FText::FromString("maxSpeed have to be positive"));

	if (maxSpeed < initialSpeed)
		context.AddError(FText::FromString("maxSpeed can not be lower then initialSpeed"));

	/*if (userType == EShooterType::EST_MAX) {
		context.AddError(FText::FromString("invalid userType"));
	}*/

	return context.GetNumErrors() > 0 ?
		EDataValidationResult::Invalid : EDataValidationResult::Valid;
}
