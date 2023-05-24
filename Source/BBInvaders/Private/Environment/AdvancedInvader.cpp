// by Dmitry Kolontay


#include "Environment/AdvancedInvader.h"
#include "BBInvadersUtils.h"
#include "Player/PlayerPawn.h"
#include "BBInvadersUtils.h"
#include "GameFramework/SpringArmComponent.h"

const FVector2D AAdvancedInvader::rotationSpeedRange = {45.f, 90.f};
const FVector2D AAdvancedInvader::descensionSpeedRange = {45.f, 90.f};

AAdvancedInvader::AAdvancedInvader() :
	orbit{ CreateDefaultSubobject<USpringArmComponent>("platformArm") },
	rotationSpeed{ 0.f }, descensionSpeed{ 0.f }
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;

	SetRootComponent(orbit);
	
	orbit->bDoCollisionTest = false;
	orbit->TargetArmLength = 0.f;

	body->SetupAttachment(orbit, USpringArmComponent::SocketName);
	body->SetRelativeRotation(BBInvadersUtils::BehindRotator);
}

void AAdvancedInvader::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	SetActorRotation(GetActorRotation() + 
		BBInvadersUtils::unitRotator * DeltaTime * rotationSpeed);

	orbit->TargetArmLength -= DeltaTime * descensionSpeed;
	//no floor
}

float AAdvancedInvader::GetOnPlanetCollisionDamage() const
{
	return 75.0f;
}

void AAdvancedInvader::BeginPlay()
{
	Super::BeginPlay();
	UWorld* world{ GetWorld() };
	if (APlayerPawn* target{ BBInvadersUtils::GetFirstActor<APlayerPawn>(world) }) {
		FVector targetLocation{ target->GetActorLocation() };
		FVector toTarget{ targetLocation - GetActorLocation() };

		orbit->TargetArmLength = toTarget.Size();
		
		body->SetRelativeRotation(FRotator::ZeroRotator);

		SetActorLocationAndRotation(targetLocation, 
			FRotationMatrix::MakeFromX(toTarget).Rotator());
		SetActorTickEnabled(true);

		rotationSpeed = FMath::RandRange(rotationSpeedRange.X, rotationSpeedRange.Y);
		if (FMath::RandBool()) {
			rotationSpeed *= -1.f;
		}
		descensionSpeed = FMath::RandRange(descensionSpeedRange.X, descensionSpeedRange.Y);
	}
}
