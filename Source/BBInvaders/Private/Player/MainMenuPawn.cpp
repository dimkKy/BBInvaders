// by Dmitry Kolontay


#include "Player/MainMenuPawn.h"
#include "Camera/CameraComponent.h"
#if WITH_EDITOR
#include "Misc/DataValidation.h"
#endif

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
	return EShooterType::EST_MenuPawn;
}

#if WITH_EDITOR
EDataValidationResult AMainMenuPawn::IsDataValid(FDataValidationContext& context) const
{
	Super::IsDataValid(context);

	if (GetWorld() && AutoPossessPlayer == EAutoReceiveInput::Disabled) {
		context.AddError(FText::FromString("Invalid AutoPossessPlayer"));
	}

	return context.GetNumErrors() > 0 ?
		EDataValidationResult::Invalid : EDataValidationResult::Valid;
}
#endif

void AMainMenuPawn::BeginPlay()
{
	Super::BeginPlay();
}

