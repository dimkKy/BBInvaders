// by Dmitry Kolontay


#include "Player/MainMenuPawn.h"
#include "Camera/CameraComponent.h"

AMainMenuPawn::AMainMenuPawn() :
	camera{ CreateDefaultSubobject<UCameraComponent>("camera") }
{
	PrimaryActorTick.bCanEverTick = false;
	SetRootComponent(camera);
}

void AMainMenuPawn::Shoot()
{
}

EShooterType AMainMenuPawn::GetShooterType() const
{
	return EShooterType::EST_Human;
}

EDataValidationResult AMainMenuPawn::IsDataValid(TArray<FText>& ValidationErrors)
{
	Super::IsDataValid(ValidationErrors);

	if (GetWorld() && AutoPossessPlayer == EAutoReceiveInput::Disabled) {
		ValidationErrors.Add(FText::FromString("Invalid AutoPossessPlayer"));
	}

	return ValidationErrors.Num() > 0 ?
		EDataValidationResult::Invalid : EDataValidationResult::Valid;
}

void AMainMenuPawn::BeginPlay()
{
	Super::BeginPlay();
}

