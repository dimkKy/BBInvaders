// by Dmitry Kolontay


#include "Environment/AdvancedInvader.h"
#include "BBInvadersUtils.h"
#include "Player/PlayerPawn.h"
#include "BBInvadersUtils.h"
#include "GameFramework/SpringArmComponent.h"

AAdvancedInvader::AAdvancedInvader() :
	//orbit{ CreateDefaultSubobject<USpringArmComponent>("platformArm") },
	rotationSpeed{ 0.f }, descensionSpeed{ 0.f }
	, target{false, FVector::ForwardVector}
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;
	
	//orbit->bDoCollisionTest = false;
	//orbit->TargetArmLength = 0.f;
}

void AAdvancedInvader::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	/*
	SetActorRotation(GetActorRotation() +
		BBInvadersUtils::unitRotator * DeltaTime * rotationSpeed);
	orbit->TargetArmLength -= DeltaTime * descensionSpeed; */
}

float AAdvancedInvader::GetOnPlanetCollisionDamage() const
{
	return 75.0f;
}

void AAdvancedInvader::BeginPlay()
{
	if (APlayerPawn* playerPawn{
		BBInvadersUtils::GetFirstActor<APlayerPawn>(GetWorld()) }) 
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

		target.Key = true;
		target.Value = playerPawn->GetActorLocation();
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
	Super::BeginPlay();
}

void AAdvancedInvader::RotateMoveToTarget(float distance/* = 0.f*/)
{
	check(target.Key);
	FVector toTarget{ target.Value - GetActorLocation() };
	body->MoveComponent(
		toTarget.GetSafeNormal() * distance,
		FRotationMatrix::MakeFromX(target.Value - GetActorLocation()).Rotator().Quaternion(), 
		false);
}
