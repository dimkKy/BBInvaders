// by Dmitry Kolontay

#include "Environment/Asteroid.h"
#include "Components/StaticMeshComponent.h"
#include "BBInvadersUtils.h"
#include "Player/PlayerPawn.h"
#include "CoreSystems/BBIGameStateBase.h"
#include "Projectiles/ProjectileData.h"

AAsteroid::AAsteroid() :
	body{CreateDefaultSubobject<UStaticMeshComponent>("body")},
	size{ EAsteroidSize::EAS_MAX }, velocity{ 0.f }
{
	PrimaryActorTick.bCanEverTick = true;
	SetRootComponent(body);

	using namespace BBInvadersUtils;

	ConfigureOverlapCollision<true>(body, ECC_Asteroid,
		ECC_WorldStatic, ECC_Projectile, ECC_WorldDynamic, ECC_Pawn);

	//asteroids->bMultiBodyOverlap = true;
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

float AAsteroid::TakeDamage(float amount, FDamageEvent const& dEvent, AController* instigator, AActor* causer)
{
	check(dEvent.IsOfType(FBBIDamageEvent::ClassID));

	FBBIDamageEvent* const BIIevent = (FBBIDamageEvent*)&dEvent;
	
	//get from type
	float actualDamage{ amount };

	health -= actualDamage;
	if (health <= /*small number*/ 0.f) {
		Split();
	}
	else {

	}

	/*float ActualDamage = DamageAmount;

	UDamageType const* const DamageTypeCDO = DamageEvent.DamageTypeClass ? DamageEvent.DamageTypeClass->GetDefaultObject<UDamageType>() : GetDefault<UDamageType>();
	if (DamageEvent.IsOfType(FPointDamageEvent::ClassID))
	{
		// point damage event, pass off to helper function
		FPointDamageEvent* const PointDamageEvent = (FPointDamageEvent*)&DamageEvent;
		ActualDamage = InternalTakePointDamage(ActualDamage, *PointDamageEvent, EventInstigator, DamageCauser);

		// K2 notification for this actor
		if (ActualDamage != 0.f)
		{
			ReceivePointDamage(ActualDamage, DamageTypeCDO, PointDamageEvent->HitInfo.ImpactPoint, PointDamageEvent->HitInfo.ImpactNormal, PointDamageEvent->HitInfo.Component.Get(), PointDamageEvent->HitInfo.BoneName, PointDamageEvent->ShotDirection, EventInstigator, DamageCauser, PointDamageEvent->HitInfo);
			OnTakePointDamage.Broadcast(this, ActualDamage, EventInstigator, PointDamageEvent->HitInfo.ImpactPoint, PointDamageEvent->HitInfo.Component.Get(), PointDamageEvent->HitInfo.BoneName, PointDamageEvent->ShotDirection, DamageTypeCDO, DamageCauser);

			// Notify the component
			UPrimitiveComponent* const PrimComp = PointDamageEvent->HitInfo.Component.Get();
			if (PrimComp)
			{
				PrimComp->ReceiveComponentDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
			}
		}
	}
	else if (DamageEvent.IsOfType(FRadialDamageEvent::ClassID))
	{
		// radial damage event, pass off to helper function
		FRadialDamageEvent* const RadialDamageEvent = (FRadialDamageEvent*)&DamageEvent;
		ActualDamage = InternalTakeRadialDamage(ActualDamage, *RadialDamageEvent, EventInstigator, DamageCauser);

		// K2 notification for this actor
		if (ActualDamage != 0.f)
		{
			FHitResult const& Hit = (RadialDamageEvent->ComponentHits.Num() > 0) ? RadialDamageEvent->ComponentHits[0] : FHitResult();
			ReceiveRadialDamage(ActualDamage, DamageTypeCDO, RadialDamageEvent->Origin, Hit, EventInstigator, DamageCauser);
			OnTakeRadialDamage.Broadcast(this, ActualDamage, DamageTypeCDO, RadialDamageEvent->Origin, Hit, EventInstigator, DamageCauser);

			// add any desired physics impulses to our components
			for (int HitIdx = 0; HitIdx < RadialDamageEvent->ComponentHits.Num(); ++HitIdx)
			{
				FHitResult const& CompHit = RadialDamageEvent->ComponentHits[HitIdx];
				UPrimitiveComponent* const PrimComp = CompHit.Component.Get();
				if (PrimComp && PrimComp->GetOwner() == this)
				{
					PrimComp->ReceiveComponentDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
				}
			}
		}
	}

	// generic damage notifications sent for any damage
	// note we will broadcast these for negative damage as well
	if (ActualDamage != 0.f)
	{
		ReceiveAnyDamage(ActualDamage, DamageTypeCDO, EventInstigator, DamageCauser);
		OnTakeAnyDamage.Broadcast(this, ActualDamage, DamageTypeCDO, EventInstigator, DamageCauser);
		if (EventInstigator != nullptr)
		{
			EventInstigator->InstigatedAnyDamage(ActualDamage, DamageTypeCDO, this, DamageCauser);
		}
	}*/

	return amount;
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
	//redo - delete?
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
	if (size == EAS::Small) {
		Destroy();
		return nullptr;
	}
	
	EAsteroidSize newSize{ EAS::Smaller(size) };
	SetSizeAssignMesh(newSize);
	health = 100.f;

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
