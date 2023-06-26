// by Dmitry Kolontay


#include "Environment/AdvancedInvader.h"
#include "BBInvadersUtils.h"
#include "Player/PlayerPawn.h"
#include "BBInvadersUtils.h"
#include "GameFramework/SpringArmComponent.h"

AAdvancedInvader::AAdvancedInvader() :
	//orbit{ CreateDefaultSubobject<USpringArmComponent>("platformArm") },
	rotationSpeed{ 0.f }, descensionSpeed{ 0.f }, 
	target{}
{
	PrimaryActorTick.bCanEverTick = true;
	//PrimaryActorTick.bStartWithTickEnabled = false;
	
	//orbit->bDoCollisionTest = false;
	//orbit->TargetArmLength = 0.f;
}

void AAdvancedInvader::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	RotateMoveToTarget(DeltaTime);
}

float AAdvancedInvader::GetOnPlanetCollisionDamage() const
{
	return 75.0f;
}

void AAdvancedInvader::SetTarget(const AActor& actor)
{
	/* That or similar operation always triggers overlap with pawn

		FVector targetLocation{ playerPawn->GetActorLocation() };
		FVector toTarget{ targetLocation - GetActorLocation() };
		check(SetRootComponent(orbit));
		SetActorLocationAndRotation(targetLocation,
			FRotationMatrix::MakeFromX(toTarget).Rotator());
		orbit->TargetArmLength = toTarget.Size();
		body->AttachToComponent(orbit,
			FAttachmentTransformRules::SnapToTargetIncludingScale,
			USpringArmComponent::SocketName);
		*/

	target.SetTarget(actor);
	RotateMoveToTarget();
	//-----------
	rotationSpeed = FMath::RandRange(
		rotationSpeedRange.first, rotationSpeedRange.second);
	if (FMath::RandBool()) {
		rotationSpeed *= -1.f;
	}
	descensionSpeed = FMath::RandRange(
		descensionSpeedRange.first, descensionSpeedRange.second);
	SetActorTickEnabled(true);
}

int32 AAdvancedInvader::GetOnKillBounty() const
{
	return Super::GetOnKillBounty() * 2;
}

void AAdvancedInvader::BeginPlay()
{
	if (APlayerPawn* playerPawn{
		BBInvadersUtils::GetFirstActor<APlayerPawn>(GetWorld()) }) 
	{
		SetTarget(*playerPawn);
	}
	check(target);
	Super::BeginPlay();
}

void AAdvancedInvader::RotateMoveToTarget(float deltatime/* = 0.f*/)
{
	if (!target) {
		return;
	}

	FVector toThis{ GetActorLocation() - target.location };
	toThis.GetSafeNormal() * descensionSpeed;
	FVector newToThis{ 
		(toThis - toThis.GetSafeNormal() * descensionSpeed * deltatime)
		.RotateAngleAxis(rotationSpeed * deltatime , target.up) };
	RootComponent->SetWorldLocationAndRotation(target.location + newToThis, 
		FRotationMatrix::MakeFromX(newToThis * -1.f).Rotator().Quaternion());
}
