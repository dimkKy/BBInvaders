// by Dmitry Kolontay


#include "CoreSystems/AsteroidTracker.h"
#include "Environment/Asteroid.h"
#include "Components/BoxComponent.h"
#include "BBInvadersUtils.h"

AAsteroidTracker::AAsteroidTracker() :
	trackBox{ CreateDefaultSubobject<UBoxComponent>("movementComp") }
{
	PrimaryActorTick.bCanEverTick = false;

	SetRootComponent(trackBox);
	BBInvadersUtils::ConfigureDefaultCollision<true>(trackBox, ECC_WorldStatic,
		BBInvadersUtils::ECC_Asteroid, BBInvadersUtils::ECC_Projectile);

	trackBox->OnComponentEndOverlap.AddDynamic(this, &AAsteroidTracker::OnOverlapEnd);
}

void AAsteroidTracker::SetTrackArea(const FTransform& transform, const FVector& halfSize)
{
	SetActorTransform(transform);
	SetTrackArea(halfSize);
}

void AAsteroidTracker::SetTrackArea(const FVector& halfSize)
{
	trackBox->SetBoxExtent(halfSize, true);
}

void AAsteroidTracker::OnOverlapEnd(UPrimitiveComponent* comp, AActor* other, 
	UPrimitiveComponent* otherComp, int32 otherBodyIndex)
{
	check(!other->IsA(AAsteroid::StaticClass()));

	FVector thisLocation{ GetActorLocation() };
	FVector otherRelativeLocation{ other->GetActorLocation() - thisLocation };

	other->SetActorLocation(thisLocation - otherRelativeLocation);
}
