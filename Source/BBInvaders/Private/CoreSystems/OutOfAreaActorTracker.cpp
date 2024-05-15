// by Dmitry Kolontay


#include "CoreSystems/OutOfAreaActorTracker.h"
#include "Environment/Asteroid.h"
#include "Components/BoxComponent.h"
#include <array>
#include "BBInvadersUtils.h"

const FVector AOutOfAreaActorTracker::zKillBoxExtent{ 1.5f, 1.5f, 0.125f };
const FVector AOutOfAreaActorTracker::xKillBoxExtent{ 0.125f, 1.5f, 1.875f };
const FVector AOutOfAreaActorTracker::yKillBoxExtent{ 1.5f, 0.125f, 1.875f };

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
	asteroidTrackBox->SetMobility(EComponentMobility::Static);
	
	SetRootComponent(asteroidTrackBox);

	BBInvadersUtils::ConfigureOverlapCollision<true>(asteroidTrackBox, ECC_WorldStatic,
		BBInvadersUtils::ECC_Asteroid);

	auto&& configureKillBox{ [this](UBoxComponent* box) {
		box->SetupAttachment(RootComponent);
		BBInvadersUtils::ConfigureOverlapCollision<true>(box, ECC_WorldStatic,
			BBInvadersUtils::ECC_Asteroid/*, BBInvadersUtils::ECC_Invader, 
			BBInvadersUtils::ECC_Projectile*/);
			
	} };

	for (auto box : zKillBoxes) {
		configureKillBox(box);
	};
	for (auto box : xyKillBoxes) {
		configureKillBox(box);
	};

	asteroidTrackBox->SetHiddenInGame(true, true);
}

void AOutOfAreaActorTracker::SetTrackArea(const FTransform& t, const FVector& halfSize)
{
	SetActorTransform(t);
	SetTrackArea(halfSize);
}

void AOutOfAreaActorTracker::SetTrackArea(const FVector& halfSize)
{
	asteroidTrackBox->SetBoxExtent(halfSize);

	//--
	FVector newKillBoxExtent{ halfSize * zKillBoxExtent };
	for (auto box : zKillBoxes) {
		box->SetBoxExtent(newKillBoxExtent);
	};
	zKillBoxes[0]->SetRelativeLocation({ 0.f, 0.f, halfSize.Z * 2.f });
	zKillBoxes[1]->SetRelativeLocation({ 0.f, 0.f, halfSize.Z * -2.f });

	//--
	newKillBoxExtent = halfSize * xKillBoxExtent;
	xyKillBoxes[0]->SetBoxExtent(newKillBoxExtent);
	xyKillBoxes[2]->SetBoxExtent(newKillBoxExtent);
	xyKillBoxes[0]->SetRelativeLocation({ halfSize.X * 1.625f, 0.f, 0.f });
	xyKillBoxes[2]->SetRelativeLocation({ halfSize.X * -1.625f, 0.f, 0.f });

	newKillBoxExtent = halfSize * yKillBoxExtent;
	xyKillBoxes[1]->SetBoxExtent(newKillBoxExtent);
	xyKillBoxes[3]->SetBoxExtent(newKillBoxExtent);
	xyKillBoxes[1]->SetRelativeLocation({ 0.f, halfSize.Y * 1.625f, 0.f });
	xyKillBoxes[3]->SetRelativeLocation({ 0.f, halfSize.Y * -1.625f, 0.f });
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


