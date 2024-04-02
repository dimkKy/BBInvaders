// by Dmitry Kolontay


#include "UI/BBIHUD.h"
#include "UI/GametimeUI.h"
#include "UI/MainMenu.h"
#include "UI/PauseMenu.h"
#include "Player/PlayerPawn.h"
#include "Player/MainMenuPawn.h"
#include "Player/BBIPlayerController.h"
#include "Components/Button.h"
#if WITH_EDITOR
#include "Misc/DataValidation.h"
#endif

ABBIHUD::ABBIHUD() :
	gametimeUI{ nullptr }, mainMenu{ nullptr }
{
}

void ABBIHUD::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	gametimeUI = CreateWidget<UGametimeUI>(PlayerOwner, gametimeUIClass);
	mainMenu = CreateWidget<UMainMenu>(PlayerOwner, mainMenuClass);
}

void ABBIHUD::OnViewTargetChange(const AActor& newTarget)
{
	if (newTarget.IsA(APlayerPawn::StaticClass())) {
		if (mainMenu->IsInViewport()) {
			mainMenu->RemoveFromParent();
		}
		if (!gametimeUI->IsInViewport()) {
			gametimeUI->AddToViewport();
		}
		
		FInputModeGameAndUI inputMode;
		inputMode.SetHideCursorDuringCapture(false);
		PlayerOwner->SetInputMode(inputMode);

		return;
	}

	if (newTarget.IsA(AMainMenuPawn::StaticClass())) {
		if (gametimeUI->IsInViewport()) {
			gametimeUI->RemoveFromParent();
		}
		if (!mainMenu->IsInViewport()) {
			mainMenu->AddToViewport();
		}

		PlayerOwner->SetInputMode(FInputModeUIOnly::FInputModeUIOnly());

		return;
	}
}

void ABBIHUD::RequestBindings(ABBIPlayerController& controller)
{
	mainMenu->newGameButton->OnClicked.AddDynamic(
		&controller, &ABBIPlayerController::OnGameplayInitiated);
	gametimeUI->pauseMenu->toMainMenuButton->OnClicked.AddDynamic(
		&controller, &ABBIPlayerController::OnGameplayEnd);
	gametimeUI->pauseButton->OnClicked.AddDynamic(
		&controller, &ABBIPlayerController::RequestPauseToggle);
	gametimeUI->pauseMenu->resumeButton->OnClicked.AddDynamic(
		&controller, &ABBIPlayerController::RequestPauseToggle);
}

UProjectileSelector* ABBIHUD::GetProjectileSelector() const
{
	//return gametimeUI->projectileSelector;
	check(false);
	return nullptr;
}

#if WITH_EDITOR
EDataValidationResult ABBIHUD::IsDataValid(FDataValidationContext& context) const
{
	Super::IsDataValid(context);

	if (!IsValid(gametimeUIClass))
		context.AddError(FText::FromString("Invalid gametimeUI class"));
	if (!IsValid(mainMenuClass))
		context.AddError(FText::FromString("Invalid mainMenu class"));

	return context.GetNumErrors() > 0 ?
		EDataValidationResult::Invalid : EDataValidationResult::Valid;
}
#endif