// by Dmitry Kolontay


#include "UI/BBInvadersHUD.h"
#include "UI/GametimeUI.h"
#include "UI/MainMenu.h"
#include "UI/PauseMenu.h"
#include "Player/PlayerPawn.h"
#include "Player/MainMenuPawn.h"
#include "CoreSystems/BBInvadersGameModeBase.h"
#include "Components/Button.h"
#if WITH_EDITOR
#include "Misc/DataValidation.h"
#endif

ABBInvadersHUD::ABBInvadersHUD() :
	gametimeUI{ nullptr }, mainMenu{ nullptr }
{
}

void ABBInvadersHUD::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	gametimeUI = CreateWidget<UGametimeUI>(PlayerOwner, gametimeUIClass);
	mainMenu = CreateWidget<UMainMenu>(PlayerOwner, mainMenuClass);
}

void ABBInvadersHUD::OnViewTargetChange(const AActor& newTarget)
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

void ABBInvadersHUD::RequestBindings(ABBInvadersGameModeBase& gameMode)
{
	mainMenu->newGameButton->OnClicked.AddDynamic(
		&gameMode, &ABBInvadersGameModeBase::StartGameplay);
	gametimeUI->pauseMenu->toMainMenuButton->OnClicked.AddDynamic(
		&gameMode, &ABBInvadersGameModeBase::GoToMainMenu);
	gametimeUI->pauseButton->OnClicked.AddDynamic(
		&gameMode, &ABBInvadersGameModeBase::TogglePause);
	gametimeUI->pauseMenu->resumeButton->OnClicked.AddDynamic(
		&gameMode, &ABBInvadersGameModeBase::TogglePause);
}

#if WITH_EDITOR
EDataValidationResult ABBInvadersHUD::IsDataValid(FDataValidationContext& context) const
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