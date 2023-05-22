// by Dmitry Kolontay


#include "CoreSystems/AsteroidTracker.h"
#include "Components/BoxComponent.h"
#include "BBInvadersUtils.h"

AAsteroidTracker::AAsteroidTracker() :
	trackBox{ CreateDefaultSubobject<UBoxComponent>("movementComp") }
{
	PrimaryActorTick.bCanEverTick = false;

	SetRootComponent(trackBox);
	trackBox->SetCollisionObjectType(ECC_WorldStatic);
	trackBox->BodyInstance.SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	trackBox->SetGenerateOverlapEvents(true);
	trackBox->OnComponentEndOverlap.AddDynamic(this, &AAsteroidTracker::OnOverlapEnd);
	trackBox->SetCollisionResponseToAllChannels(ECR_Ignore);
	trackBox->SetCollisionResponseToChannel(BBInvadersUtils::ECC_Asteroid, ECR_Overlap);
	trackBox->SetCollisionResponseToChannel(BBInvadersUtils::ECC_Projectile, ECR_Overlap);
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

void AAsteroidTracker::OnOverlapEnd(UPrimitiveComponent* comp, AActor* other, UPrimitiveComponent* otherComp, int32 otherBodyIndex)
{
	//replace
	int32 test{ 0 };
}

void AAsteroidTracker::BeginPlay()
{
	Super::BeginPlay();
	
}

void AAsteroidTracker::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}



