// by Dmitry Kolontay


#include "Projectiles/KineticMoveComponent.h"

UKineticMoveComponent::UKineticMoveComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UKineticMoveComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UKineticMoveComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);


	/*if (owner->GetWorld()->LineTraceSingleByChannel(result, from, to, ECC_Projectile)) {
		if (auto* actor{ result.GetActor() }; actor->CanBeDamaged()) {

			result.GetActor()->TakeDamage(baseDamage, BBIDamageEvents::Laser, /*controller*nullptr, owner);
		}

	}*/
	check(updatedComponent);
	if (updatedComponent)
	{
		//updatedComponent->collisionshape
		//updatedComponent->coll
		//updatedComponent->GetWorld()->SweepSingleByChannel()
		FHitResult hitRes;
		
		//updatedComponent->GetComponentTransform().GetRotation()

		updatedComponent->MoveComponent((updatedComponent->GetForwardVector() * currentSpeed + acceleration * DeltaTime * 2.f) * DeltaTime,
			updatedComponent->GetRelativeRotation(), true, &hitRes, EMoveComponentFlags::MOVECOMP_SkipPhysicsMove);

		currentSpeed += acceleration * DeltaTime;
		//check for 0 floor?
	}

	//return false;

}

void UKineticMoveComponent::RegisterComponentTickFunctions(bool bRegister)
{
	Super::RegisterComponentTickFunctions(bRegister);

	SetComponentTickEnabled(false);

	// If the owner ticks, make sure we tick first
	AActor* owner{ GetOwner() };
	//if (/*bTickBeforeOwner && */bRegister /*&& PrimaryComponentTick.bCanEverTick*/ && Owner && Owner->CanEverTick())
	if(bRegister && owner && owner->CanEverTick() )
	{
		owner->PrimaryActorTick.AddPrerequisite(this, PrimaryComponentTick);
	}
}

void UKineticMoveComponent::SetParams(float startSpeed, float acceler)
{
	check(startSpeed >= 0.f);
	currentSpeed = startSpeed;
	acceleration = acceler;

}

