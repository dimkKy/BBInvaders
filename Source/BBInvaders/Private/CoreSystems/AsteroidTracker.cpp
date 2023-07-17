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
		BBInvadersUtils::ECC_Asteroid/*, BBInvadersUtils::ECC_Projectile*/);
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

void AAsteroidTracker::BeginPlay()
{
	Super::BeginPlay();
	trackBox->OnComponentEndOverlap.AddDynamic(
		this, &AAsteroidTracker::OnOverlapEnd);
}

void AAsteroidTracker::OnOverlapEnd(UPrimitiveComponent* comp, AActor* other,
	UPrimitiveComponent* otherComp, int32 otherBodyIndex)
{
	if (other->IsA(AAsteroid::StaticClass())) {
		FVector otherLocation{ other->GetActorLocation() };
		FVector toOther{ otherLocation - GetActorLocation() };

		FVector projectForward{ toOther.ProjectOnTo(GetActorForwardVector()) };
		FVector projectRight{ toOther.ProjectOnTo(GetActorRightVector()) };
		FVector areaSize{ trackBox->GetScaledBoxExtent() };

		FVector halfCorrection{ 0.f };

		if (projectForward.SizeSquared() >= areaSize.X * areaSize.X) {
			halfCorrection += projectForward;
		}
		if (projectRight.SizeSquared() >= areaSize.Y * areaSize.Y) {
			halfCorrection += projectRight;
		}

		other->SetActorLocation(otherLocation - 2 * halfCorrection);
	}
}
