// by Dmitry Kolontay


#include "CoreSystems/OutOfAreaActorTracker.h"
#include "Environment/Asteroid.h"
#include "Components/BoxComponent.h"
#include "BBInvadersUtils.h"

AOutOfAreaActorTracker::AOutOfAreaActorTracker() :
	asteroidTrackBox{ CreateDefaultSubobject<UBoxComponent>("asteroidTrackBox") },
	zKillBoxes{ CreateDefaultSubobject<UBoxComponent>("zKillBox1"), 
		CreateDefaultSubobject<UBoxComponent>("zKillBox2") },
	xyKillBoxes{ CreateDefaultSubobject<UBoxComponent>("xyKillBox1"),
		CreateDefaultSubobject<UBoxComponent>("xyKillBox2"),
		CreateDefaultSubobject<UBoxComponent>("xyKillBox3"),
		CreateDefaultSubobject<UBoxComponent>("xyKillBox4"), }
{
	PrimaryActorTick.bCanEverTick = false;
	SetRootComponent(asteroidTrackBox);

	BBInvadersUtils::ConfigureDefaultCollision<true>(asteroidTrackBox, ECC_WorldStatic,
		BBInvadersUtils::ECC_Asteroid);

	auto&& configureKillBox{ [this](UBoxComponent* box) {
		box->SetupAttachment(RootComponent);
		BBInvadersUtils::ConfigureDefaultCollision<true>(box, ECC_WorldStatic,
			BBInvadersUtils::ECC_Asteroid/*, BBInvadersUtils::ECC_Invader, 
			BBInvadersUtils::ECC_Projectile*/);
		
	} };

	for (auto box : zKillBoxes) {
		configureKillBox(box);
	};
	for (auto box : xyKillBoxes) {
		configureKillBox(box);
	};
	
}

void AOutOfAreaActorTracker::SetTrackArea(const FTransform& t, const FVector& halfSize)
{
	SetActorTransform(t);
	SetTrackArea(halfSize);
}

void AOutOfAreaActorTracker::SetTrackArea(const FVector& halfSize)
{
	asteroidTrackBox->SetBoxExtent(halfSize, true);
}

void AOutOfAreaActorTracker::BeginPlay()
{
	Super::BeginPlay();
	auto&& configureKillBox{ [this](UBoxComponent* box) {
		box->OnComponentBeginOverlap.AddDynamic(
			this, &AOutOfAreaActorTracker::OnKillOverlapBegin);
	} };

	for (auto box : zKillBoxes) {
		configureKillBox(box);
	};
	for (auto box : xyKillBoxes) {
		configureKillBox(box);
	};

	asteroidTrackBox->OnComponentEndOverlap.AddDynamic(
		this, &AOutOfAreaActorTracker::OnTrackEnd);
}

void AOutOfAreaActorTracker::OnTrackEnd(UPrimitiveComponent* comp, 
	AActor* other, UPrimitiveComponent* otherComp, int32 otherBodyIndex)
{
	check(other->IsA(AAsteroid::StaticClass()));
	//REDO - WORKS NOT AS EXPECTED
	FVector otherLocation{ other->GetActorLocation() };
	FVector toOther{ otherLocation - GetActorLocation() };

	FVector projectForward{ toOther.ProjectOnTo(GetActorForwardVector()) };
	FVector projectRight{ toOther.ProjectOnTo(GetActorRightVector()) };
	FVector areaSize{ asteroidTrackBox->GetScaledBoxExtent() };

	FVector halfCorrection{ 0.f };

	if (projectForward.SizeSquared() >= areaSize.X * areaSize.X) {
		halfCorrection += projectForward;
	}
	if (projectRight.SizeSquared() >= areaSize.Y * areaSize.Y) {
		halfCorrection += projectRight;
	}

	other->SetActorLocation(otherLocation - 2.1f * halfCorrection);
}

void AOutOfAreaActorTracker::OnKillOverlapBegin(UPrimitiveComponent* comp, 
	AActor* other, UPrimitiveComponent* otherComp, int32 otherBodyIndex, 
	bool bFromSweep, const FHitResult& sweepResult)
{
	other->Destroy();
}


